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



typedef struct node {
	char c;
	struct node *next;
}node;
	
typedef struct charqueue {
	node *head;
}charqueue;



void qinit(charqueue *q);
void enqueue(charqueue *q, char ch);
char dequeue(charqueue *q);
int qempty(charqueue *q);
int qfull(charqueue *q); 

