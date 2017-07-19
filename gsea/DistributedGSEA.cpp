#include "DistributedGSEA.h"

DistributedGSEA::DistributedGSEA(){
	program = "DistributedGSEA";
}

void DistributedGSEA::start(string filename){
	GSEA::start(filename);
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	char hostbuff[100];
	gethostname(hostbuff,99);
	hostname = string(hostbuff);
	program = program + "_" + to_string(size);
}


void DistributedGSEA::run(){
	int times = 2000;
	for(int i =0; i < pathways.size(); ++i){
		pathways[i].simulate(genome,times/size);
		double bigP = 0;
		MPI_Reduce(&pathways[i].p,&bigP, 1, MPI_DOUBLE, MPI_SUM,0,MPI_COMM_WORLD);
		pathways[i].p = bigP/(double)(times);
	}
	sort(pathways.begin(),pathways.end());
}

void DistributedGSEA::stop(){
	if(rank == 0){
		GSEA::stop();
	}
	MPI_Finalize();
}
