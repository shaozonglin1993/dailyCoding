#include <stdio.h>

void func(int arr[], int *parr, int *pi)
{
	int num[5][5]={0};
	
	printf("%d\n",num[3,4]);

	printf("%d\n", sizeof(arr));
	printf("%d\n", sizeof(parr));
	printf("%d\n", sizeof(pi));
}



int main()
{

	int arr[5] = {0,1,2,3,4};
	int *ap = arr+2;

	printf("%d %d %d\n", *ap, ap[-1], ap[1]);
	
	printf("%\n", 2[arr]);

	int *p = arr;

	func(arr, arr, p);
	return 0;
}
