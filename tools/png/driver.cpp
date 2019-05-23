#include <iostream>
#include "../../shared/PNG.h"

using namespace std;

int main(int argc, char* argv[]){

	if(argc < 2){
		cout << "No images loaded" << endl;
		return 0;
	}

	int count = argc-1;
	int max_size = 0;
	for(int i=1; i<argc; i++){
		Image* siz = readPng(argv[i]);
		if(siz->width*siz->height > max_size){
			max_size = siz->width*siz->height;
		}
	}

	Image* merge = createImage(max_size, count);
	unsigned int full = merge->width*merge->height*4;

	for(int i=0; i<count; i++){
		Image* prt = readPng(argv[i+1]);
		for(int j = 0; j < merge->width; j++){
			if(j < prt->width*prt->height){
				merge->data[i*4*merge->width+j*4] = prt->data[j*4];
				merge->data[i*4*merge->width+j*4+1] = prt->data[j*4+1];
				merge->data[i*4*merge->width+j*4+2] = prt->data[j*4+2];
				merge->data[i*4*merge->width+j*4+3] = prt->data[j*4+3];
			}
		}
	}

	writePng("testout.png",merge);

	return 0;
}
