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
	methods[2] = hash::semiRandomSequence;
}

using std::cout;
using std::endl;
using std::fstream;
using std::setprecision;
using std::fixed;

void Test(size_t MinKB, size_t MaxKB)
{
	Init();
	//Килобайты перевожу в количество итераций
	const int sz = 8 / sizeof(int);
	const size_t Min = MinKB * 1024 * sz;
	const size_t Max = MaxKB * 1024 * sz;
	const size_t RepeatCount = 10;

	double msec[3];
	int *arr = nullptr;

	fstream csvFile = fstream("output.csv", std::ios::out);
	cout << "Size[KB]; Ordered; Reversed; Random" << endl;
	csvFile << "Size[KB];Ordered;Reversed;Random" << endl;
	for (size_t size = Min; size < Max; size*=1.8)
	{
		for (size_t m = 0; m < 3; m++)
		{
			arr = methods[m](size);
			hash::minimumTime(5, arr, size); // прогреваю хеш
			msec[m] = hash::minimumTime(RepeatCount,arr, size)/(double)size; // выполняю проход
			delete[] arr;
		}
		cout << size / (1024 * sz) << "; " << msec[0] << "; " << msec[1] << "; " << msec[2] << "; " << endl;
		csvFile << size / (1024 * sz) << ";" << setprecision(10) << fixed << msec[0] << ";" << setprecision(10) << fixed << msec[1] << ";" << setprecision(10) << fixed << msec[2] << ";" << endl;
	}
	csvFile.close();
}

void main() {
	srand(time(0));
	Test(1, 1024 * 15);
	system("pause");
}