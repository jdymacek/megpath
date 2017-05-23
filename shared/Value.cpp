#include "Value.h"
#include <vector>

using namespace std;

Value::Value(){
	data = "";
}

Value::Value(string d){
	data = d;
}

int Value::asInt(){
	return -1;
}

double Value::asDouble(){
	return -1;
}

string Value::asString(){
	return data;
}

vector<Value> Value::asVector(){
	vector<Value> vec;
	//check for the []
	if(data.size() > 0){
		//parse string?

	}


	return vec;
}

