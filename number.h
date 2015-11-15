/*Copyright (C) 2015  Rohan Patil

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/


#include "charqueue.h"

typedef struct digit {
	int value;
	struct digit *next;
	struct digit *prev;
}digit;

typedef struct number {
	struct digit *head;
	struct digit *inttail;
	struct digit *fractail;	
	int sign;
	int intlength;
	int fraclength;
}number;

void initnum(number *n);
void makenode(number *n, int x);
void removenode(number *n, int x);
void freenum(number *n);
void copynum(number *n, number *m);
void adddigit(number *n, int d);
number makenumstr(char *str);
number makenumfile(char *str);
number makenumchqueue(charqueue *q);
double makedoublenum(number x);
number makenumdouble(double d);
number add(number a, number b, int base);
number subtract(number a, number b, int base);
number multiply(number a, number b, int base);
number divide(number a, number b, int base, int scale);
number Add(number a, number b, int base);
number Subtract(number a, number b, int base);
number Multiply(number a, number b, int base);
number Divide(number a, number b, int base, int scale);
void printnum(number n);
number power(number x, number y, int base, int scale);
	
