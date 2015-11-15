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


#include <stdio.h>
#include "numstack.h"
#define SIZE 128

void init(numstack *p) {
	p->i = 0;
}

void push(numstack *p, number x) {
	p->a[p->i] = x;
	(p->i)++;
}

number pop(numstack *p) {
	(p->i)--;	
	return p->a[p->i];
}


int empty(numstack *p) {
	return p->i == 0;
}

int full(numstack *p) {
	return p->i == SIZE - 1;
}


