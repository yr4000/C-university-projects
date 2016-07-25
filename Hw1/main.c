/*
 * main.c
 *
 *  Created on: 2 באפר 2016
 *      Author: Yair
 */
#include "MathIsFun.h"
#include <stdio.h>

int main(){
	setbuf(stdout, NULL);
	printf("Welcome to Math Is Fun - beta version\n");
	printf("Supported operations are:\n");
	printf("1 - Power calculation\n");
	printf("2 - Prime test\n");
	printf("3 - Palindrome test\n");
	printf("Please enter operation number (1/2/3):\n");

	int x;
	scanf("%d", &x);
	switch(x){
	int a,b,c;
	case 1:
		printf("Please enter three space separated numbers: \n");
		scanf("%d",&a);
		scanf("%d",&b);
		scanf("%d",&c);
		printf("res = %d\n",funPow(a,b,c));
		break;
	case 2:
		printf("Please enter an integer: \n");
		scanf("%d",&a);
		if(funPrimeCheck(a)==1)
			printf("res = true\n");
		else
			printf("res = false\n");
		break;
	case 3:
		printf("Please enter an integer: \n");
		scanf("%d",&a);
		if(funPalindromeCheck(a)==1)
					printf("res = true\n");
				else
					printf("res = false\n");
		break;
	default:
		printf("you didn't choose one of the options.\nthe system will now destroy itself.\ngoodbye.");
		break;
	}
	return 0;
}
