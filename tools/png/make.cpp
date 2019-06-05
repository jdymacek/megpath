//PNG Testing
//Justin Moore
//May 22, 2019

#include <iostream>
#include "../../shared/PNG.h"
#include <time.h>

using namespace std;

int min(int r, int b){
	if(r <= b)
		return r;
	return b;
}

int max(int r, int b){
	if(r >= b)
		return r;
	return b;
}

int abs(int r){
	if(r < 0)
		return r*-1;
	return r;
}

void pngMake(){
	Image* mak = createImage(32,1024);	
	for(int i = 0; i < mak->height; i=i+4){
		int rrand = rand()%32*8;
		int grand = rand()%32*8;
		int brand = rand()%32*8;
		for(int j = 0; j < mak->width*4; j++){
			mak->data[i*mak->width*4+j] = rrand;
			mak->data[i*mak->width*4+j+1] = grand;
			mak->data[i*mak->width*4+j+2] = brand;
			mak->data[i*mak->width*4+j+3] = 255;
		}
	}

/*	for(int i = 0; i < mak->height; i++){
		for(int j = 0; j < mak->width*4; j=j+4){
			int c = i*6/mak->height%6;
			int r = j/4*6/mak->width%6;
			if(c == 0){
				if(r == 0){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 1){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 2){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 3){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 4){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else{
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}
			}else if(c == 1){
				if(r == 0){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 1){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 2){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 3){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 4){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else{
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}
			}else if(c == 2){
				if(r == 0){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 1){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 2){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 3){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 4){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else{
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}
			}else if(c == 3){
				if(r == 0){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 1){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 2){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 3){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 4){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else{
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}
			}else if(c == 4){
				if(r == 0){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 1){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 2){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 3){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 4){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else{
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}
			}else{
				if(r == 0){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 1){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 2){
					mak->data[i*mak->width*4+j] = 255;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 3){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 0;
					mak->data[i*mak->width*4+j+3] = 255;
				}else if(r == 4){
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 255;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}else{
					mak->data[i*mak->width*4+j] = 0;
					mak->data[i*mak->width*4+j+1] = 0;
					mak->data[i*mak->width*4+j+2] = 255;
					mak->data[i*mak->width*4+j+3] = 255;
				}
			}
		}
	}*/

/*	for(int i =0; i < mak->height; i++){
		for(int j = 0; j < mak->width; j++){
			int b = 128*j/mak->width + 128*i/mak->height;
			int r = 255-b;
			int g = 2*min(r,b);
			mak->data[4*(i*mak->width+j)] = g;
			mak->data[4*(i*mak->width+j)+1] = r;
			mak->data[4*(i*mak->width+j)+2] = b;
			mak->data[4*(i*mak->width+j)+3] = 255;
		}
	}*/

/*	for(int i =0; i < mak->height; i++){
		for(int j = 0; j < mak->width; j++){
			int c = 4*(i*mak->width+j);//-i);
			if(c < 0){
				c += mak->width*4;
			}
			mak->data[c] = 8*min(31,max(abs(i-93)-31,0));
			mak->data[c+1] = 8*min(31,max(62-abs(i-62),0));
			mak->data[c+2] = 8*min(31,max(62-abs(i-124),0));
			mak->data[c+3] = 255;		
		}
	}*/

	writePng("rr.png",mak);
}

int main(int argc, char* argv[]){
	srand(time(0));
	if(argc < 2){
		pngMake();
		return 0;
	}

	Image* tst;

	tst = readPng(argv[1]);

	unsigned int len = tst->width*tst->height*tst->channels;

	if(argc > 2){
		cout << argc << endl;
		for(int i = 0; i<len; i++){
			if(i%4 != 3){
				//tst->data[i] = 255 - tst->data[i];
				tst->data[i] -= tst->data[i]%8;
			}
		}
	}else{
		unsigned int wag = 0;
		for(int i = 0; i<len; i++){
			if(i%4 == 0){
				wag += tst->data[i]*2;
			}else if(i%4 == 1){
				wag += tst->data[i]*3;
			}else if(i%4 == 2){
				wag += tst->data[i];
			}else{
				wag /= 6;
				tst->data[i-1] = wag;
				tst->data[i-2] = wag;
				tst->data[i-3] = wag;
				wag = 0;
			}
		}	
	}

	writePng("output.png",tst);

	return 0;
}
