///
/// SquareMatrix.cpp
/// Honor Code: I pledge that I have neither given nor received help from anyone other than the instructor for all program components included here. 
/// Author: Zachary Haberman <habermanz@cwu.edu>
/// Created: 4/1/2016
/// Description: Object used for simple Square Matrix multiplication.
///

#include "SquareMatrix.h"
#include <cstdlib>
#include <cstdio>

///
/// Creates a SquareMatrix object from a square (l^2 size) matrix.
///
//SquareMatrix::SquareMatrix(double * a, int l) {
//	arr = new double[l * l];
//	size = l;
//	for (int i = 0; i < size * size; i++) {
//		arr[i] = *(a + i);
//		
//	}
//}

///
/// Creates a SquareMatrix object with matrix of size l^2 with random doubles between [0-1].
///
//SquareMatrix::SquareMatrix(int l) {
//	arr = new double[l * l];
//	size = l;
//	for (int i = 0; i < size * size; i++) {
//		arr[i] = ((double)rand() / (RAND_MAX));
//	}
//}

SquareMatrix::SquareMatrix() {

}

///
/// Clears any resources being used.
///
SquareMatrix::~SquareMatrix() {
	//delete[] arr;
	//delete[] iarr;
}

///
/// Returns the internal array used by the SquareMatrix.
///
//double * SquareMatrix::GetArray() {
//	/*double* _arr = new double[size * size];
//	for (int i = 0; i < size * size; i++) {
//		_arr[i] = *(arr + i);
//	}
//
//	return _arr;*/
//	return arr;
//}

double * SquareMatrix::Generate(int d) {
	double* a = new double[d * d];
	for (int i = 0; i < d * d; i++) {
		a[i] = ((double)rand() / (RAND_MAX));
	}

	return a;
}

///
/// Returns the Dimension of the SquareMatrix.
/// 
int SquareMatrix::GetDimension() {
	return size;
}

double* SquareMatrix::Multiply(double* a, double* b, int d) {
	double* newMatrix = new double[d * d];	//Not sure if this goes out of scope or not (memory leak?)...

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

	return newMatrix;
}

double* SquareMatrix::Power(double* a, int d, int p) {
	if (p < 0) {
		throw;
	}

	if (p == 0) {
		//For now, throw... until Identity implemented.
		throw;
	}

	if (p == 1) {
		//Return a copy of this.
		return a;
	}

	if (p == 2) {
		return Multiply(a, a, d);
	}

	//else...
	double * iarr = a;
	double* tarr = Copy(a, d);

	for (int ip = 0; ip < p - 1; ip++) {

		iarr = Multiply(tarr, a, d); //Leak has to be caused here... 
								  //we'll need two arrays here... seems to add inefficiency, but will save memory.

		delete[] tarr;
		tarr = Copy(iarr, d);
		delete[] iarr;
	}


	return tarr;
}

///
/// Multiplies this Matrix by a d^2-sized array. Both must have equal dimensions or an error will be thrown.
///
//double* SquareMatrix::Multiply(double *a, int d) {
//
//	//Assume we are multiplying two nxn matricies.
//	if (d != this->size) {
//		throw;
//	}
//
//
//	double* b = arr;
//	double* newMatrix = new double[d * d];	//Not sure if this goes out of scope or not (memory leak?)...
//
//	double sum = 0;
//	double aVal = 0;
//	double bVal = 0;
//
//	for (int i = 0; i < d; i++) {
//		for (int j = 0; j < d; j++) {
//			sum = 0;
//			for (int k = 0; k < d; k++) {
//				aVal = *(a + (i * d + k));
//				bVal = *(b + (j + d * k));
//				sum += aVal * bVal;
//			}
//			newMatrix[i * d + j] = sum;
//		}
//	}
//
//	return newMatrix;
//}

///
/// Raises this SquareMatrix by power b (M^p).
///
//double* SquareMatrix::Power(int p) {
//	if (p < 0) {
//		throw;
//	}
//
//	if (p == 0) {
//		//For now, throw... until Identity implemented.
//		throw;
//	}
//
//	if (p == 1) {
//		//Return a copy of this.
//		this->GetArray();
//	}
//
//	if (p == 2) {
//		return Multiply(arr, this->GetDimension());
//	}
//
//	//else...
//	int d = this->GetDimension();
//	double * iarr = this->GetArray();
//	double* tarr = Copy(iarr, d);
//
//	for (int ip = 0; ip < p - 1; ip++) {
//		
//		iarr = Multiply(tarr, d); //Leak has to be caused here... 
//		//we'll need two arrays here... seems to add inefficiency, but will save memory.
//
//		delete[] tarr;
//		tarr = Copy(iarr, d);
//		delete[] iarr;
//	}
//
//	
//	return tarr;
//
//	
//}

///
/// Returns a copy of this SquareMatrix object.
///
//SquareMatrix SquareMatrix::Copy() {
//	return SquareMatrix(this->GetArray(), this->size);
//}

double* SquareMatrix::Copy(double* a, int l) {
	double * nM = new double[l * l];
	for (int i = 0; i < l * l; i++) {
		nM[i] = *(a + i);
	}

	return nM;
}

///
/// Prints the underlying array in DxD form.
///
//void SquareMatrix::Print() {
//
//	for (int i = 0; i < size; i++) {
//		for (int j = 0; j < size; j++) {
//			printf(" %f\t", *(arr + i * size + j));
//		}
//		printf("\r\n");
//	}
//}

///
/// Prints the passed-in array in DxD form.
///
void SquareMatrix::Print(double* a, int m) {

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			printf(" %f\t", *(a + i * m + j));
		}
		printf("\r\n");
	}
}
