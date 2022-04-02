#pragma once

template <typename T>
class DynArr {
private:
	int pSize, lSize;	//physical size, logical size
	T* arr;
public:
	int getPhysSize() {
		return pSize;
	}
	T* getArrPtr() {
		return arr;
	}
	int getLogSize() {
		return lSize;
	}
	void expand(int newPhysSize) {
		T* tmp = new T[newPhysSize];
		pSize = newPhysSize;
		for (int i = 0; i < lSize; i++)
			tmp[i] = arr[i];
		delete[]arr;
		arr = tmp;
	}
	T& operator[] (int num) {
		return arr[num];
	}
	void swap(int num1, int num2) {
		T buff = arr[num1];
		arr[num1] = arr[num2];
		arr[num2] = buff;
	}
	void add(T value) {
		if (lSize == pSize) {
			expand(pSize + 1);
		}
		lSize++;
		arr[lSize - 1] = value;
	}
	void insert(T el, int pos) {
		if (lSize == pSize)
			expand(pSize + 1);
		for (int i = pos; i < lSize; i++)
			arr[i + 1] = arr[i];
		lSize++;
		arr[pos] = el;
	}
	void del(int pos) {
		for (int i = pos; i < lSize - 1; i++)
			arr[i] = arr[i + 1];
		lSize--;
	}
	void sort(bool (*condition)(T, T)) {
		for (int i = 0; i < lSize; i++) {
			int k = i;
			for (int j = i; j < lSize; j++) {
				if (condition(arr[j], arr[k]))
					k = j;
			}
			swap(i, k);
		}
	}

	DynArr() {
		pSize = lSize = 0;
	}
	DynArr(int reserveSize) {
		lSize = 0;
		pSize = reserveSize;
		arr = new T[pSize];
	}
	DynArr(T* arr_, int size) {
		pSize = lSize = size;
		arr = new T[pSize];
		for (int i = 0; i < size; i++)
			arr[i] = arr_[i];
	}
	DynArr(T* arr_, int logicSize, int physSize) {
		lSize = logicSize;
		pSize = physSize;
		arr = new T[pSize];
		for (int i = 0; i < lSize; i++)
			arr[i] = arr_[i];
	}
	~DynArr() {
		delete[]arr;
	}
};