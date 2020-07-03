# include<stdio.h>
# define SIZE 8
void revArr(void);
int a[] = {10,20,30,40,50,60,70,80,};
int start =0, end =7;
void main()
{	
	int i;
	revArr();
	for(i=0 ; i< SIZE  ; i++){
		printf("%3d",a[i]);
	}
}
void revArr()
{
	int temp;
	while(start < end){
		temp = a[start];
		a[start] = a[end];
		a[end] = temp;

		start++;
		end--;
	}
}
