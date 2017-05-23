#include "Value.h"

Value::Value(){
	data = "";
}

Value::Value(string d){
	data = d;
}

int Value::asInt() const{
	string str = data;
	return atoi(str.c_str());
}

double Value::asDouble() const{
	string str = data;
	return atof(str.c_str());
}

string Value::asString() const{
	if(data.size() <= 2){
		return "";
	}
	if(data[0] != '\"' || data[data.size()-1] != '\"'){
		return "";
	}
	return  data.substr(1,data.size()-2);
}

vector<Value> Value::asVector() const{
	vector<Value> vec;
	//check for the []
	if(data.size() > 0){
		//parse string?

	}
	return vec;
}

