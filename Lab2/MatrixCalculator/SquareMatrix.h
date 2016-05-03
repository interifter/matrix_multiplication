///
/// SquareMatrix.h
/// Honor Code: I pledge that I have neither given nor received help from anyone other than the instructor for all program components included here. 
/// Author: Zachary Haberman <habermanz@cwu.edu>
/// Created: 4/1/2016
/// Description: Object used for simple Square Matrix multiplication.
///

#pragma once
#ifndef SQMATRIX_H
#define SQMATRIX_H
#include <thread>;

class SquareMatrix {
	//double* arr;
	int size;
	std::thread* threadpool;

public:
	SquareMatrix();
	~SquareMatrix();

	int GetDimension();
	double* Copy(double*, int);
	double* Multiply(double*, double*, int);
	double* MMultiply(double*, double*, int);
	double* Power(double*, int, int);
	double* MPower(double*, int, int);
	double* Generate(int);
	void Print(double*, int);

};

#endif // !MATRIX_H
