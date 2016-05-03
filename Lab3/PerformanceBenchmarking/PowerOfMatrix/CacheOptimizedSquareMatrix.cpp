#include "CacheOptimizedSquareMatrix.h"
#include <chrono>
#include <omp.h>


CacheOptimizedSquareMatrix::CacheOptimizedSquareMatrix(int d, int level, BOUND bound) : SquareMatrix(d)
{
	InitCacheInformation(level, bound);
}

CacheOptimizedSquareMatrix::CacheOptimizedSquareMatrix(long long mem, int level, BOUND bound) : SquareMatrix(mem)
{
	InitCacheInformation(level, bound);
}

CacheOptimizedSquareMatrix::CacheOptimizedSquareMatrix(double ** arr, int d, int level, BOUND bound) : SquareMatrix(arr, d)
{
	InitCacheInformation(level, bound);
}

CacheOptimizedSquareMatrix::CacheOptimizedSquareMatrix(double * arr, int d, int level, BOUND bound) : SquareMatrix(arr, d)
{
	InitCacheInformation(level, bound);
}

CacheOptimizedSquareMatrix::~CacheOptimizedSquareMatrix()
{
	//SquareMatrix::~SquareMatrix();
}




///
/// Algorithm based on:
/// http://stackoverflow.com/questions/794632/programmatically-get-the-cache-line-size
/// https://github.com/NickStrupat/CacheLineSize
/// Windows API:
/// https://msdn.microsoft.com/en-us/library/ms681979.aspx (CACHE_DESCRIPTOR structure)
/// https://msdn.microsoft.com/en-us/library/ms683194.aspx (GetLogicalProcessorInformation function)
/// https://msdn.microsoft.com/en-us/library/ms686694.aspx (SYSTEM_LOGICAL_PROCESSOR_INFORMATION structure)
///
void CacheOptimizedSquareMatrix::InitCacheInformation(int level, BOUND bound)
{
	lineSize = 0;
	cacheSize = 0;
	DWORD buffer_size = 0;
	DWORD i = 0;
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION * buffer = 0;

	GetLogicalProcessorInformation(0, &buffer_size);
	buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
	GetLogicalProcessorInformation(&buffer[0], &buffer_size);

	for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
		if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == level) {
			lineSize = buffer[i].Cache.LineSize;
			cacheSize = buffer[i].Cache.Size;
			cacheLineCount = cacheSize / lineSize ;
			if (bound == BOUND::LOWER) {
				tileSize = floor(sqrt(cacheLineCount)) / sizeof(double);
			}
			else if (bound == BOUND::UPPER) {
				tileSize = ceil(sqrt(cacheLineCount)) / sizeof(double);
			}
			
			break;
		}
	}

	free(buffer);

}


///
/// https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
/// [1] Skiena, Steven (2008). The Algorithm Design Manual. Springer. pp. 45–46, 401–403. doi:10.1007/978-1-84800-070-4_4 (https://dx.doi.org/10.1007%2F978-1-84800-070-4_4)
/// [3] Amarasinghe, Saman; Leiserson, Charles (2010). "6.172 Performance Engineering of Software Systems, Lecture 8" (http://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-172-performance-engineering-of-software-systems-fall-2010/video-lectures/lecture-8-cache-efficient-algorithms/). MIT OpenCourseWare. Massachusetts Institute of Technology. Retrieved 27 January 2015.
/// [4] Lam, Monica S.; Rothberg, Edward E.; Wolf, Michael E. (1991). The Cache Performance and Optimizations of Blocked Algorithms. Int'l Conf. on Architectural Support for Programming Languages and Operating Systems (ASPLOS).
///
void CacheOptimizedSquareMatrix::Multiply(double **, double **, double **)
{
	double aVal = 0;
	double bVal = 0;
	double sum = 0;

	for (int I = 0; I < array_dim; I += tileSize) {
		for (int J = 0; J < array_dim; J += tileSize) {
			for (int K = 0; K < array_dim; K += tileSize) {
				for (int i = I; i < min(I + tileSize, array_dim); i++) {
					for (int j = J; j < min(J + tileSize, array_dim); j++) {
						sum = 0;
						for (int k = 0; k < min(K + tileSize, array_dim); k++) {
							aVal = *(*(a + i) + k);
							bVal = *(*(b + k) + j);
							sum += aVal * bVal;
						}

						*(*(c + i) + j) = sum;
					}
				}
			}
		}
	}

	//for (int i = 0; i < array_dim; i++) {
	//	for (int j = 0; j < array_dim; j++) {
	//		sum = 0;
	//		for (int k = 0; k < array_dim; k++) {
	//			aVal = *(*(a + i) + k);
	//			bVal = *(*(b + k) + j);
	//			sum += aVal * bVal;
	//		}

	//		*(*(c + i) + j) = sum;
	//	}
	//}
}


