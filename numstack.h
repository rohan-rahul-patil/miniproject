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


#include "number.h"
typedef struct numstack {
	number a[1000];
	int i;
}numstack;

void init(numstack *p);
void push(numstack *p, number x);
number pop(numstack *p);
int empty(numstack *p);
int full(numstack *p);
