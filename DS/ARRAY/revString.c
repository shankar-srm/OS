# include<stdio.h>

void revString(void);
char s[] = "shankar";

int  start =0, end =6;
int temp;
void revString(){
	while(start < end){
		
		temp = s[start];
		s[start] = s[end];
		s[end] = temp;

		start++;
		end--;
	}
}

void main(){
	int i;
	revString();

	for(i=0 ; i <=6 ;i++){

	printf("%c",s[i]);
	}
}
