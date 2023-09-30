#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <vector>
#include <Windows.h>
#include <time.h>

using namespace std;

struct Records 
{
	string Name;
	string Brand;
	string Category;
	string ProductLine;
	int Count = 0;
	float Price = 0;
};

vector <Records> Import(string path);
void file1(vector <Records>* arr);
void file2(vector <Records>* arr);
void file3(vector <Records>* arr);
void file4(vector <Records>* arr);
void file5(vector <Records>* arr);

int main() 
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char key = 0;
	vector <Records> Input;

	cout << "1 - Исходный файл с таблицей." << endl
		 << "2 - Задать путь к файлу." << endl;

	while (key != 27) {
		key = _getch();
		string name = "File.csv";

		switch (key) {
		case '1':
			Input = Import(name);
			if (Input.size() == 0) { // Для пустой таблицы
				return 0;
			}
			cout << "Исходный файл загружен." << endl;
			key = 27; // Выход из цикла
			break;
		case '2':
			cout << "Путь к файлу: ";
			cin >> name;
			Input = Import(name);

			key = 27; // Выход из цикла
			break;
		case 27:
			cout << "Работа программы завершена." << endl;
			return 0;
		default:
			cout << "Не корректный ввод, повторите попытку." << endl;
			break;
		}
	}

	key = 0;

	while (true) {
		cout << "1 - Сортировка брендов по алфавиту (Сортировка слиянием)." << endl
			 << "2 - Сортировка по количеству и нахождение общей суммы продаж (Сортировка расчёской)." << endl
			 << "3 - Сортировка по цене и нахождение общей суммы продаж (Сортировка вставками)." << endl
			 << "4 - Найти суммы для брендов по каждой категории и сделать сортировку по количеству (Сортировка слиянием + Сортировка расчёской)." << endl
			 << "5 - Найти суммы для брендов и сделать сортировку по сумме (Сортировка слиянием + Сортировка вставками)." << endl
			 << "ESC - Выход из программы." << endl;

		key = _getch();

		switch (key)
		{
		case '1':
			file1(&Input);
			break;
		case '2':
			file2(&Input);
			break;
		case '3':
			file3(&Input);
			break;
		case '4':
			file4(&Input);
			break;
		case '5':
			file5(&Input);
			break;
		case 27:
			cout << "Работа программы завершена." << endl;
			return 0;
		default:
			cout << "Такой команды нет, повторите попытку." << endl;
			break;
		}
	}
}

vector <Records> Import(string name) {
	vector <Records> Input;
	ifstream ifile;
	ifile.open(name);
	if (!ifile.is_open()) {
		cout << "Такого файла не существует." << endl;
	}
	else {
		char* CharString;
		string RecordFromFile;
		getline(ifile, RecordFromFile);
		char* next_token = nullptr;
		while (true)
		{
			Input.push_back(Records());
			getline(ifile, RecordFromFile);
			if (RecordFromFile == "") {
				break;
			}
			int Index = Input.size() - 1;
			CharString = strtok_s(&RecordFromFile[0], ";", &next_token);
			Input[Index].Name = string(CharString);
			CharString = strtok_s(NULL, ";", &next_token);
			Input[Index].Brand = string(CharString);
			CharString = strtok_s(NULL, ";", &next_token);
			Input[Index].ProductLine = string(CharString);
			CharString = strtok_s(NULL, ";", &next_token);
			Input[Index].Category = string(CharString);
			CharString = strtok_s(NULL, ";", &next_token);
			Input[Index].Count = atoi(CharString);
			CharString = strtok_s(NULL, ";", &next_token);
			Input[Index].Price = stof(string(CharString));
		}
		Input.pop_back();
		ifile.close();
	}
	return Input;
}

void CombSort(vector <Records>* arr) // Сортировка расчёской; arr — Название вектора
{
	int time = clock();
	double factor = 1.2473309; // Фактор уменьшения
	int step = arr[0].size() - 1; // Шаг сортировки

	while (step >= 1)
	{
		for (int i = 0; (i + step) < (int)arr[0].size(); i++)
		{
			if (arr[0][i].Count > arr[0][i + step].Count)
			{
				swap(arr[0][i], arr[0][i + step]);
			}
		}
		step /= factor;
	}
	cout << "Сортировка расчёской заняла (сек.): " << (clock() - time) / 1000.0 << endl;
}

void InsertSort(vector <Records>* arr) // Сортировка вставками
{
	int time = clock();
	int i, j;
	for (i = 1; i < (int)arr[0].size(); i++) {
		float Elem = arr[0][i].Price;
		for (j = i - 1; j >= 0 && arr[0][j].Price < Elem; j--) {
			swap(arr[0][j + 1], arr[0][j]);
		}
		arr[0][j + 1].Price = Elem;
	}
	cout << "Сортировка вставками заняла (сек.): " << (clock() - time) / 1000.0 << endl;
}

vector <Records> Merge(vector <Records> left, vector <Records> right);

vector <Records> MergeSort(vector <Records> arr) {  // Сортировка слиянием
	vector <Records> left, right, result;
	if (arr.size() <= 1) {
		return arr;
	}
	else {
		int middle = (arr.size() - 1) / 2;
		for (int i = 0; i <= middle; i++) {
			left.push_back(arr[i]);
		}
		for (int i = middle + 1; i < (int)arr.size(); i++) {
			right.push_back(arr[i]);
		}
		left = MergeSort(left);
		right = MergeSort(right);
		result = Merge(left, right);
		return result;

	}
}

vector <Records> Merge(vector <Records> left, vector <Records> right) {
	vector <Records> reversedResult;
	while (left.size() > 0 && right.size() > 0) {
		if (strcmp(left[0].Brand.c_str(), right[0].Brand.c_str()) == 0) {
			if (strcmp(left[0].Category.c_str(), right[0].Category.c_str()) == 0) {
				if (strcmp(left[0].Name.c_str(), right[0].Name.c_str()) <= 0) {
					reversedResult.push_back(left[0]);
					left.erase(left.begin());
				}
				else {
					reversedResult.push_back(right[0]);
					right.erase(right.begin());
				}
			}
			else if (strcmp(left[0].Category.c_str(), right[0].Category.c_str()) < 0) {
				reversedResult.push_back(left[0]);
				left.erase(left.begin());
			}
			else {
				reversedResult.push_back(right[0]);
				right.erase(right.begin());
			}
		}
		else if (strcmp(left[0].Brand.c_str(), right[0].Brand.c_str()) < 0) {
			reversedResult.push_back(left[0]);
			left.erase(left.begin());
		}
		else {
			reversedResult.push_back(right[0]);
			right.erase(right.begin());;
		}
	}
	while (left.size() > 0) {
		reversedResult.push_back(left[0]);
		left.erase(left.begin());
	}
	while (right.size() > 0) {
		reversedResult.push_back(right[0]);
		right.erase(right.begin());
	}
	return reversedResult;
}

void file1(vector <Records>* arr) {
	int time = clock();
	arr[0] = MergeSort(arr[0]);
	cout << "Сортировка слиянием заняла (сек.): " << (clock() - time) / 1000.0 << endl;
	ofstream ofile;
	ofile.open("file1.csv");
	if (!ofile.is_open()) {
		cout << "Не удалось создать файл. Закройте Excel." << endl;
	}
	else {
		ofile << "Наименование;Бренд;Линейка;Категория;Количество;Сумма\n";
		for (int i = 0; i < (int)arr[0].size(); i++) {
			ofile << arr[0][i].Name << ";" << arr[0][i].Brand << ";" << arr[0][i].ProductLine << ";" << arr[0][i].Category << ";" << arr[0][i].Count << ";" << arr[0][i].Price << "\n";
		}
	}
}

void file2(vector <Records>* arr) {
	CombSort(arr);

	ofstream ofile;
	ofile.open("file2.csv");
	if (!ofile.is_open()) {
		cout << "Не удалось создать файл. Закройте Excel." << endl;
	}
	else {
		ofile << "Наименование;Бренд;Линейка;Категория;Количество;Сумма;Общая Стоимость\n";
		for (int i = 0; i < (int)arr[0].size(); i++) {
			ofile << arr[0][i].Name << ";" << arr[0][i].Brand << ";" << arr[0][i].ProductLine << ";" << arr[0][i].Category << ";" << arr[0][i].Count << ";" << arr[0][i].Price << ";" << arr[0][i].Count * arr[0][i].Price << "\n";
		}
		ofile.close();
	}
}

void file3(vector <Records>* arr) {
	InsertSort(arr);

	ofstream ofile;
	ofile.open("file3.csv");
	if (!ofile.is_open()) {
		cout << "Не удалось создать файл. Закройте Excel." << endl;
	}
	else {
		ofile << "Наименование;Бренд;Линейка;Категория;Количество;Сумма;Общая Стоимость\n";
		for (int i = 0; i < (int)arr[0].size(); i++) {
			ofile << arr[0][i].Name << ";" << arr[0][i].Brand << ";" << arr[0][i].ProductLine << ";" << arr[0][i].Category << ";" << arr[0][i].Count << ";" << arr[0][i].Price << ";" << arr[0][i].Count * arr[0][i].Price << "\n";
		}
		ofile.close();
	}
}

void file4(vector <Records>* arr) {
	int time = clock();
	arr[0] = MergeSort(arr[0]); // Сортировка таблицы по алфавиту
	cout << "Сортировка слиянием заняла (сек.): " << (clock() - time) / 1000.0 << endl;

	vector <Records> arr2; // Создание второй таблицы
	float totalSum = arr[0][0].Price;
	int totalCount = arr[0][0].Count;

	for (int i = 1; i < (int)arr[0].size(); i++) {
		if (strcmp(arr[0][i].Brand.c_str(), arr[0][i - 1].Brand.c_str()) == 0 && strcmp(arr[0][i].Category.c_str(), arr[0][i - 1].Category.c_str()) == 0) {
			totalSum += arr[0][i].Price;
			totalCount += arr[0][i].Count;
		}
		else { // Новая запись таблицы для другой категории или бренда
			arr2.push_back(Records());
			int size = arr2.size() - 1;
			arr2[size].Name = arr[0][i - 1].Name;
			arr2[size].Brand = arr[0][i - 1].Brand;
			arr2[size].Category = arr[0][i - 1].Category;
			arr2[size].Count = totalCount;
			arr2[size].Price = totalSum;

			totalSum = arr[0][i].Price; // Считывание новой суммы
			totalCount = arr[0][i].Count; // Считывание нового количества
		}
	}

	CombSort(&arr2); // Сортировка новой таблицы по убыванию количества

	ofstream ofile;
	ofile.open("file4.csv");
	if (!ofile.is_open()) {
		cout << "Не удалось создать файл. Закройте Excel." << endl;
	}
	else {
		ofile << "Бренд;Категория;Количество;Сумма\n";
		for (int i = 0; i < (int)arr2.size(); i++) {
			ofile << arr2[i].Brand << ";" << arr2[i].Category << ";" << arr2[i].Count << ";" << arr2[i].Price << "\n";
		}
		ofile.close();
	}
}

void file5(vector <Records>* arr) {
	int time = clock();
	arr[0] = MergeSort(arr[0]);
	cout << "Сортировка слиянием заняла (сек.): " << (clock() - time) / 1000.0 << endl;

	vector <Records> arr2;
	float totalSum = arr[0][0].Price;
	int totalCount = arr[0][0].Count;

	for (int i = 1; i < (int)arr[0].size(); i++) {
		if (strcmp(arr[0][i].Brand.c_str(), arr[0][i - 1].Brand.c_str()) == 0) {
			totalSum += arr[0][i].Price;
			totalCount += arr[0][i].Count;
		}
		else {
			arr2.push_back(Records());
			int size = arr2.size() - 1;
			arr2[size].Name = arr[0][i - 1].Name;
			arr2[size].Brand = arr[0][i - 1].Brand;
			arr2[size].Category = arr[0][i - 1].Category;
			arr2[size].Count = totalCount;
			arr2[size].Price = totalSum;

			totalSum = arr[0][i].Price;
			totalCount = arr[0][i].Count;
		}
	}

	InsertSort(&arr2);

	ofstream ofile;
	ofile.open("file5.csv");
	if (!ofile.is_open()) {
		cout << "Не удалось создать файл. Закройте Excel." << endl;
	}
	else {
		ofile << "Бренд;Количество;Сумма\n";
		for (int i = 0; i < (int)arr2.size(); i++) {
			ofile << arr2[i].Brand << ";" << arr2[i].Count << ";" << arr2[i].Price << "\n";
		}
		ofile.close();
	}
}