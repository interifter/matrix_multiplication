#pragma once
#ifndef SQ_MATRIX_H
#define SQ_MATRIX_H


class SquareMatrix {

protected:
	double** a;
	double** b;
	double** c;
	int array_dim;
	double multElapsed;

	void Init(int, double*, double**);
	virtual void Multiply(double**, double**, double**);
	void Swap(double **&, double **&);
	void Copy(double **, double **);
	void Identity(double **);

public:
	SquareMatrix(int);
	SquareMatrix(long long);
	SquareMatrix(double**, int);
	SquareMatrix(double*, int);
	~SquareMatrix();
	virtual double** Power(int p);
	double** GetMatrix();

	double GetElapsedMultTime();
};
#endif // !SQ_MATRIX_H
