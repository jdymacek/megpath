#ifndef RANGE__H
#define RANGE__H

struct Range{
	int rowStart;
	int rowEnd;
	int colStart;
	int colEnd;
	bool contains(int r,int c){
		return c >= colStart && c <= colEnd && r >= rowStart && r <= rowEnd;
	}
	int rowSize(){
		return rowEnd - rowStart + 1;
	}
	int colSize(){
		return colEnd - colStart + 1;
	}
	bool isValid(){
		if(rowStart < 0 || rowEnd < 0 || colStart < 0 || colEnd < 0)
			return false;
		return true;
	}
	
	string toString(){
		return "{" + to_string(rowStart) + ":" + to_string(rowEnd) + "," + to_string(colStart) + ":" + to_string(colEnd) + "}";
	}

};

#endif
