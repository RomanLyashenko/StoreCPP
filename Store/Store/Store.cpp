// интернет магазин Gazon
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

string NAME = "";
float BALANCE;
string CARDNUMBER = "";
string path = "Data.txt";
string path2 = "Cards.txt";
string path3 = "Balance.txt";


void StartOfTheProgramm(string& path);
string* ReadFile(const string& path, int& size);
bool NameCheck(const string& Name, const string& path);
float BalanceCheck(const string& path3, string& NAME);
void Registration(const string& path);
bool LogIn(const string& path);
void Balance(const string& path, const string& path2); 
void RemeberCard(string path, string& NumberOfTheCard, string& CardPeriod, string& CVC);
void SaveBalance(const string& path3, float BALANCE);
void ToKnowBalance(const string& path3, string& NAME);
string CardCheck(const string& path2, const string& NumberOfTheCard);
void ToKnowCard(const string& path2, string& NAME);

int main() {
    setlocale(LC_ALL, "RU");
 
    // Попытка войти в систему, если не удается - регистрация
    StartOfTheProgramm(path);
    Balance(path, path2);

}

void StartOfTheProgramm(string& path) {
    int choose;
    cout << "\t\tG A Z O N" <<endl;
    do {
        cout << "Уже есть аккаунт?" << endl << "Нажмите 1 чтобы войти" << endl << "Нажмите 0 чтобы продолжить: ";
        cin >> choose;
    } while (choose > 1 || choose < 0);
    if (choose) {
        if (!LogIn(path)) {
            Registration(path);
        }
    }
    else
    {
        Registration(path);
    }
}

string* ReadFile(const string& path, int& size) {
    ifstream in(path);
    if (!in.is_open()) {
        cout << "Путь недействительный!" << endl;
        size = 0; // Устанавливаем размер в 0 при ошибке
        return nullptr;
    }

    // Подсчет строк для динамического выделения памяти
    size = 0;
    string str;
    while (getline(in, str)) {
        size++;
    }
    in.clear(); // Сбрасываем флаги
    in.seekg(0); // Возвращаемся в начало файла

    string* array = new string[size];
    size = 0;
    while (getline(in, str)) {
        array[size++]=str;
    }
    in.close();
    return array;
}

bool NameCheck(const string& Name, const string& path) {
    int size = 0;
    string* array = ReadFile(path, size);
    if (array == nullptr) return false;

    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(" : ");
        if (pos != string::npos) {
            if (array[i].substr(0, pos) == Name) {
                delete[] array;
                return true;
            }
        }
    }
    delete[] array;
    return false;
}

void Registration(const string& path) {
    string Name, PASSWORD;
    cout << "\t\t**Регистрация**" << endl;

    do {
        cout << "Введите ваше имя: ";
        cin >> Name;
        if (Name.size() > 8) {
            cout << "Слишком длинное имя. Введите другое." << endl;
        }
        if (NameCheck(Name, path) == true) {
            cout << "Имя занято. Введите другое." << endl;
        }
    } while (NameCheck(Name, path) == true || Name.size() > 8);
    do {
        cout << "Пароль: ";
        cin >> PASSWORD;
        if (PASSWORD.size() > 8) {
            cout << "Слишком длинный пароль. Введите другой." << endl;
        }
    } while (PASSWORD.size() > 8);

    // Запись в файл
    ofstream out(path, ios::app);
    out << Name << " : " << PASSWORD << endl;
    out.close();
    cout << "Регистрация успешна!" << endl;
    NAME = Name;
}

bool LogIn(const string& path) {
    string Name, PASSWORD;
    cout << "\t\t**Авторизация**" << endl;
    cout << "Введите ваше имя: ";
    cin >> Name;
    cout << "Пароль: ";
    cin >> PASSWORD;

    int size = 0;
    string* array = ReadFile(path, size);
    if (array->size()<1) return false;

    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(" : ");
        if (pos != string::npos) {
            string storedName = array[i].substr(0, pos);
            string storedPassword = array[i].substr(pos + 3);
            if (storedName == Name && storedPassword == PASSWORD) {
                cout << "Авторизация успешна!" << endl;
                NAME = Name;
                delete[] array;
                return true; // Успешный вход
            }
        }
    }

    cout << "Ошибка входа: неверное имя или пароль." << endl;
    delete[] array; // Освобождение памяти
    return false; // Неудача
}

void Balance(const string& path, const string& path2) {
    ToKnowBalance(path3, NAME);
    ToKnowCard(path2, NAME);
    float balance = BALANCE, choose;

    cout << "Ваш баланс: " << balance << endl;
    cout << "Желаете пополнить баланс?" << endl << "Да (нажмите 1)" << endl << "Нет (нажмите 0): ";
    cin >> choose;

    if (choose == 1) {
        cout << "\t\t***Пополнение баланса***" << endl;
        float money;

        string NumberOfTheCard, CardPeriod, CVC;

        int counter = 0;
        do {
            cout << "Введите сумму пополнения(в рублях): ";
            cin >> money;
            counter++;
        } while (money < 0); // запретить в строку money писать символы, отличные от цифр

        do {
            cout << "Введите номер карты: ";
            cin >> NumberOfTheCard;
            if(CardCheck(path2, NumberOfTheCard) == "1"){
                cout << "Эта карта уже привязана к другому аккаунту." << endl;
            }
        } while (NumberOfTheCard.size() != 16 || CardCheck(path2, NumberOfTheCard) == "1");

        do {
            cout << "Введите срок действия карты(слитно 4 цифры): ";
            cin >> CardPeriod;
        } while (CardPeriod.size() != 4);

        do {
            cout << "Введите CVC код: ";
            cin >> CVC;
        } while (CVC.size() != 3);

        cout << "Отправьте " << money << " рублей на карту СберБанк 2202 2067 0057 7831 и после обработки\nтранзакции средства поступят на ваш баланс." << endl;

        int сhoose3 = 0;
        do {
            cout << "Подтверждаете отправку средств? (нажмите 1 для подтверждения)" << endl;
            cout << "Чтобы отменить операцию (нажмите 0): ";
            cin >> сhoose3;
        } while (сhoose3 != 0 && сhoose3 != 1);

        if (сhoose3 == 1) {
            balance += money;
            int dataChoiсe = 0;
            do {
                cout << "Запомнить эту карту?" << endl << "Да (нажмите 1)" << endl << "Нет (нажмите 0): ";
                cin >> dataChoiсe;
            } while (dataChoiсe > 1 || dataChoiсe < 0);

            if (dataChoiсe == 1) {
                RemeberCard(path2, NumberOfTheCard, CardPeriod, CVC);
            }
        }
        BALANCE = balance;
        SaveBalance(path3, BALANCE);
    }
}

void RemeberCard(string path2, string& NumberOfTheCard, string& CardPeriod, string& CVC)
{
    ofstream out(path2, ios::app);
    if (out.is_open()) {
        out << NAME << endl<< NumberOfTheCard << endl << CardPeriod << endl<< CVC << endl <<"-----" << endl;
        out.close();
    }
    else {
        cout << "Ошибка при открытии файла для записи." << endl;
    }
}

string CardCheck(const string& path2, const string& NumberOfTheCard) {
    int size = 0;
    string* array = ReadFile(path2, size);
    if (array == nullptr) return "0"; // Возврат "0", если файл не открыт

    for (int i = 0; i < size; i++) {
        if (array[i] == NumberOfTheCard) { // Проверка на совпадение карты
            if (array[i - 1] == NAME) return "0";
            delete[] array;
            return "1"; // Карта уже привязана к другому аккаунту
        }
    }
    delete[] array; // Освобождение памяти
    return "0"; // Карта не привязана
}

void ToKnowCard(const string& path2, string& NAME) {
    if (NameCheck(NAME, path)) { // Проверяем, существует ли имя в файле
        CARDNUMBER = CardCheck(path2, NAME);
    }
    else {
        CARDNUMBER = ""; 
    }
}

void SaveBalance(const string& path3, float BALANCE) {
    ofstream out(path3, ios::app);
    if (out.is_open()) {
        out << NAME << " : " << BALANCE << endl;
        out.close();
    }
    else {
        cout << "Ошибка при открытии файла для записи." << endl;
    }
}

float BalanceCheck(const string& path3, string& NAME) {
    int size = 0;
    float balance = 0;
    string* array = ReadFile(path3, size);
    if (array == nullptr) return 0; // Возврат 0, если файл не открыт

    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(" : ");
        if (pos != string::npos && array[i].substr(0, pos) == NAME) {
            string balanceStr = array[i].substr(pos + 3); // Получаем строку после " : "
            balance = stof(balanceStr); // Преобразуем строку в float
            delete[] array; // Освобождаем память
            return balance; // Возвращаем результат
        }
    }
    delete[] array; // Освобождение памяти
    return 0; // Возврат 0, если ничего не найдено
}


void ToKnowBalance(const string& path3, string& NAME) {
    if (NameCheck(NAME, path)) { // Проверяем, существует ли имя в файле
        BALANCE = BalanceCheck(path3, NAME); // Записываем текущий баланс
    }
    else {
        BALANCE = 0; // Начальный баланс 0, если имя не найдено
    }
}   


//to do сделать так, чтобы 1 карту можно было бы привязать только на 1 аккаунт
