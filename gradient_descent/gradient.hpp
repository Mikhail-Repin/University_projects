#include <iostream>
#pragma once
using namespace std;
class Point
{
	double *coordinates;
	int dim;
	public:
	Point()
	{
		coordinates = NULL;
	}
	Point(double *coord, int N)
	{
        	coordinates = new double[N];
		dim = N;
		for(int i=0; i<dim; i++)
		{
			coordinates[i]=coord[i];
		}
	}
	Point(double num, int N) 
	{
		coordinates = new double[N];
		dim = N;
		for(int i=0; i<dim; i++)
		{
			coordinates[i]=num;
		}		
	}
	Point(double num, int pos, int N)
	{
		if(pos > N-1) pos = N-1;
		coordinates = new double[N];
		dim = N;
		for(int i=0; i<dim; i++)
		{
			coordinates[i]=0;
		}
		coordinates[pos] = num;	
	}
        Point(const Point &v) {(*this).CopyOnly(v);}
	void CopyOnly(const Point &v){dim = v.dim; coordinates = new double[v.dim]; for(int i=0; i<v.dim; i++){coordinates[i]=v.coordinates[i];}}
	~Point(){delete [] coordinates; coordinates = NULL;}
	void Clean(){delete [] coordinates; coordinates = NULL; dim = 0;} 
        Point& operator=(const Point &);
        double& operator[](int );
	Point operator+(const Point &);
	Point operator-(const Point &);
	Point operator*(double);
	int len() {return dim;} 
	void print() const
	{
        	printf("{");
		for(int i=0; i<dim; i++)
		{
			cout<<coordinates[i]<<";";
		}
                printf("}\n");
	}
};
double Strong_Norm(Point X_1, Point X_2);
double Weak_Norm(Point X_1, Point X_2);
double Grad_Norm(Point X, double EPS);
Point Gradient(Point X, double EPS);
double First_Derivative(Point X, int pos, double EPS);
double Second_Derivative(Point X, int pos_1, int pos_2, double EPS);
double Function(Point X);
void inversion(double** A, int N);
Point Step_I(Point X, double EPS);
Point Step_II(Point X, double EPS);
double Alpha_k(Point X, int flg, double EPS);
void Gradient_descent(Point X_0, double EPS);