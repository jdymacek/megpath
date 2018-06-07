#include <stdio.h>
#include <iostream>
#include "omp.h"
#include <omp.h>

using namespace std;


int toThread(int num)
{
	int bob = 1;
	for(int i = 0; i<num; i++)
	{
		//bob++;
		//cout << bob << endl;
	}
	return bob;
}

int main()
{
	int j = 0;
	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		int toAdd = toThread(id);
		#pragma omp critical
		j+= toAdd;

		#pragma omp single
		cout << j << endl;
	}
	cout << j << endl;

}
