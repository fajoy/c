#include <stdio.h>
#include <X11/X.h>
#include <X11/cursorfont.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xfixes.h>
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


// gcc -Wall  x11benchmark2.c -o x11benchmark2 -lX11 -lXext;./x11benchmark2 <wid>
int main(int argc,char* argv[]){
	if(argc<2){
		fprintf(stderr, "usage:x11benchmark2 <wid> \n");
		exit(1);
	}
	int wid=(int)strtol(argv[1], NULL, 0);
	sscanf(argv[1], "%x", &wid);
	Display *dpy=XOpenDisplay(getenv("DISPLAY"));
	XImage *image;
	XWindowAttributes wattr;
	XShmSegmentInfo shminfo;
	int x_off=0,y_off=0;

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

	if(!XShmQueryExtension(dpy)){
		fprintf(stderr, "can't not use shm!\n");
		exit(1);
	}
	int scr = XDefaultScreen(dpy);
	image = XShmCreateImage(dpy,
			DefaultVisual(dpy, scr),
			DefaultDepth(dpy, scr),
			ZPixmap,
			NULL,
			&shminfo,
			wattr.width,wattr.height);
	shminfo.shmid = shmget(IPC_PRIVATE,
			image->bytes_per_line * image->height,
			IPC_CREAT|0777);
	shminfo.shmaddr = image->data = shmat(shminfo.shmid, 0, 0);
	shminfo.readOnly = False;
	if (!XShmAttach(dpy, &shminfo)) {
		fprintf(stderr, "Fatal: Failed to attach shared memory!\n");
		exit(1);
	}


	long long startTs= now();
	long long c=0;
	while(isOver==0){
		if(!XShmGetImage(dpy,wid,image,x_off,y_off,AllPlanes))
		{
			break;
		}
		c++;
	}
	long long interval=(long long)(now()-startTs);
	fprintf(stderr,"wid:0x%x ms:%llu capture:%llu fps:%0.2f\n",wid,interval,c,c*1000.0/interval);
	return 0;
};
