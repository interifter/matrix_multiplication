#include "OptimizedSquareMatrix.h"
#include <omp.h>

OptimizedSquareMatrix::OptimizedSquareMatrix(int d) :SquareMatrix(d)
{
}

OptimizedSquareMatrix::OptimizedSquareMatrix(long long mem) :SquareMatrix(mem)
{
}

OptimizedSquareMatrix::OptimizedSquareMatrix(double * arr, int d) :SquareMatrix(arr, d)
{
}

OptimizedSquareMatrix::OptimizedSquareMatrix(double **arr, int d): SquareMatrix(arr, d)
{
}

OptimizedSquareMatrix::~OptimizedSquareMatrix()
{
}

void OptimizedSquareMatrix::Multiply(double ** _a, double ** _b, double ** _c)
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

double ** OptimizedSquareMatrix::Power(int p)
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
		if (2 * i <= p) {
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
