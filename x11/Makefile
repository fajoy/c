all:clean x11shot x11shot2 x11benchmark x11benchmark2
x11shot:
	gcc -Wall  x11shot.c -o x11shot -lX11 -lpng
x11benchmark:
	gcc -Wall  x11benchmark.c -o x11benchmark -lX11
x11shot2:
	gcc -Wall  x11shot2.c -o x11shot2 -lX11 -lXext -lpng
x11benchmark2:
	gcc -Wall  x11benchmark2.c -o x11benchmark2 -lX11 -lXext
clean:
	rm x11shot x11benchmark x11shot2 x11benchmark2
