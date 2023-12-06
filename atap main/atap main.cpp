//Петриченко Микола БС-21, варіант-15
//Програмний застосунок виконує розрахунки 
//апроксимації рекурсивним шляхом 

#include <iostream>
#include <cmath>
#include <iomanip> 

using namespace std;


// Функція для очищення потоку вводу
void clearCin() {
	cin.clear(); // Скидаємо стан помилки потоку вводу, якщо він є
	// Якщо в потоці залишились символи, ігноруємо їх до кінця рядка
	if (cin.rdbuf()->in_avail() > 0) {
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

// Функція для безпечного зчитування рядка тексту з консолі
void Exgetline(const char* prompt, char* buffer, int size) {
	while (true) {
		cout << prompt; // Виводимо підказку
		if (cin.peek() == '\n') {
			cin.ignore(); // Ігноруємо символ нового рядка, якщо він перший
		}
		cin.getline(buffer, size); // Зчитуємо рядок в буфер

		if (cin.fail()) {
			// Якщо введено більше символів, ніж дозволяє буфер
			cin.clear(); // Скидаємо помилку
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ігноруємо весь надлишковий ввод
			cout << "Введено забагато символів. Спробуйте ще раз" << endl;
		}
		else if (buffer[0] == '\0') {
			cout << "Ви ввели порожній рядок. Спробуйте ще раз." << endl;
		}
		else {
			break; // Вихід з циклу, якщо ввід був успішним
		}
	}
}

// Обчислення елемента ряду Тейлора для sin^2(x)
double TaylorTask(int n, double x) {
	if (n == 0) {
		return pow(x, 2); // Базовий випадок: повертаємо x^2, якщо n == 0
	}
	// Обчислення попереднього елемента ряду рекурсивно
	double previous = TaylorTask(n - 1, x);
	// Повертаємо поточний елемент ряду, використовуючи попередній
	return previous * (-4.0 * pow(x, 2)) / ((2.0 * n + 1.0) * (2.0 * n + 2.0));
}

// Наближення sin^2(x) за допомогою ряду Тейлора з точністю e
double ApproTask(double x, double e, int& n) {
	double sum_series = 0; // Сума ряду для наближення
	double term; // Поточний член ряду
	n = 0; // Лічильник кількості членів ряду

	// Додаємо члени ряду, поки вони більші за e
	do {
		term = TaylorTask(n, x); // Обчислюємо член ряду
		sum_series += term; // Додаємо член до суми
		n++; // Збільшуємо лічильник
	} while (fabs(term) > e); // Перевірка умови точності

	return sum_series; // Повертаємо суму ряду як наближення
}

// Форматує та друкує значення з заданою шириною та точністю
void PrintFormatted(double value, int width, int precision, FILE* file = nullptr) {
	char buffer[21]; // Буфер для форматованого рядка

	// Форматуємо число з вказаною шириною поля та точністю
	int num_chars = snprintf(buffer, sizeof(buffer), "%-*.*f", width, precision, value);

	// Якщо форматоване число довше вказаної ширини, скорочуємо його
	if (num_chars > width) {
		int start = width - 3;
		buffer[start++] = '.';
		buffer[start++] = '.';
		buffer[start++] = '.';
		buffer[start] = '\0';
	}
	if (file) {
		fprintf(file, "%s", buffer); // Якщо відкрито файл, друкуємо у файл
	}
	cout << buffer; // Друкуємо у консоль
}

// Відображає таблицю з результатами обчислень
void ShowTable(double from, double to, double step, double e, FILE* file) {
	int width = 20; // Ширина колонки для таблиці
	int precision = 9; // Кількість знаків після коми для виведення чисел

	// Виводимо заголовки колонок таблиці
	printf("%-*s|%-*s|%-*s|%-*s|%-*s\n", width, "x", width, "Actual sin^2(x)", width, "Approximation", width, "Error", width, "Steps");
	if (file) {
		// Якщо відкрито файл, виводимо заголовки і в файл
		fprintf(file, "%-*s|%-*s|%-*s|%-*s|%-*s\n", width, "x", width, "Actual sin^2(x)", width, "Approximation", width, "Error", width, "Steps");
	}

	// Проходимо по діапазону значень x з заданим кроком
	for (double x = from; x <= to; x += step) {
		int steps = 0; // Лічильник кроків для визначення кількості ітерацій
		double approximation = ApproTask(x, e, steps); // Наближення sin^2(x)
		double actual = pow(sin(x), 2); // Точне значення sin^2(x)
		double error = fabs(approximation - actual); // Абсолютна помилка наближення

		// Виводимо форматовані значення в рядок таблиці
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

// Відкриття файлу з заданим режимом ('r' для читання, 'w' для запису) і ім'ям
FILE* openFile(const char* mode, const char* filename, bool& saveFile) {
	FILE* file = nullptr;
	errno_t err = fopen_s(&file, filename, mode);
	if (err == 2) {
		// Якщо файл не існує, повертаємо nullptr
		return nullptr;
	}
	else if (err == 0) {
		// Якщо файл успішно відкритий, встановлюємо прапорець saveFile
		cout << "Successfully! Its name is " << filename << endl;
		saveFile = true;
	}
	else {
		// Якщо виникла інша помилка, виводимо код помилки
		cout << "Відкрити файл не вдалося. Код помилки: " << err << endl;
		return nullptr;
	}
	return file; // Повертаємо вказівник на відкритий файл
}

// Функція для налаштування файлу: вибір імені файлу та його закриття
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
		Exgetline("Введіть назву файлу: ", oFileName, 13);
		if (oFile) {
			fclose(oFile); // Якщо файл уже відкритий, закриваємо його
		}
		oFile = openFile("w", oFileName, saveFile); // Відкриваємо новий файл для запису
		break;
	case 2:
		if (oFile) {
			fclose(oFile);
			oFile = nullptr; // Скидаємо вказівник на файл
			saveFile = false; // Скидаємо прапорець збереження файлу
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

// Функція для читання налаштувань з файлу
void fileRead(FILE*& oFile, char* iFileName, bool& saveFile) {
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	Exgetline("Введіть назву файлу: ", iFileName, 13);

	if (oFile) {
		fclose(oFile); // Якщо файл вже відкритий, закриваємо його перед новим відкриттям
	}
	oFile = openFile("r", iFileName, saveFile); // Відкриваємо файл для читання

	if (oFile) {
		char line[256];

		while (fgets(line, sizeof(line), oFile)) {
			cout << line;
		}

		fclose(oFile); // Закриваємо файл після завершення читання
		saveFile = false; // Скидаємо прапорець збереження файлу
	}
	else {
		cout << "Файл не існує або не може бути відкритий." << endl;
	}
}

// Функція для видалення всіх пробілів і заміни ком на точки в рядку.
void clearSpDot(char* str) {
	char* ans = str;
	while (*str) {
		if (*str == ',') {
			*ans = '.'; // Заміняємо кому на точку.
			++ans;
		}
		else if (*str != ' ') {
			*ans = *str; // Копіюємо символ, якщо це не пробіл.
			++ans;
		}
		++str; // Переходимо до наступного символу вхідного рядка.
	}
	*ans = '\0'; // Додаємо нуль-термінатор в кінці результуючого рядка.
}

// Функція для перевірки, чи є рядок валідним числом.
bool isNumber(const char* str) {
	bool isDecimal = false; // Чи має рядок десяткову точку
	bool isSign = false;    // Чи знайдено знак на початку рядка
	bool isDigit = false;   // Чи знайдено хоча б одну цифру

	while (*str) {
		if (isdigit(static_cast<unsigned char>(*str))) {
			isDigit = true;  // Цифра знайдена
			isSign = true;   // Після знаходження цифри знак вже не може бути
		}
		else if ((*str == '+' || *str == '-') && !isSign && !isDigit) {
			isSign = true;  // Знак плюс або мінус може бути тільки на початку
		}
		else if (*str == '.') {
			if (isDecimal || !isDigit) return false; // Якщо вже є десятковий роздільник або немає цифри перед точкою, повертаємо помилку
			isDecimal = true;
		}
		else {
			return false;  // Інший символ, не цифра, не знак, і не точка - помилка
		}
		str++;
	}

	return isDigit; // Повертаємо true, якщо знайдено хоча б одну цифру або десяткову точку
}

// Функція для виведення запиту та перевірки коректності введеного числа
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
				cout << "Введене значення не входить в допустимий діапазон або дорівнює попередньому значенню.\n Спробуйте знову: ";
			}
		}
		else {
			cout << "Це не коректне число. Спробуйте ще раз: ";
		}
	}
}

// Функція для отримання від користувача початкових значень параметрів розрахунку
void SetValue(double& from, double& to, double& step, double& e) {
	getValInput(from, "Введіть початкове значення (from): ");
	getValInput(to, "Введіть кінцеве значення (to): ", from, DBL_MAX, true);
	getValInput(step, "Введіть крок (step): ", 0, (to - from));
	getValInput(e, "Введіть точність (e): ", 0, 1);
}

// Функція для виведення поточних налаштувань на екран
void ShowValues(double& from, double& to, double& step, double& e) {
	cout << left << fixed << setprecision(11); // Встановлюємо форматування з лівим вирівнюванням і фіксованою точністю
	cout << "from: " << setw(20) << from << " ";
	cout << "to: " << setw(20) << to << " ";
	cout << "step: " << setw(20) << step << " ";
	cout << "e: " << setw(20) << e;
	cout << endl;
}

int main() {
	system("chcp 1251 && cls"); // Встановлення кодової сторінки консолі на Windows-1251 та очищення консолі

	// Ініціалізація змінних
	char iFileName[13] = {}, oFileName[13] = {}; // Масиви символів для імен файлів введення/виведення
	FILE* oFile = nullptr; // Вказівник на файл, ініціалізований як нульовий
	int menu; // Змінна для зберігання вибору користувача в меню
	double from, to, step, e; // Змінні для зберігання користувацьких даних
	bool saveFile = false; // Прапорець стану збереження файлу

	// Функція для встановлення початкових значень
	SetValue(from, to, step, e); // Встановлення значень, отриманих від користувача
	system("cls");
	while (true) { // Нескінченний цикл для повторення меню
		ShowValues(from, to, step, e); // Виведення поточних значень на екран

		// Виведення інформації про збереження файлу та меню команд
		cout << "Save in file: " << (saveFile ? "true" : "false") << endl << endl << "Command: ";
		cout << left;
		cout << setw(25) << "1)Show the table";
		cout << setw(25) << "2)Set new values";
		cout << setw(25) << "3)Setup the file";
		cout << setw(25) << "4)Get values from file";
		cout << "Other)Exit" << endl;

		cin >> menu; // Отримання вибору користувача
		switch (menu) // Обробка вибору користувача
		{
		case 1:
			ShowTable(from, to, step, e, oFile); // Виведення таблиці значень
			break;
		case 2:
			SetValue(from, to, step, e); // Встановлення нових значень
			break;
		case 3:
			fileSetup(oFile, oFileName, saveFile); // Налаштування файлу для збереження
			break;
		case 4:
			fileRead(oFile, iFileName, saveFile); // Читання значень з файлу
			break;
		default:
			exit(0); // Вихід з програми
			break;
		}
		system("pause && cls"); // Пауза та очищення консолі
	}
}