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

long long now(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
	long long ms= (tv.tv_sec*1000LL)+(tv.tv_usec/1000);
	return ms;
}

int isOver=0;
void sig_handler(int signo)
{
	fprintf(stderr,"stop,received sigin %d\n",signo);
	isOver=1;
}


// gcc -Wall  x11benchmark.c -o x11benchmark -lX11;./x11benchmark <wid>
int main(int argc,char* argv[]){
	if(argc<2){
		fprintf(stderr, "usage:x11benchmark <wid> \n");
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
	signal(SIGINT, sig_handler);
	signal(SIGKILL, sig_handler);
	signal(SIGTERM, sig_handler);
	signal(SIGQUIT, sig_handler);

	long long startTs= now();
	long long c=0;
	while(isOver==0){
		image = XGetImage(dpy,
				wid
				,0,0,
				wattr.width,wattr.height,
				AllPlanes, ZPixmap);
		if (image == NULL)
		{
			break;
		}
		XDestroyImage(image);
		c++;
	}

	fprintf(stderr,"wid:0x%x ms:%llu capture:%llu\n",wid,(long long)(now()-startTs),c);
	return 0;
};
