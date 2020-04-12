#pragma once
	#include<omp.h>
	#include<stdio.h>
	#include<stdlib.h>
	#include<iostream>
	#include<random>
	#include<vector>

namespace hash {
	using std::vector;

	typedef int*(*sequenceAllocator)(size_t);

	int Random(int a, int b) {
		return (double)rand() / (RAND_MAX + 1)*(b - a) + a;
	}

	int indexOf(int item, vector<int>& vec) {
		int N = vec.size();
		for (size_t i = 0; i < N; i++)
			if (vec[i] == item)
				return i;
		return -1;
	}

	// Выполняет один проход по циклическому списку
	// Возвращает количество тактов затраченное на проход
	double runSequence(int*& sec, size_t& length) {
		double time = omp_get_wtime();
		for (size_t i = 0, j = 0; i < length; i++)
			j = sec[j];
		return (omp_get_wtime() - time) / omp_get_wtick();
	}

	// Вычисляет минимальное количество тактов процессора требуемое для прохода пол циклическому списку.
	// count - количество проходом
	// data - циклический список
	// length - длина
	long minimumTime(size_t count, int*& data, size_t& length) {
		double tmp = 0;
		double min_time = runSequence(data, length);
		for (size_t i = 0; i < count-1; i++)
		{
			tmp = runSequence(data, length);
			min_time = (tmp < min_time) ? tmp : min_time;
		}
		return min_time;
	}

	// Возрастающий циклический список
	// length - длина
	int* orderedSequence(size_t length) {
		int* arr = new int[length];
		for (int i = 0; i < length; i++)
			arr[i] = (i < length - 1) ? i + 1 : 0;
		return arr;
	}

	// Обратный циклический список
	// length - длина
	int* reversedSequence(size_t length) {
		int* arr = new int[length];
		for (int i = 0; i < length; i++)
			arr[i] = (i > 0) ? i - 1 : length - 1;
		return arr;
	}

	//Случаный циклический список
	//[Долго выполняется]
	// length - длина
	int* randomSequence(size_t length) {
		int* arr = new int[length]();
		int min = 0, max = length-1;
		int iter = 0, tmp = 0;

		vector<int> iters = vector<int>(length);
		for (int i = 0; i < length; i++)
			iters[i] = i;

		while (!iters.empty())
		{
			if (iters.size() == 1)
				tmp = 0;
			else
				tmp = iters[Random(1, iters.size()-1)];

			//удаляю элемент по индексу сдвигаясь от начала
			iters.erase(iters.begin() + indexOf(tmp, iters));

			arr[iter] = tmp;
			iter = tmp;
		}
		return arr;
	}

	//Псевдослучайный циклический список
	// length - длина
	int* semiRandomSequence(size_t length) {
		int* arr = new int[length]();
		int iter;
		int seqTemplate[5] = { 2, 3, 1, 4, 5 };

		int mod = length % 5;
		int len = length - length%5;
		int* ordered = orderedSequence(mod);

		for (size_t i = 0; i < len; i++)
		{
			iter = (i % 5 == 0) ? i : iter; // индекс начала кратного блока
			arr[i] = iter + seqTemplate[i % 5];
		}
		//заполняю некратную часть возрастающим рядом
		for (size_t i = len; i < length; i++)
			arr[i] = ordered[i % 5] + len;
		arr[length - 1] = 0;
		delete[] ordered;
		return arr;
	}
}
