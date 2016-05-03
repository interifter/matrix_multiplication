///
/// main.cpp
/// Honor Code: I pledge that I have neither given nor received help from anyone other than the instructor for all program components included here. 
/// Author: Zachary Haberman <habermanz@cwu.edu>
/// Created: 4/1/2016
/// Description: Driver for demonstrating Matrices.
///

#include <ctime>
#include <cstdio>
#include <string>
#include <thread>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include "SquareMatrix.h"
#include "Matrix.h"
#include <Windows.h>


void RunPerfTest();

void RunPerfPower();
void MatrixMultAccuracyTest();
void CompareMatrices(double*, double**, int);
void MatrixPowerPerfAnalysis(long long);
void RunInteractive();
int GetIntegerInput(int min, int max, char var, char* message);
double *GenerateArrayFromUserInput(int m, SquareMatrix* SMp);
double* PowerArrayFromUserInput(double* a, int m, int k, SquareMatrix* SMp);

void MatrixPowerPerfAnalysis(long long max_mem_bytes) {
	long long max_mem_2 = max_mem_bytes / 2;
	Matrix m1 = Matrix(max_mem_2);

	printf("\r\nMaximum Matrix size with %fMB available: %ix%i\r\n\r\n", max_mem_bytes / 1024.0, m1.GetDimension(), m1.GetDimension());

	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	std::string str;
	str.append("SquareMatrix_PerfAnalysis");
	std::ostringstream buf;
	buf << str << (now->tm_year + 1900);
	buf << std::setfill('0') << std::setw(2) << now->tm_mon;
	buf << std::setfill('0') << std::setw(2) << now->tm_mday;
	buf << std::setfill('0') << std::setw(2) << now->tm_hour;
	buf << std::setfill('0') << std::setw(2) << now->tm_min;
	buf << std::setfill('0') << std::setw(2) << now->tm_sec;
	buf << ".csv";
	str = buf.str();
	buf.clear();
	std::ofstream myfile;
	myfile.open(str);
	myfile << "Matrix Dim,Power,Ser Elapsed Time,Ser Mult Time,Ser MatMult Time,Ser MatrMult Count,Par Elapsed Time,Par Mult Time,Par MatMult Time,Par MatrMultCount,Par WaitTime,Par WaitCount\n";
	int max = m1.GetDimension();
	m1.~Matrix();
	for (int i = 20; i < 10000;) {
		for (int j = 100; j <= 1000; j+=100) {
			myfile << j;
			myfile << ",";
			myfile << i;
			myfile << ",";
			Matrix m2 = Matrix(j);


			printf("\r\nRunning Serial %ix%i Matrix Power(%i)\r\n", j, j, i);
			double** m1R = m2.Power(i);
			
			printf("Elapsed time: %f Milliseconds\r\n", m2.GetElapsedPowerMill());
			myfile << m2.GetElapsedPowerMill();
			myfile << ",";
			myfile << m2.GetElapsedMultMill();
			myfile << ",";
			myfile << m2.GetElapsedMatrMultMill();
			myfile << ",";
			myfile << m2.GetMatrMultCount();
			myfile << ",";

			printf("\r\nRunning Parallel %ix%i Matrix ConcurrentPower(%i)\r\n", j, j, i);
			m2.ClearMeasurements();
			m1R = m2.ConcurrentPower(i);
			printf("Elapsed time: %f Milliseconds\r\n", m2.GetElapsedPowerConcMill());
			myfile << m2.GetElapsedPowerConcMill();
			myfile << ",";
			myfile << m2.GetElapsedMultConcMill();
			myfile << ",";
			myfile << m2.GetElapsedMatrMultConcMill();
			myfile << ",";
			myfile << m2.GetMatrMultConcCount();
			myfile << ",";
			myfile << m2.GetElapsedWaitMill();
			myfile << ",";
			myfile << m2.GetWaitCount();
			myfile << "\n";
			myfile.flush();
		}
		
		myfile.flush();
		if (i <= 25) {
			i += 5;
		}
		else if (i <= 100) {
			i += 10;
		}
		else {
			i += 100;
		}
	}

	myfile.flush();

}

void MatrixMultAccuracyTest() {
	int count = 9;
	int index = 0;
	double test[9] = { 3, -2, 3, 4, 3, 1, 35, 5, 7 };
	double soluMatrix2[9] = { 106, 3, 28, 59, 6, 22, 370, -20, 159 };
	double soluMatrix3[9] = { 1310, -63, 517, 971, 10, 337, 6595, -5, 2203 };
	printf("Base Test Matrix: ");

	for (int i = 0; i < 9; i++) {
		printf("%f ", test[i]);
	}
	printf("\r\n");
	Matrix sm = Matrix(test, 3);	//Serial Matrix.
	Matrix pm = Matrix(test, 3);	//Parallel Matrix.

	double** smRof2 = sm.Power(2);
	double** pmRof2 = pm.ConcurrentPower(2);
	printf("\r\nSerial Matrix Power(2) Results\r\n");
	CompareMatrices(soluMatrix2, smRof2, 3);
	printf("\r\nConcurrent Matrix Power(2) Results\r\n");
	CompareMatrices(soluMatrix2, pmRof2, 3);


	double** smRof3 = sm.Power(3);
	double** pmRof3 = pm.ConcurrentPower(3);
	printf("\r\nSerial Matrix Power(3) Results\r\n");
	CompareMatrices(soluMatrix3, smRof3, 3);
	printf("\r\nConcurrent Matrix Power(3) Results\r\n");
	CompareMatrices(soluMatrix3, pmRof3, 3);
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


int main(int argc, char* argv[]) {
	MatrixMultAccuracyTest();
	/*
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	long long mem = status.ullAvailPhys;
	MatrixPowerPerfAnalysis(mem);

*/
	
	
	if (argc > 2 && std::string(argv[1]) == "-rt") {
		if (std::string(argv[2]) == "perftest") {
			RunPerfTest();
		}
		else if (std::string(argv[2]) == "test") {
			//RunTest();
		}
		else if (std::string(argv[2]) == "manual") {
			RunInteractive();
		}
		else if (std::string(argv[2]) == "mtest") {
			//RunMultithreadTest();
		}
		else {
			//ShowHelp();
		}
	}
	else {
		//ShowHelp();
	}


	return 0;
}

void RunInteractive() {

	//Input limitations based on lab assignemnt for measuring time costs.
	int m = GetIntegerInput(1, 1000, 'm', "Please enter your matrix size");
	int k = GetIntegerInput(1, 10000, 'k', "Please enter the power to raise your matrix to (Identity Matrix not yet supported)");

	if (m > 100) {
		printf("WARNING: Matrices larger than 100x100 take significant time to run with the naive Power of a Matrix algorithm. Press Ctrl^C to cancel.");
	}

	printf("Thank you for your inputs. Generating [%ix%i] Matrix M with values [0,1]...\r\n", m, m);
	SquareMatrix SM = SquareMatrix{};
	double *M = GenerateArrayFromUserInput(m, &SM);

	printf("Calculating M^%i...\r\n", k);
	double *M2 = PowerArrayFromUserInput(M, m, k, &SM);

	delete[] M;
	delete[] M2;

}

double* PowerArrayFromUserInput(double* a, int m, int k, SquareMatrix* SMp) {
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	double *M = SMp->Power(a, m, k);

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto timespan = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	printf("Array took %i microseconds to raise Matrix to power ", timespan);
	printf("%i\r\n", k); //Not sure why this won't print correctly of added to previous printf.

	return M;
}

double *GenerateArrayFromUserInput(int m, SquareMatrix* SMp) {
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	double *M = SMp->Generate(m);

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto timespan = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	printf("Array took %i microseconds to generate.\r\n", timespan);
	if (m < 7) {
		printf("Generated Matrix:\r\n");
		SMp->Print(M, m);
	}
	else {
		printf("Matrix too large to print.\r\n");
	}

	return M;
}

int GetIntegerInput(int min, int max, char var, char* message) {
	int valid = 0;
	int temp = 0;
	while (!valid) {

		std::printf("%s (%c): ", message, var);
		if (std::cin >> temp) {
			if (temp < 1 || temp > max) {
				printf("Invalid Input for %c. Expectation is an integer from %i-%i.\r\n", var, min, max);
				std::cin.clear();
				//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else {
				valid = 1;
			}
		}
		else
		{
			printf("Invalid Input for %c. Expectation is an integer from %i-%i.\r\n", var, min, max);
			std::cin.clear();
			//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

	}

	return temp;
}


void RunPerfPower() {
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	std::string str;
	str.append("SquareMatrix_run_StaMatricDynPower");
	std::ostringstream buf;
	buf << str << (now->tm_year + 1900);
	buf << std::setfill('0') << std::setw(2) << now->tm_mon;
	buf << std::setfill('0') << std::setw(2) << now->tm_mday;
	buf << std::setfill('0') << std::setw(2) << now->tm_hour;
	buf << std::setfill('0') << std::setw(2) << now->tm_min;
	buf << std::setfill('0') << std::setw(2) << now->tm_sec;
	buf << ".csv";
	str = buf.str();
	buf.clear();
	std::ofstream myfile;
	myfile.open(str);
	myfile << "Matrix Size (MxM),Power,Power Execution Time\n";

	try {

		SquareMatrix M = SquareMatrix{};
		double * m;
		for (int i = 0; i <= 100; i += 10) {

			m = M.Generate(i);


			for (int j = 10000; j > 0; j -= 250) {
				printf("%ix%i Matrix raised to %i running...\r\n", i, i, j);
				std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

				//Power Matrix... still looks like there might still be a memory leak, but it's (much) better.
				//Really need to learn best practices for mem management with objects.
				double * p = M.Power(m, i, j);
				delete[] p;
				std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

				auto timespan = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
				myfile << i;
				myfile << ",";
				myfile << j;
				myfile << ",";
				myfile << timespan;
				myfile << "\n";
				myfile.flush();

			}

			myfile.flush();

			//delete[] m; Calling this throws an exception... best guess is I'm missing an explicite Copy somewhere.
		}
	}

	catch (const std::exception& ex) {
		printf(ex.what());
	}
	catch (const std::string& ex) {
		printf(ex.data());
	}
	catch (...) {
		printf("Unhandled exception.");
	}

	myfile.close();
}

void RunPerfTest() {

	//Running Power Perf Test
	printf("Running Power Perf Test: Matrix size is static while power is changed.");
	RunPerfPower();
	
}

