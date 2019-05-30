#include "../../shared/State.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
	State s;

	s.load("testArgs.txt");

	int rank = 0;

	cin >> rank;

	Range ret = s.getRange(rank);

	cout << endl << ret.colStart << '\t' << ret.colEnd << '\t' << ret.rowStart << '\t' << ret.rowEnd << endl;
	return 0;
}
