project: calculator6.o numstack.o charstack.o charqueue.o number.o
	cc calculator6.o numstack.o charstack.o charqueue.o number.o -o project -lm
calculator6.o: calculator6.c 
	cc calculator6.c -c -Wall
numstack.o: numstack.c numstack.h
	cc numstack.c -c -Wall
charstack.o: charstack.c charstack.h
	cc charstack.c -c -Wall
charqueue.o: charqueue.c  charqueue.h
	cc charqueue.c -c -Wall
number.o: number.c number.h
	cc number.c -c -Wall
