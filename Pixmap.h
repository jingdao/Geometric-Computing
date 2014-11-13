#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int width;
	int height;
	int depth;
	int bitsPerPixel;
	void* data;
} Pixmap;

Pixmap* InitPixmap(int width,int height,int depth,int bitsPerPixel);
void DeletePixmap(Pixmap* px);
Pixmap* PixmapCopy(Pixmap* px);
Pixmap* PixmapThresholding(Pixmap* px,float cutoff);
Pixmap* PixmapThresholdingSimple(Pixmap* px,float cutoff); 
void PixmapErosion(Pixmap* px, int connectivity);
void PixmapErosionSimple(Pixmap* px, int connectivity);
