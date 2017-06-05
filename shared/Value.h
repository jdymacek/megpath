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
		int asInt() const;
		double asDouble() const;
		string asString() const;
		vector<Value> asVector() const;
		string toString() const;
	private:
		string data;
};

#endif
