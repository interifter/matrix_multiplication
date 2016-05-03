#include "SquareMatrix.h"
#include <Windows.h>

#pragma once
#ifndef CA_SQ_MATRIX_H
#define CA_SQ_MATRIX_H

enum BOUND {
	UPPER,
	LOWER
};


class CacheOptimizedSquareMatrix : public SquareMatrix {

protected:
	
	size_t lineSize;
	size_t cacheSize;
	size_t cacheLineCount;
	size_t tileSize;

	void InitCacheInformation(int, BOUND);
	virtual void Multiply(double**, double**, double**);


public:
	CacheOptimizedSquareMatrix(int, int, BOUND);
	CacheOptimizedSquareMatrix(long long, int, BOUND);
	CacheOptimizedSquareMatrix(double**, int, int, BOUND);
	CacheOptimizedSquareMatrix(double*, int, int, BOUND);
	~CacheOptimizedSquareMatrix();
};
#endif // !CA_SQ_MATRIX_H

