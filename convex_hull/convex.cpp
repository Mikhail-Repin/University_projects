#include <iostream>
#include <list>
#include <iterator>
#include "convex.hpp"
#define eps 1e-15
using namespace std;

int eq (double a, double b) {return fabs(a-b) <= eps * fmax(fabs(a), fabs(b));}

CDot& CDot::operator= (const CDot &a)
{
fst = a.x(); 
scnd = a.y();   
proc = a.proc; 
return *this;
}


int CDot::operator!=(const CDot &b) {return ((fabs(fst-b.fst) > eps * fmax(fabs(fst), fabs(b.fst))) || (fabs(scnd-b.scnd) > eps * fmax(fabs(scnd), fabs(b.scnd))));}

int CDot::operator==(const CDot &b) {return ((fabs(fst-b.fst) <= eps * fmax(fabs(fst), fabs(b.fst))) && (fabs(scnd-b.scnd) <= eps * fmax(fabs(scnd), fabs(b.scnd))));}

list<CDot> convex_hul(list<CDot> pts)
{
	int pos=0, neg=0, i=0, fl1=0, fl2=0;
	list<CDot> Q, R, res;
	double A, B, C, t;
	list<CDot>::iterator It = pts.begin();
	list<CDot>::iterator Jt = ++It;
	--It;
        A = (*Jt).y()-(*It).y();
	B = (*It).x()-(*Jt).x();
	C = -(B*(*It).y()) - A*(*It).x();
	for(list<CDot>::iterator it = pts.begin(); it != pts.end(); ++it)
	{
		t = (*it).x() * A + (*it).y() * B + C;
		if(eq(t, 0)) ++pos;
		++i;
	}
	if(pos == i)
	{
		--Jt;
		CDot mi, ma;
		mi = ma = (*Jt);
		for(list<CDot>::iterator it = pts.begin(); it != pts.end(); ++it)
		{
	        	if(((*it).x() > ma.x() && eq((*it).y(), ma.y())) || ((*it).y() > ma.y() && eq((*it).x(), ma.x())) || ((*it).x() > ma.x() && (*it).y() > ma.y())) ma = (*it);	
		        if(((*it).x() < mi.x() && eq((*it).y(), mi.y())) || ((*it).y() < mi.y() && eq((*it).x(), mi.x())) || ((*it).x() < mi.x() && (*it).y() < mi.y())) mi = (*it);
		}
		res.push_front(ma);
		res.push_front(mi);
		return res;
	}
	pos = 0;
	for(list<CDot>::iterator it = pts.begin(); it != pts.end(); ++it)
	{
	        It=(++it);
		--it;
		for(list<CDot>::iterator jt = It; jt != pts.end(); ++jt)
		{
			A = (*jt).y()-(*it).y();
			B = (*it).x()-(*jt).x();
			C = -(B*((*it).y())) - A*((*it).x());
			for(list<CDot>::iterator itt = pts.begin(); itt != pts.end(); ++itt)
			{
				t = (*itt).x() * A + (*itt).y() * B + C;
				if(t > 0 ) ++pos;
				if(t < 0 ) ++neg;
			}
			if(pos == 0 || neg == 0)
			{
				for(list<CDot>::iterator itt = Q.begin(); itt != Q.end(); ++itt)
				{
					if((*itt) == (*it))  ++fl1;
					if((*itt) == (*jt))  ++fl2;
				}
				if(fl1 == 0) Q.push_back((*it));
				if(fl2 == 0) Q.push_back((*jt));
				R.push_back((*it)); R.push_back((*jt));
				fl1=fl2=0;
			}
			pos=neg=0;	
		}
	}

	for(list<CDot>::iterator it = Q.begin(); it != Q.end(); ++it)
	{
		for(list<CDot>::iterator jt = it; jt != Q.end(); ++jt)
		{
			A = (*jt).y()-(*it).y();
			B = (*it).x()-(*jt).x();
			C = -(B*((*it).y())) - A*((*it).x());
			for(list<CDot>::iterator itt = Q.begin(); itt != Q.end(); ++itt)
			{
				t = (*itt).x() * A + (*itt).y() * B + C;
				if(eq(t, 0))
				{
					double t1 = pow((((*itt).x()-(*it).x())*((*itt).x()-(*it).x()) + ((*itt).y()-(*it).y())*((*itt).y()-(*it).y())), 0.5) / pow((((*jt).x()-(*it).x())*((*jt).x()-(*it).x()) + ((*jt).y()-(*it).y())*((*jt).y()-(*it).y())), 0.5);
					double t3 = pow((((*itt).x()-(*jt).x())*((*itt).x()-(*jt).x()) + ((*itt).y()-(*jt).y())*((*itt).y()-(*jt).y())), 0.5) / pow((((*jt).x()-(*it).x())*((*jt).x()-(*it).x()) + ((*jt).y()-(*it).y())*((*jt).y()-(*it).y())), 0.5);
                                        if (t1>0 && t1<1 && t3>0 && t3<1)
					{
						Q.erase(itt);
					}
				}
			}
		}
	}
	for(list<CDot>::iterator it = R.begin(); it != R.end(); ++it)
	{
		fl1=fl2=0;
		for(list<CDot>::iterator jt = Q.begin(); jt != Q.end(); ++jt)
		{
			if((*it) == (*jt)) ++fl1;
		}
		++it;
		for(list<CDot>::iterator jt = Q.begin(); jt != Q.end(); ++jt)
		{
			if((*it) == (*jt)) ++fl2;
		}
		--it;
		if(fl1 == 0 || fl2 == 0)
		{
			R.erase(it);
			++it;
			R.erase(it);
			continue;
		}
		fl1=fl2=0;
		++it;
	}
	It = R.begin();
	CDot tmp = (*It), tmp1;
	res.push_back((*It)); 
	It = R.erase(It);
    	res.push_back((*It));
	tmp1= (*It);
	R.erase(It);
	do
	{
		i = 0;
		for(list<CDot>::iterator it = R.begin(); it != R.end(); ++it)
		{
			if(tmp1 == (*it)) 
			{
				It = it;
				if(i%2 == 0)
				{
					++it;
					res.push_back((*it));
					tmp1= (*it);
					R.erase(it);
					R.erase(It); 
					break;
				}
				if(i%2 == 1)
				{
					--it;
					res.push_back((*it));
					tmp1= (*it);
					R.erase(it);
					R.erase(It);
					break;
				}
			}
			++i;
		}
	}while(tmp1 != tmp);
	return res;	
}
