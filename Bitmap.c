#include "pcd2bmp.h"
#define MARGIN 0.1f
#define DEFAULT_FOCAL_LENGTH 1.0f


int verboseLogging = 0;

void int2byte(int i, int numBytes, FILE* f) {
	int j;
	unsigned char *c = (unsigned char*)&i;
	for (j = 0; j < numBytes; j++) {
		fputc(c[j], f);
	}
}

Bitmap* NewBitmap(unsigned int width, unsigned int height, unsigned short bitsPerPixel) {
	Bitmap* bmp = (Bitmap*)malloc(sizeof(Bitmap));
	bmp->width = width;
	bmp->height = height;
	bmp->bitsPerPixel = bitsPerPixel;
	bmp->imageSize = bmp->width * bmp->height * bmp->bitsPerPixel / 8;
//	bmp->data = (char*)malloc(bmp->imageSize);
	return bmp;
}

void DeleteBitmap(Bitmap* bmp) {
	if (bmp) {
		if (bmp->data) free(bmp->data);
		free(bmp);
	}
}

void SaveBitmap(Bitmap* bmp, char* fileName) {
	if (!bmp) return;
	FILE* f = fopen(fileName, "wb");
	if (!f) {
		printf("File not found: %s\n", fileName);
	}
	else {
		int i;
		fwrite("BM", 1, 2, f);
		int2byte(54 + bmp->imageSize, 4, f);
		fwrite("\0\0\0\0", 1, 4, f);
		int2byte(54, 4, f);
		int2byte(40, 4, f);
		int2byte(bmp->width, 4, f);
		int2byte(bmp->height, 4, f);
		int2byte(1, 2, f);
		int2byte(bmp->bitsPerPixel, 2, f);
		fwrite("\0\0\0\0", 1, 4, f);
		int2byte(bmp->imageSize, 4, f);
		for (i = 0; i < 16; i++) fputc('\0', f);
		fwrite(bmp->data, 1, bmp->imageSize, f);
		fclose(f);
	}
}

int SetRGB(Bitmap* bmp, int x, int y, int r, int g, int b) {
	if (!bmp || bmp->bitsPerPixel != 24 || x < 0 || y < 0 || x >= bmp->width || y >= bmp->height) return 0;
	int offset = (x + (bmp->height - 1 - y)*bmp->width) * 3;
	bmp->data[offset] = (unsigned char)(b);
	bmp->data[offset + 1] = (unsigned char)(g);
	bmp->data[offset + 2] = (unsigned char)(r);
	return 1;
}

