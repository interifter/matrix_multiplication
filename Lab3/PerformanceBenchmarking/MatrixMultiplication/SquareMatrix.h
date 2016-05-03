#ifndef SQ_MATRIX_H
#define SQ_MATRIX_H


class SquareMatrix {

	//Private variables
	double** a;
	double** b;
	double** c;
	int array_dim;

	//Private functions
	void Init(int, double*, double**);
	void Multiply(double**, double**, double**);
	void Swap(double **, double **);
	void Copy(double **, double **);
	void Identity(double **);

public:
	//Public constructors
	SquareMatrix(int);
	SquareMatrix(long long);
	SquareMatrix(double**, int);
	SquareMatrix(double*, int);

	double** Power(int p);
};
#endif // !SQ_MATRIX_H
