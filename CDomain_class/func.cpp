#include <iostream>
#include "CDomain.hpp"
#include <math.h>
#define eps 1e-15
using namespace std;

double Det (double a, double b, double c, double d) 
{
	return a * d - b * c;
}

int eq (double a, double b) {return fabs(a-b) <= eps * fmax(fabs(a), fabs(b));}

int intersect (CDot a, CDot b,CDot o, CDot vec, CDot & res) 
{
	double A1,B1,C1,A2,B2,C2, zn;
        A1=b.y()-a.y();
        B1=a.x()-b.x();
        C1= -(B1*a.y()) - A1*a.x();
        A2=vec.y();
        B2= -(vec.x());
        C2= vec.x()*o.y()-vec.y()*o.x();
	zn = Det (A1, B1, A2, B2);
	if (fabs (zn) < eps && fabs(Det (A1, C1, A2, C2)) < eps && fabs(Det(B1, C1, B2, C2)) < eps) return -1;
	else if(fabs (zn) < eps) return 0;
	res=CDot(- Det (C1, B1, C2, B2) / zn, - Det (A1, C1, A2, C2) / zn);
	return 1;
}

void PrintLL(CList2<CList2<CDot>> & NEW)
{
  for(CList2<CList2<CDot>>::iterator it=NEW.first(); it!=NEW.last(); ++it)
  {
  	for(CList2<CDot>::iterator jt=(it.cur->v.begin); jt!=(it.cur->v.end); ++jt) 
        {
		(*jt).print();
        }
        cout<<endl;
  }
  return;
}

void printcommands()
{
	cout<<"\nCommands:\n";
        cout<<"\n0 - Help (print commands again)\n";
	cout<<"\n1 - Print Domain\n";
	cout<<"\n2 - Create an empty Domain\n";
	cout<<"\n3 - Fill in Domain with array of points\n";
	cout<<"\n4 - Change a certain point\n";
	cout<<"\n5 - Intersection with another Domain\n";
	cout<<"\n6 - Union with another Domain\n";
	cout<<"\n7 - Add dot to side\n";
	cout<<"\n8 - Delete a certain dot\n";
	cout<<"\n9 - Where is a sertain dot relatively the Domain\n";
	cout<<"\n10 - Sizes of Domains\n";
	cout<<"\n11 - Exit\n";
	return;
}

CDot& CDot::operator= (const CDot &a)
{
fst = a.x(); 
scnd = a.y(); 
cross = a.cross;
ins = a.ins; 
ent_ex = a.ent_ex; 
t = a.t;
dop = a.dop; 
nextPoly = a.nextPoly; 
neib = a.neib; 
proc = a.proc; 
return *this;
}


int CDot::operator!=(const CDot &b) {return ((fabs(fst-b.fst) > eps * fmax(fabs(fst), fabs(b.fst))) || (fabs(scnd-b.scnd) > eps * fmax(fabs(scnd), fabs(b.scnd))));}

int CDot::operator==(const CDot &b) {return ((fabs(fst-b.fst) <= eps * fmax(fabs(fst), fabs(b.fst))) && (fabs(scnd-b.scnd) <= eps * fmax(fabs(scnd), fabs(b.scnd))));}

int CDomain::ChangeDot(CDot &d,int n) 
{
	if(domain.IsEmpty()) return -1;
	if(n>Size()) return -2;
	CDot A, B;
	double t;
	t=0;
        domain.GoToBegin();
	for(int i=1; i<n; i++) {domain.GoToNext();}
        if(IsInside(d)==3) goto label;
	if(!domain.GoToPrev()) {domain.GetCur(A); domain.GoToNext();} else goto label;
        if(!domain.GoToNext()) {domain.GetCur(B); domain.GoToPrev();} else goto label;
        t=Det(d.x()-A.x(), B.x()-A.x(), d.y()-A.y(),B.y()-A.y());
        if(fabs(t-0) <= eps * fmax(fabs(t), fabs(0.))) return -3;
	label:;
	domain.AddAfter(d); domain.DelCur();
	return 0;
}

int CDomain::AddDot(CDot &d, int n)
{
	if(domain.IsEmpty()) return -1;
	if(n>Size()) return -2;
	CDot A, B;
	double t;
	t=0;
        domain.GoToBegin();
	for(int i=1; i<n; i++) {domain.GoToNext();}
	if(IsInside(d)==3) goto label;
	if(!domain.GoToPrev()) {domain.GetCur(A); domain.GoToNext();} else goto label;
        if(!domain.GoToNext()) {domain.GetCur(B); domain.GoToPrev();} else goto label;
        t=Det(d.x()-A.x(), B.x()-A.x(), d.y()-A.y(),B.y()-A.y());
        if(fabs(t-0) <= eps * fmax(fabs(t), fabs(0.))) //return -3;
	label:;
	domain.AddAfter(d);
	return 0;
} 

int CDomain::Size() 
{
	int n=0;
	if(domain.IsEmpty()) return 0; 
	for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it) {n++;};
	return n;
}

int CDomain::DelDot(int n) 
{	
	
	if(domain.IsEmpty()) return -1;
	if(n>Size()) return -2;
        domain.GoToBegin();
	for(int i=1; i<n; i++) {domain.GoToNext();}
        domain.DelCur();
        return 0;
}

int CDomain::AddToSide(double t, int n)
{
	if (t<0 || t>1) return -4;
	if(domain.IsEmpty()) return -1;
	if(n>Size()) return -2;
	CDot A, B;
        domain.GoToBegin();
        for(int i=1; i<n; i++) {domain.GoToNext();}
        domain.GetCur(A);
        if(!domain.GoToNext()) {domain.GetCur(B); domain.GoToPrev();} else return -3;
        CDot New(A.x() + t*(B.x()-A.x()), A.y() + t*(B.y()-A.y()));
        domain.AddAfter(New);
        return 0;
}

int CDomain::IsInside(CDot d)
{
	if(domain.IsEmpty()) return 2;
	int i1, i2, bol, flg;
	i1 = i2 = flg =  0;
        for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
        {
                CDot a = *it, vec(1,0), res; ++it; CDot b = *it; --it;
                bol =  intersect (a , b ,d, vec , res);
                if(bol==1)
                {
                	if((res.x() > d.x()))
                	{
                		if ( (a == res &&  b.y() < d.y()) || (b == res &&  a.y() < d.y()) ) ++i1;
                		else if (a != res && (((fabs((res.x()-a.x()) / (b.x()-a.x())) < 1) && (fabs((res.x()-b.x()) / (b.x()-a.x())) < 1)) || ((fabs((res.y()-a.y()) / (b.y()-a.y())) < 1) && (fabs((res.y()-b.y()) / (b.y()-a.y())) < 1)))) ++i1; 
                	}
                	else if ((res.x() < d.x()))
                	{
                                if ( (a == res &&  b.y() < d.y()) || (b == res &&  a.y() < d.y()) ) ++i2;
                		else if (a != res && (((fabs((res.x()-a.x()) / (b.x()-a.x())) < 1) && (fabs((res.x()-b.x()) / (b.x()-a.x())) < 1)) || ((fabs((res.y()-a.y()) / (b.y()-a.y())) < 1) && (fabs((res.y()-b.y()) / (b.y()-a.y())) < 1)))) ++i2;
                	}
                	if(res == d) {if(d == a || d == b || (d.x() > a.x() && d.x() < b.x()) || (d.x() > b.x() && d.x() < a.x()) || (d.y() > a.y() && d.y() < b.y()) || (d.y() > b.y() && d.y() < a.y()))  flg++;}
                }
                if(bol==-1)
                {
                	if(d == a || d == b || (d.x() > a.x() && d.x() < b.x()) || (d.x() > b.x() && d.x() < a.x()))  flg++;
                }
	}
        if(flg) return 3;
        if(i1 % 2 == 1 && i2 % 2 == 1) return 1;
        if(i1 % 2 == 0 && i2 % 2 == 0) return 2;
        return 0;
}

CList2<CList2<CDot>> CDomain::Intersection(CDomain B)
{
	CList2<CList2<CDot>> NEW;
	CList2<CDot> SB;
	CList2<CDot> CL;
	CDot D1,D2, fict;
	int i, j, z, log1=0, log2=0, log3=0;
	z=i=j=0;
	for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
	{
		++i;
		CDot S1 = *it; ++it; CDot S2 = *it;  --it;
		for(CList2<CDot>::iterator jt=B.domain.first(); jt!=B.domain.last(); ++jt)
		{
			++j;
                        CDot C1 = *jt, res; ++jt; CDot C2 = *jt; CDot vec (C2.x()-C1.x(), C2.y()-C1.y()); --jt;
			int bol=intersect(S1, S2, C1, vec, res);
			if(bol==1)
			{ 
                        	double t1 = pow(((res.x()-S1.x())*(res.x()-S1.x()) + (res.y()-S1.y())*(res.y()-S1.y())), 0.5) / pow(((S2.x()-S1.x())*(S2.x()-S1.x()) + (S2.y()-S1.y())*(S2.y()-S1.y())), 0.5);
                                double t2 = pow(((res.x()-C1.x())*(res.x()-C1.x()) + (res.y()-C1.y())*(res.y()-C1.y())), 0.5) / pow(((C2.x()-C1.x())*(C2.x()-C1.x()) + (C2.y()-C1.y())*(C2.y()-C1.y())), 0.5);
                                double t3 = pow(((res.x()-S2.x())*(res.x()-S2.x()) + (res.y()-S2.y())*(res.y()-S2.y())), 0.5) / pow(((S2.x()-S1.x())*(S2.x()-S1.x()) + (S2.y()-S1.y())*(S2.y()-S1.y())), 0.5);
                                double t4 = pow(((res.x()-C2.x())*(res.x()-C2.x()) + (res.y()-C2.y())*(res.y()-C2.y())), 0.5) / pow(((C2.x()-C1.x())*(C2.x()-C1.x()) + (C2.y()-C1.y())*(C2.y()-C1.y())), 0.5);
				log1 = (t1>0 && t1<1 && t2>0 && t2<1 && t3>0 && t3<1 && t4>0 && t4<1);
				log2 = ( (t1>0 && t1<1 && t3>0 && t3<1 &&(res == C1)) || (t2>0 && t2<1 && t4>0 && t4<1 &&(res == S1) ));
				log3 = ((res == S1 && res == C1));
				if ( (log1 || log2 || log3))
				{
					 ++z;
                                         CDot res2; res2 = res;
                                         res.t = t1; res2.t = t2;
                                         res.cross = 1; res2.cross = 1;
				         if(res != S1 && res != S2) {
						res.ins = i; 
						SB.AddAfter(res); SB.GoToNext();
					 }
					 else 
					 {
						res.vert = 1;
					 }
					 if(res != C1 && res != C2){ 
					 	res2.ins = j;
						CL.AddAfter(res2); CL.GoToNext();
					 }
					 else {res2.vert = 1;}
                                         res.neib = jt.cur; res2.neib = it.cur;
					 if(res == S1) {it.cur->v = res;}
					 if(res == S2) {++it; it.cur->v = res; --it;}
					 if(res == C1) {jt.cur->v = res;}
					 if(res == C2) {++jt; jt.cur->v = res; --jt;}
				}	
			}
		}
		j=0;
	}
	SB.GoToEnd(); SB.AddAfter(fict);
	CL.GoToEnd(); CL.AddAfter(fict);
			for(CList2<CDot>::iterator itt=SB.first(); itt!=SB.last(); ++itt)
			{
				(*itt);
				CList2<CDot>::iterator jt=SB.last(); --jt;
				for(CList2<CDot>::iterator jtt=jt; jtt!=itt; --jtt)
				{
					--jtt;  CList2<CDot>::iterator jtt1 = jtt; ++jtt;
					if((*jtt1).ins>(*jtt).ins)
					{
						CDot tmp =  (*jtt1);
						jtt1.cur->v = (*jtt);
						jtt.cur->v = tmp;
					}
				}	
			} 

	for(CList2<CDot>::iterator it=SB.first(); it!=SB.last(); ++it)
	{
		if((*it).cross == 1 && (*it).ins > 0)
		{
			CList2<CDot>::iterator jt = it;
			do
			{
			 ++jt;
			}while((*jt).ins == (*it).ins);
			for(CList2<CDot>::iterator itt=it; itt!=jt; ++itt)
			{
				--jt;
				for(CList2<CDot>::iterator jtt=jt; jtt!=itt; --jtt)
				{
					--jtt;  CList2<CDot>::iterator jtt1 = jtt; ++jtt;
					if((*jtt1).t>(*jtt).t)
					{
						CDot tmp =  (*jtt1);
						jtt1.cur->v = (*jtt);
						jtt.cur->v = tmp;
					}
				}
				++jt;	
			}	
		}
	}
	for(CList2<CDot>::iterator it=SB.first(); it!=SB.last(); ++it)
	{
		if((*it).cross == 1 && (*it).ins > 0)
		{
			++it;
			for(CList2<CDot>::iterator jt=it; jt!=SB.last(); ++jt)
			{
				if((*jt).cross == 1 && (*jt).ins > 0)
				{
					((jt.cur->v).ins)++;
				}
			}
			--it;
		}
	}
	SB.GoToEnd(); SB.DelCur();
			for(CList2<CDot>::iterator itt=CL.first(); itt!=CL.last(); ++itt)
			{
				CList2<CDot>::iterator jt=CL.last(); --jt;
				for(CList2<CDot>::iterator jtt=jt; jtt!=itt; --jtt)
				{
					--jtt;  CList2<CDot>::iterator jtt1 = jtt; ++jtt;
					if((*jtt1).ins>(*jtt).ins)
					{
						CDot tmp =  (*jtt1);
						jtt1.cur->v = (*jtt);
						jtt.cur->v = tmp;
					}
				}	
			}
	for(CList2<CDot>::iterator it=CL.first(); it!=CL.last(); ++it)
	{
		if((*it).cross == 1 && (*it).ins > 0)
		{
			CList2<CDot>::iterator jt = it;
			do
			{
			 ++jt;
			}while((*jt).ins == (*it).ins);
			for(CList2<CDot>::iterator itt=it; itt!=jt; ++itt)
			{
				--jt;
				for(CList2<CDot>::iterator jtt=jt; jtt!=itt; --jtt)
				{
					--jtt;  CList2<CDot>::iterator jtt1 = jtt; ++jtt;
					if((*jtt1).t>(*jtt).t)
					{
						CDot tmp =  (*jtt1);
						jtt1.cur->v = (*jtt);
						jtt.cur->v = tmp;
					}
				}
				++jt;	
			}	
		}
	}
	for(CList2<CDot>::iterator it=CL.first(); it!=CL.last(); ++it)
	{
		if((*it).cross == 1 && (*it).ins > 0)
		{
			++it;
			for(CList2<CDot>::iterator jt=it; jt!=CL.last(); ++jt)
			{
				if((*jt).cross == 1 && (*jt).ins > 0)
				{
					((jt.cur->v).ins)++;
				}
			}
			--it;
		}
	}
	CL.GoToEnd(); CL.DelCur();
	SB.GoToBegin();
	CL.GoToBegin();
	if(!SB.IsEmpty()){
	do
	{
		SB.GetCur(D1);
		AddDot(D1,D1.ins);
	}
	while(!SB.GoToNext()); }
	if(!CL.IsEmpty())
	{
	do
	{
		CL.GetCur(D2);
		B.AddDot(D2,D2.ins);
	}
	while(!CL.GoToNext()); }
	for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
	{
		for(CList2<CDot>::iterator jt=B.domain.first(); jt!=B.domain.last(); ++jt)
		{
			if(((*it) == (*jt)) && (*it).cross == 1)
			{
				(it.cur->v).neib=jt.cur;
				(jt.cur->v).neib=it.cur;
			}
		}	
	}
		if(!z)
		{
                        CList2<CDot>::iterator it=domain.first();
                        CList2<CDot>::iterator jt=B.domain.first();
			if(B.IsInside(*it)==1 || B.IsInside(*it)==3) NEW.AddAfter(B.domain);
                        if(IsInside(*jt)==1) NEW.AddAfter(domain);
                        else {NEW.AddAfter(domain); NEW.AddAfter(B.domain);}
                        return NEW;
		}
        int stat;
        for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
        {
        	if(it==domain.first())
                {
                	int bol = B.IsInside(*it);
                	if(bol == 1 || bol == 3) {stat = 0;}
                	else {stat = 1;}
               	}
                if((*it).cross == 1) {(it.cur->v).ent_ex = stat; stat=(!stat);}
        }

        for(CList2<CDot>::iterator it=B.domain.first(); it!=B.domain.last(); ++it)
        {
        	if(it==B.domain.first())
                {
                	int bol = IsInside(*it);
                	if(bol == 1 || bol == 3) {stat = 0;}
                	else {stat = 1;}
               	}
                if((*it).cross == 1) {(it.cur->v).ent_ex = stat; stat=(!stat);}
        }
            
        for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
        {
                if((*it).cross == 1 && (*it).proc != 1)
                {
                	CList2<CDot> news;
                        CList2<CDot>::iterator jt=it;
                        CDot A=*jt;
			news.AddAfter((A));
                        (jt.cur->v).proc = 1;
                	do
                	{
				(jt.cur->v).proc = 1;
                        	news.GoToNext();
                		if((*jt).ent_ex==1)
                		{
                			do
                			{
                                        	if(jt==domain.last()) jt=domain.first();
                                                if(jt==B.domain.last())jt=B.domain.first();
                				++jt;
                                                A=*jt;
                                               	news.AddAfter(A);
                                               	news.GoToNext();
                                                (jt.cur->v).proc = 1;
                			}while((*jt).cross != 1); 
                		}
                		else
                		{
                			do
                			{
                                        	if(jt==domain.first()) jt=domain.last();
                                                if(jt==B.domain.first())jt=B.domain.last();
                				--jt;
                                                A=*jt;
                                               	news.AddAfter(A);
                                                news.GoToNext();
                                                (jt.cur->v).proc = 1;
                			}while((*jt).cross != 1);
	                        }
	                        jt.cur = (*jt).neib;
                	} while((*jt) != (*it));
                	NEW.AddAfter(news);
                	NEW.GoToNext();
		}
	}
        NEW.GoToEnd();
        CList2<CDot> tmp;
        NEW.AddAfter(tmp);
        NEW.GoToBegin();
        i = 0;
                for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
                {
                        ++i;
                	if((*it).cross == 1) {DelDot(i); --i;}	
                }
                i = 0;
                for(CList2<CDot>::iterator it=B.domain.first(); it!=B.domain.last(); ++it)
                {
                        ++i;
                	if((*it).cross == 1) {B.DelDot(i); --i;}	
                }
        return NEW;
}

        CList2<CList2<CDot>> CDomain::Union(CDomain B)
        {
	CList2<CList2<CDot>> NEW;
	CList2<CDot> SB;
	CList2<CDot> CL;
	CDot D1,D2, fict;
	int i, j, z, log1=0, log2=0, log3=0;
	z=i=j=0;
	for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
	{
		++i;
		CDot S1 = *it; ++it; CDot S2 = *it;  --it;
		for(CList2<CDot>::iterator jt=B.domain.first(); jt!=B.domain.last(); ++jt)
		{
			++j;
                        CDot C1 = *jt, res; ++jt; CDot C2 = *jt; CDot vec (C2.x()-C1.x(), C2.y()-C1.y()); --jt;
			int bol=intersect(S1, S2, C1, vec, res);
			if(bol==1)
			{ 
                        	double t1 = pow(((res.x()-S1.x())*(res.x()-S1.x()) + (res.y()-S1.y())*(res.y()-S1.y())), 0.5) / pow(((S2.x()-S1.x())*(S2.x()-S1.x()) + (S2.y()-S1.y())*(S2.y()-S1.y())), 0.5);
                                double t2 = pow(((res.x()-C1.x())*(res.x()-C1.x()) + (res.y()-C1.y())*(res.y()-C1.y())), 0.5) / pow(((C2.x()-C1.x())*(C2.x()-C1.x()) + (C2.y()-C1.y())*(C2.y()-C1.y())), 0.5);
                                double t3 = pow(((res.x()-S2.x())*(res.x()-S2.x()) + (res.y()-S2.y())*(res.y()-S2.y())), 0.5) / pow(((S2.x()-S1.x())*(S2.x()-S1.x()) + (S2.y()-S1.y())*(S2.y()-S1.y())), 0.5);
                                double t4 = pow(((res.x()-C2.x())*(res.x()-C2.x()) + (res.y()-C2.y())*(res.y()-C2.y())), 0.5) / pow(((C2.x()-C1.x())*(C2.x()-C1.x()) + (C2.y()-C1.y())*(C2.y()-C1.y())), 0.5);
				log1 = (t1>0 && t1<1 && t2>0 && t2<1 && t3>0 && t3<1 && t4>0 && t4<1);
				log2 = ( (t1>0 && t1<1 && t3>0 && t3<1 &&(res == C1)) || (t2>0 && t2<1 && t4>0 && t4<1 &&(res == S1) ));
				log3 = ((res == S1 && res == C1));
				if ( (log1 || log2 || log3))
				{
					 ++z;
                                         CDot res2; res2 = res;
                                         res.t = t1; res2.t = t2;
                                         res.cross = 1; res2.cross = 1;
				         if(res != S1 && res != S2) {
						res.ins = i; 
						SB.AddAfter(res); SB.GoToNext();
					 }
					 else 
					 {
						res.vert = 1;
					 }
					 if(res != C1 && res != C2){ 
					 	res2.ins = j;
						CL.AddAfter(res2); CL.GoToNext();
					 }
					 else {res2.vert = 1;}
                                         res.neib = jt.cur; res2.neib = it.cur;
					 if(res == S1) {it.cur->v = res;}
					 if(res == S2) {++it; it.cur->v = res; --it;}
					 if(res == C1) {jt.cur->v = res;}
					 if(res == C2) {++jt; jt.cur->v = res; --jt;}
				}	
			}
		}
		j=0;
	}
	SB.GoToEnd(); SB.AddAfter(fict);
	CL.GoToEnd(); CL.AddAfter(fict);
			for(CList2<CDot>::iterator itt=SB.first(); itt!=SB.last(); ++itt)
			{
				(*itt);
				CList2<CDot>::iterator jt=SB.last(); --jt;
				for(CList2<CDot>::iterator jtt=jt; jtt!=itt; --jtt)
				{
					--jtt;  CList2<CDot>::iterator jtt1 = jtt; ++jtt;
					if((*jtt1).ins>(*jtt).ins)
					{
						CDot tmp =  (*jtt1);
						jtt1.cur->v = (*jtt);
						jtt.cur->v = tmp;
					}
				}	
			} 

	for(CList2<CDot>::iterator it=SB.first(); it!=SB.last(); ++it)
	{
		if((*it).cross == 1 && (*it).ins > 0)
		{
			CList2<CDot>::iterator jt = it;
			do
			{
			 ++jt;
			}while((*jt).ins == (*it).ins);
			for(CList2<CDot>::iterator itt=it; itt!=jt; ++itt)
			{
				--jt;
				for(CList2<CDot>::iterator jtt=jt; jtt!=itt; --jtt)
				{
					--jtt;  CList2<CDot>::iterator jtt1 = jtt; ++jtt;
					if((*jtt1).t>(*jtt).t)
					{
						CDot tmp =  (*jtt1);
						jtt1.cur->v = (*jtt);
						jtt.cur->v = tmp;
					}
				}
				++jt;	
			}	
		}
	}
	for(CList2<CDot>::iterator it=SB.first(); it!=SB.last(); ++it)
	{
		if((*it).cross == 1 && (*it).ins > 0)
		{
			++it;
			for(CList2<CDot>::iterator jt=it; jt!=SB.last(); ++jt)
			{
				if((*jt).cross == 1 && (*jt).ins > 0)
				{
					((jt.cur->v).ins)++;
				}
			}
			--it;
		}
	}
	SB.GoToEnd(); SB.DelCur();
			for(CList2<CDot>::iterator itt=CL.first(); itt!=CL.last(); ++itt)
			{
				CList2<CDot>::iterator jt=CL.last(); --jt;
				for(CList2<CDot>::iterator jtt=jt; jtt!=itt; --jtt)
				{
					--jtt;  CList2<CDot>::iterator jtt1 = jtt; ++jtt;
					if((*jtt1).ins>(*jtt).ins)
					{
						CDot tmp =  (*jtt1);
						jtt1.cur->v = (*jtt);
						jtt.cur->v = tmp;
					}
				}	
			}
	for(CList2<CDot>::iterator it=CL.first(); it!=CL.last(); ++it)
	{
		if((*it).cross == 1 && (*it).ins > 0)
		{
			CList2<CDot>::iterator jt = it;
			do
			{
			 ++jt;
			}while((*jt).ins == (*it).ins);
			for(CList2<CDot>::iterator itt=it; itt!=jt; ++itt)
			{
				--jt;
				for(CList2<CDot>::iterator jtt=jt; jtt!=itt; --jtt)
				{
					--jtt;  CList2<CDot>::iterator jtt1 = jtt; ++jtt;
					if((*jtt1).t>(*jtt).t)
					{
						CDot tmp =  (*jtt1);
						jtt1.cur->v = (*jtt);
						jtt.cur->v = tmp;
					}
				}
				++jt;	
			}	
		}
	}
	for(CList2<CDot>::iterator it=CL.first(); it!=CL.last(); ++it)
	{
		if((*it).cross == 1 && (*it).ins > 0)
		{
			++it;
			for(CList2<CDot>::iterator jt=it; jt!=CL.last(); ++jt)
			{
				if((*jt).cross == 1 && (*jt).ins > 0)
				{
					((jt.cur->v).ins)++;
				}
			}
			--it;
		}
	}
	CL.GoToEnd(); CL.DelCur();
	SB.GoToBegin();
	CL.GoToBegin();
	if(!SB.IsEmpty()){
	do
	{
		SB.GetCur(D1);
		AddDot(D1,D1.ins);
	}
	while(!SB.GoToNext()); }
	if(!CL.IsEmpty())
	{
	do
	{
		CL.GetCur(D2);
		B.AddDot(D2,D2.ins);
	}
	while(!CL.GoToNext()); }
	for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
	{
		for(CList2<CDot>::iterator jt=B.domain.first(); jt!=B.domain.last(); ++jt)
		{
			if(((*it) == (*jt)) && (*it).cross == 1)
			{
				(it.cur->v).neib=jt.cur;
				(jt.cur->v).neib=it.cur;
			}
		}	
	}
		if(!z)
		{
                        CList2<CDot>::iterator it=domain.first();
                        CList2<CDot>::iterator jt=B.domain.first();
			if(B.IsInside(*it)==1 || B.IsInside(*it)==3) NEW.AddAfter(B.domain);
                        if(IsInside(*jt)==1) NEW.AddAfter(domain);
                        else {NEW.AddAfter(domain); NEW.AddAfter(B.domain);}
                        return NEW;
		}
                int stat;
                for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
                {
                        if(it==domain.first())
                        {
                        	int bol = B.IsInside(*it);
                		if(bol == 1 || bol == 3) {stat = 0;}
                		else {stat = 1;}
                	}
                        if((*it).cross == 1) {(it.cur->v).ent_ex = stat; stat=(!stat);}
                }

                for(CList2<CDot>::iterator it=B.domain.first(); it!=B.domain.last(); ++it)
                {
                        if(it==B.domain.first())
                        {
                        	int bol = IsInside(*it);
                		if(bol == 1 || bol == 3) {stat = 0;}
                		else {stat = 1;}
                	}
                        if((*it).cross == 1) {(it.cur->v).ent_ex = stat; stat=(!stat);}
                }
            
                for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
                {
                	if((*it).cross == 1 && (*it).proc != 1)
                	{
                                CList2<CDot> news;
                                CList2<CDot>::iterator jt=it;
                                CDot A=*jt;
				news.AddAfter((A));
                                (jt.cur->v).proc = 1;
                		do
                		{
					(jt.cur->v).proc = 1;
                                        news.GoToNext();
                			if((*jt).ent_ex==0)
                			{
                				do
                				{
                                                        if(jt==domain.last()) jt=domain.first();
                                                        if(jt==B.domain.last())jt=B.domain.first();
                					++jt;
                                                        A=*jt;
                                                	news.AddAfter(A);
                                                	news.GoToNext();
                                                        (jt.cur->v).proc = 1;
                				}while((*jt).cross != 1); 
                			}
                			else
                			{
                				do
                				{
                                                        if(jt==domain.first()) jt=domain.last();
                                                        if(jt==B.domain.first())jt=B.domain.last();
                					--jt;
                                                        A=*jt;
                                                	news.AddAfter(A);
                                                        news.GoToNext();
                                                        (jt.cur->v).proc = 1;
                				}while((*jt).cross != 1);
	                                }
	                                jt.cur = (*jt).neib;
                		} while((*jt) != (*it));
                		NEW.AddAfter(news);
                		NEW.GoToNext();
	                }
                }
                NEW.GoToEnd();
                CList2<CDot> tmp;
                NEW.AddAfter(tmp);
                NEW.GoToBegin();
                i = 0;
                for(CList2<CDot>::iterator it=domain.first(); it!=domain.last(); ++it)
                {
                        ++i;
                	if((*it).cross == 1) {DelDot(i); --i;}	
                }
                i = 0;
                for(CList2<CDot>::iterator it=B.domain.first(); it!=B.domain.last(); ++it)
                {
                        ++i;
                	if((*it).cross == 1) {B.DelDot(i); --i;}	
                }
                return NEW;
	} 