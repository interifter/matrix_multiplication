#include "Matrix.h"
#include <cstdlib>
#include <cstdio>

SquareMatrix::SquareMatrix(double * a, int l)
{
	arr = new double[l * l];
	size = l;
	for (int i = 0; i < size * size; i++) {
		arr[i] = *(a + i);
		
	}
}

SquareMatrix::SquareMatrix(int l) {
	arr = new double[l * l];
	size = l;
	for (int i = 0; i < size * size; i++) {
		arr[i] = ((double)rand() / (RAND_MAX));
	}
}

SquareMatrix::~SquareMatrix()
{
	delete[] arr;
}

double * SquareMatrix::GetArray()
{
	double* _arr = new double[size * size];
	for (int i = 0; i < size * size; i++) {
		_arr[i] = *(arr + i);
	}

	return _arr;
}


int SquareMatrix::GetDimension()
{
	return size;
}

SquareMatrix SquareMatrix::Multiply(SquareMatrix* m)
{
	int d = m->GetDimension();
	double* mm = Multiply(m->GetArray(), d);
	return SquareMatrix(mm, d);
}

double* SquareMatrix::Multiply(double *a, int d) {

	//Assume we are multiplying two nxn matricies.
	if (d != this->size) {
		throw;
	}


	double* b = this->GetArray();
	double* newMatrix = new double[d * d];

	double sum = 0;
	double aVal = 0;
	double bVal = 0;

	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			sum = 0;
			for (int k = 0; k < d; k++) {
				aVal = *(a + (i * d + k));
				bVal = *(b + (j + d * k));
				sum += aVal * bVal;
			}
			newMatrix[i * d + j] = sum;
		}
	}

	delete[] b;
	return newMatrix;
}

SquareMatrix SquareMatrix::Power(int p)
{
	if (p < 0) {
		throw;
	}

	if (p == 0) {
		//For now, throw... until Identity implemented.
		throw;
	}

	if (p == 1) {
		//Return a copy of this.
		return SquareMatrix(this->GetArray(), this->GetDimension());
	}

	if (p == 2) {
		return Multiply(this);
	}

	//else...
	int d = this->GetDimension();
	double* a = this->Copy().GetArray();

	for (int ip = 0; ip < p - 1; ip++) {
		a = Multiply(a, d);
	}


	return SquareMatrix(a, d);

	
}

SquareMatrix SquareMatrix::Copy()
{
	return SquareMatrix(this->arr, this->size);
}

void SquareMatrix::Print() {

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf(" %f\t", *(arr + i * size + j));
		}
		printf("\r\n");
	}
}
