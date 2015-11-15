/*Copyright (C) 2015  Rohan Patil

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.*/








#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "numstack.h"
#include "charstack.h"
#include <math.h>

#define OPERAND 	1
#define OPERATOR 	2
#define INVALID		3
#define BRACKET		4
#define FUNCTION	5
#define SIZE		128

struct variable v;

number answer;
int base = 10;
int scale = 10;
void filehandling(char *inputfile);
char *stringreplace(char *t, char *o, char *n);
char *newstring(char *str);
int precedance(char c);
number operation(number x, number y, char c);	
number infix(char *string);
double function(double d, char c);
void varassignment(struct variable *v, char *str);

char *stringreplace(char *t, char *o, char *n) {                      //replaces one string with given string
	int i, j, k;
	char *s = o;
	char *p = n;
	char *l;
	char * text;
	text = t;
	while(*t != '\0') {
		if(*t == *o) {
			t++;
			l = t;
			o++;
		}
		else {
			if(o == s) {
				t++;
				l = t;
			}
			else {
				o = s;
			}
		}
		if(*o == '\0') {
			t = t - (o - s);
			o = s;
			while(1) {
				*t = *n;
				t++;
				n++;
				if(*n == '\0') {
					break;
				}
				if( t == l) {
					i = 0;
					while(l[i] != '\0') {
						i = i + 1;
					}
					while(i != -1) {
						l[i+1] = l[i];
						i--;
					}
				l = l + 1;
				}
			}
			if(t != l && *n == '\0') {
				j = l - t;
				i = 0;
				while(l[i] != '\0') {
					i = i + 1;
				}
				for(k = 0; k <= i; k++) {
					l[k - j] = l[k]; 
				}
			}
		}
		n = p;
	}
	return text;
}

char *newstring(char *str) {					// used for execution of functions
	char *s;
	s = stringreplace(str, "sin(", "(s");
	s = stringreplace(s, "cos(", "(c");
	s = stringreplace(s, "tan(", "(t");
	s = stringreplace(s, "cot(", "(o");
	s = stringreplace(s, "sec(", "(y");
	s = stringreplace(s, "cosec(", "(x");
	s = stringreplace(s, "e", "2.71828");
	s = stringreplace(s, "ln(", "(l");
	s = stringreplace(s, "log(", "(g");
	return s;
}

int i = -1;
int f = -1;

struct variable {
	char varname[128][128];
	char varvalue[128][1000];
}; 				

void varassignment(struct variable *v, char *str) {                      //for assignment of variables on commandline
	int j = 0, k = 0, check = 0, x;
	int y = 0;
	while(str[j] != '\0') {
		if(str[j] == '	' || str[j] == ' ') {
			j++;
		}
		else if(str[j] == '\0')
			break;
		else if((('0' <= str[j] && str[j] <= '9') || str[j] == '.') || (str[j] == 'a' && str[j + 1] == 'n' && str[j + 2] == 's') || 			(str[j] == '-')) {
			if(check == 0) 
				break;
			i++;
			k = 0;
			if(str[j] == 'a' && str[j + 1] == 'n' && str[j + 2] == 's') {
				digit *p;
				p = answer.head;
				if(answer.sign == 1) {
					v->varvalue[i][k] = '-';
					k++;
				}
				while(p) {
					if(0 <= p->value && p->value <= 9) {
						v->varvalue[i][k] = p->value + '0';
						k++;
					}
					else if(10 <= p->value && p->value <= 15) {
						v->varvalue[i][k] = p->value - 10 + 'A';
						k++;
					}
					if(p == answer.inttail) {
						v->varvalue[i][k] = '.';
						k++;
					}
					p = p->next;
				}
				v->varvalue[i][k] = '\0';
				j = j + 3;
				check = 0;
				for(k = 0; k < f; k++) {
					x = strcmp(v->varname[k], v->varname[f]);
					if(x == 0) {
						strcpy(v->varvalue[k], v->varvalue[f]);
						f--;
						i--;
						break;
					}
				}	
			}
			else {
				while(str[j] != '\0' && str[j] != ' ' && str[j] != '	') {
					v->varvalue[i][k] = str[j];
					k++;
					j++;
				}
				v->varvalue[i][k] = '\0';
				check = 0;
				for(k = 0; k < f; k++) {
					x = strcmp(v->varname[k], v->varname[f]);
					if(x == 0) {
						strcpy(v->varvalue[k], v->varvalue[f]);
						f--;
						i--;
						break;
					}
				}
				if(y == 1) {
					y = 0;
					k = 0;
					while(v->varvalue[i][k] != '\0') {
						y = y * 10 + v->varvalue[i][k] - '0';
						k++;
					}
					base = y;
					y = 0;
					i--;
				}
				if(y == 2) {
					y = 0;
					k = 0;
					while(v->varvalue[i][k] != '\0') {
						y = y * 10 + v->varvalue[i][k] - '0';
						k++;
					}
					scale  = y;
					y = 0;
					i--;
				}		
				if(str[j] == '\0')
					break;
				j++;
			}
		}
		else if(str[j] == '=') {
			check = 1;
			j++;
		}
		else if((64 < str[j] && str[j] < 91) || (96 < str[j] && str[j] < 123)) {
			if(str[j] == 'b' && str[j + 1] == 'a' && str[j + 2] == 's' && str[j + 3] == 'e') {
				y = 1;
				j = j + 4;
				continue;
			}
			else if(str[j] == 's' && str[j + 1] == 'c' && str[j + 2] == 'a' && str[j + 3] == 'l' && str[j + 4] == 'e') {
				y = 2;
				j = j + 5;
			}
			else {			
				f++;
				k = 0;	
				while(str[j] != '\0' && str[j] != ' ' && str[j] != '	') {
					if(str[j] == '=') {
						j--;
						break;
					}
					v->varname[f][k] = str[j];
					k++;
					j++;
				}
				v->varname[f][k] = '\0';
				j++;
			}
		}
	
	}
}	
typedef struct token {
	int type; 
	union data {
		number num;
		char operator;
		char bracket;
		char funct;
	}d;
}token;
token *getnext(char *string, int *reset);

enum states {SPC, NUM, OPR, END, ERROR, BRAC, DOT, FUNCT};

token *getnext(char *string, int *reset) {				//converts a given string into tokens and returns the tokens
	static int currstate, nextstate;
	static int i;
	int done = 0;
	static charqueue q;
	token *p = (token *)malloc(sizeof(token));
	if(p == NULL)
		return NULL;
	if(*reset == 1) {
		*reset = 0;
		currstate = SPC;
		i = 0;
	}
	while(1) {
		switch(string[i]) {
			case '1': case'2': case '3': case '4':
			case '5': case'6': case '7': case '8':
			case '9': case'0': case'A': case'B': case'C':
			case 'D': case'E': case'F': 
				nextstate = NUM;
				break;
			case '+': case '-': case '*': case '/':
			case '^':
				nextstate = OPR;
				break;
			case '\0':
				nextstate = END;
				break;
			case ' ': case '	':
				nextstate = SPC;
				break;
			case '(': case ')':
				nextstate = BRAC;
				break;
			case '.':
				nextstate = DOT;
				break;
			case 's': case 'c': case 't': case 'o': case 'x': case 'y': case 'l': case 'g':
				nextstate = FUNCT;
				break;
			default:
				nextstate = ERROR;
				break;
		}
		switch(currstate) {
			case SPC:
				switch(nextstate){
					case SPC:
						break;
					case NUM:
						qinit(&q);
						enqueue(&q, string[i]);	
						break;
					case OPR:
						break;
					case END:
						break;
					case ERROR:
						break;
					case BRAC:
						break;
					case DOT:
						break;
					case FUNCT:
						break;
					default:
						break;
				}
				break;
			case NUM:
				switch(nextstate){
					case SPC:
						p->type = OPERAND;
						p->d.num = makenumchqueue(&q);
						done = 1;
						break;
					case NUM:
						enqueue(&q, string[i]);
						break;
					case OPR:
						p->type = OPERAND;
						p->d.num = makenumchqueue(&q);
						done = 1;
						break;
					case END:
						p->type = OPERAND;
						p->d.num = makenumchqueue(&q);
						done = 1;
						break;
					case ERROR:
						p->type = OPERAND;
						p->d.num = makenumchqueue(&q);
						done = 1;
						break;
					case BRAC:
						p->type = OPERAND;
						p->d.num = makenumchqueue(&q);
						done = 1;
						break;
					case DOT:
						enqueue(&q, string[i]);
						break;	
					case FUNCT:
						p->type = OPERAND;
						p->d.num = makenumchqueue(&q);
						done = 1;
						break;
					default:
						break;
				}
				break;
			case OPR:
				switch(nextstate){
					case SPC:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case NUM:
						qinit(&q);
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						enqueue(&q, string[i]);
						done = 1;
						break;
					case OPR:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case END:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case ERROR:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case BRAC:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case DOT:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case FUNCT:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					default:
						break;
				}
				break;	
			case BRAC:
				switch(nextstate){
					case SPC:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case NUM:
						qinit(&q);
						enqueue(&q, string[i]);
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case OPR:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case END:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case ERROR:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case BRAC:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case DOT:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					case FUNCT:
						p->type = BRACKET;
						p->d.bracket = string[i - 1];
						done = 1;
						break;
					default:
						break;
				}
				break;
			case DOT:
				switch(nextstate){
					case SPC:
						break;
					case NUM:
						enqueue(&q, string[i]);
						break;
					case OPR:
						break;
					case END:
						break;
					case ERROR:
						break;
					case BRAC:
						break;
					case DOT:
						break;
					case FUNCT:
						break;
					default:
						break;
				}
				break;
			case FUNCT:
				switch(nextstate){
					case SPC:
						p->type = FUNCTION;
						p->d.funct = string[i-1];
						done = 1;
						break;
					case NUM:
						qinit(&q);
						enqueue(&q, string[i]);
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case OPR:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case END:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case ERROR:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case BRAC:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case DOT:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					case FUNCT:
						p->type = FUNCTION;
						p->d.funct = string[i - 1];
						done = 1;
						break;
					default:
						break;
				}
				break;
			case END:
				nextstate = END;
				done = 1;
				p = NULL;
				break;
			case ERROR:
				nextstate = ERROR;
				p->type = INVALID;
				done = 1;
				break;
			default:
				break;
		}
		currstate = nextstate;
		i++;
		if(done == 1) {
			return p;
		}
	}
	return NULL;	
}

int precedance(char c) {				//checks the precedance of various operations
	if(c == '(' || c == ')') 
		return 30;
	if(c == 's' || c == 'c' || c == 't' || c == 'o' || c == 'x' || c == 'y' || c == 'l' || c == 'g')
		return 50;
	else if(c == '^')
		return 300;
	else if(c == '*' || c == '/') 
		return 200;
	else
		return 100;
}
number operation(number x, number y, char c) {		//performs arithmatic operations
	if(c == '*') 
		return Multiply(x, y, base);
	else if(c == '/') 
		return Divide(x, y, base, scale);
	else if(c == '+') 
		return Add(x, y, base);
	else if(c == '-') 
		return Subtract(x, y, base);
	else if(c == '^');
		return power(x, y, base, scale);
	return x;
}	
number infix(char *string) {				//evaluates the infix expression 
	int reset = 1, m = 0, n = 0, x, y, check = 0;
	number w, z;
	char a, b;
	double d;
	int state = OPR;
	token *p;
	numstack numstack;
	charstack charstack;
	init(&numstack);
	initc(&charstack);
	while((p = getnext(string, &reset)) && !full(&numstack) && !fullc(&charstack)) {
		if(p->type == OPERAND) {
			if(check == 1) {
				w = p->d.num;
				if(w.sign == 1) 
					w.sign = 0;
				if(w.sign == 0)
					w.sign = 1;
				push(&numstack, w);
				check = 0;
			}
			else {
				push(&numstack, p->d.num);
				m = m + 1;
			}
			state = NUM;	
		}
		if(p->type == OPERATOR)	{
			n = n + 1;
			if(emptyc(&charstack) && state != OPR) {
				pushc(&charstack, p->d.operator);
			}
			else if(state == OPR || state == BRAC || state == FUNCT) {
				if( p->d.operator == '-')
					check++;
			}
			else {
				a = popc(&charstack);
				b = p->d.operator;
				if(precedance(b) > precedance(a)) {
					pushc(&charstack, a);
					pushc(&charstack, b);
				}
				else {
					w = pop(&numstack);
					z = pop(&numstack);
					w = operation(z, w, a);
					push(&numstack, w);
					pushc(&charstack, b);
				}
			}
			state = OPR;		
		}
		
		if(p->type == FUNCTION) {
			pushc(&charstack, p->d.funct);
			state = FUNCT;
		}
		if(p->type == BRACKET) {
			if(p->d.bracket == '(') {
				pushc(&charstack, p->d.bracket); 
				x = x + 1;
				state = BRAC;
			}
			else if(p->d.bracket == ')') {
				a = popc(&charstack);
				b = p->d.bracket;
				y = y + 1;
				while(precedance(a) != precedance(b)) {
					if(a == 's' || a == 'c' || a == 't' || a == 'o' || a == 'x' || a == 'y' || a == 'l' || a == 'g') {
						w = pop(&numstack);
						d = makedoublenum(w);
						d = function(d, a);
						w = makenumdouble(d);	
					}
					else {
						w = pop(&numstack);
						z = pop(&numstack);
						w = operation(z, w, a);
					}
					push(&numstack, w);
					a = popc(&charstack);
				}
			}		
		}
		
	}
	while(!(emptyc(&charstack))) {
		w = pop(&numstack);
		z = pop(&numstack);
		a = popc(&charstack);
		w = operation(z, w, a);
		push(&numstack, w);
	}
	return pop(&numstack);
}

double function(double d, char c) {
	if(c == 's')
		return sin(d);
	if(c == 'c')
		return cos(d);
	if(c == 't')
		return tan(d);
	if(c == 'o')
		return 1 / tan(d);
	if(c == 'y')
		return 1 / cos(d);
	if(c == 'x') 
		return 1 / sin(d);
	if(c == 'l');
		return log(d);
	if(c == 'g');
		return log10(d);
}

int main(int argc, char *argv[]) {
	char str[10000];
	char *s, c;
	int x, j;
	if(argc == 2) {
		printf("\n\n");
		filehandling(argv[1]);
		printf("\n\n");
		return 0;
	}
	while(1) {
		j = 0;
		while((c = getchar()) != '\n') {
			str[j] = c;
			j++;
		}
		str[j] = '\0';
		s = str;
		if(str[0] == 'q')
			break;
		if(str[0] == 'a') {	
			j = 0;
			while((c = getchar()) != '\n') {
				str[j] = c;
				j++;
			}
			str[j] = '\0';
			varassignment(&v, str);	
			continue;
		}
		if(i != -1) {
			s = stringreplace(str, (&v)->varname[0], (&v)->varvalue[0]);
			for(x = 1; x <= i; x++) 
				s = stringreplace(s, (&v)->varname[x], (&v)->varvalue[x]);
		}
		s = newstring(s);	
		answer = infix(s);
		printnum(answer);
		printf("\n");
		printf("\n");
	}
	return 0;
}

void filehandling(char *inputfile) {			//takes infix expression from a file
	int ip, x = 1, check = 0, j;
	char c[10000];
	char *s;
	ip = open(inputfile, O_RDONLY | O_CREAT); 
	int k;
	s = c;
		while(1) {
			k = 0;
			if(x == 0)
				break;
			x = read(ip, &c[k], sizeof(char));
			if(x == -1) {
				printf("error\n");
			}
			if(x == 0) {
				c[k] = '\0';
				break;
			}
			while(1) {
				k++;
				x = read(ip, &c[k], sizeof(char));
				if(c[k] == '\n'|| x == 0) {
					c[k] = '\0';
					break;
				}
			}
			if(x == 0)
				break;
			
			if(check == 0 && s[0] != 'a' && s[1] != '\0') {
				if(i != -1) {
					s = stringreplace(s, (&v)->varname[0], (&v)->varvalue[0]);
					for(j = 1; j <= i; j++) {
						s = stringreplace(s, (&v)->varname[j], (&v)->varvalue[j]);
					}
				}
			}
			if(check == 1) {
				varassignment(&v, s);
				check = 0;
				continue;
			}
		        if(s[0] == 'a' && s[1] == '\0') {
				check = 1;
				continue;
			}
			s = newstring(c);
			answer = infix(s);
			printnum(answer);
			printf("\n");
		}
	close(ip);
}
				


		
		
	
	
	
















