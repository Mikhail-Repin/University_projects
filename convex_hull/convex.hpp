#include <iostream>
#include <math.h>
#define eps 1e-15
#pragma once
using namespace std;

class CDot
{
 double fst, scnd;
 public:
	int cross, ent_ex, proc;
	double t;
        CDot() {fst=0; scnd=0; proc=0;}
	CDot(double x, double y) {this->fst=x; this->scnd=y; proc=0;}
	~CDot() {}
	double x()const{return fst;}
	double y()const{return scnd;}
        CDot& operator= (const CDot& a);
        CDot(const CDot &a){*this = a;}
        void print() const {cout<<"("<<fst<<";"<<scnd<<")"<<" ";}
        int operator!=(const CDot&);
        int operator==(const CDot&);
};
list<CDot> convex_hul(list<CDot>);
