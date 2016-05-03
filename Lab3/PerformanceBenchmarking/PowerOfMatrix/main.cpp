#include "SquareMatrix.h"
#include "CacheOptimizedSquareMatrix.h"
#include "CacheImprovedSquareMatrix.h"
#include "MathOptimizedSquareMatrix.h"
#include "OptimizedSquareMatrix.h"
#include <cstdio>
#include <ctime>
#include <string>
#include <thread>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <fstream>

void CompareMatrices(double*, double**, int);
void CompareMatrices(double**, double**, int);
bool CompareMatrices(double**, double**, int, bool);
void MatrixPerformanceCompareTest();
void MatrixMultAccuracyTest();



int main(int arc, char* argv) {
	MatrixMultAccuracyTest();
	MatrixPerformanceCompareTest();
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

void CompareMatrices(double** expected, double** actual, int d) {
	//*(arr1d + i * d + j);
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			double expectedValue = *(*(expected + i) + j);
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

bool CompareMatrices(double** expected, double** actual, int d, bool b) {
	b = true;
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			double expectedValue = *(*(expected + i) + j);
			double actual__Value = *(*(actual + i) + j);

			if (abs(expectedValue - actual__Value) > 0.05) {
				b = false;
				//printf("**ERR** Expected: %f. Actual: %f\r\n", expectedValue, actual__Value);
			}
			else {
				//printf("*PASS* %f == %f\r\n", expectedValue, actual__Value);
			}
		}
	}

	return b;
}


void MatrixMultAccuracyTest() {
	int count = 9;
	int index = 0;
	double test[9] = { 3, -2, 3, 4, 3, 1, 35, 5, 7 };
	double soluMatrix0[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	double soluMatrix2[9] = { 106, 3, 28, 59, 6, 22, 370, -20, 159 };
	double soluMatrix3[9] = { 1310, -63, 517, 971, 10, 337, 6595, -5, 2203 };
	double soluMatrix4[9] = { 21773, -224, 7486, 14748, -227, 5282, 96870, -2190, 35201 };
	printf("Base Test Matrix: ");

	for (int i = 0; i < 9; i++) {
		printf("%f ", test[i]);
	}
	printf("\r\n");
	 SquareMatrix sm = SquareMatrix(test, 3);	//Serial Matrix.
	 MathOptimizedSquareMatrix mosm = MathOptimizedSquareMatrix(test, 3);
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

	double** smRof4 = sm.Power(4);
	printf("\r\nSerial Matrix Power(4) Results\r\n");
	CompareMatrices(smRof4, mosm.Power(4), 3);

	for (int i = 5; i <= 10; i++) {
		double** smRD = sm.Power(i);
		double** mosmRD = mosm.Power(i);
		printf("\r\nSerial Matrix Power(%i) Results\r\n", i);
		CompareMatrices(smRD, mosmRD, 3);
	}
}

void MatrixPerformanceCompareTest() {
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	std::string str;
	str.append("C:\\Code\\Perf\\SquareMatrix_run_DynMatrixStaPower");
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
	myfile << "Size,Power,A-Naive,B-CachImp,C-MathOpt,D-ComOpt\n";
	bool equal = true;
	for (int d = 5; d <= 1000; d+= 5) {
		for (int p = 5; p <= 1000; p += 5) {
		

			printf("\r\n=====\r\nExecuting %ix%i matrix to Power(%i).\r\n", d, d, p);
			SquareMatrix sm = SquareMatrix(d);
			//CacheOptimizedSquareMatrix cosm_l1_l = CacheOptimizedSquareMatrix(sm.GetMatrix(), d, 1, BOUND::LOWER);
			//CacheOptimizedSquareMatrix cosm_l2_l = CacheOptimizedSquareMatrix(sm.GetMatrix(), d, 2, BOUND::LOWER);
			//CacheOptimizedSquareMatrix cosm_l3_l = CacheOptimizedSquareMatrix(sm.GetMatrix(), d, 3, BOUND::LOWER);
			//CacheOptimizedSquareMatrix cosm_l1_u = CacheOptimizedSquareMatrix(sm.GetMatrix(), d, 1, BOUND::UPPER);
			//CacheOptimizedSquareMatrix cosm_l2_u = CacheOptimizedSquareMatrix(sm.GetMatrix(), d, 2, BOUND::UPPER);
			//CacheOptimizedSquareMatrix cosm_l3_u = CacheOptimizedSquareMatrix(sm.GetMatrix(), d, 3, BOUND::UPPER);
			CacheImprovedSquareMatrix cism = CacheImprovedSquareMatrix(sm.GetMatrix(), d);
			MathOptimizedSquareMatrix mosm = MathOptimizedSquareMatrix(sm.GetMatrix(), d);
			OptimizedSquareMatrix osm = OptimizedSquareMatrix(sm.GetMatrix(), d);


			double **smA = sm.Power(p);
			//double **smO_l1_l = cosm_l1_l.Power(p);
			//double **smO_l2_l = cosm_l2_l.Power(p);
			//double **smO_l3_l = cosm_l3_l.Power(p);
			//double **smO_l1_u = cosm_l1_u.Power(p);
			//double **smO_l2_u = cosm_l2_u.Power(p);
			//double **smO_l3_u = cosm_l3_u.Power(p);
			double **cismA = cism.Power(p);
			double **mosmA = mosm.Power(p);
			double **osmA = osm.Power(p);

			

			printf("STD MATRIX: Multiplication Execution Time: %f\r\n", sm.GetElapsedMultTime());
			//printf("L1 LOWER M: Multiplication Execution Time: %f\r\n", cosm_l1_l.GetElapsedMultTime());
			//printf("L2 LOWER M: Multiplication Execution Time: %f\r\n", cosm_l2_l.GetElapsedMultTime());
			//printf("L3 LOWER M: Multiplication Execution Time: %f\r\n", cosm_l3_l.GetElapsedMultTime());
			//printf("L1 UPPER M: Multiplication Execution Time: %f\r\n", cosm_l1_u.GetElapsedMultTime());
			//printf("L2 UPPER M: Multiplication Execution Time: %f\r\n", cosm_l2_u.GetElapsedMultTime());
			//printf("L3 UPPER M: Multiplication Execution Time: %f\r\n", cosm_l3_u.GetElapsedMultTime());
			printf("CISM T1  M: Multiplication Execution Time: %f\r\n", cism.GetElapsedMultTime());
			printf("MOSM T1  M: Multiplication Execution Time: %f\r\n", mosm.GetElapsedMultTime());
			printf("COSM T1  M: Multiplication Execution Time: %f\r\n", osm.GetElapsedMultTime());


			myfile << d;
			myfile << ",";
			myfile << p;
			myfile << ",";
			printf("Comparing A to B, C...\r\n");
			equal = CompareMatrices(smA, cismA, d, equal);
			if (equal) {
				//myfile << "TRUE";
				printf("A === B\r\n");
			}
			else {
				//myfile << "FALSE";
				printf("A !== B\r\n");
			}
			//myfile << ",";
			equal = CompareMatrices(smA, mosmA, d, equal);
			if (equal) {
				//myfile << "TRUE";
				printf("A === C\r\n");
			}
			else {
				//myfile << "FALSE";
				printf("A !== C\r\n");
			}
			//myfile << ",";
			myfile << sm.GetElapsedMultTime();
			myfile << ",";
			//myfile << cosm_l3_l.GetElapsedMultTime();
			//myfile << ",";
			//myfile << cosm_l3_u.GetElapsedMultTime();
			myfile << cism.GetElapsedMultTime();
			myfile << ",";
			myfile << mosm.GetElapsedMultTime();
			myfile << ",";
			myfile << osm.GetElapsedMultTime();
			myfile << ",";
			myfile << "\n";

		}

		myfile.flush();


	}

	myfile.flush();
	myfile.close();
}
