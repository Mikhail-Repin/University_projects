#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "CComplexVector.hpp"

void CComplexVector::Resize(int i) 
{
	CComplex *w; 
	w = new CComplex[i+1]; 
	for(int t=0; t<len; t++) {w[t]=vect[t];} 
	for(int j=len; j<i+1; j++) w[j]=CComplex(0,0); 
	Clean(); len = i+1; 
	(*this)=CComplexVector(w,len); 
	delete [] w; w = NULL;
}

CComplexVector& CComplexVector::operator=(const CComplexVector &v)
{
	if (this!=&v)
	{
		Clean(); len = v.len; 
		(*this).CopyOnly(v);
	} 
	return *this;
}

CComplex& CComplexVector::operator[](int i)
{
	if(i < len) return vect[i];
	else {(*this).Resize(i);} return vect[i];
}

CComplexVector CComplexVector::operator+(const CComplexVector &v)
{
	CComplex *tmp; tmp = new CComplex[len];
	for(int i=0; i<len; i++)
	{
		tmp[i]=(vect[i]+v.vect[i]);
	}
        CComplexVector res(tmp, len);
        delete [] tmp; tmp = NULL;
	return res;
}

CComplexVector CComplexVector:: operator-(const CComplexVector &v) 
{
	CComplex *tmp; tmp = new CComplex[len];
	for(int i=0; i<len; i++)
	{
		tmp[i]=(vect[i]-v.vect[i]);	
	}
        CComplexVector res(tmp, len);
        delete [] tmp; tmp = NULL;
	return res;
}

CComplex CComplexVector:: operator*(const CComplexVector &v)
{
	CComplex res(0,0);
	for(int i=0; i<len; i++)
	{
		res=res+vect[i]*v.vect[i];	
	}
	return res;
}
