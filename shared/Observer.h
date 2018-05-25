#ifndef OBSERVER__H
#define OBSERVER__H

class Observer{
	public:
		virtual void monteCallback(int iter){}
		virtual bool annealCallback(int iter){return true;}
		virtual void annealPrintCallback(int iter){}
		virtual void montePrintCallback(int iter){}
		int iterations;
};

#endif
