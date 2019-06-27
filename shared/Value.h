#ifndef VALUE_CPP
#define VALUE_CPP
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

class Value{
	public:
		Value();
		Value(string d);
		Value(int d);
		string toString() const;

		operator bool() const;
		operator int() const;
		operator double() const;
		operator string() const;
		operator vector<Value>() const;
		operator vector<int>() const;
	private:
		string data;
};

#endif
