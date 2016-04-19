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

void CompareMatrices(double*, double*);
void RunPerfTest();
void RunPerfMatrix();
void RunPerfPower();
void RunTest();
void RunInteractive();
void ShowHelp();
int GetIntegerInput(int, int, char, char*);
double *GenerateArrayFromUserInput(int m, SquareMatrix* SMp);
double* PowerArrayFromUserInput(double*, int, int, SquareMatrix*);

int main(int argc, char* argv[]) {

	if (argc > 2 && std::string(argv[1]) == "-rt") {
		if (std::string(argv[2]) == "perftest") {
			RunPerfTest();
		}
		else if (std::string(argv[2]) == "test") {
			RunTest();
		}
		else if (std::string(argv[2]) == "manual") {
			RunInteractive();
		}
		else {
			ShowHelp();
		}
	}
	else {
		ShowHelp();
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
				std::cin.ignore(std::numeric_limits<std::streamsize> ::max(), '\n');
			}
			else {
				valid = 1;
			}
		}
		else
		{
			printf("Invalid Input for %c. Expectation is an integer from %i-%i.\r\n", var, min, max);
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize> ::max(), '\n');
		}

	}

	return temp;
}


void ShowHelp() {
	printf("Please use -rt [test|perftest|manual].\r\n");
	printf("Note: perftest does not print any results and runs for hours. Results are stored in a timestamped CSV.");
}

void RunTest() {
	double test[9] = { 3, -2, 3, 4, 3, 1, 35, 5, 7 };
	double soluMatrix2[9] = { 106, 3, 28, 59, 6, 22, 370, -20, 159 };
	double soluMatrix3[9] = { 1310, -63, 517, 971, 10, 337, 6595, -5, 2203 };

	SquareMatrix sq1 = SquareMatrix{};
	double* sq2 = sq1.Power(test, 3, 2);
	double* sq3 = sq1.Power(test, 3, 3);

	CompareMatrices(test, test);
	CompareMatrices(sq2, soluMatrix2);
	CompareMatrices(sq3, soluMatrix3);
	CompareMatrices(test, test);
}

void RunPerfMatrix() {
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	std::string str;
	str.append("SquareMatrix_run_DynMatrixStaPower");
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
		for (int j = 10000; j > 0; j -= 250) {
			
			for (int i = 0; i <= 100; i += 10) {

			m = M.Generate(i);


			
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
	

	printf("Running Matrix Perf Test: Matrix size is changed while power is static.");
	RunPerfMatrix();

	//Running Power Perf Test
	printf("Running Power Perf Test: Matrix size is static while power is changed.");
	RunPerfPower();
	
}



void CompareMatrices(double *actual, double* expected) {
	for (int i = 0; i < 9; i++) {
		double actVal = *(actual + i);
		double expVal = *(expected + i);
		if (actVal == expVal) {
			printf("[%i]: %f == %f\r\n", i, actVal, expVal);
		}
		else {
			printf("Mismatch at [%i]: Actual: %f, Expected: %f\r\n", i, actVal, expVal);
		}
	}
}
