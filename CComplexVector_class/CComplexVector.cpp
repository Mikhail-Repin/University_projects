#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "CComplexVector.hpp"
int main(void)
{ 
    char  act, filename[256], tmp, str[512],*s, *s0;
    int log = 0, N=0;
    double a, b;
    CComplex r(0, 0), *t;
    CComplexVector v1, v2, res;
    FILE* fp;
    printf("Enter your filename: \n");
    scanf("%s", filename);
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fclose(fp);
        printf("Cannot open the file!\n");
        return -1;
    }
    if (!fgets(str, sizeof(str)-1, fp)) {printf("Empty file!\n"); return -2;}
    s = str;
    while((s0 = strchr(s, ';')) != NULL){N++; s = s0+1;}
    N++;
    t = new CComplex [N];
    rewind(fp);
    do
    {
        for (int i = 0; i < N; i++)
        {
            if (!fscanf(fp, "%lg + %lgi", &a, &b) || !fscanf(fp, "%c", &tmp))
            {
                fclose(fp);
                printf("Wrong data!\n");
                delete [] t; t = NULL;
                return -2;
            }
            t[i] = CComplex(a, b);
        }
        if (log) {v2=CComplexVector(t,N);break; }
        else {v1=CComplexVector(t,N);}
        if (!fscanf(fp, "%c", &act)) { fclose(fp); printf("Wrong data!\n"); delete [] t; t = NULL; return -2; }
        log++;
    } while (log < 2);
    fclose(fp);
    delete [] t; t = NULL;
    if (act == '+') res = v1 + v2;
    else if (act == '-') res = v1 - v2;
    else if (act == '*') { r = v1 * v2; r.print(); return 1; }
    else { printf("Wrong action!\n");  return -3; }
    v1.print();
    printf("%c\n", act);
    v2.print();
    printf("=\n");
    res.print();
    printf("Also check [] operator: enter your Complex number: \n");
    scanf("%lg + %lgi", &a, &b);
    r =  CComplex(a, b);
    CComplex z1, z2;
    z2 = r;
    printf("Enter your coordinate: \n");
    scanf("%d", &N);
    printf("Right case (just required coordinate): "); z1 = v1[N]; (z1).print();
    printf("Left case (changed vector): ");  v1[N] = z2; v1.print(); 
    /*
    printf("Also try to divide your complex numbers. Enter two nubers:\n");
    if(!scanf("%lg + %lgi", &a, &b))
    {
    	printf("Wrong data!\n");
        return-1;
    }
    one = CComplex(a,b);
    if(!scanf("%lg + %lgi", &a, &b))
    {
    	printf("Wrong data!\n");
        return-1;
    }
    if(a == 0 & b == 0) {printf("You try to make undefined operation!\n"); return-1;}
    else {two = CComplex(a,b); one.print(); printf("devided by\n"); two.print();  printf("is\n"); (one/two).print();}*/ 
    return 0;
}
