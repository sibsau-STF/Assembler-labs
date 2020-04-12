#include <locale.h>
#include <iostream>
#include <time.h>
#include "../Headers/CashTest.h"
#include <fstream>

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

void Test(size_t MinKB, size_t MaxKB)
{
	Init();
	//Килобайты перевожу в количество итераций
	size_t Min = MinKB * 2 * 1024; // sizeof(int)*2 = 1Byte
	size_t Max = MaxKB * 2 * 1024;
	size_t RepeatCount = 10;
	fstream csvFile = fstream("output.csv", std::ios::out);
	cout << "Size[Byte]; Ordered; Reversed; Random" << endl;
	csvFile << "Size[Byte];Ordered;Reversed;Random" << endl;
	for (size_t size = Min; size < Max; size*=1.25)
	{
		double msec[3];
		for (size_t m = 0; m < 3; m++)
		{
			int * arr = methods[m](size);
			hash::minimumTime(50, arr, size); // прогреваю хеш
			msec[m] = hash::minimumTime(RepeatCount, arr, size); // выполняю проход
			delete[] arr;
		}
		cout << size / (2 * 1024) << "; " << msec[0] << "; " << msec[1] << "; " << msec[2] << "; " << endl;
		csvFile << size / (2 * 1024) << ";" << msec[0] << ";" << msec[1] << ";" << msec[2] << ";" << endl;
	}
	csvFile.close();
}

void main() {
	srand(time(0));
	//Test(64, 1024 * 6);
	size_t len = 9;
	int* arr = hash::reversedSequence(len);
	for (size_t i = 0; i < len; i++)
	{
		cout << arr[i] << "  ";
	}
	cout << endl;
	hash::minimumTime(2, arr, len);
	system("pause");
}