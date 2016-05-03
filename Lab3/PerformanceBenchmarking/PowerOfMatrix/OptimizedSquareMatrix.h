#pragma once
#ifndef O_SQ_MATRIX
#define O_SQ_MATRIX

#include "SquareMatrix.h"

class OptimizedSquareMatrix : public SquareMatrix {
public:
	OptimizedSquareMatrix(int);
	OptimizedSquareMatrix(long long);
	OptimizedSquareMatrix(double*, int);
	OptimizedSquareMatrix(double**, int);
	~OptimizedSquareMatrix();

	virtual void Multiply(double**, double**, double**);
	virtual double** Power(int);
};
#endif // !O_SQ_MATRIX

