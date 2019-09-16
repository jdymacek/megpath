#ifndef PNGIMAGE__H
#define PNGIMAGE__H

#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <string.h>

typedef struct Image{
	int width;
	int height;
	int channels;
	unsigned char* data;
} Image;

Image* createImage(int width,int height);
Image* destroyImage(Image* img);

Image* readPng(const char* filename);
void writePng(const char* filename,Image* image);

#endif
