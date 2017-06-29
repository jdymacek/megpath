#include "State.h"

State::State(){
	analysis = "";
	directory = "";
	filename = "";
	time = "";
	printRuns = 1000;
}

bool State::load(string argFileName){
	int ROWS = 0;
	int COLUMNS = 0;
	int PATTERNS = 0;

	ArgFile args;
	CSVFile file;

	vector<Value> origin;
	vector<Value> controls;
	vector<Value> columns;
	vector<Value> patternArgs;
	vector<Value> idArgs;		

	//grab arguments
	args.load(argFileName);

	if(args.isArgument("analysis")){
		Value val = args.getArgument("analysis");
		analysis = val.asString();
		analysis = analysis + "_";
	}

	if(args.isArgument("max_runs")){
		Value val = args.getArgument("max_runs");
		MAX_RUNS = val.asInt();
	}

	cout << "Using analysis: " << analysis.substr(0,analysis.size()-1) << "\n";

	if(args.isArgument(analysis + "filename")){
		Value val = args.getArgument(analysis + "filename");
		filename = val.asString();
	}

	if(args.isArgument(analysis + "patterns")){
		Value val = args.getArgument(analysis + "patterns");
		patternArgs = val.asVector();
		PATTERNS = patternArgs.size();
		int i = 0;
		while(i < PATTERNS){
			patternNames.push_back(patternArgs[i].toString());
			++i;
		}
	}

	if(args.isArgument(analysis + "origin")){
		Value val = args.getArgument(analysis + "origin");
		origin = val.asVector();
	}else{
		Value val;
		origin.push_back(val);
		origin.push_back(val);
	}

	if(args.isArgument(analysis + "directory")){
		Value val = args.getArgument(analysis + "directory");
		directory = val.asString();
	}

	if(args.isArgument(analysis + "controls")){
		Value val = args.getArgument(analysis + "controls");
		controls = val.asVector();
	}

	if(args.isArgument("print_runs")){
		Value val = args.getArgument("print_runs");
		printRuns = val.asInt();
	}

	if(args.isArgument(analysis+"ids")){
		Value val = args.getArgument(analysis + "ids");
		idArgs = val.asVector();
	}

	vector<vector<Value> > csv = file.readCSV(filename);

	//expression matrix
	ROWS = csv.size() - origin[1].asInt();
	if(args.isArgument(analysis + "columns")){
		Value val = args.getArgument(analysis + "columns");
		columns = val.asVector();
		COLUMNS = columns.size();
	}else{
		COLUMNS = csv[0].size() - origin[0].asInt();
		for(int i = 0; i < COLUMNS;++i){
			Value newVal(i);
			columns.push_back(newVal);
		}
	}

	cout << ROWS << "x" << COLUMNS << endl;

	expression = MatrixXd(ROWS,COLUMNS);
	expression = MatrixXd::Zero(ROWS,COLUMNS);

	if(columns.size() != controls.size() && args.isArgument(analysis + "controls")){
		cout << "Columns and controls must be the same size.";
		return false;
	}

	for(int i = 0; i < ROWS; ++i){
		for(int j = 0; j < COLUMNS; ++j){
			if(columns.size() == controls.size()){
				expression(i,j) = csv[i+origin[1].asInt()][columns[j].asInt()+origin[0].asInt()].asDouble() - csv[i+origin[1].asInt()][controls[j].asInt()+origin[0].asInt()].asDouble();
			}else{
				expression(i,j) = csv[i+origin[1].asInt()][columns[j].asInt()+origin[0].asInt()].asDouble();
			}
		}
		//read columns to make an id
		if(idArgs.size() > 0){
			string id = "";
			for(int k =0; k < idArgs.size(); ++k){
				id += (csv[i+origin[1].asInt()][idArgs[k].asInt()]).toString();
				if(k != idArgs.size()-1){
					id += ",";
				}
			}
			ids.push_back(id);
		}
	}

	normalize();

	//should be PATTERNS and COLUMNS
	patterns.resize(PATTERNS,COLUMNS);
	//should be ROWS and PATTERNS
	coefficients.resize(ROWS,PATTERNS);	

	for(int i = 0; i < patternArgs.size(); ++i){
		vector<Value> intoMatrix;
		string findPattern = patternArgs[i].asString();
		if(args.isArgument(findPattern)){
			Value newVal = args.getArgument(findPattern);
			intoMatrix = newVal.asVector();

			ShiftPF* shared = new ShiftPF();			
			vector<Entry> constraints(intoMatrix.size(),{0,0,0});
			for(int j = 0; j < intoMatrix.size(); ++j){
				patterns.matrix(i,j) = intoMatrix[j].asDouble();
				patterns.functions(i,j) = shared;	
				constraints[j].x = j;
				constraints[j].y = i;
				constraints[j].val = patterns.matrix(i,j);	
			}
			shared->setEntries(constraints);
		}
	}
	return true;
}

void State::normalizeMatrix(MatrixXd& mat){
	double min = mat.minCoeff();
	mat = mat.array() - min;
	double max = mat.maxCoeff();
	mat = mat/max;
}

void State::patternMatch(MatrixXd& other){

	PermutationMatrix<Dynamic> perm(patterns.matrix.rows());
	perm.setIdentity();

	int rows = patterns.matrix.rows();
	for(int i =0; i < rows; ++i){
		perm.indices().data()[i] = i;
	}

	int index = 0;
	double minError = -1;
	for(int i =0; i < rows; ++i){
		MatrixXd p = patterns.matrix.row(i);
		normalizeMatrix(p);
		minError = -1;
		for(int j =index; j < rows; ++j){
			//Get Pattern
			MatrixXd q = other.row(perm.indices().data()[j]);
			normalizeMatrix(q);

			//Compare to p
			q = q-p;
			double e = q.cwiseAbs().sum();
			if(e < minError || minError == -1){
				minError = e;
				cout << "index: " << index << " j:" << perm.indices().data()[j] << endl;
				cout << "error: " << minError << endl;
				if(j != index){
					swap(perm.indices().data()[index],perm.indices().data()[j]);
				}
				cout << perm.indices() << endl;
			}
		}
		//move past the last mapping
		index += 1;
	}

	patterns.matrix = perm*patterns.matrix;
	coefficients.matrix = coefficients.matrix*perm;

	patterns.functions = perm*patterns.functions;
	coefficients.functions = coefficients.functions*perm;

	/*still need to swap around functions*/

}


void State::normalize(){
	double max = expression.maxCoeff();
	double min = expression.minCoeff();
	expression = expression.array() - min;
	expression = expression/(max-min);
}

