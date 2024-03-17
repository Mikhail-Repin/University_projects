#include <iostream>
#include "CDomain.hpp"
#include <math.h>
#include <cstring>
#define eps 1e-15
using namespace std;

int main (void)
{
 CDot *mas, *two, *three;
 int i, N, N1, N2, command, n;
 double a, b;
 char name[256], str[256];
 CList2<CDomain> D;
 CList2<CDomain>::iterator jt;
 CList2<CList2<CDot>> NEW, NEW1;
 FILE *fp;
 cout<<"Enter the name of file: \n";
 cin>>name;
 fp=fopen(name,"r");
 if (fp == NULL)
 {
 	fclose(fp);
        cout<<"Cannot open the file!\n";
        return -1;
 }
 if (!fgets(str, sizeof(str)-1, fp)) {fclose(fp); printf("Empty file!\n"); return -2;}
 sscanf(str, "%d", &N);
 mas = new CDot [N];
 for (int i = 0; i < N; i++)
 { 
 	if (!fscanf(fp, "(%lg ; %lg) ", &a, &b))
 	{
 		fclose(fp);
        	printf("Wrong data!\n");
        	delete [] mas; mas = NULL;
        	return -2;
 	}
 	mas[i] = CDot(a, b);
 }
 CDomain A(mas, N);
 delete [] mas; mas = NULL;
 if (!fgets(str, sizeof(str)-1, fp)) {fclose(fp); printf("Empty file!\n"); return -2;}
 sscanf(str, "%d", &N1);
 two = new CDot [N1];
 for (int i = 0; i < N1; i++)
 { 
 	if (!fscanf(fp, "(%lg ; %lg) ", &a, &b))
 	{
 		fclose(fp);
        	printf("Wrong data!\n");
        	delete [] two; two = NULL;
        	return -2;
 	}
 	two[i] = CDot(a, b);
 }
 if (!fgets(str, sizeof(str)-1, fp)) {fclose(fp); printf("Empty file!\n"); return -2;}
 sscanf(str, "%d", &N2);
 three = new CDot [N2];
 for (int i = 0; i < N2; i++)
 { 
 	if (!fscanf(fp, "(%lg ; %lg) ", &a, &b))
 	{
 		fclose(fp);
        	printf("Wrong data!\n");
        	delete [] three; three = NULL;
        	return -2;
 	}
 	three[i] = CDot(a, b);
 }
 printcommands();
 for(;;)
 {
 	cout<<"\nEnter your command (0 - help, 11 - exit): \n";
 	cin>>command;
 	switch(command)
 	{
 		case 0:
			printcommands();
	                break;
	        case 1:
                        if(!D.IsEmpty())
                        {
			for(jt=D.first(); jt!=D.last(); ++jt)
			{
				(*jt).print(); cout<<endl;
			}
                        (*jt).print(); cout<<endl;
                        }
                        else cout<<"The domain is empty!\n";
                        break;
                case 2:
                        cout<<"Empty domain is created\n";
                        break;
                case 3:
                        try
                        {       
				CDomain B(two, N1);
                                CDomain C(three, N2);
                        	D.AddAfter(B);
	                        D.AddAfter(C);
	                        D.GoToNext();
                                break;
                        }
                        catch(...)
                        {
                        	cout<<"Empty domain has not created yet!\n";
                                break;
                        }
                case 4:
                        jt=D.first();
                        if(!D.IsEmpty())
                        {
				cout<<"Enter your coordinates:\n ";
				cin>>a>>b;
               			CDot t(a, b);
				cout<<"Enter number of dot:\n ";
				cin>>n;
				CDomain T;
				T=(*jt);
				T.ChangeDot(t, n);
				cout<<"The 1 Domain now: "; T.print(); cout<<endl;
		        }
		        else cout<<"The domain is empty!\n";
                        break;
 		case 5:
			cout<<"Intersecting (test) Domain A: ";
			A.print(); cout<<endl;
			for(jt=D.first(); jt!=D.last(); ++jt)
			{
				NEW = (*jt).Intersection(A);
 				cout<<"Intersect with Domain A: \n";
 				PrintLL(NEW);
				NEW.Clean();
			}
				NEW = (*jt).Intersection(A);
 				cout<<"Intersect with Domain A: \n";
 				PrintLL(NEW);
                                NEW.Clean();
                        break;
		case 6:
			cout<<"Uniting (test) Domain A: ";
			A.print(); cout<<endl;
			for(jt=D.first(); jt!=D.last(); ++jt)
			{
				NEW = (*jt).Union(A);
 				cout<<"Union with Domain A: \n";
 				PrintLL(NEW);
                                NEW.Clean();
			}
				NEW = (*jt).Union(A);
 				cout<<"Union with Domain A: \n";
 				PrintLL(NEW);
                                NEW.Clean();
                       break;
		case 7:
                        jt=D.first();
                        if(!D.IsEmpty())
                        {
				CDomain T1;
				T1=(*jt);
				cout<<"Enter number of side:\n ";
				cin>>n;
				cout<<"Enter the ratio:\n ";
				cin>>a;
				T1.AddToSide(a,n);
				cout<<"New dot added to the reqiured side of 1 Domain: "; T1.print(); cout<<endl;
		        }
		        else cout<<"The domain is empty!\n";
                        break;
		case 8:
                        jt=D.first();
                        if(!D.IsEmpty())
                        {
				CDomain T2;
				T2=(*jt);
				cout<<"1 Domain before deleting dot:\n";
				T2.print(); cout<<endl;
				cout<<"Enter number of dot:\n ";
				cin>>n;
				T2.DelDot(n);
				cout<<"1 Domain after deleting required dot:\n"; T2.print(); cout<<endl;
		        }
		        else cout<<"The domain is empty!\n";
                        break;
		case 9:
                        jt=D.first();
                        if(!D.IsEmpty())
                        {
                                int bol;
				CDomain T3;
				T3=(*jt);
				cout<<"Enter your coordinates:\n ";
				cin>>a>>b;
				CDot tmp(a,b);
				bol=T3.IsInside(tmp);
				cout<<"For 1 Domain required dot is";
 				if(bol == 1) cout<<" Inside"<<endl;
 				if(bol == 2) cout<<" Outside"<<endl;
 				if(bol == 3) cout<<" On the frame"<<endl;
		        }
		        else cout<<"The domain is empty!\n";
                        break;
		case 10:
                        i=0;
			for(jt=D.first(); jt!=D.last(); ++jt)
			{
				++i;
				cout<<"For"<<i<<" Domain size is: "<<(*jt).Size()<<endl;		
	                }
				++i;
				cout<<"For"<<i<<" Domain size is: "<<(*jt).Size()<<endl;
                        break;
		case 11:
			cout<<"Have a nice day!\n";
			delete [] two; two = NULL;
			delete [] three; three = NULL;
			return 0;
		default:
			cout<<"Wrong command!\n";
			delete [] two; two = NULL;
			delete [] three; three = NULL;
			return -1;
 	}
 }
 return 0;
}
