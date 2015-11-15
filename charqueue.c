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



#include<stdio.h>
#include<stdlib.h>
#include "charqueue.h"

void qinit(charqueue *q) {
	q->head = NULL;
}

void enqueue(charqueue *q, char ch) {
	node *p;
	node *temp;
	p = q->head;
	temp  = (node *)malloc(sizeof(node));
	if(p) {
		while(p->next) {
			p = p->next;
		}
		p->next = temp;
	}
	else
		q->head = temp;
	temp->c = ch;
	temp->next = NULL;
}
		
char dequeue(charqueue *q) {
	char ch;
	node *temp;
	temp = q->head;
	if(q->head->next) 
		q->head = q->head->next;
	else 
		q->head = NULL;
	ch = temp->c;
	free(temp);
	return ch;
}
	
int qempty(charqueue *q) {
	if(q->head)
		return 0;
	else 
		return 1;
}
int qfull(charqueue *q) {
	return 0;
}


