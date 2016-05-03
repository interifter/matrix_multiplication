#include "Matrix.h"
#include <chrono>
#include <omp.h>

int NUM_THREADS = 8;


std::mutex Matrix::lockObject;
std::mutex Matrix::copyLockObject;

///
/// Creates an initial matrix based on the passed in dimension.
/// Note: Up to three matrices will be created. Be wary of memory usage.
///
Matrix::Matrix(int d)
{
	Init(d, NULL, NULL);
}

Matrix::Matrix(double* arr, int d) {
	Init(d, arr, NULL);
}

///
/// Pass in the maximum memory allowed for this object to use (in bytes).
/// Based on the allowable memory, will determine the dimension of the matrices.
/// Since the Matrix object can create up to three matrices, the dimension is 70% of 1/3 of the allowed memory passed in.
///
Matrix::Matrix(long long mem)
{
	Init((int)sqrt(mem / sizeof(double) / 3 * .60), NULL, NULL);
}

///
/// Copies the passed-in array and inits a Matrix object.
///
Matrix::Matrix(double** arr, int d) {
	Init(d, NULL, arr);	
}


void Matrix::Init(int d, double* arr1d, double** arr2d) {
	array_dim = d;
	a = new double*[array_dim];
	b = new double*[array_dim];
	c = new double*[array_dim];


	paramArr = new MULTMATRIXPARAMS[array_dim * array_dim];
	

	callbackstack = 0;
	

	copytime_elapsed = 0;
	copycount = 0;

	matrixmulttime_elapsed = 0;
	matrixmultcount = 0;

	multtime_elapsed = 0;
	multcount = 0;

	powertime_elapsed = 0;
	powercount = 0;

	concurrentcopytime_elapsed = 0;
	concurrentcopycount = 0;

	concurrentmulttime_elapsed = 0;
	concurrentmultcount = 0;

	concurrentmatrixmulttime_elapsed = 0;
	concurrentmatrixmultcount = 0;

	concurrentpowertime_elapsed = 0;
	concurrentpowercount = 0;

	waittime_elapsed = 0;
	waitcount = 0;

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

void Matrix::ConcurrentCall(int i)
{

	int x = -1;
	//bool empty = true;
	//MULTMATRIXPARAMS params;
	while (killswitch != 0) {
		lockObject.lock();
		
		if (!multmaxqueue.empty()) {
			x = multmaxqueue.front();
			multmaxqueue.pop();
		}

		lockObject.unlock();

		if (x != -1) {
			double aVal = 0;
			double bVal = 0;
			double sum = 0;

			//for (int k = 0; k < array_dim; k++) {
			//	printf("Thread %i. A[%i][%i] = %f\r\n", i, paramArr[x].i, k, *(*(paramArr[x].a_in + paramArr[x].i) + k));
			//	printf("Thread %i. B[%i][%i] = %f\r\n", i, k, paramArr[x].j, *(*(paramArr[x].b_in + k) + paramArr[x].j));
			//	/*aVal = *(*(paramArr[x].a_in + paramArr[x].i) + k);
			//	bVal = *(*(paramArr[x].b_in + k) + paramArr[x].j);
			//	sum += aVal * bVal;*/
			//}
			ConcurrentMultMax(paramArr[x]);
			lockObject.lock();
			callbackstack--;
			lockObject.unlock();
			//printf("Thread %i. -->C[%i][%i]: %f\r\n", i, paramArr[x].i, paramArr[x].j, *(*(paramArr[x].c_out + paramArr[x].i) + paramArr[x].j));
			
			x = -1;
		}

	}
}

double Matrix::Sqrt(double x)
{
	if (x <= 0)
		return 0;
	int exp = 0;
	x = frexp(x, &exp);
	if (exp & 1) {
		exp--;
		x *= 2;
	}
	double y = (1 + x) / 2;
	double z = 0;
	while (y != z) {
		z = y;
		y = (y + x / y) / 2;
	}
	return ldexp(y, exp / 2);
}

void Matrix::Multiply(double** _a, double** _b, double** _c)
{

	double millstart = omp_get_wtime() * 1000;
	MULTMATRIXPARAMS params;
	params.a_in = _a;
	params.b_in = _b;
	params.c_out = _c;

	for (int i = 0; i < array_dim; i++) {
		params.i = i;
		for (int j = 0; j < array_dim; j++) {
			params.j = j;
			params.set = true;
			MultMat(params);
		}
	}

	double millend = omp_get_wtime() * 1000;
	multtime_elapsed += millend - millstart;
	multcount++;

}

void Matrix::ConcurrentMultiply(double ** _a, double ** _b, double ** _c)
{
	double millstart = omp_get_wtime() * 1000;
	
	
	

	for (int i = 0; i < array_dim; i++) {
		
		for (int j = 0; j < array_dim; j++) {

			lockObject.lock();
			callbackstack++;
			lockObject.unlock();
			paramArr[i * array_dim + j].a_in = _a;
			paramArr[i * array_dim + j].b_in = _b;
			paramArr[i * array_dim + j].c_out = _c;
			paramArr[i * array_dim + j].j = j;
			paramArr[i * array_dim + j].i = i;
			paramArr[i * array_dim + j].set = 1;
			lockObject.lock();
			multmaxqueue.push(i * array_dim + j);
			lockObject.unlock();
			//ConcurrentMultMax(paramArr[i * array_dim + j]);
			//for (int k = 0; k < array_dim; k++) {
			//	printf("Thread %i. A[%i][%i] = %f\r\n", i, paramArr[i * array_dim + j].i, k, *(*(paramArr[i * array_dim + j].a_in + paramArr[i * array_dim + j].i) + k));
			//	printf("Thread %i. B[%i][%i] = %f\r\n", i, k, paramArr[i * array_dim + j].j, *(*(paramArr[i * array_dim + j].b_in + k) + paramArr[i * array_dim + j].j));
			//	/*aVal = *(*(paramArr[x].a_in + paramArr[x].i) + k);
			//	bVal = *(*(paramArr[x].b_in + k) + paramArr[x].j);
			//	sum += aVal * bVal;*/
			//}

		}

	}




	Wait(0);

	double millend = omp_get_wtime() * 1000;
	concurrentmulttime_elapsed += millend - millstart;
	concurrentmultcount++;
}

void Matrix::MultMat(MULTMATRIXPARAMS params)
{
	double millstart = omp_get_wtime() * 1000;

	double aVal = 0;
	double bVal = 0;
	double sum = 0;
	for (int k = 0; k < array_dim; k++) {
		aVal = *(*(params.a_in + params.i) + k);
		bVal = *(*(params.b_in + k) + params.j);
		sum += aVal * bVal;
	}

	*(*(params.c_out + params.i) + params.j) = sum;

	double millend = omp_get_wtime() * 1000;
	matrixmulttime_elapsed += millend - millstart;
	matrixmultcount++;


}

void Matrix::ConcurrentMultMax(MULTMATRIXPARAMS params)
{
	double millstart = omp_get_wtime() * 1000;

	double aVal = 0;
	double bVal = 0;
	double sum = 0;

	for (int k = 0; k < array_dim; k++) {
		aVal = *(*(params.a_in + params.i) + k);
		bVal = *(*(params.b_in + k) + params.j);
		sum += aVal * bVal;
	}

	*(*(params.c_out + params.i) + params.j) = sum;

	double millend = omp_get_wtime() * 1000;

	lockObject.lock();
	//callbackstack++;

	concurrentmatrixmulttime_elapsed += millend - millstart;
	concurrentmatrixmultcount++;

	lockObject.unlock();
}

///
/// Waits for all threads to complete or for the max milliseconds.
/// Note, if the max milliseconds are reached, it will throw an exception.
///
void Matrix::Wait(int milliseconds)
{

	double millstart = omp_get_wtime() * 1000;
	while (!multmaxqueue.empty() && callbackstack > 0) {


		if (milliseconds == 0) {
			//Do nothing
		}
		else {
			double millinterval = omp_get_wtime() * 1000;
			if (millinterval - millstart > milliseconds) {
				throw std::runtime_error("Elapsed time to wait exceeded");
			}
		}
	}

	

	

	double millend = omp_get_wtime() * 1000;
	waittime_elapsed += millend - millstart;
	waitcount++;
}

void Matrix::Join() {
	for (int i = 0; i < NUM_THREADS; i++) {
		threadpool[i].join();
	}
}


void Matrix::Copy(double ** _a, double ** _b)
{
	double millstart = omp_get_wtime() * 1000;
	for (int i = 0; i < array_dim; i++) {
		for (int j = 0; j < array_dim; j++) {
			*(*(_b + i) + j) = *(*(_a + i) + j);
		}
	}
	double millend = omp_get_wtime() * 1000;
	copytime_elapsed += millend - millstart;
	copycount++;
}

void Matrix::ConcurrentCopy(COPYPARAMS params)
{
	for (int i = 0; i < array_dim; i++) {
		for (int j = 0; j < array_dim; j++) {
			*(*(params.b_out + i) + j) = *(*(params.a_in + i) + j);
		}
	}
}

void Matrix::Delete(double **)
{
}


double ** Matrix::Power(int p)
{
	double millstart = omp_get_wtime() * 1000;

	if (p < 0) {
		throw; //Don't even think about it.
	}
	
	if (p == 0) {
		throw; //Pretend we don't know this is supposed to return the identity matrix.
	}
	
	if (p == 1) {
		Copy(a, c);
		return c;
	}

	Copy(a, b);

	//Multiply b by a, Modify c.
	for (int i = 1; i < p; i++) {
		Multiply(a, b, c);
		//b = c;
		double ** temp = c;
		c = b;
		b = temp;
		//Copy(c, b);
	}

	double millend = omp_get_wtime() * 1000;
	powertime_elapsed = millend - millstart;
	powercount++;

	return c;

}

double ** Matrix::ConcurrentPower(int p)
{
	
	double millstart = omp_get_wtime() * 1000;
	killswitch = 1;

	threadpool = new std::thread[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++) {
		threadpool[i] = std::thread(&Matrix::ConcurrentCall, this, i);
	}

	if (p < 0) {
		throw; //Don't even think about it.
	}

	if (p == 0) {
		throw; //Pretend we don't know this is supposed to return the identity matrix.
	}

	if (p == 1) {
		Copy(a, c);
		return c;
	}

	Copy(a, b);

	//Multiply b by a, Modify c.
	for (int i = 1; i < p; i++) {
		ConcurrentMultiply(a, b, c);
		Copy(c, b);
	}

	double millend = omp_get_wtime() * 1000;
	concurrentpowertime_elapsed += millend - millstart;
	concurrentpowercount++;
	killswitch = 0;
	Wait(0);
	Join();
	return c;
}

Matrix::~Matrix()
{
	for (int i = 0; i < array_dim; i++) {
		delete[] a[i];
		delete[] b[i];
		delete[] c[i];
	}
	delete[] paramArr;
	delete[] a;
	delete[] b;
	delete[] c;
	//delete[] threadpool;
	//delete[] copyThreadPool;
	callbackstack = 0;
	
}

double Matrix::GetElapsedMatrMultMill()
{
	return matrixmulttime_elapsed;
}

double Matrix::GetElapsedMatrMultConcMill()
{
	return concurrentmatrixmulttime_elapsed;;
}

double Matrix::GetElapsedMultMill()
{
	return multtime_elapsed;
}

double Matrix::GetElapsedMultConcMill()
{
	return concurrentmulttime_elapsed;
}

double Matrix::GetElapsedMatrCopyMill()
{
	return copytime_elapsed;
}

double Matrix::GetElapsedPowerMill()
{
	return powertime_elapsed;
}

double Matrix::GetElapsedPowerConcMill()
{
	return concurrentpowertime_elapsed;
}

double Matrix::GetElapsedWaitMill()
{
	return waittime_elapsed;
}

int Matrix::GetMatrMultCount()
{
	return matrixmultcount;
}

int Matrix::GetMatrMultConcCount()
{
	return concurrentmatrixmultcount;
}

int Matrix::GetMultCount()
{
	return multcount;
}

int Matrix::GetMultConcCount()
{
	return concurrentmultcount;
}

int Matrix::GetCopyCount()
{
	return copycount;
}

int Matrix::GetPowerCount()
{
	return powercount;
}

int Matrix::GetConcurrentPowerCount()
{
	return concurrentpowercount;
}

int Matrix::GetWaitCount()
{
	return waitcount;
}

int Matrix::GetDimension() {
	return array_dim;
}

void Matrix::ClearMeasurements()
{
	copytime_elapsed = 0;
	copycount = 0;

	matrixmulttime_elapsed = 0;
	matrixmultcount = 0;

	multtime_elapsed = 0;
	multcount = 0;

	powertime_elapsed = 0;
	powercount = 0;

	concurrentcopytime_elapsed = 0;
	concurrentcopycount = 0;

	concurrentmulttime_elapsed = 0;
	concurrentmultcount = 0;

	concurrentmatrixmulttime_elapsed = 0;
	concurrentmatrixmultcount = 0;

	concurrentpowertime_elapsed = 0;
	concurrentpowercount = 0;

	waittime_elapsed = 0;
	waitcount = 0;
}
