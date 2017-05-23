#ifndef VALUE_CPP
#define VALUE_CPP
#include <vector>
#include <cstdlib>

using namespace std;

class Value{
	public:
		Value();
		Value(string d);
		int asInt() const;
		double asDouble() const;
		string asString() const;
		vector<Value> asVector() const;
	private:
		string data;
};

#endif
