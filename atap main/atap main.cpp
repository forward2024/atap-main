#include <iostream>
#include <cmath>
#include <iomanip> 

using namespace std;

double TaylorTask(int n, double x) {
	if (n == 0) {
		return pow(x, 2);
	}
	double previous = TaylorTask(n - 1, x);
	return previous * (-4.0 * pow(x, 2)) / ((2.0 * n + 1.0) * (2.0 * n + 2.0));
}

double ApproTask(double x, double e, int& n) {
	double sum_series = 0;
	double term;
	n = 0;

	do {
		term = TaylorTask(n, x);
		sum_series += term;
		n++;
	} while (fabs(term) > e);

	return sum_series;
}

void PrintFormatted(double value, int width, int precision, FILE* file = nullptr) {
	char buffer[21];

	int num_chars = snprintf(buffer, sizeof(buffer), "%-*.*f", width, precision, value);

	if (num_chars > width) {
		int start = width - 3;
		buffer[start++] = '.';
		buffer[start++] = '.';
		buffer[start++] = '.';
		buffer[start] = '\0';
	}
	if (file) {
		fprintf(file, "%s", buffer);
	}
	cout << buffer;
}

void ShowTable(double from, double to, double step, double e, FILE* file) {
	int width = 20;
	int precision = 9;

	printf("%-*s|%-*s|%-*s|%-*s|%-*s\n", width, "x", width, "Actual sin^2(x)", width, "Approximation", width, "Error", width, "Steps");
	if (file) {
		fprintf(file, "%-*s|%-*s|%-*s|%-*s|%-*s\n", width, "x", width, "Actual sin^2(x)", width, "Approximation", width, "Error", width, "Steps");
	}

	for (double x = from; x <= to; x += step) {
		int steps = 0;
		double approximation = ApproTask(x, e, steps);
		double actual = pow(sin(x), 2);
		double error = fabs(approximation - actual);

		PrintFormatted(x, width, precision, file); printf("|");
		if (file) fprintf(file, "|");
		PrintFormatted(actual, width, precision, file); printf("|");
		if (file) fprintf(file, "|");
		PrintFormatted(approximation, width, precision, file); printf("|");
		if (file) fprintf(file, "|");
		PrintFormatted(error, width, precision, file); printf("|");
		if (file) fprintf(file, "|");
		PrintFormatted(steps, width, 0, file); printf("\n");
		if (file) fprintf(file, "\n");
	}
}


FILE* openFile(const char* mode, const char* filename, bool& saveFile) {
	FILE* file = nullptr;
	errno_t err = fopen_s(&file, filename, mode);
	if (err == 2) {
		return nullptr;
	}
	else if (err == 0) {
		cout << "Successfully! Its name is " << filename << endl;
		saveFile = true;
	}
	else {
		cout << "Відкрити файл не вдалося. Код помилки: " << err << endl;
		return nullptr;
	}
	return file;
}

void writeToFile(FILE* file, double data) {
	if (file != nullptr) {
		fwrite(&data, sizeof(double), 1, file);

		char space = ' ';
		fwrite(&space, sizeof(char), 1, file);
	}
	else {
		cout << "file is nullptr" << endl;
	}
}

void GetValuesFromFile(double& from, double& to, double& step, double& e, FILE* file) {
	size_t result;

	result = fread(&from, sizeof(double), 1, file);
	if (result != 1) {
		cout << "Помилка читання 'from' файл не коректний." << endl;
	}
	else {
		cout << "from is " << from << endl;
		fseek(file, sizeof(char), SEEK_CUR);
	}

	result = fread(&to, sizeof(double), 1, file);
	if (result != 1) {
		cout << "Помилка читання 'to' файл не коректний." << endl;
	}
	else {
		cout << "to is " << to << endl;
		fseek(file, sizeof(char), SEEK_CUR);
	}

	result = fread(&step, sizeof(double), 1, file);
	if (result != 1) {
		cout << "Помилка читання 'step' файл не коректний." << endl;
	}
	else {
		cout << "step is " << step << endl;
		fseek(file, sizeof(char), SEEK_CUR);
	}

	result = fread(&e, sizeof(double), 1, file);
	if (result != 1) {
		cout << "Помилка читання 'e' файл не коректний." << endl;
	}
	else {
		cout << "e is " << e << endl;
	}
}

bool isNumber(const char* str) {
	bool isDecimal = false, isSign = false, isDigit = false;

	while ((*str == ' ' && !isSign) || (*str == '+' || *str == '-'))
	{
		if (*str == '+' || *str == '-') {
			isSign = true;
			++str;
			break;
		}
		++str;
	}
	while (*str) {
		if (*str == '.') {
			if (isDecimal) return false;
			isDecimal = true;
		}
		else if (!isdigit(*str)) {
			return false;
		}
		else {
			isDigit = true;
		}
		++str;
	}

	return isDigit || (isDigit && isDecimal);
}

void Exgetline(const char* prompt, char* buffer, int size) {
	while (true) {
		cout << prompt;
		if (cin.peek() == '\n') {
			cin.ignore();
		}
		cin.getline(buffer, size);

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Введено забагато символів. Спробуйте ще раз" << endl;
		}
		else if (buffer[0] == '\0') {
			cout << "Ви ввели порожній рядок. Спробуйте ще раз." << endl;
		}
		else {
			break;
		}
	}
}

void setValue(double& value, const char* prompt, FILE* file) {
	char inputBuffer[12];

	while (true) {
		Exgetline(prompt, inputBuffer, 12);

		if (isNumber(inputBuffer)) {
			value = atof(inputBuffer);
			if (file) {
				writeToFile(file, value);
			}
			break;
		}
		else {
			cout << "Це не коректне число. Спробуйте ще раз: ";
		}
	}
}


void clearCin() {
	cin.clear();
	if (cin.rdbuf()->in_avail() > 0) {
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

void SetValue(double& from, double& to, double& step, double& e, FILE* file = nullptr) {
	setValue(from, "Введіть початкове значення (from): ", file);
	cout << from << endl;
	while (true) {
		setValue(to, "Введіть кінцеве значення (to): ", file);
		if (to > from) {
			cout << to << endl;
			break;
		}
		else {
			cout << "(to) повинно бути більше за " << from << endl;
		}
	}
	while (true) {
		setValue(step, "Введіть крок (step): ", file);
		if ((to - from) / step >= 1) {
			cout << step << endl;
			break;
		}
		else {
			cout << "(step) повинно бути більше за 0 менше за " << (to - from) << endl;
		}
	}
	while (true) {
		setValue(e, "Введіть точність (e): ", file);
		if (e > 0 && e < 1) {
			cout << e << endl;
			break;
		}
		else {
			cout << "(e) повинно бути більше за 0 та менше за 1" << endl;
		}
	}
}


void ShowValues(double& from, double& to, double& step, double& e) {
	cout << left << fixed << setprecision(6);
	cout << "from: " << setw(15) << from << " ";
	cout << "to: " << setw(15) << to << " ";
	cout << "step: " << setw(15) << step << " ";
	cout << "e: " << setw(15) << e;

	cout << endl;
}


int main() {
	system("chcp 1251 && cls");

	char iFileName[13] = {}, oFileName[13] = {};
	FILE* oFile = nullptr;
	int menu;
	double from, to, step, e;
	bool saveFile = false;

	SetValue(from, to, step, e);
	while (true) {
		ShowValues(from, to, step, e);

		cout << left;
		cout << setw(25) << "1)Show the table";
		cout << setw(25) << "2)Set new values";
		cout << setw(25) << "3)Setup the file";
		cout << setw(25) << "4)Get values from file";
		cout << "Other)Exit" << endl;
		cout << "Save in file: " << (saveFile ? "true" : "false") << endl << endl << "Command: ";

		cin >> menu;
		if (menu == 1) {
			ShowTable(from, to, step, e, oFile);
		}
		else if (menu == 2) {
			SetValue(from, to, step, e);
		}
		else if (menu == 3) {
			cout << left;
			cout << setw(25) << "\t1)Set/Change file name";
			cout << setw(25) << "\t2)Close the file";
			cout << "\tOther)Exit" << endl << endl << "Command: ";

			cin >> menu;
			switch (menu)
			{
			case 1:
				clearCin();
				Exgetline("Введіть назву файлу: ", oFileName, 13);
				if (oFile) {
					fclose(oFile);
				}
				oFile = openFile("w", oFileName, saveFile);
				break;
			case 2:
				if (oFile) {
					fclose(oFile);
					oFile = nullptr;
					saveFile = false;
					cout << "Successfully!" << endl;
				}
				else {
					cout << "File is not found" << endl;
					system("pause && cls");
					continue;
				}
				break;
			default:
				cout << "Input is error" << endl;
				clearCin();
				system("pause && cls");
				continue;
				break;
			}
		}
		else if (menu == 4) {
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			Exgetline("Введіть назву файлу: ", iFileName, 13);

			if (oFile) {
				fclose(oFile);
			}
			oFile = openFile("r", iFileName, saveFile);

			if (oFile) {
				char line[256];

				while (fgets(line, sizeof(line), oFile)) {
					cout << line;
				}

				fclose(oFile);
				saveFile = false;
			}
			else {
				cout << "Файл не існує або не може бути відкритий." << endl;
			}
		}
		else {
			exit(0);
		}

		system("pause && cls");
	}
}