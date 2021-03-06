#include "Value.h"

Value::Value(){
	data = "";
}

Value::Value(string d){
	data = d;
}

Value::Value(int d){
	stringstream ss;
	ss << d;
	data = ss.str();
}

string Value::toString() const{
	return data;
}


Value::operator bool() const{
	string str = data;
	if(atoi(str.c_str()))
		return true;
	return false;
}

Value::operator int() const{
	string str = data;
	return atoi(str.c_str());
}

Value::operator double() const{
	string str = data;
	return atof(str.c_str());
}

Value::operator string() const{
	if(data.size() <= 2){
		return "";
	}
	if(data[0] != '\"' || data[data.size()-1] != '\"'){
		return "";
	}
	return  data.substr(1,data.size()-2);
}

Value::operator vector<Value>() const{
	vector<Value> vec;
	if(data.size() <= 2){
		return vec;
	}
	if(data[0] != '[' || data[data.size()-1] != ']'){
		return vec;
	}
	string token = "";
	bool inString = false;
	string str = data.substr(1,data.size()-2);	
	//walk along string looking for commas not inside of ""
	for(int i =0; i < str.size(); ++i){
		if(inString){
			token += str[i];		
			if(str[i] == '\"'){
				inString = false;
			}
		}else{
			if(str[i] == '\"'){
				token = "\"";
				inString = true;
			}else if(str[i] == ','){
				Value v(token);
				vec.push_back(v);
				token = "";
			}else{
				token += str[i];
			}
		}
	}
	Value v(token);
	vec.push_back(token);

	return vec;
}

Value::operator vector<int>() const{
	vector<int> vec;
	if(data.size() <= 2){
		return vec;
	}
	if(data[0] != '[' || data[data.size()-1] != ']'){
		return vec;
	}
	string token = "";
	bool inString = false;
	string str = data.substr(1,data.size()-2);	
	//walk along string looking for commas not inside of ""
	for(int i =0; i < str.size(); ++i){
		if(inString){
			token += str[i];		
			if(str[i] == '\"'){
				inString = false;
			}
		}else{
			if(str[i] == '\"'){
				token = "\"";
				inString = true;
			}else if(str[i] == ','){
				Value v(token);
				vec.push_back((int)v);
				token = "";
			}else{
				token += str[i];
			}
		}
	}
	Value v(token);
	vec.push_back((int)v);

	return vec;
}
