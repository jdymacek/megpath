/*
	Distributed main file
		Matthew Dyer
			Created on 5/31/2017
			Last Modified: 6/1/2017
*/

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <mpi.h>

using namespace std;

int main(int argc, char*argv[]){

	int process = 0;
	int id = 0;
	char hostname[100];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &id);
	MPI_Comm_size (MPI_COMM_WORLD, &process);
	gethostname(hostname,99);

	if(id == 0){
		cout << "Hello from the master! " << "I created " << process << " processes.\n";
	}else{
		string myName = "";
		cout << hostname << " with process " << id << " says hello.\n";
		ifstream inFile;
		inFile.open("workingHosts");
		while(inFile){
			inFile >> myName;
			if(myName == hostname){
				cout << hostname << " on " << id << " found " << myName << ".\n";
				break;
			}
		}
		inFile.close();
	}

	MPI_Finalize();
	return 0;
}
