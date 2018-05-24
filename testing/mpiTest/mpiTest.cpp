//created by Dakota Martin
//5/24/2018
//to test if mpi works
#include <mpi.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main()
{
	MPI_Init(NULL, NULL);
	int rank = 0;
	int size = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	char hostbuff[100];
	gethostname(hostbuff, 99);
	string hostname = string(hostbuff);
	
	cout << hostname << endl;
	
	MPI_Finalize();
}
