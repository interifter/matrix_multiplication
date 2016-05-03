#pragma once
#ifndef MO_SQ_MATRIX
#define MO_SQ_MATRIX

#include "SquareMatrix.h"

class MathOptimizedSquareMatrix : public SquareMatrix {
public:
	MathOptimizedSquareMatrix(int);
	MathOptimizedSquareMatrix(long long);
	MathOptimizedSquareMatrix(double*, int);
	MathOptimizedSquareMatrix(double**, int);
	double** Power(int);
};

#endif // !MO_SQ_MATRIX

