#include "../../shared/PNG.h"
#include "../../shared/CSVFile.h"
#include "../../shared/Value.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
	if(argc < 3){
		cout << "Not enough arguments." << endl;
		return 0;
	}

	vector<Image*> db;
	CSVFile csvin;
	vector<vector<Value> > cos = csvin.readCSV(argv[1]);

	for(int i = 2; i<argc; i++){
		db.push_back(readPng(argv[i]));
	}

	double vals[db[0]->width*db[0]->height];
	double min = 255.0;
	double max = 0.0;

	//For each original image
	for(int a = 0; a < cos.size(); a++){
		//For each loaded image
		for(int i = 0; i < db.size(); i++){
			//For each pixel in the image
			for(int j = 0; j < db[0]->width*db[0]->height; j++){
				//Set-or-add to vals the pixel * the image's coefficient
				if(i == 0)
					vals[j] = db[i]->data[j*4]*cos[cos.size()-a-1][i+1].asDouble();
				else
					vals[j] += db[i]->data[j*4]*cos[cos.size()-a-1][i+1].asDouble();
				//Once vals[j] is fully enumerated, check if it's the new min/max
				if(i == db.size()-1){
					if(j == 0){
						min = vals[j];
						max = vals[j];
					}
					else if(vals[j] < min)
						min = vals[j];
					if(vals[j] > max)
						max = vals[j];
				}
			}
		}
		
		cout << min << '\t' << max << endl;
		for(int i = 0; i < db[0]->width*db[0]->height; i++){
			//		cout << i << '\t' << vals[i] << endl;
			vals[i] = vals[i]/max;
			//		cout << i << '\t' << vals[i] << endl;
		}

		Image* finale = createImage(db[0]->width,db[0]->height);

		for(int i = 0; i<finale->height; i++){
			for(int j = 0; j<finale->width; j++){
				finale->data[i*finale->width*4+j*4] = 255*vals[i*finale->width+j];
				finale->data[i*finale->width*4+j*4+1] = 255*vals[i*finale->width+j];
				finale->data[i*finale->width*4+j*4+2] = 255*vals[i*finale->width+j];
				finale->data[i*finale->width*4+j*4+3] = 255;
			}
		}
		string finame = "finale-" + to_string(a) + ".png";
		writePng(finame.c_str(),finale);
	}

	//	cout << min << '\t' << max << endl;

	//	for(int i = 0; i<cos[95].size(); i++){
	//		cout << i << '\t' << cos[95][i].asDouble() << endl;
	//	}

	return 0;
}
