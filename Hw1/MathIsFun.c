#include "MathIsFun.h"

/*
 * Calculates the largest integer less or equal than the square root of x.
 * funSqrt(10) = 3
 * funSqrt(16) = 4
 * @param x - An integer for which the function applies
 * @return the value of |_sqrt(x)_|, if x is negative then the function
 * returns -1.
 */
int funSqrt(int x); // ==> Declaration is always in the beginning of the file.

int funPow(int x, int n, int d) {
		int res = 1;
		if(n==0){
			return 1;
		}
		else if(n==1){
			return x%d;
		}
		else if(n%2==0){
			res*= (funPow(x,n/2,d)*funPow(x,n/2,d))%d;
		}
		else{
			res*=(x*funPow(x,(n-1)/2,d)*funPow(x,(n-1)/2,d))%d;
		}

		if(res<0){
			res += d;
		}
		return res;
	}

int funSqrt(int x) {
	if(x<0)
		return -1;
		int small = 0;
		int big = x;
		float res = x;

		while(1==1){
			if(res<x && res*res<=x && x<(res+1)*(res+1)){
				return (int)res;}
			else if(res*res<x){
				small = (int)res;
			}
			else{
				big = (int)res;
			}
			if((big+small)%2==0)
				res = (float)(small+big)/2;
			else{
				res = (float)(small+big)/2;
				res-=0.5;
			}
		}
		return -1;

	}

bool funPrimeCheck(int x) {
	if(x<0)
		return false;//lama ze sababa?
	int sqx = funSqrt(x);
	int i;
	for(i=2 ;i<=sqx; i++){
		if(x%i==0)
			return false;
	}
	return true;
}

bool funPalindromeCheck(int x) {
	if (x<0)
		return false;
	int checker=x;
	int counter=0;
	while (checker>=1){
		checker/=10;
		counter++;
	}
	int number[counter];
	checker = x;
	int i;
	for(i=counter;i>0;i--){
			number[i-1]=(int)checker%10;
			checker = (checker-(checker%10))/10;
		}
	int n;
	if(counter%2==0)
		n=counter/2;
	else
		n=counter/2-0.5;
	for(i=0;i<=n;i++){
		if(number[i]!=number[counter-1-i])
			return false;
	}
	return true;
}
