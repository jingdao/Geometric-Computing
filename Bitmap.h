#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    unsigned int bmpSize;
    int width;
    int height;
    unsigned short bitsPerPixel;
    unsigned int imageSize;
    char* data;
} Bitmap;

Bitmap* NewBitmap(unsigned int width, unsigned int height, unsigned short bitsPerPixel);
void DeleteBitmap(Bitmap* bmp);
void SaveBitmap(Bitmap* bmp, char* fileName);
