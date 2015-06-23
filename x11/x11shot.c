#include <stdio.h>
#include <X11/X.h>
#include <X11/cursorfont.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <sys/shm.h>
#include <sys/time.h>
#include<signal.h>
#include<unistd.h>
/* png */
#include <png.h>

//ref http://www.codemadness.nl/downloads/xscreenshot.c
static void
die(const char *s) {
	fprintf(stderr, "xscreenshot: %s\n", s);
	exit(EXIT_FAILURE);
}

/* LSBFirst: BGRA -> RGBA */
static void
convertrow_lsb(unsigned char *drow, unsigned char *srow, XImage *img) {
	int sx, dx;

	for(sx = 0, dx = 0; dx < img->bytes_per_line - 4; sx += 4) {
		drow[dx++] = srow[sx + 2]; /* B -> R */
		drow[dx++] = srow[sx + 1]; /* G -> G */
		drow[dx++] = srow[sx];     /* R -> B */
		if(img->depth == 32)
			drow[dx++] = srow[sx + 3]; /* A -> A */
		else
			drow[dx++] = 255;
	}
}

/* MSBFirst: ARGB -> RGBA */
static void
convertrow_msb(unsigned char *drow, unsigned char *srow, XImage *img) {
	int sx, dx;

	for(sx = 0, dx = 0; dx < img->bytes_per_line - 4; sx += 4) {
		drow[dx++] = srow[sx + 1]; /* G -> R */
		drow[dx++] = srow[sx + 2]; /* B -> G */
		drow[dx++] = srow[sx + 3]; /* A -> B */
		if(img->depth == 32)
			drow[dx++] = srow[sx]; /* R -> A */
		else
			drow[dx++] = 255;
	}
}

	static void
pngstdout(XImage *img)
{
	png_structp png_struct_p;
	png_infop png_info_p;
	void (*convert)(unsigned char *, unsigned char *, XImage *);
	unsigned char *drow = NULL, *srow;
	int h;

	png_struct_p = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
			NULL);
	png_info_p = png_create_info_struct(png_struct_p);

	if(!png_struct_p || !png_info_p || setjmp(png_jmpbuf(png_struct_p)))
		die("failed to initialize libpng");

	png_init_io(png_struct_p, stdout);
	png_set_IHDR(png_struct_p, png_info_p, img->width, img->height, 8,
			PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_struct_p, png_info_p);

	srow = (unsigned char *)img->data;
	drow = calloc(1, img->width * 4); /* output RGBA */
	if(!drow)
		die("Can't calloc");

	if(img->byte_order == LSBFirst)
		convert = convertrow_lsb;
	else
		convert = convertrow_msb;

	for(h = 0; h < img->height; h++) {
		convert(drow, srow, img);
		srow += img->bytes_per_line;
		png_write_row(png_struct_p, drow);
	}
	png_write_end(png_struct_p, NULL);
	free(drow);
	png_free_data(png_struct_p, png_info_p, PNG_FREE_ALL, -1);
	png_destroy_write_struct(&png_struct_p, NULL);
}

// gcc -Wall  x11shot.c -o x11shot -lX11 -lpng;./x11shot <wid> >/tmp/screenshot.png
int main(int argc,char* argv[]){
	if(argc<2){
		fprintf(stderr, "usage:x11shot <wid> >/tmp/screenshot.png\n");
		exit(1);
	}
	int wid=(int)strtol(argv[1], NULL, 0);
	sscanf(argv[1], "%x", &wid);
	Display *dpy=XOpenDisplay(":0");
	XImage *image;
	XWindowAttributes wattr;

	if (dpy == NULL) {
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	}
	XGetWindowAttributes(dpy,wid,&wattr);
	fprintf(stderr,"wid:0x%x wattr x:%d y:%d w:%d h:%d begin capture\n"
			,wid
			,wattr.x,wattr.y
			,wattr.width,wattr.height);
	image = XGetImage(dpy,
			wid
			,0,0,
			wattr.width,wattr.height,
			AllPlanes, ZPixmap);
	if (image == NULL)
	{
		die("Can't get image");
	}
	pngstdout(image);
	return 0;
};
