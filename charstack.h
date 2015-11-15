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


typedef struct charstack {
	char c[128];
	int j;
}charstack;

void initc(charstack *q);
void pushc(charstack *q, char y);
char popc(charstack *q);
int emptyc(charstack *q);
int fullc(charstack *q);
