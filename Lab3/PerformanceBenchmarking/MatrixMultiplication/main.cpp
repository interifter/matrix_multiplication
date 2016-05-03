#include "SquareMatrix.h"
#include <cstdio>

void CompareMatrices(double*, double**, int);
void MatrixMultAccuracyTest();



int main(int arc, char* argv) {
	MatrixMultAccuracyTest();
}

void CompareMatrices(double* expected, double** actual, int d) {
	//*(arr1d + i * d + j);
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			double expectedValue = *(expected + i * d + j);
			double actual__Value = *(*(actual + i) + j);

			if (expectedValue != actual__Value) {
				printf("**ERR** Expected: %f. Actual: %f\r\n", expectedValue, actual__Value);
			}
			else {
				printf("*PASS* %f == %f\r\n", expectedValue, actual__Value);
			}
		}
	}
}


void MatrixMultAccuracyTest() {
	int count = 9;
	int index = 0;
	double test[9] = { 3, -2, 3, 4, 3, 1, 35, 5, 7 };
	double soluMatrix0[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	double soluMatrix2[9] = { 106, 3, 28, 59, 6, 22, 370, -20, 159 };
	double soluMatrix3[9] = { 1310, -63, 517, 971, 10, 337, 6595, -5, 2203 };
	printf("Base Test Matrix: ");

	for (int i = 0; i < 9; i++) {
		printf("%f ", test[i]);
	}
	printf("\r\n");
	SquareMatrix sm = SquareMatrix(test, 3);	//Serial Matrix.

	double** smRof0 = sm.Power(0);

	printf("\r\nSerial Matrix Power(0) Results\r\n");
	CompareMatrices(soluMatrix0, smRof0, 3);

	double** smRof1 = sm.Power(1);

	printf("\r\nSerial Matrix Power(1) Results\r\n");
	CompareMatrices(test, smRof1, 3);

	double** smRof2 = sm.Power(2);

	printf("\r\nSerial Matrix Power(2) Results\r\n");
	CompareMatrices(soluMatrix2, smRof2, 3);


	double** smRof3 = sm.Power(3);
	printf("\r\nSerial Matrix Power(3) Results\r\n");
	CompareMatrices(soluMatrix3, smRof3, 3);
}
