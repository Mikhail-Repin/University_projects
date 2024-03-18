#include <stdio.h>              
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <stdio.h> 
#include <math.h>
#include "Fredholm.hpp"
using namespace std;


/*const int X0 = 100, Y0 = 400;// координаты центра координат
const float k = 15;// масштаб в точках одного деления на графике
int osx (float x)
{
return X0+k*x;
}

int osy (float y)
{
return Y0-k*y;
}

void Axes()
{
int i, xe,ye,y1;
char s[10];
line ( X0, 0, X0, 599 ); // ось ox
line ( 0, Y0, 799, Y0 ); // ось oy
// метки на оси ox
for ( i = 1; i <= (800-X0)/k; i ++ )
{
xe = osx ( i );
line ( xe, Y0-2, xe, Y0+2 ); // рисуем деление разметки
sprintf ( s, "%d", i ); // записываем в строковую переменную число деления
outtextxy ( xe-10, Y0+4, s ); // вывод числа
}
// метки на оси oy
for ( i = 1; i <= (Y0)/k; i ++ )
{
ye = osy( i+1 );
line ( X0-2, ye, X0+2, ye ); // рисуем деление
sprintf ( s, "%d",i  ); //  записываем в строковую переменную число деления
outtextxy ( X0+10, ye+4, s ); // вывод числа
}
}

void Point ( float x, float y, int color )
{
int xe, ye;
xe = osx(x);
ye = osy(y);
if ( xe >= 0 && xe < 800 && ye >= 0 && ye < 600 )
putpixel(xe, ye, color);
}*/

double F(double x)
{
	double res=0;
	//res = exp(-x);
	//res = cos(2*x);
	//res = 1 - x * (exp(x) - exp(-x));
	res = (5. / 6.) * x;
	//res = 1.;
	return res;
}

double K(double x, double y)
{
	double res=0;
	//res = 0.5*x*exp(y);
	//res = sin(x) * cos(y);
	//res = x * x * exp(x * y);
	res = 0.5 * x * y;
	//res = x * y * y;
	return res;
}

double Answer(double x)
{
	double res = 0;
	res = (x + exp(-x));
	//res = cos(2 * x);
	//res = 1.;
	res = x;
	//res = 1. + 4./9. * x;
	return res;
}

/*double Function(double t, double x)
{
	if(t = NULL) return F(x);
	else return K(t, x)*F(x);
}

double Simpson_old(double a, double b, double t)
{
	double length = fabs(b-a), res = 0.;
	if(b<a) {double tmp = a; a = b; b = tmp;}
	res = length/6*(Function(t, a) = 4*Function(t, (a+b)/2) + Function(t, b));
	return res;
}

double NK_Integral_old(double a, double b, double t, double EPS)
{
	double h = fabs(b-a), ro = 0., S_cur = 0., S_pred = 0., length = fabs(b-a), res = 0.;
	int N = 0;
	if(b<a) {double tmp = a; a = b; b = tmp;}
	N = reinterpret_cast<int>(length/h);
	for(int i = 0; i<N; i++)
	{
		S_cur += Simpson((a+h*i), (a+h*(i+1)), t);	
	}
	S_pred = S_cur;
	do
	{
		S_cur = 0.;
		h = h/2;
		N = reinterpret_cast<int>(length/h);
		for(int i = 0; i<N; i++)
		{
			S_cur += Simpson((a+h*i), (a+h*(i+1)), t);	
		}
`		ro =  (S_cur - S_pred)/(1-pow(0.5, 4));
		S_pred = S_cur;	
	}
	while(ro > EPS);
	res = S_cur;
	return res;
}*/

double* SLAU(double** A,double *f, int N, int M)
{
	double *res, c = 0., sum=0.;
	int cnt = M, tmp=0;
	res = new double[M];
	for(int i=0; i<N-1; i++)
	{
		if (fabs(A[i][i] - 0) < 1e-10) {continue; }
		for(int j=(i+1); j<N; j++)
		{
			c = A[j][i] / A[i][i];
			for(int k=i; k<M; k++)
			{
				A[j][k] = A[j][k] - c*A[i][k];
			}
			f[j] = f[j] - c * f[i];
		}
	}

	for(int i=N-1; i>=0; i--)
	{
		tmp = cnt;
		for(int k=(M-1); k>=0; k--)
		{
			if(!(fabs(A[i][k]-0) < 1e-10)) cnt = i;
		}
		if(cnt < tmp-1)
		{
			for (int j = tmp - 1; j > cnt; j--)
			{
				res[j] = 1;
			}
		}
		if (cnt >= tmp) continue;
		sum = f[i];
		for(int j=M-1; j>cnt; j--)
		{
			sum -= A[i][j]*res[j];			
		}
		res[cnt] = sum/A[i][cnt];
	}
	return res;	
}

Function Approx_coeff(int N, double a, double b)
{
	double tmp = 0.,h = fabs(b - a), length = fabs(b - a), * t, * f, * C_tmp, * res, ** A, C[5] = { 14. / 45., 64. / 45., 24. / 45., 64. / 45., 14. / 45. }, B[4] = { 64. / 45., 24. / 45., 64. / 45., 14. / 45. };
	int k1 = 0, k2 = 0;
	Function U;
	h = length /(4*N);
	N = N * 4 + 1;
	A = new double* [N];
	t = new double[N];
	C_tmp = new double[N];
	f = new double[N];
	if(b<a) {double tmp = a; a = b; b = tmp;}
	for (int i = 0; i < N; i++)
	{
		A[i] = new double [N];
		t[i] = a + h*i;
		f[i] = F(t[i]);
	}

	for (int i = 0; i < N; i++)
	{
		if (i < 5)
		{
			C_tmp[i] = h * C[i];
		}
		else
		{
			C_tmp[i] = h * B[k1 % 4];
			k1++;
		}
		if ((i % 4 == 0) && (i != (N - 1)) && (i != 0)) C_tmp[i] *= 2;
	}


	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			A[i][j] = -C_tmp[j]* K(t[i], t[j]);
		}
		A[i][i] += 1;
	}
	res = SLAU(A, f, N, N);
	return Function(C_tmp, t, res, N);	
}

double Gauss(double a, double b, Function U1, Function U2)
{
	double t[3] = {-sqrt(15)/5, 0, sqrt(15)/5}, c[3] = {25./18., -7./9., 25./18.}, res = 0., A = (a+b)/2, B = fabs(b-a)/2;
	if(b<a) {double tmp = a; a = b; b = tmp;}
	for(int i= 0; i<3; i++)
	{
		res += c[i]*pow((U2.Calc((A + B*t[i])) - U1.Calc((A + B*t[i]))), 2);
	}
	return (res*B);	
}

double Adapt_Integrate(double a, double b, Function U1, Function U2, double EPS)
{
	double h =  fabs(b-a), alpha, beta, p, delta = 0., I_cur = 0., I_next = 0., I = 0.;
	int k = 0, kmax = 10;
	if(b<a) {double tmp = a; a = b; b = tmp;}
	alpha = a; beta = b;
	p = (Gauss(alpha, alpha+h/2, U1, U2) - Gauss(alpha, alpha+h, U1, U2))/(Gauss(alpha, alpha+h/4, U1, U2) - Gauss(alpha, alpha+h/2, U1, U2));
	while(alpha<beta && fabs(alpha - beta) > EPS)
	{
		//cout << alpha << " " << beta << endl;
		//cout << fabs(alpha - beta) << endl;
		h = beta - alpha;
		I_cur = Gauss(alpha, alpha+h, U1, U2);
		I_next = Gauss(alpha, alpha+h/2, U1, U2);
		delta = (I_next - I_cur)/(8 - 1);
		if(fabs(delta) < EPS*h/(b-a))
		{
			k = 0;
			alpha = beta; beta = b;
			I = I + I_cur + delta;
		}
		else
		{
			beta = (alpha + beta)/2;
			k += 1;
			if(k > kmax)
			{
				k = 0;
				alpha = beta; beta = b;
				I = I + I_cur + delta;
			}
		}		
	}
	return I;	
}

void Fredholm(double a, double b, double EPS)
{
	double Norm = 1.;
	int N = 1;
	Function u_pred, u_cur, right;
	right.Make_Ans();
	cout<<"Start solving Fredholm equation..."<<endl;
	u_cur = Approx_coeff(N, a, b);
	N++;
	while(Norm > EPS)
	{
		u_pred = u_cur;
		u_cur = Approx_coeff(N, a, b);
		Norm = sqrt(Adapt_Integrate(a, b, u_pred, u_cur, EPS));
		cout<<"	N = "<<N<<" L2 norm = "<<Norm<<endl;	
		N++;
	}
	cout << "The answer is: " << endl;
	u_cur.print();
	/*for (int i = 0; i < u_cur.dim(); i++)
	{
		cout<< endl << right.Calc(u_cur.T(i)) << endl;
	}*/
	cout <<endl<< "L2 norm of difference between right funtion and answer: " << sqrt(Adapt_Integrate(a, b, right, u_cur, EPS))<<endl;
}

int main(void)
{
	double a = 0., b = 1., EPS = 1e-8, **A, f[3] = {0,-5, -2}, *res;
	A = new double*[3];
	for (int i = 0; i < 3; i++) A[i] = new double[2];
	A[0][0] = 22; A[0][1] = 34; A[0][2] = -23; 
	A[1][0] = 13; A[1][1] = 56; A[1][2] = -3;
	A[2][0] = 1; A[2][1] = 4; A[2][2] = 7;
	//res = SLAU(A, f, 3, 3);
	//for (int i = 0; i < 3; i++) cout << res[i] << endl;
	Fredholm(a, b, EPS);
	return 0;
}