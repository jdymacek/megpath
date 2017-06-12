#include "State.h"

int main(int argc, char** argv){
	State* state = new State();
	state->load(argv[1]);

	return 0;
}
