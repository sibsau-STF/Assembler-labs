#include <conio.h>
#include <iostream>
#include <fstream>
#include <iomanip>

// Закоментировать для использования для замера времени omp.h вместо time.h
#define TIME_H

#ifdef TIME_H
#include <time.h>
#else
#include <omp.h>
#endif // TIME_H

#define CACHE_SIZE	(8 * 1024 * 1024)						// Размер кэш-памяти					(В Byte)		(Используется 8 Mb)
#define TYPE_SIZE	(4)										// Тип данных							(В Byte)		(Используется int (4 byte))
#define FRAG_COUNT	(32)									// Число фрагментов						(Количество)	(Используется 32)
#define FRAG_OFFSET	(CACHE_SIZE / TYPE_SIZE)				// Растояние между началами фрагментов	(Количество)
#define FRAG_SIZE	(CACHE_SIZE / FRAG_COUNT / TYPE_SIZE)	// Размер фрагмента						(Количество)
#define ARR_SIZE	(FRAG_COUNT * FRAG_OFFSET)				// Размер массива						(Количество)

//Функция подготовки массива
void prepareArr(int *arr, int size)
{
	int temp = 0;
	for (int i = 0; i < size; i++)
		arr[i] = -1;
	for (int i = 0; i < size && temp < FRAG_SIZE;)
	{
		arr[i] = i + FRAG_OFFSET;
		if (arr[i] >= size)
		{
			temp++;
			arr[i] = temp;
		}
		i = arr[i];
	}
}

int main()
{
	std::cout << "\nInfo:" << std::endl;
	std::cout << " Cache size . . . . . . . : " << CACHE_SIZE << " b (" << CACHE_SIZE / (1024.0 * 1024.0) << " Mb)" << std::endl;
	std::cout << " Data type size . . . . . : " << TYPE_SIZE << " b" << std::endl;
	std::cout << " Number of fragments. . . : " << FRAG_COUNT << std::endl;
	std::cout << " Fragment offset. . . . . : " << FRAG_OFFSET << std::endl;
	std::cout << " Fragment size. . . . . . : " << FRAG_SIZE << std::endl;
	std::cout << " Max array size . . . . . : " << ARR_SIZE << " (" << ARR_SIZE * TYPE_SIZE << " b)\n" << std::endl;
	std::cout << "Results:" << std::endl;
	std::cout << " [fragments];\t[time]" << std::endl;
	std::ofstream outputFile("results.txt");
	for (int frag = 1; frag <= FRAG_COUNT; frag++)
	{
		int *arr = new int[frag * FRAG_OFFSET];
		prepareArr(arr, frag * FRAG_OFFSET);
#ifdef TIME_H
			double temp_time = -clock();
#else
			double temp_time = -omp_get_wtime();
#endif // TIME_H
		for (int i = 0; i >= 0 && i < frag * FRAG_OFFSET;)
			i = arr[i];
#ifdef TIME_H
			temp_time += clock();
#else
			temp_time += omp_get_wtime();
#endif // TIME_H
		temp_time /= (frag * FRAG_OFFSET);
		std::cout << " " << frag << ";\t" << std::fixed << std::setprecision(18) << temp_time << std::endl;
		outputFile << frag << ";" << std::fixed << std::setprecision(18) << temp_time << std::endl;
		delete[]arr;
	}
	outputFile.close();
	std::cout << "\nPress eny key . . . ";
	_getch();
	return 0;
}
