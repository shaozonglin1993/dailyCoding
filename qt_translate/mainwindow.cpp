#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // ui->comboBox->addItem("CN");
    // ui->comboBox->addItem("EN");
    // ui->comboBox->addItem("VN");

    ui->setupUi(this);

    QWidget *centralWidget = this->centralWidget(); // 获取中心部件
    QVBoxLayout *layout = new QVBoxLayout(centralWidget); // 创建一个垂直布局
    centralWidget->setLayout(layout); // 将布局设置为中心部件的布局
    btn = new QPushButton(tr("按钮"), this);
    layout->addWidget(btn);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//调用installTranslator后，系统会给窗体发送信号将产生了changeEvent槽产生event
void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
    {
        ui->retranslateUi(this);
    }
        break;
    default:
        break;
    }

    qInfo()<<trans.language();
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1 == "CN")
    {
        trans.load(":/untitled4_zh_CN.qm");
    }else if(arg1 == "EN")
    {
        trans.load(":/qt_en.qm");
    }
    else if(arg1 == "VN")
    {
        trans.load(":/qt_vn.qm");

    }
    else
    {}

    btn->setText(tr("按钮"));
    qApp->installTranslator(&trans);
}


void MainWindow::on_pushButton_5_clicked()
{
    // 更新按钮文本
    QString translatedText = qApp->translate("MainWindow", "按钮", nullptr);

    if(!translatedText.isEmpty())
    {
        qInfo()<<" - "<<translatedText;
        btn->setText(translatedText);
    }
}

