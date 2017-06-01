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
	int rank = 0;
	char hostname[100];
	int tag = 0;
	int source;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &process);
	gethostname(hostname,99);

	if(rank == 0){
		int x = 2;
		cout << "Hello from the master! " << "I created " << process << " processes.\n";
		for(int i = 1; i < process; ++i){
			MPI_Send(&x,sizeof(int),MPI_INT,i,tag,MPI_COMM_WORLD);
		}
	}else{

		string myName = "";
		cout << hostname << " with process " << rank << " says hello.\n";
		ifstream inFile;
		inFile.open("workingHosts");
		while(inFile){
			inFile >> myName;
			if(myName == hostname){
				cout << hostname << " on " << rank << " found " << myName << ".\n";
				break;
			}
		}
		inFile.close();

		int y = 0;
		MPI_Recv(&y,sizeof(int),MPI_INT,0,tag,MPI_COMM_WORLD,&status);
		y = y * 2;
		MPI_Send(&y,sizeof(int),MPI_INT,0,tag,MPI_COMM_WORLD);
	}

	if(rank == 0){
		int x;
		cout << "Begin receiving: \n";
		for(source = 1; source < process; ++source){
			MPI_Recv(&x,sizeof(int),MPI_INT,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);
			cout << "From " << source << " : " << x << endl;
		}
	}

	MPI_Finalize();
	return 0;
}
