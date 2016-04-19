#pragma once
#ifndef SQMATRIX_H
#define SQMATRIX_H

class SquareMatrix {
	double* arr;
	int size;
public:
	SquareMatrix(double*, int);
	SquareMatrix(int);
	~SquareMatrix();
	double* GetArray();
	int GetDimension();

	SquareMatrix Multiply(SquareMatrix*);
	double* Multiply(double*, int);
	SquareMatrix Power(int p);
	SquareMatrix Copy();
	void Print();

};

#endif // !MATRIX_H
