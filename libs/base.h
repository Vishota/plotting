#pragma once

#include <iostream>

#define EPS 0.00001

using namespace std;


void error() {
		printf("Error");
		exit(-1);
	}
void error(const char mess[]) {
		printf("Error: %s", mess);
		exit(-1);
	}

int charLen(const char* chars) {
		int i = 0;
		while (chars[i] != '\0')
			i++;
		return i;
	}

bool isEqual(double a, double b) {
	return (a - b) <= EPS;
}

int rand(int a, int b) {
	return (a + rand() % (b - a + 1));
}

//getNumber
enum class Signs		{ more, less, more_or_eq, less_or_eq };

template <typename T>
T getNumber() {
	T input;
	while (!(std::cin >> input))
	{
		std::cout << "Invalid input detected. Try again\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
	}
	return input;
}

//template <typename T>
//T getNumber(char* sign, T limit) {
//	T input = getNumber<T>();
//	switch (sign) {
//	case ">":
//		while (input <= limit) {
//			printf("This number must be more than %f. Try again\n", (double)limit);
//			input = getNumber<T>();
//		}
//		break;
//	case ">=":
//		while (input < limit) {
//			printf("This number must be more or equal to %f. Try again\n", (double)limit);
//			input = getNumber<T>();
//		}
//		break;
//	case "<":
//		while (input >= limit) {
//			printf("This number must be less than %f. Try again\n", (double)limit);
//			input = getNumber<T>();
//		}
//		break;
//	case "<=":
//		while (input > limit) {
//			printf("This number must be less or equal to %f. Try again\n", (double)limit);
//			input = getNumber<T>();
//		}
//		break;
//	}
//	return input;
//}

template <typename T>
T getNumber(Signs sign, T limit) {
	T input = getNumber<T>();
	switch (sign) {
	case Signs::more:
		while (input <= limit) {
			printf("This number must be more than %f. Try again\n", (double)limit);
			input = getNumber<T>();
		}
		break;
	case Signs::more_or_eq:
		while (input < limit) {
			printf("This number must be more or equal to %f. Try again\n", (double)limit);
			input = getNumber<T>();
		}
		break;
	case Signs::less:
		while (input >= limit) {
			printf("This number must be less than %f. Try again\n", (double)limit);
			input = getNumber<T>();
		}
		break;
	case Signs::less_or_eq:
		while (input > limit) {
			printf("This number must be less or equal to %f. Try again\n", (double)limit);
			input = getNumber<T>();
		}
		break;
	}
	return input;
}

template <typename T>
T getNumber(T min, T max, bool isStrict) {
	T input;
	input = getNumber<T>();
	while (isStrict ? !(input > min && input < max) : !(input >= min && input <= max)) {
		printf(isStrict ? "This value must be more than %f and less than %f. Try again\n" : "This value must be more or equal to %f and less or equal to %f. Try again\n", (double)min, (double)max);
		input = getNumber<T>();
	}
	return input;
}

