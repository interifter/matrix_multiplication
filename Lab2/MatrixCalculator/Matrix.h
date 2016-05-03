///
/// Matrix.h
/// Honor Code: I pledge that I have neither given nor received help from anyone other than the instructor for all program components included here. 
/// Author: Zachary Haberman <habermanz@cwu.edu>
/// Created: 4/19/2016
/// Description: Object used for simple Square Matrix multiplication.
///

#pragma once
#ifndef MATRIX_H
#define MATRIX_H
#include <thread>
#include <mutex>
#include <queue>

class Matrix {

	struct MULTMATRIXPARAMS {
		double** a_in;
		double** b_in;
		double** c_out;
		int i;
		int j;
		int set;
	};

	struct COPYPARAMS {
		double** a_in;
		double** b_out;
	};

	double** a;
	double** b;
	double** c;
	int array_dim;
	int killswitch;
	std::queue<int> multmaxqueue;

	MULTMATRIXPARAMS *paramArr;
	std::thread* threadpool;
	static std::mutex lockObject;	//Must be static unless I want to figure out this error: error C2280: attempting to reference a deleted function.
									//See stackoverflow: http://stackoverflow.com/questions/25522118/c-how-to-use-stdmutex-and-stdlock-guard-with-functor?lq=1 
									//				and http://stackoverflow.com/questions/21409439/error-c2280-attempting-to-reference-a-deleted-function

	std::thread* copyThreadPool;
	static std::mutex copyLockObject;

	int callbackstack;

	double copytime_elapsed;
	int copycount;

	double matrixmulttime_elapsed;
	int matrixmultcount;

	double multtime_elapsed;
	int multcount;

	double powertime_elapsed;
	int powercount;

	double concurrentcopytime_elapsed;
	int concurrentcopycount;

	double concurrentmulttime_elapsed;
	int concurrentmultcount;

	double concurrentmatrixmulttime_elapsed;
	int concurrentmatrixmultcount;

	double concurrentpowertime_elapsed;
	int concurrentpowercount;

	double waittime_elapsed;
	int waitcount;




	double Sqrt(double x);
	void Multiply(double**, double**, double**);
	void ConcurrentMultiply(double**, double**, double**);
	void ConcurrentMultMax(MULTMATRIXPARAMS params);
	void MultMat(MULTMATRIXPARAMS params);
	void Wait(int);
	void Join();

	void Copy(double**, double**);
	void ConcurrentCopy(COPYPARAMS params);
	void Delete(double**);
	void Init(int, double*, double**);
	void ConcurrentCall(int);
	

public:
	Matrix(int);
	Matrix(long long);
	Matrix(double**, int);
	Matrix(double*, int);
	double** Power(int p);
	double** ConcurrentPower(int p);
	~Matrix();

	double GetElapsedMatrMultMill();
	double GetElapsedMatrMultConcMill();
	double GetElapsedMultMill();
	double GetElapsedMultConcMill();
	double GetElapsedMatrCopyMill();

	double GetElapsedPowerMill();
	double GetElapsedPowerConcMill();
	double GetElapsedWaitMill();

	int GetMatrMultCount();
	int GetMatrMultConcCount();
	int GetMultCount();
	int GetMultConcCount();
	int GetCopyCount();

	int GetPowerCount();
	int GetConcurrentPowerCount();
	int GetWaitCount();

	int GetDimension();

	void ClearMeasurements();

};

#endif