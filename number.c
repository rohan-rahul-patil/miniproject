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
#include <stdlib.h>
#include "number.h"

void initnum(number *n) {
	n->head = NULL;
	n->inttail = NULL;
	n->fractail = NULL;
	n->sign = 0;
	n->intlength = 0;
	n->fraclength = 0;
}

void makenode(number *n, int x) {
	digit *temp;
	temp = (digit *)malloc(sizeof(digit));
	if(n->head == NULL) {
		n->head = temp;
		n->fractail = temp;
		temp->next = NULL;
		temp->prev = NULL;
	}
	else {
		if(x == 1) {
			n->fractail->next = temp;
			temp->prev = n->fractail;
			temp->next = NULL;
			n->fractail = temp;
		}
		else if(x == 0) {
			temp->prev = NULL;
			temp->next = n->head;
			n->head->prev = temp;
			n->head = temp;
		}
	}
}

void removenode(number *n, int x) {
	digit *p;
	if(x == 0) {
		p = n->head;
		n->head = n->head->next;
		free(p);
		n->head->prev = NULL;
	}
	if(x == 1) {
		p = n->fractail;
		n->fractail = n->fractail->prev;
		free(p);
		n->fractail->next = NULL;
	}
}
		
void freenum(number *n) {
	digit *p;
	p = n->head;
	while(p) {
		n->head = n->head->next;
		free(p);
		p = n->head;
	}
}

void copynum(number *n, number *m) {
	digit *p;
	initnum(n);
	p = m->head;
	n->intlength = m->intlength;
	n->fraclength = m->fraclength;
	while(p) {
		makenode(n, 1);
		n->fractail->value = p->value;
		if(p == m->inttail) {
			n->inttail = n->fractail; 
		}
		p = p->next;
	}
	n->sign = m->sign;
}
		
void adddigit(number *n, int d) {
	if(n->inttail == n->fractail) {
		makenode(n, 1);
		n->fractail->value = d;
		n->inttail = n->fractail;
	}
}				

number makenumchqueue(charqueue *q) {
	number x;
	int check = 0;
	initnum(&x);
	char c;
	while(!qempty(q)) {
		c = dequeue(q);
		if(c == '-') 
			x.sign = 1;
		else if(('0' <= c) && (c <= '9')) {
			makenode(&x, 1);
			x.fractail->value = c - '0';
			if(check == 0) {
				(x.intlength)++;
			}
			if(check == 1) {
				(x.fraclength)++;
			}
		}
		else if(('A' <= c) && (c <= 'F')) {
			makenode(&x, 1);
			x.fractail->value = c - 'A' + 10;
			if(check == 0) {
				(x.intlength)++;
			}
			if(check == 1) {
				(x.fraclength)++;
			}
		}
		else if(c == '.') {
			x.inttail = x.fractail;
			check = 1;
		}	
	}	
	if(check == 0) {
		x.inttail = x.fractail;
	}
		
	return x;
}

int comparenum(number a, number b) {
	digit *p, *q;
	p = a.head;
	q = b.head;
	if(a.intlength > b.intlength) {
		return 5;
	}
	else if(a.intlength < b.intlength) {
		return -5;
	}
	else if(a.intlength == b.intlength) {
		while(p != NULL && q != NULL && (p->value == q->value)) {
			p = p->next;
			q = q->next;
		}
		if(p == NULL && q == NULL) {
			return 0;
		}
		else if(p == NULL || q == NULL) {
			if(q == NULL) {
				while(p->value == 0) {
					p = p->next;
				}
				if(p == NULL) 
					return 0;
				return 5;
			}
			if(p == NULL) {
				while(q->value == 0) {
					q = q->next;
				}
				if(q == NULL)
					return 0;
				return -5;
			}
		}
		else {
			if(p->value > q->value) 
				return 5;
			else if(p->value < q->value) 
				return -5;
		}
	}
	return 0;
}

double makedoublenum(number x) {
	double d = 0, k = 1;
	int i;
	digit *p;
	p = x.inttail;
	while(p) {
		d = d + p->value * k;
		k = k * 10;
		p = p->prev;
	}
	p = x.inttail->next;
	if(p == NULL)
		return d;
	k = 10;
	for(i = 0; i < 20 && p; i++) {
		d = d + p->value / k;
		k = k * 10;
		p = p->next;
	}
	return d;
}
				
	
number makenumdouble(double d) {
	double k;
	number a;
	initnum(&a);
	int x, t = 10, y, sign = 0;
	long int z;
	if(d < 0) {
		sign = 1;
		d = (-1) * d;
	}
	x = (int)(d);
	while(x > 0) {
		y = x - (x / t) * t;
		x = x / t;
		makenode(&a, 0);
		a.head->value = y;
		(a.intlength)++;
	}
	if(sign == 1) {
		a.sign = 1;
	}
	y = 0;
	x = (int)(d);
	d = d - x;
	k = d;
	t = 0;
	while(t < 20) {
		k = k * 10;
		z = (long int)(k);
		t++;
	}
	a.inttail = a.fractail;
	if(a.head == NULL && a.fractail == NULL) {
		makenode(&a, 1);
		a.inttail = a.fractail;
		a.head->value = 0;
	}
	if(z == 0) {
		return a;
	}
	t = 0;
	while(t < 20) {
		x = d * 10;
		d = d * 10 - x;
		makenode(&a, 1);
		a.fractail->value = x;
		(a.fraclength)++;
		t++;
	}
	return a;
}
number add(number a, number b, int base) {
	int x, carry = 0;
	digit *p, *q;
	if(comparenum(a, b) >= 0) {
		x = a.fraclength - b.fraclength;
		p = a.fractail;
		q = b.fractail;
		if(x > 0) {
			while(x > 0) {
				p = p->prev;
				x--;
			}
		}
		else if(x < 0) {
			x = -1 * x;	
			while(x > 0) {
				makenode(&a, 1);
				a.fractail->value = 0;
				x--;
				(a.fraclength)++;
			}
			p = a.fractail;
		}
		while(q) {
			p->value = p->value + q->value + carry; 
			carry = 0;
			if(p->value >= base) {
				carry = (p->value) / base;
				p->value = (p->value) % base;
			}
			p = p->prev;
			q = q->prev;
		}
		if(carry != 0) {
			while(carry != 0) {
				if(p == NULL) {
					makenode(&a, 0);
					a.head->value = 0;
					p = a.head;
					(a.intlength)++;
				}
				p->value = p->value + carry;
				carry = 0;
				if(p->value >= base) {
					carry = (p->value) / base;
					p->value = (p->value) % base;
				}
				p = p->prev;
			}
		}
		freenum(&b);
		return a;
	}
	if(comparenum(a, b) < 0) {
		x = b.fraclength - a.fraclength;
		p = b.fractail;
		q = a.fractail;
		if(x > 0) {
			while(x > 0) {
				p = p->prev;
				x--;
			}
		}
		else if(x < 0) {
			x = -1 * x;	
			while(x > 0) {
				makenode(&b, 1);
				b.fractail->value = 0;
				x--;
				(b.fraclength)++;
			}
			p = b.fractail;
		}
		while(q) {
			p->value = p->value + q->value + carry; 
			carry = 0;
			if(p->value >= base) {
				carry = (p->value) / base;
				p->value = (p->value) % base;
			}
			p = p->prev;
			q = q->prev;
		}
		if(carry != 0) {
			while(carry != 0) {
				if(p == NULL) {
					makenode(&b, 0);
					b.head->value = 0;
					p = b.head;
					(b.intlength)++;
				}
				p->value = p->value + carry;
				carry = 0;
				if(p->value >= base) {
					carry = (p->value) / base;
					p->value = (p->value) % base;
				}
				p = p->prev;
			}
		}
		freenum(&a);
		return b;
	}
	return a;
}
number subtract(number a, number b, int base) {
	int x, borrow = 0, check = 0;
	digit *p, *q;
	if(comparenum(a, b) > 0) {
		x = a.fraclength - b.fraclength;
		p = a.fractail;
		q = b.fractail;
		if(x > 0) {
			while(x > 0) {
				p = p->prev;
				x--;
			}
		}
		else if(x < 0) {
			x = -1 * x;	
			while(x > 0) {
				makenode(&a, 1);
				a.fractail->value = 0;
				x--;
				(a.fraclength)++;
			}
			p = a.fractail;
		}
		x = a.fraclength;
		while(q) {
			p->value = p->value - borrow;
			if(p->value < q->value) {
				p->value = p->value + base;
				p->value = p->value - q->value;
				borrow = 1;
			}
			else {
				p->value = p->value - q->value;
				borrow = 0;
			}
			if(p->value == 0 && check == 0 && x != 0) { 
				removenode(&a, 1);
				x--;
				(a.fraclength)--;
			}
			else
				check = 1;
			p = p->prev;
			q = q->prev;	
		}
		while(borrow != 0) {
			p->value = p->value - borrow;
			if(p->value < 0) {
				p->value = p->value + base;
				borrow = 1;
			}
			else
				borrow = 0;
			p = p->prev;
		}
		p = a.head;
		x = a.intlength - 1;
		while(p->value == 0 && x != 0) {
			removenode(&a, 0);
			p = p->next; 
			(a.intlength)--;
			x--;
		}
		return a;
	}
	if(comparenum(a, b) < 0) {
		x = b.fraclength - a.fraclength;
		p = b.fractail;
		q = a.fractail;
		if(x > 0) {
			while(x > 0) {
				p = p->prev;
				x--;
			}
		}
		else if(x < 0) {
			x = -1 * x;	
			while(x > 0) {
				makenode(&b, 1);
				b.fractail->value = 0;
				(b.fraclength)++;
				x--;
			}
			p = b.fractail;
		}
		x = b.fraclength;
		while(q) {
			p->value = p->value - borrow;
			if(p->value < q->value) {
				p->value = p->value + base;
				p->value = p->value - q->value;
				borrow = 1;
			}
			else {
				p->value = p->value - q->value;
				borrow = 0;
			}
			if(p->value == 0 && check == 0 && x != 0) { 
				removenode(&b, 1);
				x--;
				(b.fraclength)--;
			}
			else
				check = 1;
			p = p->prev;
			q = q->prev;	
		}
		while(borrow != 0) {
			p->value = p->value - borrow;
			if(p->value < 0) 
				borrow = 1;
			else
				borrow = 0;
			p = p->prev;
		}
		p = b.head;
		x = b.intlength;
		while(p->value == 0 && x != 0) {
			removenode(&b, 0);
			p = p->next; 
			(b.intlength)--;
			x--;
		}
		return b;
	}
	else {
		number n;
		initnum(&n);
		makenode(&n, 1);
		a.head->value = 0;
		(a.intlength)++;
		return n;
	}	
}
number multiply(number a, number b, int base) {
	if((a.head->next == NULL && a.head->value == 0)) 
		return a;
	else if((b.head->next == NULL && b.head->value == 0))
		return b;
	int i = 0, j;
	number x, y;
	initnum(&y);
	makenode(&y, 1);
	y.head->value = 0;
	digit *p, *q;
	q = b.fractail;
	while(q) {
		p = a.fractail;
		initnum(&x);
		while(p) {
			makenode(&x, 0);
			x.head->value = p->value * q->value;
			(x.intlength)++;
			p = p->prev;
		}
		for(j = 0; j < i; j++) {
			makenode(&x, 1);
			x.fractail->value = 0;
			(x.intlength)++;
		}
		x.inttail = x.fractail;
		y = add(y, x, base);
		i++;
		q = q->prev;
	}
	j = a.fraclength + b.fraclength;
	y.fraclength = j;
	for(i = 0; i < j; i++) {
		y.inttail = y.inttail->prev;
		(y.intlength)--;
		if(y.inttail == y.head) {
			makenode(&y, 0);
			y.head->value = 0;
		}
	}
	p = y.head;
	while(p->value == 0) {
		removenode(&y, 0);
		(y.intlength)--;
		p = p->next;
		if(p == y.inttail)
			break;
	}
	return y;	
}
number divide(number a, number b, int base, int scale) {
	digit *p, *y;
	int i, sign = 0, k;
	number q, r, t, s, q1;
	initnum(&t);
	p = a.inttail->next;
	y = b.inttail->next;
	while(p != NULL || y != NULL) {
		if(p == NULL) {
			makenode(&a, 1);
			a.fractail->value = 0;
			p = a.fractail;
		}
		if(y == NULL) {
			makenode(&b, 1);
			b.fractail->value = 0;
			y = b.fractail;
		}
		(a.intlength)++;
		(b.intlength)++;
		p = p->next;
		y = y->next;
	}
	a.fraclength = 0;
	b.fraclength = 0;
	a.inttail = a.fractail;
	b.inttail = b.fractail;
	for(i = 0; i < scale; i++) {
		makenode(&a, 1);
		a.fractail->value = 0;
		a.intlength++;
	}
	a.inttail = a.fractail;
	copynum(&r, &a);
	initnum(&q);
	makenode(&q, 1);
	q.fractail->value = 0;
	q.inttail = q.fractail;
	p = a.head;
	k = a.intlength - 1;
	while(p->value == 0 && k != 0) {
		removenode(&a, 0);
		(a.intlength)--;
		p = p->next;
		k--;
	}
	copynum(&t, &a);	
	while(comparenum(r, b) >= 0) {
		initnum(&q1);
		initnum(&s);
		for(i = 0; i < (r.intlength - b.intlength + 1); i++) {
			makenode(&q1, 1);
			if(i == 0) {
				if(r.head->value < b.head->value) {
					q1.fractail->value = (base * r.head->value + r.head->next->value) / (b.head->value);
					i++;
				}
				else
					q1.fractail->value = (r.head->value) / (b.head->value);
			}
			else
				q1.fractail->value = 0;
			(q1.intlength)++;
		}
		q1.inttail = q1.fractail;
		if(sign == 0) {
			q = add(q, q1, base);
		}
		else {
			q = subtract(q, q1, base);
		}
		s = multiply(q, b, base);
		if(comparenum(t, s) >= 0) 
			sign = 0;
		else
			sign = 1;
		r = subtract(t, s, base);
		copynum(&t, &a);
	}
	p = q.inttail;
	for(i = 0; i < scale; i++) {
		if(p->prev == NULL) {
			makenode(&q, 0);
			q.head->value = 0;
		}
		if(q.intlength > 1)
			(q.intlength)--;
		(q.fraclength)++;
		p = p->prev;
	}
	q.inttail = p;
	return q;		
}

number Add(number a, number b, int base) {
	number c;
	int sign;
	if(a.sign == 0 && b.sign == 0) {
		sign = 0;
		c = add(a, b, base);
		c.sign = sign;
	}
	if(a.sign == 0 && b.sign == 1) {
		if(comparenum(a,b) >= 0)
			sign = 0;
		else
			sign = 1;
		c = subtract(a, b, base);
		c.sign = sign;
	}
	if(a.sign == 1 && b.sign == 0) {
		if(comparenum(a,b) > 0) 
			sign = 1;
		else
			sign = 0;
		c = subtract(a, b, base);
		c.sign = sign;
	}
	if(a.sign == 1 && b.sign == 1) {
		sign = 1;
		c = add(a, b, base);
		c.sign = sign;
	}
	return c;
}

number Subtract(number a, number b, int base) {	
	number c;
	int sign;
	if(a.sign == 0 && b.sign == 0) {
		if(comparenum(a, b) >= 0) {
			sign = 0;
		}
		else
			sign = 1;
		c = subtract(a, b, base);
		c.sign = sign;
	}
	if(a.sign == 0 && b.sign == 1) {
		sign = 0;
		c = add(a, b, base);
		c.sign = sign;
	}
	if(a.sign == 1 && b.sign == 0) {
		sign = 1;
		c = add(a, b, base);
		c.sign = sign;
	}					
	if(a.sign == 1 && b.sign == 1) {
		if(comparenum(a, b) > 0)
			sign = 1;
		else
			sign = 0;
		c = subtract(a, b, base);
		c.sign = sign;
	}
	return c;
}

number Multiply(number a, number b, int base) {
	number c;
	if(a.sign == 0 && b.sign == 0) {
		c = multiply(a, b, base);
		c.sign = 0;
	}
	if(a.sign == 0 && b.sign == 1) {
		c = multiply(a, b, base);
		c.sign = 1;
	}
	if(a.sign == 1 && b.sign == 0) {
		c = multiply(a, b, base);
		c.sign = 1;
	}
	if(a.sign == 1 && b.sign == 1) {
		c = multiply(a, b, base);
		c.sign = 0;
	}
	return c;
}

number Divide(number a, number b, int base, int scale) {
	number c;
	if(a.sign == 0 && b.sign == 0) {
		c = divide(a, b, base, scale);
		c.sign = 0;
	}
	if(a.sign == 0 && b.sign == 1) {
		c = divide(a, b, base, scale);
		c.sign = 1;
	}
	if(a.sign == 1 && b.sign == 0) {
		c = divide(a, b, base, scale);
		c.sign = 1;
	}
	if(a.sign == 1 && b.sign == 1) {
		c = divide(a, b, base, scale);
		c.sign = 0;
	}
	return c;
}
void printnum(number n) {
	digit *p;
	p = n.head;
	if(n.sign == 1)
		printf("%c", '-');
	while(p) {
		if(0 <= p->value && p->value <= 9)
			printf("%c", p->value + '0');
		else if(10 <= p->value && p->value <= 15)
			printf("%c", p->value - 10 + 'A');
		if(p == n.inttail && p->next != NULL) {
			printf("%c", '.');
		}
		p = p->next;
	}
}
		
number power(number x, number y, int base, int scale) {
	number t, a, b, c, d, e;
	int k = 0;
	initnum(&e);
	initnum(&t);
	initnum(&a);
	initnum(&d);
	copynum(&t, &x);
	makenode(&a, 1);
	a.head->value = 1;
	a.inttail = a.fractail;
	a.intlength = 1;
	makenode(&d, 1);
	d.head->value = 2;
	d.inttail = d.fractail;
	d.intlength = 1;
	if(y.head == y.fractail && y.head->value == 0) 
		return a;
	if(y.head == y.fractail && y.head->value == 1 && y.sign == 0)
		return x;
	if(y.head == y.fractail && y.head->value == 1 && y.sign == 1) 
		return Divide(a, t, base, scale);
		
	if(y.sign == 0) {
		while(1) {
			initnum(&b);
			initnum(&c);
			copynum(&b, &y);
			copynum(&c, &y);
			b = Divide(b, d, base, 0);
			b = Multiply(b, d, base);
			c = Subtract(c, b, base);
			copynum(&e, &t);
			if(c.head->value == 1) {
				a = Multiply(a, e, base);
				if(k == 1)
					break;
			}
			t = Multiply(t, e, base);
			y = Divide(y, d, base, 0);
			if(k == 1) 
				break;
			if(y.head == y.fractail && y.head->value < 2) {
				k = 1;
			}
		}
		return a;
	}
	while(1) {
		initnum(&b);
		initnum(&c);
		copynum(&b, &y);
		copynum(&c, &y);
		b = Divide(b, d, base, 0);
		b = Multiply(b, d, base);
		c = Subtract(c, b, base);
		copynum(&e, &t);
		if(c.head->value == 1) {
			a = Divide(a, e, base, scale);
			if(k == 1)
				break;	
		}
		t = Multiply(t, e, base);
		y = Divide(y, d, base, 0);
		if(k == 1)
			break;
		if(y.head == y.fractail && y.head->value < 2) 
			k = 1;
	}
	return a;
}
	
		
	
	







	




