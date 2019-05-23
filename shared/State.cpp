#include "State.h"
#include <math.h>

State::State(){
	both = true;
	analysis = "";
	directory = "";
	filename = "";
	printRuns = 1000;
	interuptRuns = 1000;
	annealCutOff = 1.5;
	errorAvg = 0;
	errorCount = 0;
	start_error = 0.2;
	end_error = 0.001;
	start_prob = 0.67;
	end_prob = 0.1;
}


void State::reset()
{
	both = true;
	//this is broken -- fix if all patterns are constrained
    //if(numPatterns == patternArgs.size()){
    //    both = false;
    //}
	patterns.reset();
	coefficients.reset();
}

vector<vector<int>> State::splitRanges(int by)
{
	int rowSize = coefficients.rows/by;
	int colSize = patterns.columns/by;
	int rowStart = 0;
	int colStart = 0;
	
	vector<vector<int>> rv;

	for(int i =0; i < by; ++i){
        	int rowEnd = rowStart + rowSize;
        	int colEnd = colStart + colSize;
        	if(i < coefficients.rows%by)
	            rowEnd += 1;
	        if(i < patterns.columns%by)
	            colEnd += 1;
		vector<int> tmp;
		tmp.push_back(colStart);
		tmp.push_back(colEnd);
		tmp.push_back(rowStart);
		tmp.push_back(rowEnd);
		rv.push_back(tmp);
			
        	rowStart = rowEnd;
        	colStart = colEnd;
    	}	 
	return rv;
}

double State::calcT(){
	//start_error = errorAvg/errorCount;
	return (-start_error)/log(start_prob);
}
double State::calcAlpha(double t){
	return pow((-end_error / (log(end_prob)*t)),(1/(MAX_RUNS*annealCutOff)));
}

bool State::load(string argFileName){
	int ROWS = 0;
	int COLUMNS = 0;
	int PATTERNS = 0;
	constrained = false;
	img = false;

	ArgFile args;
	CSVFile file;
	Image* png;


	vector<Value> origin;
	vector<Value> controls;
	vector<Value> columns;
	vector<Value> patternArgs;
	vector<Value> idArgs;		

	//grab arguments
	args.fromString("analysis = \"default\"\nmax_runs = 1000\ndebug = false\nstart_error = 0.2\nend_error = 0.001\nstart_prob = 0.67\nend_prob = 0.1\nstats = \"none\"\nanneal_cut_off = 1.5\ndefault_filename = \"mixed.csv\"\ndefault_patterns = [\"\",\"\",\"\",\"\",\"\"]\ndefault_origin = {0,0}\ndefault_directory = \"../testing/csv/\"\nprint_runs = 1000\ninterrupt_runs = 1000");
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

	if(args.isArgument("debug")){
		Value val = args.getArgument("debug");
		debug = val.asBool();
	}else{
		debug = false;
	}
	if(args.isArgument("start_error")){
		Value val = args.getArgument("start_error");
		start_error = val.asDouble();
	}
	if(args.isArgument("end_error")){
		Value val = args.getArgument("end_error");
		end_error = val.asDouble();
	}
	if(args.isArgument("start_prob")){
	       Value val = args.getArgument("start_prob");
	       start_prob = val.asDouble();
	}
	if(args.isArgument("end_prob")){
		Value val = args.getArgument("end_prob");
		end_prob = val.asDouble();
	}

	if(args.isArgument("stats")){
		Value val = args.getArgument("stats");
		stats = val.asString();
	}
	if(args.isArgument("anneal_cut_off")){
		Value val = args.getArgument("anneal_cut_off");
		annealCutOff = val.asDouble();
	}
	if(debug){
		cout << "Using analysis: " << analysis.substr(0,analysis.size()-1) << "\n";	
	}

	if(args.isArgument(analysis + "filename")){
		Value val = args.getArgument(analysis + "filename");
		filename = val.asString();

		if(filename.substr(filename.size()-4,4) == ".png"){
			cout << "PNG Found" << endl;
			img = true;
		}
	}

	if(args.isArgument(analysis + "patterns")){
		Value val = args.getArgument(analysis + "patterns");
		patternArgs = val.asVector();
		PATTERNS = patternArgs.size();
		int i = 0;
		both = false;
		while(i < PATTERNS){
			patternNames.push_back(patternArgs[i].toString());
			if(patternArgs[i].asString() == ""){
				both = true;
			}
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
	if(args.isArgument("interupt_runs")){
		Value val = args.getArgument("interupt_runs");
		interuptRuns = val.asInt();
	}

	if(args.isArgument(analysis+"ids")){
		Value val = args.getArgument(analysis + "ids");
		idArgs = val.asVector();
	}

	vector<vector<Value> > res;
	if(img){
		png = readPng(filename.c_str());
		res = pixlToVal(png);
	}else{
		res = file.readCSV(filename);
	}	
	//expression matrix
	ROWS = res.size() - origin[1].asInt();
	if(args.isArgument(analysis + "columns")){
		Value val = args.getArgument(analysis + "columns");
		columns = val.asVector();
		COLUMNS = columns.size();
	}else{
		COLUMNS = res[0].size() - origin[0].asInt();
		for(int i = 0; i < COLUMNS;++i){
			Value newVal(i);
			columns.push_back(newVal);
		}
	}
	
	if(debug){
		cout << ROWS << "x" << COLUMNS << endl;
	}

	expression = MatrixXd(ROWS,COLUMNS);
	expression = MatrixXd::Zero(ROWS,COLUMNS);

	if(columns.size() != controls.size() && args.isArgument(analysis + "controls")){
		cout << "Columns and controls must be the same size.";
		return false;
	}

	for(int i = 0; i < ROWS; ++i){
		for(int j = 0; j < COLUMNS; ++j){
			if(columns.size() == controls.size()){
				expression(i,j) = res[i+origin[1].asInt()][columns[j].asInt()+origin[0].asInt()].asDouble() - res[i+origin[1].asInt()][controls[j].asInt()+origin[0].asInt()].asDouble();
			}else{
				expression(i,j) = res[i+origin[1].asInt()][columns[j].asInt()+origin[0].asInt()].asDouble();
			}
		}
		//read columns to make an id
		if(idArgs.size() > 0){
			string id = "";
			for(int k =0; k < idArgs.size(); ++k){
				id += (res[i+origin[1].asInt()][idArgs[k].asInt()]).toString();
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


	int numPatterns = 0;
	for(int i = 0; i < patternArgs.size(); ++i){
		vector<Value> intoMatrix;
		string findPattern = patternArgs[i].asString();
		if(args.isArgument(findPattern)){
			++numPatterns;
			Value newVal = args.getArgument(findPattern);
			intoMatrix = newVal.asVector();

			ShiftPF* shared = new ShiftPF();			
			vector<Entry> constraints;
			for(int j = 0; j < intoMatrix.size(); ++j){
				string str = intoMatrix[j].asString();
				if(debug){
					cout << str << endl;
				}
				if(str != "?"){
					if(debug){
						cout << "adding to constraint" << endl;
					}
					constrained = true;
					Entry e;
					patterns.matrix(i,j) = intoMatrix[j].asDouble();
					patterns.functions(i,j) = shared;	
					e.x = j;
					e.y = i;
					e.val = patterns.matrix(i,j);	
					constraints.push_back(e);
				}
			}
			shared->setEntries(constraints);
		}
	}

//	cout << patterns.matrix << endl;

	if(numPatterns == patternArgs.size()){
		both = false;
	}

	string print = args.toString();
	cout << print << endl;

	if(img){
		Image* ret;
		ret = createImage(png->width, png->height);
		cout << COLUMNS << '\t' << ret->width << endl;
		if(COLUMNS == ret->width*3){
			for(int i = 0; i<ROWS; i++){
				for(int j = 0; j<COLUMNS; j=j+3){
					ret->data[j/3*4+COLUMNS/3*4*i] = (int)(expression(i,j)*255);
					ret->data[j/3*4+COLUMNS/3*4*i+1] = (int)(expression(i,j+1)*255);
					ret->data[j/3*4+COLUMNS/3*4*i+2] = (int)(expression(i,j+2)*255);
					ret->data[j/3*4+COLUMNS/3*4*i+3] = 255;
				}
			}
		}else if(COLUMNS == ret->width){
			for(int i = 0; i<ROWS; i++){
				for(int j = 0; j<COLUMNS; j++){
					int temp = (int)(expression(i,j)*255);
					ret->data[4*j+4*COLUMNS*i] = temp;
					ret->data[4*j+4*COLUMNS*i+1] = temp;
					ret->data[4*j+4*COLUMNS*i+2] = temp;
					ret->data[4*j+4*COLUMNS*i+3] = 255;
				}
			}
		}else{
			cout << "expression-to-PNG failed" << endl;
			return true;
		}
		writePng("return.png",ret);
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
		if(!patterns.isConstrained(i)){
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
					//cout << "index: " << index << " j:" << perm.indices().data()[j] << endl;
					//cout << "error: " << minError << endl;
					if(j != index){
						swap(perm.indices().data()[index],perm.indices().data()[j]);
					}
					//cout << perm.indices() << endl;
				}
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

vector<vector<Value> > State::pixlToVal(Image* png){
	bool gray = true;
	for(int i = 0; i<png->width*png->height*4; i = i+4){
		if(png->data[i] != png->data[i+1] || png->data[i] != png->data[i+2]){
			gray = false;
			break;
		}
	}

	if(!gray){
		vector<vector<Value> > rv(png->height, vector<Value>(png->width*3,1));
		for(int i = 0; i<png->height; i++){
			for(int j = 0; j<png->width*4; j=j+4){
				//rv[i/4][j] = Value(png->data[i+4*png->width*j]<<16 | png->data[i+4*png->width*j+1]<<8 | png->data[i+4*png->width*j+2]);
				rv[i][j/4*3] = Value(png->data[4*png->width*i+j]);
				rv[i][j/4*3+1] = Value(png->data[4*png->width*i+j+1]);
				rv[i][j/4*3+2] = Value(png->data[4*png->width*i+j+2]);
				int per = png->height/10;
				for(int k=1; k<=10; k++){
					if(j == 0 && i == per*k){
						cout << 10*k << endl;
					}
				}
				//rv[i/4][j] = Value(png->data[i+4*png->width*j]);
				//cout << j/4*3 << '\t' << i << '\t' << rv[i][j/4*3].asInt() << endl; 
				//cout << j/4*3+1 << '\t' << i << '\t' << rv[i][j/4*3+1].asInt() << endl; 
				//cout << j/4*3+2 << '\t' << i << '\t' << rv[i][j/4*3+2].asInt() << endl; 
			}
		}
		return rv;
	}else{
		vector<vector<Value> > rv(png->height, vector<Value>(png->width,1));
		for(int i = 0; i<png->height; i++){
			for(int j = 0; j<png->width*4; j=j+4){
				//rv[i/4][j] = Value(png->data[i+4*png->width*j]<<16 | png->data[i+4*png->width*j+1]<<8 | png->data[i+4*png->width*j+2]);
				rv[i][j/4] = Value(png->data[4*png->width*i+j]);
				//cout << j/4 << '\t' << i << '\t' << rv[i][j/4].asInt() << endl; 
				int per = png->height/10;
				for(int k=1; k<=10; k++){
					if(j == 0 && i == per*k){
						cout << 10*k << endl;
					}
				}
			}
		}
		return rv;
	}
}
//Old State split functions
/*int ParallelPatterns::findStart(int myRank, int curSize, int numRows){
	int startRow = 0;
	int split = numRows/curSize;
	int leftover = numRows%curSize;
	if(myRank < leftover){
		split = split + 1;
		startRow = myRank*split;
	}else{
		startRow = numRows - (curSize - myRank) * split;
	}
	return startRow;
}

int ParallelPatterns::findRows(int myRank, int curSize, int numRows){
	int split = numRows/curSize;
	int leftover = numRows%curSize;
	if(myRank < leftover){
		split = split + 1;
	}
	return split;
}*/


