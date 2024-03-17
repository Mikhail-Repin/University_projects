#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CTree.hpp"
using namespace std;
void printcommands()
{
	cout<<"\nCommands:\n";
        cout<<"\n0 - Help (print commands again)\n";
	cout<<"\n1 - Add pair: key and data from standard input\n";
	cout<<"\n2 - Find data by key\n";
	cout<<"\n3 - Add pair: key and random data\n";
	cout<<"\n4 - Remove key and data\n";
	cout<<"\n5 - Check iterator\n";
	cout<<"\n6 - Print TREE and SIZE\n";
	cout<<"\n7 - Exit\n";
	return;
}

void gen_random(char *s, const int len) {
static const char mapp[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";
srand(time(NULL));
for (int i = 0; i < len; ++i) {
s[i] = mapp[rand() % (sizeof(mapp) - 1)];
}

s[len] = 0;
}