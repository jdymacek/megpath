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
		bool asBool() const;
		int asInt() const;
		double asDouble() const;
		string asString() const;
		vector<Value> asVector() const;
		string toString() const;

		operator int() const;
	private:
		string data;
};

#endif
