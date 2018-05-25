#ifndef OBSERVER__H
#define OBSERVER__H

class Observer{
	public:
		virtual void monteCallback(double error){}
		virtual void annealCallback(double error){}
		virtual void annealPrintCallback(int iter){}
		virtual void montePrintCallback(int iter){}
		int iterations;
};

#endif
