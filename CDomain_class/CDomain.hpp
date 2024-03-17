#include <iostream>
#include <math.h>
#define eps 1e-15
#pragma once
using namespace std;

double Det (double , double , double , double );
int eq (double , double );
template<class T> class CList2;
class CDot;
class CDomain;
void PrintLL(CList2<CList2<CDot>>&);
void printcommands();

template<class T> class CList2Node{public: T v; CList2Node<T> *prev, *next; CList2Node() {prev = next = NULL;} };

template<class T> class CList2
{
 CList2Node<T> *cur, *begin, *end;
 public:
        class iterator
 	{
 	  public:
          	CList2Node<T> *cur;
 		iterator() {cur=NULL;}
                iterator(CList2Node<T> *cur) {this->cur = cur;}
                int operator!=(const iterator &b) {return cur!=b.cur;}
                int operator==(const iterator &b) {return cur==b.cur;}
                void operator++() {if(cur) cur=cur->next;}
                void operator--() {if(cur) cur=cur->prev;}
                T operator*() {return cur->v;}
	};
	CList2() {SetZero();}
	void SetZero() {cur=begin=NULL;}
	~CList2() {Clean();}
	void Clean() {GoToBegin(); while(!DelCur()); SetZero();}
        CList2(const CList2 &b) {CopyOnly(b);}
        void CopyOnly (const CList2 &b) 
	{ 
		if(!b.IsEmpty())
		{
			Clean();
			CList2<T>::iterator it;
	                for(it=b.first(); it!=b.last(); ++it)
	                {
	                	AddAfter(it.cur->v);
	                	GoToNext();
	                }
               		AddAfter(it.cur->v);
		}
	}
	CList2& operator=(const CList2 &a){CopyOnly(a); return *(this);}
        void GoToBegin() {cur=begin;}
        void GoToEnd() {cur=end;}
        int GoToNext() {if(IsEmpty()) return -1; if(cur->next==NULL) return -2; cur = cur->next; return 0;}
	int GoToPrev() {if(IsEmpty()) return -1; if(cur->prev==NULL) return -2; cur = cur->prev; return 0;}
        int GetCur(T &x) {if(IsEmpty()) return -1; x=cur->v; return 0;}
        int IsEmpty() const {return cur==NULL;}
        void AddAfter (T &x);
        void AddBefore (T &x);
        int DelCur ();
        iterator first() const {return iterator(begin);}
        iterator last() const {return iterator(end);}
        friend class CDomain;
        friend void PrintLL(CList2<CList2<CDot>>&);
};

class CDot
{
 double fst, scnd;
 public:
 	CList2Node<CDot> *nextPoly, *neib;
	int cross, ent_ex, proc, vert, ins, dop;
	double t;
        CDot() {fst=0; scnd=0; cross=0; nextPoly=neib=NULL; proc=0; dop = 0; ins = -1; ent_ex=0;}
	CDot(double x, double y) {this->fst=x; this->scnd=y; nextPoly=neib=NULL; proc=0; cross=0; dop = 0; ins = -1; ent_ex=0;}
	~CDot() {nextPoly=neib=NULL;}
	double x()const{return fst;}
	double y()const{return scnd;}
        CDot& operator= (const CDot& a);
        CDot(const CDot &a){*this = a;}
        void print() const {cout<<"("<<fst<<";"<<scnd<<")"<<" ";}
        int operator!=(const CDot&);
        int operator==(const CDot&);
};

int intersect (CDot , CDot ,CDot , CDot , CDot &);

class CDomain
{
 CList2<CDot> domain;
 public:
 	CDomain() {CDot t; domain.AddAfter(t);}
 	CDomain(CDot *x,int n) 
	{
		for(int i=0; i<n; i++)
		{  
			domain.AddAfter(x[i]);
                        domain.GoToNext();
		} 
		if(x[0] != x[n-1]) domain.AddAfter(x[0]);
	} 
    	CDomain(const CDomain &b) 
    	{
        	domain.CopyOnly(b.domain);
    	}
    	CDomain& operator= (const CDomain &a) {domain.CopyOnly(a.domain); return(*this);}                                           
    	int ChangeDot(CDot&, int);
    	int AddDot(CDot&, int);
    	int Size();
    	int DelDot(int );
    	int AddToSide(double , int);
    	int IsInside(CDot);
    	CList2<CList2<CDot>> Intersection(CDomain );
    	CList2<CList2<CDot>> Union(CDomain);
	void print() 
	{
		for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it) {(*it).print();}
	}
};
        
template<class T> void CList2<T>::AddAfter (T &x)
{
 CList2Node<T> *n = new CList2Node<T>(); n->v = x;
 if(IsEmpty()) {cur=begin=end=n;}
 else
 {
 	n->next=cur->next; n->prev=cur;
 	cur->next=n;
 	if(n->next)n->next->prev=n;  else end=n;
 }
}

template<class T> void CList2<T>::AddBefore (T &x)
{
 CList2Node<T> *n = new CList2Node<T>(); n->v = x;
 if(IsEmpty()) {cur=begin=end=n;}
 else
 {
 	n->prev=cur->prev; n->next=cur;
 	cur->prev=n;
 	if(n->prev)n->prev->next=n;  else begin=n;
 }
}

template<class T> int CList2<T>::DelCur ()
{
 if(IsEmpty()) return -1; 
 CList2Node<T> *cur_save = cur;
 if(cur->prev) cur->prev->next=cur->next; else begin=cur->next;
 if(cur->next) cur->next->prev=cur->prev; else end=cur->prev;
 if(cur->prev) cur=cur->prev; else cur=cur->next;
 delete cur_save;
 return 0;
}
