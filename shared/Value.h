#ifndef VALUE_CPP
#define VALUE_CPP

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
