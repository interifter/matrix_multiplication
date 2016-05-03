#pragma once
#ifndef CI_SQ_MATRIX
#define CI_SQ_MATRIX

#include "SquareMatrix.h"
class CacheImprovedSquareMatrix : public SquareMatrix {

protected:
	void Multiply(double**, double**, double**);

public:
	CacheImprovedSquareMatrix(int);
	CacheImprovedSquareMatrix(long long);
	CacheImprovedSquareMatrix(double*, int);
	CacheImprovedSquareMatrix(double**, int);
	~CacheImprovedSquareMatrix();
	
};
#endif // !MO_SQ_MATRIX
