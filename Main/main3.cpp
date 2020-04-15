#include <locale.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <time.h>
#include "../Headers/CashTest.h"

hash::sequenceAllocator* methods;



void Init() {
	methods = new hash::sequenceAllocator[3];
	methods[0] = hash::orderedSequence;
	methods[1] = hash::reversedSequence;
	methods[2] = hash::randomSequence2;
}

using std::cout;
using std::endl;
using std::fstream;
using std::setprecision;
using std::fixed;

void Test(size_t MinKB, size_t MaxKB, float K)
{
	Init();
	const int sz = sizeof(int);
	//Килобайты перевожу в количество итераций
	// КБ * 1024 / sizeof(int) = Байты / sizeof(int) = количество итераций
	const size_t Min = MinKB * 1024 / sz; 
	const size_t Max = MaxKB * 1024 / sz;
	const size_t RepeatCount = 20;

	double msec[3];

	fstream csvFile = fstream("output.csv", std::ios::out);
	cout << "Size[KB]; Ordered; Reversed; Random" << endl;
	csvFile << "Size[KB];Ordered;Reversed;Random" << endl;
	for (size_t size = Min; size < Max; size*=K)
	{
		for (size_t m = 0; m < 3; m++)
		{
			int *arr = methods[m](size);
			hash::minimumTime(5, arr, size); // прогреваю кэш
			msec[m] = hash::minimumTime(RepeatCount,arr, size)/(double)size; // выполняю проход
			delete[] arr;
		}
		cout << size / (1024 / sz) << "; " << msec[0] << "; " << msec[1] << "; " << msec[2] << "; " << endl;
		csvFile << size / (1024 / sz) << ";" << setprecision(10) << fixed << msec[0] << ";" << setprecision(10) << fixed << msec[1] << ";" << setprecision(10) << fixed << msec[2] << ";" << endl;
	}
	csvFile.close();
}

void main() {
	srand(time(0));
	cout << "Min KB" << endl;
	size_t MinKB;
	std::cin >> MinKB;

	cout << "Max MB" << endl;
	float MaxMB;
	std::cin >> MaxMB;

	cout << "Growth coefficient" << endl;
	float K;
	std::cin >> K;

	Test(MinKB, 1024 * MaxMB, K);
	system("pause");
}