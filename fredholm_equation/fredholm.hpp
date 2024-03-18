#include <iostream>
#pragma once
using namespace std;

class Function;

/*int osx(float x);
int osy(float y);
void Axes();
void Point(float x, float y, int color);*/

double F(double);
double K(double, double);
double Answer(double );
double* SLAU(double** ,double*, int , int);
Function Approx_coeff(int , double , double );
double Gauss(double a, double b, Function U1, Function U2);
double Adapt_Integrate(double a, double b, Function U1, Function U2, double EPS);
void Fredholm(double , double , double );

class Function
{
	double* C, * t, * U;
	int N, ans_flg = 0;
public:
	Function()
	{
		N = 0;
		C = t = U = NULL;
	}
	Function(double* C, double* t, double* U, int N)
	{
		(*this).N = N;
		(*this).C = new double[N];
		(*this).t = new double[N];
		(*this).U = new double[N];
		for (int i = 0; i < N; i++)
		{
			(*this).C[i] = C[i];
			(*this).t[i] = t[i];
			(*this).U[i] = U[i];
		}
	}
	Function(int N)
	{
		(*this).N = N;
		(*this).C = new double[N];
		(*this).t = new double[N];
		(*this).U = new double[N];
		for (int i = 0; i < N; i++)
		{
			(*this).C[i] = 0;
			(*this).t[i] = 0;
			(*this).U[i] = 0;
		}
	}
	Function(const Function& Func) { (*this).CopyOnly(Func); }
	void CopyOnly(const Function& Func)
	{
		N = Func.N;
		ans_flg = Func.ans_flg;
		(*this).C = new double[Func.N];
		(*this).t = new double[Func.N];
		(*this).U = new double[Func.N];
		for (int i = 0; i < Func.N; i++)
		{
			(*this).C[i] = Func.C[i];
			(*this).t[i] = Func.t[i];
			(*this).U[i] = Func.U[i];
		}
	}
	~Function() { delete[] C; C = NULL; delete[] t; t = NULL; delete[] U; U = NULL; }
	void Clean() { delete[] C; C = NULL; delete[] t; t = NULL; delete[] U; U = NULL; N = 0; ans_flg = 0; }
	Function& operator=(const Function& Func)
	{
		if (this != &Func)
		{
			Clean(); N = Func.N; ans_flg = Func.ans_flg;
			(*this).CopyOnly(Func);
		}
		return *this;
	}
	int dim() { return N; }
	void Make_Ans() { ans_flg = 1; };
	double T(int i)
	{
		return t[i];
	}
	double Calc(double x)
	{
		double res = 0.;
		if (!ans_flg)
		{
			for (int i = 0; i < N; i++)
			{
				res += C[i] * U[i] * K(x, t[i]);
			}
			res += F(x);
		}
		if (ans_flg)
		{
			res = Answer(x);
		}
		return res;
	}
	void print() const
	{
		cout << "Dim of net: "<< N << endl;
		printf("C		t		U\n");
		for (int i = 0; i < N; i++)
		{
			cout << C[i] << "		" << t[i] << "		" << U[i] << endl;
		}
	}
	/*void draw() const
	{
		for (int i = 0; i < N; i++)
		{
			Point(t[i], this->Calc(t[i]), WHITE);
		}
	}*/
};
