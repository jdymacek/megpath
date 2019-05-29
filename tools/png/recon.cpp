#include "../../shared/PNG.h"
#include <iostream>
#include <cstdlib>
#include <sstream>

using namespace std;

int sqrt(int sqr){
	int i = 1;

	while((i+1)*(i+1) <= sqr)
		i++;	

	return i;
}

int main(int argc, char* argv[]){
	if(argc < 2){
		cout << "No image loaded." << endl;
		return 0;
	}

	int w = 0;
	Image* b4 = readPng(argv[1]);

	if(argc > 2){
		w = atoi(argv[2]);
	}else{
		w = sqrt(b4->width);
	}

	Image* aftr = createImage(w,b4->width/w);

	for(int i = 0; i<b4->height; i++){
		for(int j = 0; j<aftr->width*4; j=j+4){
			for(int k = 0; k<aftr->height; k++){
				aftr->data[k*aftr->width*4+j] = b4->data[i*b4->width*4+k*aftr->width*4+j];
				aftr->data[k*aftr->width*4+j+1] = b4->data[i*b4->width*4+k*aftr->width*4+j+1];
				aftr->data[k*aftr->width*4+j+2] = b4->data[i*b4->width*4+k*aftr->width*4+j+2];
				aftr->data[k*aftr->width*4+j+3] = 255;
			}
		}
		string out = "out-" + to_string(i) + ".png";
		writePng(out.c_str(),aftr);
	}

	return 0;
}
