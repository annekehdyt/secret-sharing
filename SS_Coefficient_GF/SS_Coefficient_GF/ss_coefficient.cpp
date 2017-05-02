#include <iostream>
#include <fstream>
#include <array>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "GFheader.h"

#define T_VALUE 2
#define PRIME 50000

using namespace std;

void polynomial(int secret[], int t, int n, int *shares);
void reconstruction(int t_share, int shares[][2], int final[]);

void swap(int *left, int *middle, int degree);
void convolution(int left[], int right[], int *middle, int degree);

void main(){ 
	int secret[4] = {8,155,157,158};
	int share[10];
	int final[10]={};
	int t=4;
	int n=4;

	polynomial(secret, t,n, share);	
	
	for(int i=0; i<n; i++){
		cout << "Secret " << i+1 << " : " << share[i] << endl;
	}

	int shares[4][2] = {{1, 144}, {2,251}, {3,10}, {4,205}}; 
	reconstruction(4, shares, final);
	
	for(int i=t-1; i>=0; i--){
		cout << "Final " << i+1 << " : " << final[i] << endl;
	}
}


/*  Shamirs Secret Sharing
	Recontructing the Shares

	** Input **
	m_share		: total m shares
	shares[][]	: Subset of Shares

	** Output **
	The secret message
*/

void reconstruction(int m_share, int shares[][T_VALUE], int final[]){
	int denom = 1;
	int sum = 0;
	int start, next, f;
	int left[5]={}, right[2]={};
	int middle[10]={};
	int degree;
	int param;
	
	/*
	printf("Share List\n");
	for(i = 0; i < m_share; i++)
		printf("(%d, %d)\n", (int)shares[i][0], (int)shares[i][1]);
	*/

	for(int i = 0; i < m_share; i++){
		denom = 1;
		left[0] = 1;
		right[0] = 1;
		degree = 1;
		for(int j = 0; j < m_share; j++){
			if(i == j)
				continue;
			start = shares[i][0];
			next = shares[j][0];

			if(degree != 1)
				swap(left, middle,degree);

			right[1] = next;
			convolution(left, right, middle, degree);
			
			degree++;
			denom = multiGF(denom, addSubGF(start,next));
		}

		f = shares[i][1];
		for(int k=0; k<degree; k++){
			param = multiGF(multiGF(middle[k],f), multiInvGF(denom));
			final[k] =  addSubGF(final[k],param);
			middle[k] = 0;
		}
	}
}

/****
	Multiplying the algebra using convolution method

	left[]	: Left parentheses
	right[]	: right parentheses
	middle	: The polynomial result
	degree	: degree of polynomial / round
****/
void convolution(int left[], int right[], int *middle, int degree){
	int pLeft=0;
	int pRight=1;

	for(int i=0; i<(degree+1); i++){
		for(int j=1; j>=0; j--){
			if(i==0 && j==1)
				continue;
			middle[i] = addSubGF(middle[i],multiGF(left[pLeft],right[j]));

			if(i==degree && j==1)
				break;
			if(j==1)
				pLeft++;
		}
	}
}

/*  Shamirs Secret Sharing
	Polynomial for splitting the shares

	** Input **
	secret		: secret message
	available	: n participants
	needed		: t-threshold value
*/

void polynomial(int secret[], int t, int n, int *shares){
	int sum;
	int exp;
	int x;
	int multi;

	/** check whether the secret total is same as t-1
	if((sizeof(secret)/sizeof(int)) != t-1){
		cout << "t = " << t << "     size : " << sizeof(secret)/sizeof(int) << endl; 
		cout << "The threshold size is not the same with the secret message" << endl;
		exit(1);
	}**/

	/** Display the secret message 
	for(int i=0; i<t; i++){
		cout << "Secret " << i+1 << " : " << secret[i] << endl;
	}**/

	for(x = 1; x <= n; x++){
		sum = secret[0];
		for(exp = 1; exp < t; exp++){
			multi = multiGF(secret[exp], powerGF(x,exp));
			sum = addSubGF(sum, multi);
		}
		shares[x-1] = modGF(sum);
	}
}



void swap(int *left, int *middle, int degree){
	for(int i=0; i<(degree+1); i++){
		left[i] = middle[i];
		middle[i] = 0;
	}
}