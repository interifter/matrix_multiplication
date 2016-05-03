#include "SquareMatrix.h"
#include <chrono>

///
/// Creates an initial matrix based on the passed in dimension.
/// Note: Up to three matrices will be created. Be wary of memory usage.
///
SquareMatrix::SquareMatrix(int d)
{
	Init(d, NULL, NULL);
}

SquareMatrix::SquareMatrix(double* arr, int d) {
	Init(d, arr, NULL);
}

///
/// Pass in the maximum memory allowed for this object to use (in bytes).
/// Based on the allowable memory, will determine the dimension of the matrices.
/// Since the Matrix object can create up to three matrices, the dimension is 70% of 1/3 of the allowed memory passed in.
///
SquareMatrix::SquareMatrix(long long mem)
{
	Init((int)sqrt(mem / sizeof(double) / 3 * .60), NULL, NULL);
}

///
/// Copies the passed-in array and inits a Matrix object.
///
SquareMatrix::SquareMatrix(double** arr, int d) {
	Init(d, NULL, arr);
}

void SquareMatrix::Init(int d, double* arr1d, double** arr2d) {
	array_dim = d;
	a = new double*[array_dim];
	b = new double*[array_dim];
	c = new double*[array_dim];

	for (int i = 0; i < array_dim; i++) {
		(*(a + i)) = new double[array_dim];
		(*(b + i)) = new double[array_dim];
		(*(c + i)) = new double[array_dim];
		if (arr1d != NULL) {
			for (int j = 0; j < array_dim; j++) {
				*(*(a + i) + j) = *(arr1d + i * d + j);
			}
		}
		else if (arr2d != NULL) {
			for (int j = 0; j < array_dim; j++) {
				*(*(a + i) + j) = *(*(arr2d + i) + j);
			}
		}
		else {
			for (int j = 0; j < array_dim; j++) {
				*(*(a + i) + j) = ((double)rand() / (RAND_MAX));
			}
		}
	}
}

void SquareMatrix::Multiply(double **, double **, double **)
{
	double aVal = 0;
	double bVal = 0;
	double sum = 0;

	for (int i = 0; i < array_dim; i++) {
		for (int j = 0; j < array_dim; j++) {
			for (int k = 0; k < array_dim; k++) {
				aVal = *(*(a + i) + k);
				bVal = *(*(b + k) + j);
				sum += aVal * bVal;
			}

			*(*(c + i) + j) = sum;
		}
	}
}

void SquareMatrix::Swap(double ** _a, double ** _b)
{
	double ** temp = _a;
	_a = _b;
	_b = temp;
}

void SquareMatrix::Copy(double ** _a, double ** _b)
{
	for (int i = 0; i < array_dim; i++) {
		for (int j = 0; j < array_dim; j++) {
			*(*(_b + i) + j) = *(*(_a + i) + j);
		}
	}
}

void SquareMatrix::Identity(double ** _a)
{
	for (int i = 0; i < array_dim; i++) {
		for (int j = 0; j < array_dim; j++) {
			if (i == j) {
				*(*(_a + i) + j) = 1;
			}
			else {
				*(*(_a + i) + j) = 0;
			}
		}
	}
}

double ** SquareMatrix::Power(int p)
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
	for (int i = 1; i < p; i++) {
		Multiply(a, b, c);
		Swap(b, c);
	}


	Copy(c, b);
	return c;

}
