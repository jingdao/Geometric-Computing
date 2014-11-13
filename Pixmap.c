#include "Pixmap.h"


Pixmap* InitPixmap(int width,int height,int depth,int bitsPerPixel) {
	if ((width*bitsPerPixel)%8!=0) return NULL;
	Pixmap* px = (Pixmap*) malloc(sizeof(Pixmap));
	if (!px) return NULL;
	px->width=width;
	px->height=height;
	px->depth=depth;
	px->bitsPerPixel=bitsPerPixel;
	px->data=malloc(width*height*depth*bitsPerPixel/8); 
	if (!px->data) {
		free(px);
		return NULL;
	}
	return px;
}

void DeletePixmap(Pixmap* px) {
	if (px) {
		if (px->data) free(px->data);
		free(px);
		}
}

Pixmap* PixmapCopy(Pixmap* px) {
	if (!px) return NULL;
	Pixmap* res = InitPixmap(px->width,px->height,px->depth,px->bitsPerPixel);
	memcpy(res->data,px->data,px->width*px->height*px->depth*px->bitsPerPixel/8+1);
	return res;
} 

Pixmap* PixmapThresholding(Pixmap* px,float cutoff) {
	if (!px) return NULL;
	Pixmap* res = InitPixmap(px->width,px->height,px->depth,1);
	int i,j,threshold,bitsPerColor,v;
	char r,g,b;
	char* src=(char*)px->data;
	char* dest=(char*)res->data;
	memset(dest,0,px->width*px->height*px->depth/8);
	if (px->bitsPerPixel==24) {
		bitsPerColor=8;
		threshold=(int)((1<<bitsPerColor)*3*cutoff);
		for (j=0;j<px->height;j++) {
			for (i=0;i<px->width;i++) {
				b=*(src++);
				g=*(src++);
				r=*(src++);
				v=r+g+b;
				if (v<=threshold) { 
					(*dest)|=1;
				}
				if ((i&7)==7)
					dest++;
				else
					*dest=(*dest)<<1;
			}
		}
	}
	return res;
}

Pixmap* PixmapThresholdingSimple(Pixmap* px,float cutoff) {
	if (!px) return NULL;
	Pixmap* res = InitPixmap(px->width,px->height,px->depth,24);
	int i,j,threshold,bitsPerColor,v,tt=0;
	char r,g,b;
	char* src=(char*)px->data;
	char* dest=(char*)res->data;
	if (px->bitsPerPixel==24) {
		bitsPerColor=8;
		threshold=(int)((1<<bitsPerColor)*3*cutoff);
//		printf("threshold: %d\n",threshold);
		for (j=0;j<px->height;j++) {
			for (i=0;i<px->width;i++) {
				b=*(src++);
				g=*(src++);
				r=*(src++);
				v=r+g+b;
				if (v<=threshold) { 
					*dest=0xff; dest++;
					*dest=0xff; dest++;
					*dest=0xff;
//					tt++;
				}
				else {	
					*dest=0x00; dest++;
					*dest=0x00; dest++;
					*dest=0x00;
				}
				dest++;
			}
		}
//		printf("tt: %d\n",tt);
	} else {
		threshold=1<<px->bitsPerPixel;
	}
	return res;
}

void PixmapErosion(Pixmap* px,int connectivity) {
	int* oldPixels = (int*)px->data;
	px->data = malloc(px->width*px->height*px->depth/8);
	int i,j,pitch=px->width/sizeof(int)/8;
	int* top=oldPixels;
	int* middle=top+pitch;
	int *left,*right;
	int* bottom=middle+pitch;
	int* dest=(int*)px->data;
	memset(dest,0,pitch*sizeof(int));
	dest+=pitch;
	for (j=1;j<px->height-1;j++) {
		right=middle+1;
		*dest=(*top)&(*bottom)&(*middle)&(*middle>>1)&((*middle<<1)|(*right>>(sizeof(int)-1)));
		left=middle;
		middle=right;
		right++;
		dest++;
		for (i=1;i<pitch-1;i++) {
			*dest=(*top)&(*bottom)&(*middle)&((*middle>>1)|(*left<<(sizeof(int)-1)))
				&((*middle<<1)|(*right>>(sizeof(int)-1)));
			top++;
			bottom++;
			left=middle;
			middle=right;
			right++;
		dest++;
		}
		*dest=(*top)&(*bottom)&(*middle)&((*middle>>1)|(*left<<(sizeof(int)-1)))&(*middle<<1);
		top++;
		bottom++;
		middle++;
		dest++;
	}
	memset(dest,0,pitch*sizeof(int));
	free(oldPixels);
}


void PixmapErosionSimple(Pixmap* px,int connectivity) {
	char* oldPixels = (char*)px->data;
	px->data = malloc(px->width*px->height*px->depth*px->bitsPerPixel/8);
	return;
	int i,j,pitch=3*px->width;
	char* src=oldPixels;
	char* dest=(char*)px->data;
	if (connectivity==0) {
		for (j=0;j<px->height;j++) {
			if (j==0||j==px->height-1) {
				*dest=0x00;dest++;*dest=0x00;dest++;*dest=0x00;dest++;
				src+=pitch;
			} else {
				for (i=0;i<px->width;i++) {
					if (i==0||i==px->width-1) {
						*dest=0x00;dest++;*dest=0x00;dest++;*dest=0x00;dest++;
					} else {
						if (*src&&*(src-3)&&*(src+3)&&*(src-pitch)&&*(src+pitch)) {
							*dest=0xff;dest++;*dest=0xff;dest++;*dest=0xff;dest++;
						} else {
							*dest=0x00;dest++;*dest=0x00;dest++;*dest=0x00;dest++;	
						}
					}
					src+=3;
				}
			}
		}
	} else {
		for (j=0;j<px->height;j++) {
			if (j==0||j==px->height-1) {
				*dest=0x00;dest++;*dest=0x00;dest++;*dest=0x00;dest++;
				src+=pitch;
			} else {
				for (i=0;i<px->width;i++) {
					if (i==0||i==px->width-1) {
						*dest=0x00;dest++;*dest=0x00;dest++;*dest=0x00;dest++;
					} else {
						if (*src&&*(src-3)&&*(src+3)&&*(src-pitch)&&*(src+pitch)
							&&*(src-pitch-3)&&*(src-pitch+3)&&*(src+pitch-3)&&*(src+pitch+3)) {
							*dest=0xff;dest++;*dest=0xff;dest++;*dest=0xff;dest++;
						} else {
							*dest=0x00;dest++;*dest=0x00;dest++;*dest=0x00;dest++;	
						}
					}
					src+=3;
				}
			}
		}
	}
	free(oldPixels);
}
