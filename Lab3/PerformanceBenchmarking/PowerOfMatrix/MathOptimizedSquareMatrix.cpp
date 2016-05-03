#include "MathOptimizedSquareMatrix.h"
#include <omp.h>

MathOptimizedSquareMatrix::MathOptimizedSquareMatrix(int d) : SquareMatrix(d)
{
}

MathOptimizedSquareMatrix::MathOptimizedSquareMatrix(long long mem) : SquareMatrix(mem)
{
}

MathOptimizedSquareMatrix::MathOptimizedSquareMatrix(double * arr, int d) : SquareMatrix(arr, d)
{
}

MathOptimizedSquareMatrix::MathOptimizedSquareMatrix(double ** arr, int d) : SquareMatrix(arr, d)
{
}

double ** MathOptimizedSquareMatrix::Power(int p)
{
	if (p < 0) {
		throw; //Don't even think about it.
	}

	if (p == 0) {
		Identity(c);
		return c;
	}

	if (p == 1) {
		Copy(a, c);
		return c;
	}

	Copy(a, b);


	//Multiply b by a, Modify c.
	for (int i = 1; i < p;) {
		double mstart = omp_get_wtime();
		if (2*i <= p) {
			Multiply(b, b, c);
			i *= 2;
		}
		else {
			Multiply(a, b, c);
			i++;
		}

		Swap(c, b);
		double mend = omp_get_wtime();
		multElapsed += mend - mstart;
		
	}


	Copy(b, c);
	return c;
}
