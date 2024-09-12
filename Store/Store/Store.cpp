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
bool Balance(const string& path, const string& path2); 
void RemeberCard(string path, string& NumberOfTheCard, string& CardPeriod, string& CVC);
void SaveBalance(const string& path3, const string& NAME, float BALANCE);
void ToKnowBalance(const string& path3, string& NAME);
string CardCheck(const string& path2, const string& NumberOfTheCard);
void ToKnowCard(const string& path2, string& NAME);
string GetSavedCard(const string& path2, const string& NAME);
void RenameFile(const string& oldName, const string& newName);
void PutAProductUpForSale();

int main() {
    setlocale(LC_ALL, "RU");
 
    // Попытка войти в систему, если не удается - регистрация
    StartOfTheProgramm(path);
    Balance(path, path2);
    int choose;
    do {
        cout << "Желаете выставить товар на продажу?" << endl << "Дa (нажмите 1)" << endl << "Нет (нажмите 0): ";
        cin >> choose;
    } while (choose > 1 || choose < 0);
    if (choose) {
        PutAProductUpForSale();
    }
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

bool Balance(const string& path, const string& path2) {
    ToKnowBalance(path3, NAME);
    ToKnowCard(path2, NAME);
    float balance = BALANCE, choose;

    cout << "Ваш баланс: " << balance << endl;
    do {
        cout << "Желаете пополнить баланс?" << endl << "Да (нажмите 1)" << endl << "Нет (нажмите 0): ";
        cin >> choose;
    } while (choose > 1 || choose < 0);

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

        int choose2;
        if (GetSavedCard(path2, NAME) != "") {
            do {
                cout << "Желаете использовать уже привязанную к аккаунту карту?" << endl << "Да (нажмите 1)" << endl << "Нет (нажмите 0): ";
                cin >> choose2;
            } while (choose2 > 1 || choose2 < 0);
            if (choose2) {
                cout << "Отправьте " << money << " рублей на карту СберБанк 2202 2067 0057 7831 с карты: " << endl << GetSavedCard(path2, NAME) << endl << "и после обработки транзакции средства поступят на ваш баланс." << endl;

                int сhoose3 = 0;
                do {
                    cout << "Подтверждаете отправку средств? (нажмите 1 для подтверждения)" << endl;
                    cout << "Чтобы отменить операцию (нажмите 0): ";
                    cin >> сhoose3;
                } while (сhoose3 != 0 && сhoose3 != 1);
                if (сhoose3 == 1) {
                    balance += money;
                }
                BALANCE = balance;
                SaveBalance(path3, NAME, BALANCE);
                return true;
            }
        }

            do {
                cout << "Введите номер карты: ";
                cin >> NumberOfTheCard;
                if (CardCheck(path2, NumberOfTheCard) == "1") {
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
            SaveBalance(path3, NAME, BALANCE);
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

string GetSavedCard(const string& path2, const string& NAME) {
    ifstream file(path2);
    if (!file.is_open()) {
        cout << "Ошибка при открытии файла с картами." << endl;
        return ""; // Возврат пустой строки, если файл не открыт
    }

    string line;
    string cardData;
    while (getline(file, line)) {
        if (line == NAME) {
            for (int i = 0; i < 3; ++i) { // Читаем три строки с данными карты
                getline(file, line);
                cardData += line + (i < 2 ? "\n" : ""); // Добавить новую строку только для первых двух
            }
            break; // Выходим из цикла, если данные карты найдены
        }
        // Пропускаем разделитель
        if (line == "-----") {
            continue;
        }
    }

    file.close();
    return cardData; // Возврат данных карты или пустой строка
}

void SaveBalance(const string& path3, const string& NAME, float BALANCE) {
    string tempPath = "temp.txt"; // Путь к временным файлу
    ifstream in(path3);
    ofstream out(tempPath); // Открываем временный файл для записи

    if (!in.is_open() || !out.is_open()) {
        cout << "Ошибка при открытии файла." << endl;
        return;
    }

    string line;
    bool balanceUpdated = false; // Флаг для отслеживания, обновили ли баланс

    // Читаем файл построчно
    while (getline(in, line)) {
        size_t pos = line.find(" : ");
        if (pos != string::npos && line.substr(0, pos) == NAME) {
            // Обновляем строку с балансом
            out << NAME << " : " << BALANCE << endl;
            balanceUpdated = true; // Устанавливаем флаг
        }
        else {
            // Пишем строку без изменений
            out << line << endl;
        }
    }

    // Если пользователь не найден, добавляем новую строку
    if (!balanceUpdated) {
        out << NAME << " : " << BALANCE << endl;
    }

    in.close();
    out.close();

    // Удаляем старый файл и переименовываем временный
    remove(path3.c_str());
    //rename(tempPath.c_str(), path3.c_str());
    RenameFile(tempPath, path3);
    remove(tempPath.c_str());
}

void RenameFile(const string& oldName, const string& newName) {
    ifstream in(oldName);
    ofstream out(newName);

    string line;
    while (getline(in, line)) {
        out << line << endl;
    }

    in.close();
    out.close();

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

void PutAProductUpForSale() { 
    int choose;
    cout << "\t*Выберите категорию вашего товара*" << endl
        << "1. Еда" << endl
        << "2. Одежда и обувь" << endl
        << "3. Техника" << endl
        << "4. Медицина" << endl
        << "5. Предметы личной гигиены" << endl
        << "6. Книги" << endl
        << "7. Аксессуары" << endl
        << "8. Предметы роскоши" << endl
        << "9. Строительство и ремонт" << endl
        << "10. Транспорт и запчасти" << endl
        << "11. Зоотовары" << endl
        << "12. Иное" << endl
        << "Ваш выбор: ";

    do {
        cin >> choose;
    } while (choose < 1 || choose > 12);

    cin.ignore(); // Игнорировать оставшуюся строку

    string Сategory = "";
    string NameOfProduct = "";
    string Producer = "";
    string Time = "";
    string Description = "";
    float Price;
    int Amount;

    switch (choose) {
    case 1:
        Сategory = "Еда";
        break;
    case 2:
        Сategory = "Одежда и обувь";
        break;
    case 3:
        Сategory = "Техника";
        break;
    case 4:
        Сategory = "Медицина";
        break;
    case 5:
        Сategory = "Предметы личной гигиены";
        break;
    case 6:
        Сategory = "Книги";
        break;
    case 7:
        Сategory = "Аксессуары";
        break;
    case 8:
        Сategory = "Предметы роскоши";
        break;
    case 9:
        Сategory = "Строительство и ремонт";
        break;
    case 10:
        Сategory = "Транспорт и запчасти";
        break;
    case 11:
        Сategory = "Зоотовары";
        break;
    case 12:
        Сategory = "Иное";
        break;
    default:
        cout << "Ошибка выбора категории." << endl;
        return; // Возврат, если выбранный вариант некорректен
    }

    cout << "Введите название товара: ";
    getline(cin, NameOfProduct);
    cout << "Введите производителя: ";
    getline(cin, Producer);
    cout << "Введите время выпуска/производства товара (год): ";
    getline(cin, Time);
    cout << "Описание товара: ";
    getline(cin, Description);
    cout << "Введите количество товара (штук): ";
    cin >> Amount;
    
    do {
        cout << "Введите цену товара: ";
        cin >> Price;
    } while (Price < 0);

    // Запись данных в файл
    ofstream outSellers("Sellers/" + NAME + ".txt", ios::app); // Используем одинарные слэши
    if (outSellers.is_open()) {


        outSellers << Сategory << endl << NameOfProduct << endl << Producer << endl
            << Time << endl << Description << endl << Amount << endl << Price << endl << "-----" << endl;
        outSellers.close();
    }
    else {
        cout << "Ошибка при открытии файла для записи!" << endl;
    }

    // Запись в категорию
    ofstream outCategories("Сategories/" + Сategory + ".txt", ios::app);
    if (outCategories.is_open()) {
        outCategories << NAME << endl << NameOfProduct << endl << Producer << endl
            << Time << endl << Description << endl << Amount << endl << Price << endl << "-----" << endl;
        outCategories.close();
    }
    else {
        cout << "Ошибка при открытии файла для записи!" << endl;
    }
}