#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include "gradient.hpp"
using namespace std;


//Class Point functions----------------
Point& Point::operator=(const Point &v)
{
	if (this!=&v)
	{
		Clean(); dim = v.dim; 
		(*this).CopyOnly(v);
	} 
	return *this;
}

double& Point::operator[](int i)
{
	if(i < dim) return coordinates[i];
	else cout<<"invalid call for coordinate # %d !"<<i;
	return coordinates[dim - 1];
}

Point Point::operator+(const Point &v)
{
	double *tmp; tmp = new double[dim];
	for(int i=0; i<dim; i++)
	{
		tmp[i]=(coordinates[i]+v.coordinates[i]);
	}
        Point res(tmp, dim);
        delete [] tmp; tmp = NULL;
	return res;
}

Point Point:: operator-(const Point &v) 
{
	double *tmp; tmp = new double[dim];
	for(int i=0; i<dim; i++)
	{
		tmp[i]=(coordinates[i]-v.coordinates[i]);	
	}
        Point res(tmp, dim);
        delete [] tmp; tmp = NULL;
	return res;
}
Point Point::operator*(double c)
{
	double *tmp; tmp = new double[dim];
	for(int i=0; i<dim; i++)
	{
		tmp[i]=(coordinates[i]*c);	
	}
	Point res(tmp, dim);
        delete [] tmp; tmp = NULL;
	return res;	
}
//End Class Point functions----------------


//Gradient descent functions---------------
double Strong_Norm(Point X_1, Point X_2)
{
	double squared_norm=0;
	Point difference = X_1-X_2;
	for(int i=0; i<difference.len(); i++)
	{
		squared_norm += pow(fabs(difference[i]), 2);
	}
	return sqrt(squared_norm);
}

double Weak_Norm(Point X_1, Point X_2)
{
	return(fabs(Function(X_1) - Function(X_2)));	
}

double Grad_Norm(Point X, double EPS)
{
	Point zero(0., X.len());
	return Strong_Norm(Gradient(X, EPS), zero);
}

Point Gradient(Point X, double EPS)
{
	Point res(0., X.len());
	for(int i=0; i<res.len(); i++)
	{
		res[i] = First_Derivative(X, i, EPS);
	}
	return res;
}

double First_Derivative(Point X, int pos, double EPS)
{
	double tau = 0.1*sqrt(EPS);
	Point T(tau, pos, X.len());
	return (Function(X+T) - Function(X-T))/(2*tau);
}

double Second_Derivative(Point X, int pos_1, int pos_2, double EPS)
{
	double tau = 0.1*sqrt(EPS), num, denom;
	Point T_1(tau, pos_1, X.len()), T_2(tau, pos_2, X.len());
	num = 0.5 * (Function(X + T_1) + Function(X + T_2) - Function(X + T_1 - T_2) - Function(X - T_1 + T_2) + Function(X - T_2) + Function(X - T_1) - 2 * Function(X));
	denom =  pow(tau, 2);
	return num/denom;
}

double Function(Point X)
{
	double res=0;
	/*for(int i=0; i<X.len(); i++)
	{
		if (i / 2) res += pow(X[i]-1, 2);
		else res += pow(X[i]+1, 2);
	}*/
	res = pow((X[0]), 2) + 2*pow((X[1]), 2);
	res = exp(res);
	return res;
}

void inversion(double **A, int N)
{
    double temp;
 
    double **E = new double *[N];
 
    for (int i = 0; i < N; i++)
        E[i] = new double [N];
 
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            E[i][j] = 0.0;
 
            if (i == j)
                E[i][j] = 1.0;
        }
 
    for (int k = 0; k < N; k++)
    {
        temp = A[k][k];
 
        for (int j = 0; j < N; j++)
        {
            A[k][j] /= temp;
            E[k][j] /= temp;
        }
 
        for (int i = k + 1; i < N; i++)
        {
            temp = A[i][k];
 
            for (int j = 0; j < N; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
 
    for (int k = N - 1; k > 0; k--)
    {
        for (int i = k - 1; i >= 0; i--)
        {
            temp = A[i][k];
 
            for (int j = 0; j < N; j++)
            {
                A[i][j] -= A[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
 
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = E[i][j];
 
    for (int i = 0; i < N; i++)
        delete [] E[i];
 
    delete [] E;
}

Point Step_I(Point X, double EPS)
{
	Point res(0., X.len());
	res = res - Gradient(X, EPS);
	return  res;
}

Point Step_II(Point X, double EPS)
{
	int N = X.len();
	double **D_F = new double *[N], *res = new double [N];
	Point grad(0., N);
 
    	for (int i = 0; i < N; i++)
	{
		D_F[i] = new double [N];
		res[i] = 0;
	}
 
    	for (int i = 0; i < N; i++)
        	for (int j = 0; j < N; j++)
        	{
        		D_F[i][j] = Second_Derivative(X, i, j, EPS);
        	}
	inversion(D_F, N);
	grad = Gradient(X, EPS);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			res[i] -=  D_F[i][j]*grad[j];
		}
	for (int i = 0; i < N; i++)
        delete [] D_F[i];
    	delete [] D_F;
	return Point(res, N);		
}

double Alpha_k(Point X, int flg, double EPS)
{
	if (flg == 3)
	{
		double a = 0, b = 1, r = (3 - sqrt(5))/2, c, d;
		c = a + r*(b-a);
		d = b - r*(b-a);
		Point H(0., X.len());
		H = H - Gradient(X, EPS);
		while((b-a) > EPS)
		{
			//cout << a << " " << b << endl;
			//cout << Function(X + (H * c)) << " " << Function(X + (H * d)) << endl;
			if(Function(X + (H*c)) < Function(X + (H*d)))
			{
				b = d;
				d = b - r * (b - a);;
				c = a + r*(b-a);
				continue;
			}
			else
			{
				a = c;
				c = a + r * (b - a);
				d = b - r*(b-a);	
			}
		}
		return (b+a)/2;
	}

	if (flg == 1)
	{
		double a, b = 2, p = 2;
		a = b;
		Point H(0., X.len());
		H = H - Gradient(X, EPS);
		if(Function(X + (H*a)) < Function(X))
		{
			a = b*2;
			while((Function(X + (H*a)) < Function(X + (H*b))) and (a < b*10))
			{
				//cout << "Uvelich" << endl;
				a = a*p;
			}
			return a/p;
		}
		else
		{
			//or (fabs(Function(X + (H * a)) - Function(X)) < EPS)
			while((Function(X + (H*a)) > Function(X)))
			{
				//cout << "Droblen " <<a<< endl;
				a = a/p;	
			}
			return a;
		}
	}
	
}

void Gradient_descent (Point X_0, double EPS)
{
	int cnt = 0;
	double a_k = 0;
	Point X_cur(X_0), X_next(X_0);
	cout<<"I method started...\n";
	while(1)
	{
		cnt+=1;
		a_k = Alpha_k(X_next, 3, EPS);
		X_cur = X_next;
		X_next = X_next + (Step_I(X_next, EPS)* Alpha_k(X_next, 3, EPS));
		cout << "	Step: " << cnt << " Alpha_k: " << Alpha_k(X_cur, 3, EPS) << "; Point: "; X_next.print();
		if((Strong_Norm(X_cur, X_next) < sqrt(EPS)) and (Weak_Norm(X_cur, X_next) < sqrt(EPS)) and (Grad_Norm(X_next, EPS) < sqrt(EPS)))
		{
			break;
		}	
	}
	cnt = 0;
	if ((Strong_Norm(X_cur, X_next) < EPS) and (Weak_Norm(X_cur, X_next) < EPS) and (Grad_Norm(X_next, EPS) < EPS)) { cout << "Requred accuracy achieved" << endl; return; }
	cout<<"II method started...\n";
	X_cur = X_next;
	a_k = 0;
	while(1)
	{
		cnt+=1;
		X_cur = X_next;
		X_next = X_next + (Step_II(X_next, EPS) * Alpha_k(X_next, 1, EPS));
		cout << "	Step: " << cnt << " Alpha_k: " << Alpha_k(X_cur, 1, EPS) << "; Point: "; X_next.print();
		if((Strong_Norm(X_cur, X_next) < EPS) and (Weak_Norm(X_cur, X_next) < EPS) and (Grad_Norm(X_next, EPS) < EPS))
		{
			break;
		}		
	}	
}

int main(void)
{
double tmp1[2] = { -2, -1 }, tmp2[2] = { 0.2,0.3}, EPS = 1e-8;
Point X1(tmp1, 2), X2(tmp2, 2);
//cout << First_Derivative(X2, 1, EPS) << endl;
//Step_I(X2, EPS).print();
//(X1 + (Step_I(X1, EPS) * Alpha_k(X1, 3, EPS))).print();
//Alpha_k(X1, 1, EPS);
Gradient_descent(X2, EPS);
/*<<Strong_Norm(X1, X1)<<endl;
cout<<Weak_Norm(X1, X1)<<endl;
cout<<Grad_Norm(X1, EPS) << endl;
cout<<Function(X1) << endl;
Step_I(X1, EPS).print();
cout << Alpha_k(X1, 3, EPS) << endl;*/
return 0;
}