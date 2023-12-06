//���������� ������ ��-21, ������-15
//���������� ���������� ������ ���������� 
//������������ ����������� ������ 

#include <iostream>
#include <cmath>
#include <iomanip> 

using namespace std;


// ������� ��� �������� ������ �����
void clearCin() {
	cin.clear(); // ������� ���� ������� ������ �����, ���� �� �
	// ���� � ������ ���������� �������, �������� �� �� ���� �����
	if (cin.rdbuf()->in_avail() > 0) {
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

// ������� ��� ���������� ���������� ����� ������ � ������
void Exgetline(const char* prompt, char* buffer, int size) {
	while (true) {
		cout << prompt; // �������� �������
		if (cin.peek() == '\n') {
			cin.ignore(); // �������� ������ ������ �����, ���� �� ������
		}
		cin.getline(buffer, size); // ������� ����� � �����

		if (cin.fail()) {
			// ���� ������� ����� �������, �� �������� �����
			cin.clear(); // ������� �������
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // �������� ���� ����������� ����
			cout << "������� �������� �������. ��������� �� ���" << endl;
		}
		else if (buffer[0] == '\0') {
			cout << "�� ����� ������� �����. ��������� �� ���." << endl;
		}
		else {
			break; // ����� � �����, ���� ��� ��� �������
		}
	}
}

// ���������� �������� ���� ������� ��� sin^2(x)
double TaylorTask(int n, double x) {
	if (n == 0) {
		return pow(x, 2); // ������� �������: ��������� x^2, ���� n == 0
	}
	// ���������� ������������ �������� ���� ����������
	double previous = TaylorTask(n - 1, x);
	// ��������� �������� ������� ����, �������������� ���������
	return previous * (-4.0 * pow(x, 2)) / ((2.0 * n + 1.0) * (2.0 * n + 2.0));
}

// ���������� sin^2(x) �� ��������� ���� ������� � ������� e
double ApproTask(double x, double e, int& n) {
	double sum_series = 0; // ���� ���� ��� ����������
	double term; // �������� ���� ����
	n = 0; // ˳������� ������� ����� ����

	// ������ ����� ����, ���� ���� ����� �� e
	do {
		term = TaylorTask(n, x); // ���������� ���� ����
		sum_series += term; // ������ ���� �� ����
		n++; // �������� ��������
	} while (fabs(term) > e); // �������� ����� �������

	return sum_series; // ��������� ���� ���� �� ����������
}

// ������� �� ����� �������� � ������� ������� �� �������
void PrintFormatted(double value, int width, int precision, FILE* file = nullptr) {
	char buffer[21]; // ����� ��� ������������� �����

	// ��������� ����� � �������� ������� ���� �� �������
	int num_chars = snprintf(buffer, sizeof(buffer), "%-*.*f", width, precision, value);

	// ���� ����������� ����� ����� ������� ������, ��������� ����
	if (num_chars > width) {
		int start = width - 3;
		buffer[start++] = '.';
		buffer[start++] = '.';
		buffer[start++] = '.';
		buffer[start] = '\0';
	}
	if (file) {
		fprintf(file, "%s", buffer); // ���� ������� ����, ������� � ����
	}
	cout << buffer; // ������� � �������
}

// ³������� ������� � ������������ ���������
void ShowTable(double from, double to, double step, double e, FILE* file) {
	int width = 20; // ������ ������� ��� �������
	int precision = 9; // ʳ������ ����� ���� ���� ��� ��������� �����

	// �������� ��������� ������� �������
	printf("%-*s|%-*s|%-*s|%-*s|%-*s\n", width, "x", width, "Actual sin^2(x)", width, "Approximation", width, "Error", width, "Steps");
	if (file) {
		// ���� ������� ����, �������� ��������� � � ����
		fprintf(file, "%-*s|%-*s|%-*s|%-*s|%-*s\n", width, "x", width, "Actual sin^2(x)", width, "Approximation", width, "Error", width, "Steps");
	}

	// ��������� �� �������� ������� x � ������� ������
	for (double x = from; x <= to; x += step) {
		int steps = 0; // ˳������� ����� ��� ���������� ������� ��������
		double approximation = ApproTask(x, e, steps); // ���������� sin^2(x)
		double actual = pow(sin(x), 2); // ����� �������� sin^2(x)
		double error = fabs(approximation - actual); // ��������� ������� ����������

		// �������� ���������� �������� � ����� �������
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

// ³������� ����� � ������� ������� ('r' ��� �������, 'w' ��� ������) � ��'��
FILE* openFile(const char* mode, const char* filename, bool& saveFile) {
	FILE* file = nullptr;
	errno_t err = fopen_s(&file, filename, mode);
	if (err == 2) {
		// ���� ���� �� ����, ��������� nullptr
		return nullptr;
	}
	else if (err == 0) {
		// ���� ���� ������ ��������, ������������ ��������� saveFile
		cout << "Successfully! Its name is " << filename << endl;
		saveFile = true;
	}
	else {
		// ���� ������� ���� �������, �������� ��� �������
		cout << "³������ ���� �� �������. ��� �������: " << err << endl;
		return nullptr;
	}
	return file; // ��������� �������� �� �������� ����
}

// ������� ��� ������������ �����: ���� ���� ����� �� ���� ��������
void fileSetup(FILE*& oFile, char* oFileName, bool& saveFile) {
	int menu;
	cout << left;
	cout << setw(25) << "\t1)Set/Change file name";
	cout << setw(25) << "\t2)Close the file";
	cout << "\tOther)Exit" << endl << endl << "Command: ";

	cin >> menu;
	switch (menu) {
	case 1:
		clearCin();
		Exgetline("������ ����� �����: ", oFileName, 13);
		if (oFile) {
			fclose(oFile); // ���� ���� ��� ��������, ��������� ����
		}
		oFile = openFile("w", oFileName, saveFile); // ³�������� ����� ���� ��� ������
		break;
	case 2:
		if (oFile) {
			fclose(oFile);
			oFile = nullptr; // ������� �������� �� ����
			saveFile = false; // ������� ��������� ���������� �����
			cout << "Successfully!" << endl;
		}
		else {
			cout << "File is not found" << endl;
		}
		break;
	default:
		clearCin();
		break;
	}
}

// ������� ��� ������� ����������� � �����
void fileRead(FILE*& oFile, char* iFileName, bool& saveFile) {
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	Exgetline("������ ����� �����: ", iFileName, 13);

	if (oFile) {
		fclose(oFile); // ���� ���� ��� ��������, ��������� ���� ����� ����� ���������
	}
	oFile = openFile("r", iFileName, saveFile); // ³�������� ���� ��� �������

	if (oFile) {
		char line[256];

		while (fgets(line, sizeof(line), oFile)) {
			cout << line;
		}

		fclose(oFile); // ��������� ���� ���� ���������� �������
		saveFile = false; // ������� ��������� ���������� �����
	}
	else {
		cout << "���� �� ���� ��� �� ���� ���� ��������." << endl;
	}
}

// ������� ��� ��������� ��� ������ � ����� ��� �� ����� � �����.
void clearSpDot(char* str) {
	char* ans = str;
	while (*str) {
		if (*str == ',') {
			*ans = '.'; // �������� ���� �� �����.
			++ans;
		}
		else if (*str != ' ') {
			*ans = *str; // ������� ������, ���� �� �� �����.
			++ans;
		}
		++str; // ���������� �� ���������� ������� �������� �����.
	}
	*ans = '\0'; // ������ ����-��������� � ���� ������������� �����.
}

// ������� ��� ��������, �� � ����� ������� ������.
bool isNumber(const char* str) {
	bool isDecimal = false; // �� �� ����� ��������� �����
	bool isSign = false;    // �� �������� ���� �� ������� �����
	bool isDigit = false;   // �� �������� ���� � ���� �����

	while (*str) {
		if (isdigit(static_cast<unsigned char>(*str))) {
			isDigit = true;  // ����� ��������
			isSign = true;   // ϳ��� ����������� ����� ���� ��� �� ���� ����
		}
		else if ((*str == '+' || *str == '-') && !isSign && !isDigit) {
			isSign = true;  // ���� ���� ��� ���� ���� ���� ����� �� �������
		}
		else if (*str == '.') {
			if (isDecimal || !isDigit) return false; // ���� ��� � ���������� ��������� ��� ���� ����� ����� ������, ��������� �������
			isDecimal = true;
		}
		else {
			return false;  // ����� ������, �� �����, �� ����, � �� ����� - �������
		}
		str++;
	}

	return isDigit; // ��������� true, ���� �������� ���� � ���� ����� ��� ��������� �����
}

// ������� ��� ��������� ������ �� �������� ���������� ��������� �����
bool getValInput(double& value, const char* prompt, double min = -DBL_MAX, double max = DBL_MAX, bool notEqual = false) {
	char inputBuffer[12];

	while (true) {
		Exgetline(prompt, inputBuffer, 12);
		clearSpDot(inputBuffer);

		if (isNumber(inputBuffer)) {
			double temp = atof(inputBuffer);
			if ((temp > min && temp < max) || (notEqual && temp != min)) {
				value = temp;
				cout << value << endl;
				return true;
			}
			else {
				cout << "������� �������� �� ������� � ���������� ������� ��� ������� ������������ ��������.\n ��������� �����: ";
			}
		}
		else {
			cout << "�� �� �������� �����. ��������� �� ���: ";
		}
	}
}

// ������� ��� ��������� �� ����������� ���������� ������� ��������� ����������
void SetValue(double& from, double& to, double& step, double& e) {
	getValInput(from, "������ ��������� �������� (from): ");
	getValInput(to, "������ ������ �������� (to): ", from, DBL_MAX, true);
	getValInput(step, "������ ���� (step): ", 0, (to - from));
	getValInput(e, "������ ������� (e): ", 0, 1);
}

// ������� ��� ��������� �������� ����������� �� �����
void ShowValues(double& from, double& to, double& step, double& e) {
	cout << left << fixed << setprecision(11); // ������������ ������������ � ���� ������������ � ���������� �������
	cout << "from: " << setw(20) << from << " ";
	cout << "to: " << setw(20) << to << " ";
	cout << "step: " << setw(20) << step << " ";
	cout << "e: " << setw(20) << e;
	cout << endl;
}

int main() {
	system("chcp 1251 && cls"); // ������������ ������ ������� ������ �� Windows-1251 �� �������� ������

	// ����������� ������
	char iFileName[13] = {}, oFileName[13] = {}; // ������ ������� ��� ���� ����� ��������/���������
	FILE* oFile = nullptr; // �������� �� ����, ������������� �� ��������
	int menu; // ����� ��� ��������� ������ ����������� � ����
	double from, to, step, e; // ���� ��� ��������� �������������� �����
	bool saveFile = false; // ��������� ����� ���������� �����

	// ������� ��� ������������ ���������� �������
	SetValue(from, to, step, e); // ������������ �������, ��������� �� �����������
	system("cls");
	while (true) { // ����������� ���� ��� ���������� ����
		ShowValues(from, to, step, e); // ��������� �������� ������� �� �����

		// ��������� ���������� ��� ���������� ����� �� ���� ������
		cout << "Save in file: " << (saveFile ? "true" : "false") << endl << endl << "Command: ";
		cout << left;
		cout << setw(25) << "1)Show the table";
		cout << setw(25) << "2)Set new values";
		cout << setw(25) << "3)Setup the file";
		cout << setw(25) << "4)Get values from file";
		cout << "Other)Exit" << endl;

		cin >> menu; // ��������� ������ �����������
		switch (menu) // ������� ������ �����������
		{
		case 1:
			ShowTable(from, to, step, e, oFile); // ��������� ������� �������
			break;
		case 2:
			SetValue(from, to, step, e); // ������������ ����� �������
			break;
		case 3:
			fileSetup(oFile, oFileName, saveFile); // ������������ ����� ��� ����������
			break;
		case 4:
			fileRead(oFile, iFileName, saveFile); // ������� ������� � �����
			break;
		default:
			exit(0); // ����� � ��������
			break;
		}
		system("pause && cls"); // ����� �� �������� ������
	}
}