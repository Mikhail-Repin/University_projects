#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "CTree.hpp"
using namespace std;

int main(void)
{
CTree<string> T;
int command;
string S, key;
char log = '>';
char data[256];
/*
CTree<int>::CNode v(NULL, 5, "b");
T.insert(0,"ae");
T.insert(-1, "ad");
T.insert(4, "aed");
T.insert(2, "aeb");
T.insert(5, "aef");
T.insert(3, "aec");
T.insert(1, "aea");
T.insert(-2, "ac");
T.insert(-3, "ab");
cout<<"TREE:"<<" size:"<<T.size()<<endl;
cout<<"_______________________________"<<endl;
T.print(T.GetRoot(), 0);
cout<<"_______________________________"<<endl;
for(CTree<int>::iterator it=T.begin(); it!=T.end(); ++it)
{
	cout<<*it<<" "<<"height: "<<it.height()<<" balance: "<<it.balance()<<endl;
}
cout<<endl;
T.remove(3);
T.remove(5);
cout<<"TREE:"<<" size:"<<T.size()<<endl;
cout<<"_______________________________"<<endl;
T.print(T.GetRoot(), 0);
cout<<"_______________________________"<<endl;
for(CTree<int>::iterator it=T.begin(); it!=T.end(); ++it)
{
	cout<<*it<<" "<<"height: "<<it.height()<<" balance: "<<it.balance()<<endl;
}
T2.insert(9, "bd");
T2.insert(8, "bc");
T2.insert(12, "bfc");
T2.insert(11, "bfa");
T2.insert(7, "ba");
T2.insert(10, "bf");
T.Union(T2.GetRoot(), &v, T2.size());
cout<<"TREE:"<<" size:"<<T.size()<<endl;
cout<<"_______________________________"<<endl;
T.print(T.GetRoot(), 0);
cout<<"_______________________________"<<endl;*/
printcommands();
for(;;)
{
 	cout<<"\nEnter your command (0 - help, 7 - exit): \n";
 	cin>>command;
 	switch(command)
	{
		case 0:
			printcommands();
			break;
		case 1: 
			cout<<"after adding your data print '>' if you want to continue adiing, else: print '.'"<<endl;	
	                while(log=='>')
			{
				cout<<"\nEnter the key: "<<endl;
				cin>>key;
				cout<<"\nEnter data (string): "<<endl;
				cin>>S;
				if(T.insert(key, S)) cout<<"successfully"<<endl;
				else cout<<"wrong key!"<<endl;
				S.clear(); key.clear();
				cout<<"\nEnter > or . : "<<endl;
				cin>>log;
			}
			log = '>';
			break;
		case 2:
			const CTree<string>::CNode *n;
			cout<<"\nEnter the key: "<<endl;
			cin>>key;
			n = T.Find(key);
			cout<<(*(*n))<<"("<<n->height()<<" "<<n->balance()<<")"<<"::"<<n->Dat()<<endl;
			delete n; n=NULL;
			break;
		case 3:
			cout<<"after adding your data print '>' if you want to continue adiing, else: print '.'"<<endl;
			while(log=='>')
			{
				cout<<"\nEnter the key: "<<endl;
				cin>>key;
				gen_random(data, 10);
				S.insert(0, data);
				if(T.insert(key, S)) {cout<<"successfully"<<endl; cout<<"added: \n"<<data<<endl;}
				else cout<<"wrong key!"<<endl;
				S.clear(); key.clear();
				cout<<"\nEnter > or . : "<<endl;
				cin>>log;				
			}
			log = '>';
			break;		
		case 4:
			cout<<"\nEnter the key: "<<endl;
			cin>>key;
			if(T.remove(key)) cout<<"complited"<<endl;
			else cout<<"wrong key!"<<endl;
			key.clear();
			break;
		case 5:
			for(CTree<string>::iterator it=T.begin(); it!=T.end(); ++it)
			{
				cout<<(*it)<<" "<<"height: "<<it.height()<<" balance: "<<it.balance()<<" data: "<<it.Dat()<<endl;
			}
			break;
		case 6:
			cout<<"TREE:"<<" size:"<<T.size()<<endl;
			cout<<"_______________________________"<<endl;
			T.print(T.GetRoot(), 0);
			cout<<"_______________________________"<<endl;
			break;
		case 7:
			S.clear(); 
			cout<<"Have a nice day!\n";
			return 0;
		default:
			cout<<"Wrong command!\n";
			return -1;							
	}
}
return 0;
}