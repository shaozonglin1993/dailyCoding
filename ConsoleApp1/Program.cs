// See https://aka.ms/new-console-template for more information
//Console.WriteLine("Hello, World!");

using System;
using System.Drawing;
using static System.Drawing.Bitmap;
using System.IO.MemoryMappedFiles;
using System.Drawing.Imaging;

namespace ReadImageApplication
{

    //视觉内存读图

    class Image
    {
        private static Bitmap ByteToGrayBitmap(byte[] rawValues, int width, int height)
        {
            try
            {
                //申请目标位图的变量，并将其内存区域锁定
                Bitmap bmp = new(width, height, PixelFormat.Format8bppIndexed);
                BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height),
                 ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);

                //获取图像参数
                int stride = bmpData.Stride;  // 扫描线的宽度  
                int offset = stride - width;  // 显示宽度与扫描线宽度的间隙  
                IntPtr iptr = bmpData.Scan0;  // 获取bmpData的内存起始位置  
                int scanBytes = stride * height;// 用stride宽度，表示这是内存区域的大小  

                //下面把原始的显示大小字节数组转换为内存中实际存放的字节数组
                int posScan = 0, posReal = 0;// 分别设置两个位置指针，指向源数组和目标数组  
                byte[] pixelValues = new byte[scanBytes];  //为目标数组分配内存  

                for (int x = 0; x < height; x++)
                {
                    //下面的循环节是模拟行扫描
                    for (int y = 0; y < width; y++)
                    {
                        pixelValues[posScan++] = rawValues[posReal++];
                    }
                    posScan += offset;  //行扫描结束，要将目标位置指针移过那段“间隙”  
                }

                //用Marshal的Copy方法，将刚才得到的内存字节数组复制到BitmapData中
                System.Runtime.InteropServices.Marshal.Copy(pixelValues, 0, iptr, scanBytes);
                //解锁内存区域  
                bmp.UnlockBits(bmpData);

                //下面的代码是为了修改生成位图的索引表，从伪彩修改为灰度
                ColorPalette tempPalette;
                using (Bitmap tempBmp = new Bitmap(1, 1, PixelFormat.Format8bppIndexed))
                {
                    tempPalette = tempBmp.Palette;
                }
                for (int i = 0; i < 256; i++)
                {
                    tempPalette.Entries[i] = System.Drawing.Color.FromArgb(i, i, i);
                }
                bmp.Palette = tempPalette;
                return bmp;
            }
            catch (Exception ex)
            {
                //LogListView(saveLog,$"内存读图异常 {ex.Message}", LogVariety.Error, "Image");
                return null;
            }
        }
        private static Bitmap ReadImageFromMemory()
        {
            try
            {
                int width = 4024;
                int height = 3036;
                //测试内存交互
                MemoryMappedFile memoryMappedFile = MemoryMappedFile.OpenExisting("RawImgMap");//使用前申请
                bool mutexCeated;
                Bitmap bitmap = new(/*width*/ width, /*height*/ height);
                Mutex mutex = new(false, "RawImgMapMutex", out mutexCeated);
                mutex.WaitOne();

                //读取虚拟内存
                using (MemoryMappedViewStream mvStrem = memoryMappedFile.CreateViewStream())
                {
                    BinaryReader binaryReader = new(mvStrem);
                    byte[] vs = binaryReader.ReadBytes(width * height);
                    mutex.ReleaseMutex();
                    binaryReader.Dispose();
                    binaryReader.Close();

                    bitmap = Image.ByteToGrayBitmap(vs, width, height);
                    GC.Collect();
                }
                return bitmap;
            }
            catch (Exception ex)
            {
                //LogListView_Main(saveLog, $"内存读图异常 {ex.Message}", LogVariety.Error, "Image");
                return null;
            }
        }

        /* main函数 */
        static void Main(string[] args)
        {
            Image.ReadImageFromMemory().Save("ph.jpg");
        }
    }
}


//using System;
//using System.IO;
//using System.IO.MemoryMappedFiles;
//using System.Runtime.InteropServices;
//using System.Text;

//class Program
//{
//    static void Main()
//    {
//         要写入内存映射文件的数据  
//        string dataToWrite = "Hello, Memory-Mapped Files!";

//         创建一个新的内存映射文件，大小为数据长度加上一些额外空间（如果需要）  
//         注意：MemoryMappedFile.CreateNew 创建一个与磁盘文件相关联的内存映射文件  
//         如果不需要与磁盘文件关联，可以使用 MemoryMappedFile.CreateOrOpen  
//        using (var mmf = MemoryMappedFile.CreateNew("testmap", dataToWrite.Length * Encoding.UTF8.GetMaxByteCount(1)))
//         {
//             创建一个访问器来写入数据  
//            using (var accessor = mmf.CreateViewAccessor())
//            {
//                 将数据写入内存映射文件  
//                var buffer = Encoding.UTF8.GetBytes(dataToWrite);
//                accessor.WriteArray(0, buffer, 0, buffer.Length);
//            }

//             此时，内存映射文件已经包含了我们写入的数据  
//             可以在其他进程中通过 OpenExisting 方法打开它，并读取数据  

//             示例：打开并读取刚才创建的内存映射文件  
//            using (var mmfToRead = MemoryMappedFile.OpenExisting("testmap"))
//            using (var accessor = mmfToRead.CreateViewAccessor())
//            {
//                // 读取数据  
//                var buffer = new byte[dataToWrite.Length * Encoding.UTF8.GetMaxByteCount(1)];
//                accessor.ReadArray(0, buffer, 0, buffer.Length);

//                // 将字节转换为字符串并输出  
//                string dataRead = Encoding.UTF8.GetString(buffer).TrimEnd('\0'); // 去除可能的空字符  
//                Console.WriteLine("Read from memory-mapped file: " + dataRead);
//            }

//            MemoryMappedFile memoryMappedFile = MemoryMappedFile.OpenExisting("testmap");//使用前申请
//            using (MemoryMappedViewStream mvStrem = memoryMappedFile.CreateViewStream())
//            {
//                BinaryReader binaryReader = new(mvStrem);
//                byte[] vs = binaryReader.ReadBytes(10);
//                string asciiString = System.Text.Encoding.ASCII.GetString(vs);
//                Console.WriteLine(asciiString); // 输出: Hello
//            }
//        }

//         当所有using块结束时，内存映射文件和访问器都将被自动释放  
//    }
//}