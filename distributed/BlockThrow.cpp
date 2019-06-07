#include "BlockThrow.h"

BlockThrow::BlockThrow(): BlockParallel(){
	program = "BlockThrow";
}

void BlockThrow::start(){
	BlockParallel::start();
	Range fixRange;
	fixRange.rowStart = 0;
	fixRange.rowEnd = state->patterns.rows - 1;
	fixRange.colStart = state->patterns.columns;
	if(rank == 0){
		cout << state->patterns.columns << '\n' << state->patterns.matrix << "\n\n";
	}
	state->patterns.resize(state->patterns.rows, state->patterns.columns + (int)(systemSize/2));
	fixRange.colEnd = state->patterns.columns - 1;
	state->patterns.fixRange(fixRange);
	if(rank == 0){
		cout << state->patterns.columns << '\n' << state->patterns.matrix << '\n';
	}
//	state->patterns.createBuffers();
}

void BlockThrow::run(){
	
}

void BlockThrow::stop(){
	
}
