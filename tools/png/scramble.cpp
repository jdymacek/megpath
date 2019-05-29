#include <iostream>
#include "../../shared/PNG.h"
#include <time.h>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
	if(argc < 2){
		cout << "Not enough arguments." << endl;
		return 0;
	}

	srand(time(0));
	int swp = 0;
	int swpc = 0;
	vector<int> memory;

	Image* scrmbl = readPng(argv[1]);
	for(int j=0; j<scrmbl->height; j++){
		swp = j;
			if(argc >= 2 && argc != 3){
				swp = rand()%scrmbl->height;
			}
		for(int k=0; k<scrmbl->width*4; k=k+4){
			if(argc < 3){
				swpc = k;
			}else{
				if(j == 0){
					swpc = (rand()%scrmbl->width)*4;
					memory.push_back(swpc);
				}else{
					swpc = memory[k/4];
				}
			}

			int tmp = scrmbl->data[k+j*scrmbl->width*4];
			scrmbl->data[k+j*scrmbl->width*4] = scrmbl->data[swpc+swp*scrmbl->width*4];
			scrmbl->data[swpc+swp*scrmbl->width*4] = tmp;

			tmp = scrmbl->data[k+j*scrmbl->width*4+1];
			scrmbl->data[k+j*scrmbl->width*4+1] = scrmbl->data[swpc+swp*scrmbl->width*4+1];
			scrmbl->data[swpc+swp*scrmbl->width*4+1] = tmp;

			tmp = scrmbl->data[k+j*scrmbl->width*4+2];
			scrmbl->data[k+j*scrmbl->width*4+2] = scrmbl->data[swpc+swp*scrmbl->width*4+2];
			scrmbl->data[swpc+swp*scrmbl->width*4+2] = tmp;

			tmp = scrmbl->data[k+j*scrmbl->width*4+3];
			scrmbl->data[k+j*scrmbl->width*4+3] = scrmbl->data[swpc+swp*scrmbl->width*4+3];
			scrmbl->data[swpc+swp*scrmbl->width*4+3] = tmp;
		}
	}
	writePng("scramble.png",scrmbl);

	return 0;
}
