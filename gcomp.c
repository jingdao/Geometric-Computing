#include "Queue.h"
#include "SDL_image.h"
#include "Profiler.h"
#include "Pixmap.h"
#include "Bitmap.h"
#define FRAME_WIDTH 500
#define FRAME_HEIGHT 500
#define USE_BITFIELD 1

Pixmap* PixmapFromSdlSurface(SDL_Surface* sf) {
	Pixmap* p = InitPixmap(sf->w,sf->h,1,8*sf->pitch/sf->w);
//	memcpy(p->data,sf->pixels,sf->pitch*sf->h);
	free(p->data);
	p->data=sf->pixels;
	return p;
}

SDL_Surface* SdlSurfaceFromPixmap(Pixmap* px) {
	SDL_Surface* res = SDL_CreateRGBSurface(0,px->width,px->height,px->bitsPerPixel,0,0,0,0);
	res->pixels=px->data;
	return res;
}

void SavePixmap(Pixmap* px, char* name) {
	Bitmap* b =NewBitmap(px->width,px->height,px->bitsPerPixel); 
	b->data=(char*)px->data;
	SaveBitmap(b,name);
	b->data=NULL;
	DeleteBitmap(b);
}

void main() {
//	char c[]="Hello World";
//	Queue* q = InitQueue();
	int i=0;
//	for (i=0;i<5;i++) {QueuePush(q,&(c[i]));}
//	for (i=0;i<5;i++) printf("%c",*(char*)QueuePop(q));
//	for (i=5;i<11;i++) {QueuePush(q,&(c[i]));}
//	for (i=0;i<5;i++) printf("%c",*(char*)QueuePop(q));
//
//	printf("%d\n",q->size);
//	DeleteQueue(q);

	IMG_Init(0);
	SDL_Surface* sf = IMG_Load("1.jpg");
	IMG_Quit();

	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen = SDL_SetVideoMode( FRAME_WIDTH*2, FRAME_HEIGHT, 24, SDL_SWSURFACE );
	SDL_Rect r = {0,0,FRAME_WIDTH,FRAME_HEIGHT};
	SDL_Event event;
	char buf[256];
	Pixmap* px = PixmapFromSdlSurface(sf);
#if USE_BITFIELD
	printf("Using bitfield optimization ...\n");
	Pixmap* px2 = PixmapThresholding(px,0.2);
#else
	Pixmap* px2 = PixmapThresholdingSimple(px,0.2);
	SavePixmap(px2,"1.bmp");
#endif
	Pixmap* dup = PixmapCopy(px2);
	Timer* timer = TimerStart();
	for (i=0;i<1;i++) {
#if USE_BITFIELD
		PixmapErosion(px2,1);
#else
		PixmapErosionSimple(px2,1);
#endif
	}
	SavePixmap(px2,"2.bmp");
	printf("time taken: %fs\n",TimerEnd(timer));
//	SDL_BlitSurface(sf,&r,screen,NULL);
	SDL_Surface* res = SdlSurfaceFromPixmap(dup);
	SDL_BlitSurface(res,&r,screen,NULL);
	r.x=FRAME_WIDTH;
	res = SdlSurfaceFromPixmap(px2);
	SDL_BlitSurface(res,NULL,screen,&r);
	SDL_Flip(screen);

	while (1) {
		if (SDL_PollEvent(&event)) {
			if (event.type==SDL_QUIT) break;
		}
	}

	printf("Memory used: %s\n",GetMemoryRepr(buf,MemoryInfo()));
	DeletePixmap(px);
	DeletePixmap(px2);
//	SDL_FreeSurface(res);
//	SDL_FreeSurface(sf);
//	SDL_FreeSurface(screen);
	SDL_Quit();
	printf("Memory used: %s\n",GetMemoryRepr(buf,MemoryInfo()));


}
