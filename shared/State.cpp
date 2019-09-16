#include "State.h"
#include <math.h>

State::State(){
	both = true;
	gray = true;
	analysis = "";
	directory = "";
	filename = "";
	printRuns = 1000;
	interruptRuns = 1000;
	annealCutOff = 1.5;
	errorAvg = 0;
	errorCount = 0;
	start_error = 0.2;
	end_error = 0.001;
	start_prob = 0.67;
	end_prob = 0.1;
	dist = "1";
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

/*vector<vector<int>> State::splitRanges(int by)
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
  }*/





// tejas 
void State::errorToPNG2(const char* name){
		MatrixXd diff = expression - coefficients.matrix*patterns.matrix;
		// diff = diff.cwiseAbs();
		diff *= 255;
/*
if spot is less than zero its gone get 0 or else its gone get -spot.
if less than zero it is red
grater than zero it is blue 
if equal to zero black. 
*/
		Image* vis = createImage(patterns.columns(),coefficients.rows());
		for(int i = 0; i < vis->height; i++){
			for(int j = 0; j < vis->width; j++){
				int spot = (int)diff(i,j);
					if(spot < 0){
						vis->data[i*vis->width*4+j*4] = -spot;
						vis->data[i*vis->width*4+j*4+1] = 0;
						vis->data[i*vis->width*4+j*4+2] = 0;
						vis->data[i*vis->width*4+j*4+3] = 255;
				}else{
						vis->data[i*vis->width*4+j*4] = 0;
						vis->data[i*vis->width*4+j*4+1] = 0;
						vis->data[i*vis->width*4+j*4+2] = spot;
						vis->data[i*vis->width*4+j*4+3] = 255;
				}
		}
		writePng(name,vis);
        
}
    destroyImage(vis);
}



void State::errorToPNG(){
		MatrixXd diff = expression - coefficients.matrix*patterns.matrix;
		diff = diff.cwiseAbs();
		diff *= 255;

		Image* vis = createImage(patterns.columns(),coefficients.rows());
		for(int i = 0; i < vis->height; i++){
			for(int j = 0; j < vis->width; j++){
				int spot = (int)diff(i,j);
				vis->data[i*vis->width*4+j*4] = spot;
				vis->data[i*vis->width*4+j*4+1] = spot;
				vis->data[i*vis->width*4+j*4+2] = spot;
				vis->data[i*vis->width*4+j*4+3] = 255;
			}
		}
		writePng("vis.png",vis);
}

Range State::getRange(int rank){
	Range r;
	vector<int> parse;

	int push = 0;
	for(int i = 0; i<dist.size(); i++){
		if(dist[i] >= '0' && dist[i] <= '9'){
			push = push*10 + dist[i]-'0';
		}

		if(dist[i] == '+' || i == dist.size()-1){
			if(push == 0)
				push = 1;
			parse.push_back(push);
			push = 0;
			if(parse.size()%2 == 1)
				parse.push_back(1);
		}else if(dist[i] == '*' || dist[i] == 'x'){
			if(push == 0)
				push = 1;
			parse.push_back(push);
			push = 0;
		}
	}
	vector<Range> lookup;
	if(dist[0] == '~'){
		int rTotal = 0;
		int dTotal = 0;
		for(int i = 0; i < parse.size(); i=i+2){
			rTotal += parse[i];
			dTotal += (parse[i]*parse[i+1]);
		}
		if(rTotal > coefficients.rows()){
			rTotal = coefficients.rows();
		}

		int rHeight = coefficients.rows()/rTotal;

		for(int i = 0; i<rTotal; i++){
			int sub = 0;
			int up = parse[0];
			while(i >= up){
				sub += 2;
				up += parse[sub];
			}

			r.rowStart = i*rHeight;
			if(i < coefficients.rows()%rTotal)
				r.rowStart += i;
			else
				r.rowStart += coefficients.rows()%rTotal;

			r.rowEnd = r.rowStart + rHeight - 1;
			if(i < coefficients.rows()%rTotal)
				r.rowEnd += 1;

			int cStep = parse[sub+1];
			if(parse[sub+1] > patterns.columns())
				cStep = patterns.columns();

			int cWidth = patterns.columns()/cStep;

			for(int j = 0; j < cStep; j++){
				r.colStart = j*cWidth;
				if(j < patterns.columns()%cStep)
					r.colStart += j;
				else
					r.colStart += patterns.columns()%cStep;

				r.colEnd = r.colStart + cWidth - 1;
				if(j < patterns.columns()%cStep)
					r.colEnd += 1;

				lookup.push_back(r);
			}
		}
	}else{
		int cTotal = 0;
		int dTotal = 0;
		for(int i = 1; i < parse.size(); i=i+2){
			cTotal += parse[i];
			dTotal += (parse[i-1]*parse[i]);
		}
		if(cTotal > patterns.columns()){
			cTotal = patterns.columns();
		}

		int cWidth = patterns.columns()/cTotal;

		for(int i = 0; i<cTotal; i++){
			int sub = 1;
			int up = parse[1];
			while(i >= up){
				sub += 2;
				up += parse[sub];
			}

			r.colStart = i*cWidth;
			if(i < patterns.columns()%cTotal)
				r.colStart += i;
			else
				r.colStart += patterns.columns()%cTotal;

			r.colEnd = r.colStart + cWidth - 1;
			if(i < patterns.columns()%cTotal)
				r.colEnd += 1;

			int rStep = parse[sub-1];
			if(parse[sub-1] > coefficients.rows())
				rStep = coefficients.rows();

			int rHeight = coefficients.rows()/rStep;

			for(int j = 0; j < rStep; j++){

				r.rowStart = j*rHeight;
				if(j < coefficients.rows()%rStep)
					r.rowStart += j;
				else
					r.rowStart += coefficients.rows()%rStep;

				r.rowEnd = r.rowStart + rHeight - 1;
				if(j < coefficients.rows()%rStep)
					r.rowEnd += 1;

				lookup.push_back(r);
			}
		}
	}

	if(rank >= lookup.size()){
		r.rowStart = -1;
		r.rowEnd = -1;
		r.colStart = -1;
		r.colEnd = -1;
		return r;
	}else{
		return lookup[rank];
	}
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

	vector<int> origin;
	vector<int> controls;
	vector<int> columns;
	vector<Value> patternArgs;
	vector<int> idArgs;

	//grab arguments
	args.fromString("analysis = \"default\"\nmax_runs = 1000\ndebug = false\nstart_error = 0.2\nend_error = 0.001\nstart_prob = 0.67\nend_prob = 0.1\nstats = \"none\"\nanneal_cut_off = 1.5\ndefault_filename = \"mixed.csv\"\ndefault_patterns = [\"\",\"\",\"\",\"\",\"\"]\ndefault_origin = {0,0}\ndefault_directory = \"\"\nprint_runs = 1000\ninterrupt_runs = 1000\ndistribution = 1*1+2*2+3*3");
	args.load(argFileName);

	if(args.isArgument("analysis")){
		analysis = (string)args.getArgument("analysis");
//		analysis = val.asString();
		analysis = analysis + "_";
	}

	if(args.isArgument("max_runs")){
		MAX_RUNS = args.getArgument("max_runs");
//		Value val = args.getArgument("max_runs");
//		MAX_RUNS = val.asInt();
	}

	if(args.isArgument("debug")){
		debug = args.getArgument("debug");
//		debug = val.asBool();
	}else{
		debug = false;
	}
	if(args.isArgument("start_error")){
		start_error = args.getArgument("start_error");
//		start_error = val.asDouble();
	}
	if(args.isArgument("end_error")){
		end_error = args.getArgument("end_error");
//		end_error = val.asDouble();
	}
	if(args.isArgument("start_prob")){
		start_prob = args.getArgument("start_prob");
//		start_prob = val.asDouble();
	}
	if(args.isArgument("end_prob")){
		end_prob = args.getArgument("end_prob");
//		end_prob = val.asDouble();
	}

	if(args.isArgument("stats")){
		stats = (string)args.getArgument("stats");
//		stats = val.asString();
	}
	if(args.isArgument("anneal_cut_off")){
		annealCutOff = args.getArgument("anneal_cut_off");
//		annealCutOff = val.asDouble();
	}
	if(debug){
		cout << "Using analysis: " << analysis.substr(0,analysis.size()-1) << "\n";
	}

	if(args.isArgument(analysis + "filename")){
		filename = (string)args.getArgument(analysis + "filename");
//		filename = val.asString();

		if(filename.substr(filename.size()-4,4) == ".png"){
			img = true;
		}
	}

	if(args.isArgument(analysis + "patterns")){
		patternArgs = args.getArgument(analysis + "patterns");
//		patternArgs = val.asVector();
		PATTERNS = patternArgs.size();
		int i = 0;
		both = false;
		while(i < PATTERNS){
			patternNames.push_back(patternArgs[i].toString());
			if((string)patternArgs[i] == ""){
				both = true;
			}
			++i;
		}
	}

	if(args.isArgument(analysis + "origin")){
		origin = args.getArgument(analysis + "origin");
//		origin = val.asVector();
	}else{
		Value val;
		origin.push_back(val);
		origin.push_back(val);
	}

	if(args.isArgument(analysis + "directory")){
		directory = (string)args.getArgument(analysis + "directory");
//		directory = val.asString();
	}

	if(args.isArgument(analysis + "controls")){
		controls = args.getArgument(analysis + "controls");
//		controls = val.asVector();
	}

	if(args.isArgument("print_runs")){
		printRuns = args.getArgument("print_runs");
//		printRuns = val.asInt();
	}
	if(args.isArgument("interrupt_runs")){
		interruptRuns = args.getArgument("interrupt_runs");
//		interruptRuns = val.asInt();
	}

	if(args.isArgument(analysis+"ids")){
		idArgs = args.getArgument(analysis + "ids");
//		idArgs = val.asVector();
	}

	if(args.isArgument("distribution")){
		dist = (string)args.getArgument("distribution");
//		dist = val.asString();
	}

	vector<vector<Value> > res;
	if(img){
		png = readPng(filename.c_str());
		res = pixlToVal(png, gray);
	}else{
		res = file.readCSV(filename);
	}
	//expression matrix
	ROWS = res.size() - origin[1];
	if(args.isArgument(analysis + "columns")){
		columns = args.getArgument(analysis + "columns");
//		columns = val.asVector();
		COLUMNS = columns.size();
	}else{
		COLUMNS = res[0].size() - origin[0];
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
				expression(i,j) = (double)res[i+origin[1]][columns[j]+origin[0]] - (double)res[i+origin[1]][controls[j]+origin[0]];
			}else{
				expression(i,j) = (double)res[i+origin[1]][columns[j]+origin[0]];
			}
		}
		//read columns to make an id
		if(idArgs.size() > 0){
			string id = "";
			for(int k =0; k < idArgs.size(); ++k){
				id += (res[i+origin[1]][idArgs[k]]).toString();
				if(k != idArgs.size()-1){
					id += ",";
				}
			}
			ids.push_back(id);
		}else{
			string id = "row-" + to_string(i);
			ids.push_back(id);
		}
	}

	normalize();
	sortZero();
	MatrixXd temp = expression.block(0,0,expression.rows(),expression.cols()-zeroes);
	expression = temp;
	
	rPerm = PermutationMatrix<Dynamic>(expression.rows());
	cPerm = PermutationMatrix<Dynamic>(expression.cols());
	rPerm.setIdentity();
	cPerm.setIdentity();
	random_shuffle(rPerm.indices().data(),rPerm.indices().data()+rPerm.indices().size());
	random_shuffle(cPerm.indices().data(),cPerm.indices().data()+cPerm.indices().size());
	expression = rPerm * expression * cPerm;

	patterns.prototype = new WeightedPF();
	patterns.resize(PATTERNS,expression.cols());

	coefficients.prototype = new WeightedPF();
	coefficients.resize(expression.rows(),PATTERNS);

	int numPatterns = 0;
	for(int i = 0; i < patternArgs.size(); ++i){
		vector<Value> intoMatrix;
		string findPattern = (string)patternArgs[i];
		if(args.isArgument(findPattern)){
			++numPatterns;
			intoMatrix = args.getArgument(findPattern);
//			intoMatrix = newVal.asVector();

			ShiftPF* shared = new ShiftPF();
			vector<Entry> constraints;
			for(int j = 0; j < intoMatrix.size(); ++j){
				string str = (string)intoMatrix[j];
				if(debug){
					cout << str << endl;
				}
				if(str != "?"){
					if(debug){
						cout << "adding to constraint" << endl;
					}
					constrained = true;
					Entry e;
					patterns.matrix(i,j) = (double)intoMatrix[j];
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

	if(numPatterns == patternArgs.size()){
		both = false;
	}
	return true;
}

void State::sortZero(){
	zCol = PermutationMatrix<Dynamic>(expression.cols());
	zCol.setIdentity();
	int i = 0;
	int j = expression.cols()-1;
	zeroes = 0;
	while(i < j){
		while(i < j && expression.col(i).sum() != 0){	
			i++;
		}
		while(j >= i && expression.col(j).sum() == 0){	
			if(j < i)
				zeroes--;
			j--;
			zeroes++;
		}
		if(i < j){
			swap(zCol.indices().data()[i],zCol.indices().data()[j]);
			zeroes++;
		}
		i++;
		j--;
		if(i == j && expression.col(j).sum() == 0){
			zeroes++;
		}
	}
	expression = expression * zCol;
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

void State::unshufflePC(){
	coefficients.matrix = rPerm.inverse() * coefficients.matrix;
	patterns.matrix = patterns.matrix * cPerm.inverse();
	patterns.matrix.conservativeResize(patterns.rows(),patterns.columns()+zeroes);
	patterns.matrix.topRightCorner(patterns.rows(),zeroes) = MatrixXd::Zero(patterns.rows(),zeroes);
	patterns.matrix = patterns.matrix * zCol.inverse();
	expression = rPerm.inverse() * expression * cPerm.inverse();
	expression.conservativeResize(expression.rows(),expression.cols()+zeroes);
	expression.topRightCorner(expression.rows(),zeroes) = MatrixXd::Zero(expression.rows(),zeroes);
	expression = expression * zCol.inverse();
}

void State::reshufflePC(){
	coefficients.matrix = rPerm * coefficients.matrix;
	patterns.matrix = patterns.matrix * zCol;
	MatrixXd temp = patterns.matrix.block(0,0,patterns.rows(),patterns.columns()-zeroes);
	patterns.matrix = temp;
	patterns.matrix = patterns.matrix * cPerm;
	expression = expression * zCol;
	temp = expression.block(0,0,expression.rows(),expression.cols()-zeroes);
	expression = temp;
	expression = rPerm * expression * cPerm;
}

vector<vector<Value> > State::pixlToVal(Image* png, bool& gray){
	gray = true;
	for(int i = 0; i<png->width*png->height*4; i = i+4){
		if(png->data[i] != png->data[i+1] || png->data[i] != png->data[i+2]){
			gray = false;
			break;
		}
	}

	if(!gray){
		vector<vector<Value> > rv(png->height, vector<Value>(png->width*3,1));
		for(int i = 0; i<png->height; i++){
			for(int j = 0; j<png->width; j++){
				rv[i][j*3] = Value(png->data[4*png->width*i+j*4]);
				rv[i][j*3+1] = Value(png->data[4*png->width*i+j*4+1]);
				rv[i][j*3+2] = Value(png->data[4*png->width*i+j*4+2]);
			}
		}
		return rv;
	}else{
		vector<vector<Value> > rv(png->height, vector<Value>(png->width,1));
		for(int i = 0; i<png->height; i++){
			for(int j = 0; j<png->width; j++){
				rv[i][j] = Value(png->data[4*png->width*i+j*4]);
			}
		}
		return rv;
	}
}

// Tejas
// change g to int
// error to png new func
//  const char* name

/*	Image* ret;
	if(!g){
		ret = createImage(c/3, r);
		for(int i = 0; i<r; i++){
			for(int j = 0; j<c; j=j+3){
                // 
				ret->data[j/3*4+c/3*4*i] = (int)(mat(i,j)*255);
				ret->data[j/3*4+c/3*4*i+1] = (int)(mat(i,j+1)*255);
				ret->data[j/3*4+c/3*4*i+2] = (int)(mat(i,j+2)*255);
				ret->data[j/3*4+c/3*4*i+3] = 255;
			}
		}
	}else{
		ret = createImage(c, r);
		for(int i = 0; i<r; i++){
			for(int j = 0; j<c; j++){                
                //
				int temp = (int)(mat(i,j)*255);
				ret->data[4*j+4*c*i] = temp;
				ret->data[4*j+4*c*i+1] = temp;
				ret->data[4*j+4*c*i+2] = temp;
				ret->data[4*j+4*c*i+3] = 255;
			}
		}
	}
	writePng(name,ret);
}
*/


void State::MXdToPNG(MatrixXd mat, int r, int c, bool g, const char* name){
	Image* ret;
	if(!g){
		ret = createImage(c/3, r);
		for(int i = 0; i<r; i++){
			for(int j = 0; j<c; j=j+3){
                
                // 
				ret->data[j/3*4+c/3*4*i] = (int)(mat(i,j)*255);
				ret->data[j/3*4+c/3*4*i+1] = (int)(mat(i,j+1)*255);
				ret->data[j/3*4+c/3*4*i+2] = (int)(mat(i,j+2)*255);
				ret->data[j/3*4+c/3*4*i+3] = 255;
			}
		}
	}else{
		ret = createImage(c, r);
		for(int i = 0; i<r; i++){
			for(int j = 0; j<c; j++){                
                //
				int temp = (int)(mat(i,j)*255);
				ret->data[4*j+4*c*i] = temp;
				ret->data[4*j+4*c*i+1] = temp;
				ret->data[4*j+4*c*i+2] = temp;
				ret->data[4*j+4*c*i+3] = 255;
			}
		}
	}
	writePng(name,ret);
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
