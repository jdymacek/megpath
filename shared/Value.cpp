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
	return 0;
}

double Value::asDouble(){

}

string Value::asString(){
	return data;
}

vector<Value> Value::asVector(){
	vector<Value> vec;
	return vec;
}

