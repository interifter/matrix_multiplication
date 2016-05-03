#include "CacheImprovedSquareMatrix.h"

void CacheImprovedSquareMatrix::Multiply(double ** _a, double ** _b, double ** _c)
{
	double aVal = 0;
	
	for (int i = 0; i < array_dim; i++) {
		
		for (int j = 0; j < array_dim; j++) {
			aVal = *(*(_a + i) + j);
			for (int k = 0; k < array_dim; k++) {
				
				//Since we're not using a sum...
				if (j == 0) {
					*(*(_c + i) + k) = 0;
				}

				*(*(_c + i) + k) += aVal * *(*(_b + j) + k);

			}
		}
	}
}

CacheImprovedSquareMatrix::CacheImprovedSquareMatrix(int d) : SquareMatrix(d)
{
}

CacheImprovedSquareMatrix::CacheImprovedSquareMatrix(long long mem) : SquareMatrix(mem)
{
}

CacheImprovedSquareMatrix::CacheImprovedSquareMatrix(double * arr, int d) : SquareMatrix(arr, d)
{
}

CacheImprovedSquareMatrix::CacheImprovedSquareMatrix(double ** arr, int d) : SquareMatrix(arr, d)
{
}

CacheImprovedSquareMatrix::~CacheImprovedSquareMatrix()
{
}
