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
#include "charstack.h"
#define SIZE 128

void initc(charstack *q) {
	q->j = 0;
}

void pushc(charstack *q, char y) {
	q->c[q->j] = y;
	(q->j)++;
}

char popc(charstack *q) {
	(q->j)--;
	return q->c[q->j];
}

int emptyc(charstack *q) {
	return q->j == 0;
}

int fullc(charstack *q) {
	return q->j == SIZE - 1;
}

