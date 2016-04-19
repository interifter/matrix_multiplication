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

class SquareMatrix {
	//double* arr;
	int size;
public:
	//SquareMatrix(double*, int);
	//SquareMatrix(int);
	SquareMatrix();
	~SquareMatrix();
	//double* GetArray();
	int GetDimension();

	//double* Multiply(double*, int);
	//double* Power(int p);
	//SquareMatrix Copy();
	double* Copy(double*, int);
	double* Multiply(double*, double*, int);
	double* Power(double*, int, int);
	double* Generate(int);
	//void Print();
	void Print(double*, int);

};

#endif // !MATRIX_H
