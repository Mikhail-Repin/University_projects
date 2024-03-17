#include <iostream>
#include <list>
#include <iterator>
#include "convex.hpp"
#define eps 1e-15
int main(void)
{
double a, b;
list<CDot> pts, res;
char name[256];
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
while(fscanf(fp, "%lg %lg\n", &a, &b) == 2)
{
	pts.push_back(CDot(a, b));	
}
fclose(fp);
res = convex_hul(pts); 
for(list<CDot>::iterator it = res.begin(); it != res.end(); ++it)
{
	(*it).print(); cout<<endl;
}
return 0;
}