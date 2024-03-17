#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
class CComplex
{
	double re, im;
	public:
                CComplex() {re=0; im=0;}
		CComplex(double re, double im) {this->re=re; this->im=im;}
		double Re()const{return re;}
		double Im()const{return im;}
                CComplex& operator= (const CComplex &a){re = a.Re();  im = a.Im(); return *this;}
                CComplex(const CComplex &a){*this = a;}
		CComplex operator+ (const CComplex &b){CComplex res(re+b.Re(), im+b.Im()); return res;}
                CComplex operator- (const CComplex &b){CComplex res(re-b.Re(), im-b.Im()); return res;}
                CComplex operator* (const CComplex &b){CComplex res(re*b.Re()-im*b.Im(), im*b.Re()+re*b.Im()); return res;}
                CComplex operator/ (const CComplex &b){CComplex res((re*b.Re()+im*b.Im())/(b.Re()*b.Re()+b.Im()*b.Im()),(im*b.Re()-re*b.Im())/(b.Re()*b.Re()+b.Im()*b.Im())); return res;}
                void print() const {printf("%lg + %lgi \n", re, im);}
}; 
              
class CComplexVector
{
	CComplex *vect;
	int len; 
	public:
	CComplexVector()
	{
		vect = NULL;
	}
        CComplexVector(CComplex *v, int N)
	{
		vect = new CComplex[N];
		len = N;
		for(int i=0; i<len; i++)
		{
			vect[i]=v[i];
		}
	}
        CComplexVector(const CComplexVector &v) {(*this).CopyOnly(v);}
	void CopyOnly(const CComplexVector &v){vect = new CComplex[len]; for(int i=0; i<v.len; i++){vect[i]=v.vect[i];}}
	~CComplexVector(){delete [] vect; vect = NULL;}
	void Clean(){delete [] vect; vect = NULL; len = 0;}
        void Resize(int); 
        CComplexVector& operator=(const CComplexVector &);
        CComplex& operator[](int );
	CComplexVector operator+(const CComplexVector &);
	CComplexVector operator-(const CComplexVector &); 
	CComplex operator*(const CComplexVector &);
	void print() const
	{
        	printf("{");
		for(int i=0; i<len; i++)
		{
			printf("%lg + %lgi; ", vect[i].Re(), vect[i].Im());
		}
                printf("}\n");
	}
};
