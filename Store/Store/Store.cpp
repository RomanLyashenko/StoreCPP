#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

using namespace std;

string NAME = "";
float BALANCE=0.0f;
string CARDNUMBER = "";
string path = "Data.txt";
string path2 = "Cards.txt";
string path3 = "Balance.txt";

// Структура для хранения информации о товаре
struct Product {
    string seller; // Имя продавца
    string name; // Название товара
    string producer; // Производитель
    string year; // Год выпуска
    string description; // Описание
    int availableAmount = 0; // Доступное количество
    float price = 0.0f; // Цена
    string category; // Категория товара
};

// Функция для записи данных о товаре в файл
void WriteProductToFile(ofstream& file, const Product& product) {
    file << product.seller << endl; // Записываем имя продавца
    file << product.name << endl;
    file << product.producer << endl;
    file << product.year << endl;
    file << product.description << endl;
    file << product.availableAmount << endl;
    file << product.price << endl;
    file << product.category << endl;
}

bool ReadProductFromFile(ifstream& inFile, Product& product) {
    string line;
    if (getline(inFile, product.seller)) {
        cout << "seller: " << product.seller << endl; // Добавлен cout для отладки
        // ... остальной код ... 
        getline(inFile, line); // Считываем строку с количеством
        cout << "line: " << line << endl; // Добавлен cout для отладки
        // ... остальной код ... 
        getline(inFile, line); // Считываем строку с ценой
        cout << "line: " << line << endl; // Добавлен cout для отладки
        // ... остальной код ...
        getline(inFile, product.category);
        cout << "category: " << product.category << endl; // Добавлен cout для отладки
        return true;
    }
    return false;
}

// Структура для представления товара из файла basket
struct Product2 {
    string seller;
    string name;
    string producer;
    string year;
    string description;
    int availableAmount;
    float price;
    string category;
    int quantity; // Количество товара в корзине
};

// Функция для чтения данных о товаре из файла basket
bool ReadProductFromFile2(ifstream& inFile, Product2& product) {
    string line;
    if (getline(inFile, product.seller)) {
        getline(inFile, product.name);
        getline(inFile, product.producer);
        getline(inFile, product.year);
        getline(inFile, product.description);
        getline(inFile, line); // Считываем строку с количеством
        line.erase(remove_if(line.begin(), line.end(), isspace),
            remove_if(line.begin(), line.end(), isspace));
        if (!line.empty()) {
            try {
                product.availableAmount = stoi(line);
            }
            catch (const exception&) {
                return false; // Пропускаем товар
            }
        }
        else {
            return false; // Пропускаем товар
        }
        getline(inFile, line); // Считываем строку с ценой
        line.erase(remove_if(line.begin(), line.end(), isspace),
            remove_if(line.begin(), line.end(), isspace));
        if (!line.empty()) {
            try {
                product.price = stof(line);
            }
            catch (const exception&) {
                return false; // Пропускаем товар
            }
        }
        else {
            return false; // Пропускаем товар
        }
        getline(inFile, product.category);
        // Считываем количество товара
        getline(inFile, line);
        product.quantity = stoi(line); // Добавляем количество товара в структуру
        return true;
    }
    return false;
}

void StartOfTheProgramm(string& path);
string* ReadFile(const string& path, int& size);
bool NameCheck(const string& Name, const string& path);
float BalanceCheck(const string& path3);
void Registration(const string& path);
bool LogIn(const string& path);
void RemoveBalanceData(const string& path3);
bool Balance(const string& path, const string& path2);
void RemeberCard(string path, string& NumberOfTheCard, string& CardPeriod, string& CVC);
void SaveBalance(const string& path3, float BALANCE);
void ToKnowBalance(const string& path3);
string CardCheck(const string& path2, const string& NumberOfTheCard);
bool ToKnowCard(const string& path2);
string GetSavedCard(const string& path2);
void RenameFile(const string& oldName, const string& newName);
void ForSellers();
void PutAProductUpForSale();
void PersonalAccount();
void DeleteProduct(string& category, string productName);
void Shopping();
void Basket();

int main() {
    SetConsoleTitleA("\t\tG A Z O N");
    // Русский язык
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Product product;

    // Попытка войти в систему, если не удается - регистрация
    StartOfTheProgramm(path);
    system("cls");
    Balance(path, path2);
    system("cls");
    int choose;
    do {
        cout << "Перейти в управление товарами?" << endl << "Да [нажмите 1]" << endl << "Нет [нажмите 0]: ";
        cin >> choose;
        if (choose) ForSellers();
    } while (choose > 1 || choose < 0);
    const int MAX_PRODUCTS = 100;
    system("cls");
    Shopping();
    system("cls");
 
}

void StartOfTheProgramm(string& path) {
    int choose;
    cout << "\t\tG A Z O N" << endl;
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
        array[size++] = str;
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

// Функция для регистрации
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

// Функция для входа в систему
bool LogIn(const string& path) {
    cout << "\t\t*Авторизация*" << endl;
    string Name, Password;
    cout << "Введите имя: ";
    cin >> Name;
    cout << "Введите пароль: ";
    cin >> Password;

    int size = 0;
    string* array = ReadFile(path, size);
    if (array == nullptr) return false;

    // Счетчик попыток
    int attempts = 4;
    while (attempts > 0) {
        for (int i = 0; i < size; i++) {
            size_t pos = array[i].find(" : ");
            if (pos != string::npos) {
                if (array[i].substr(0, pos) == Name && array[i].substr(pos + 3) == Password) {
                    NAME = Name;
                    delete[] array;
                    return true;
                    break;
                }
            }
        }
        cout << "Неверный пароль. Попробуйте снова. Осталось попыток: " << attempts-1 << endl;
        cout << "Введите пароль: ";
        cin >> Password;
        attempts--;
    }
    delete[] array;

    cout << "Слишком много неправильных попыток. Вы будете перенаправлены на регистрацию." << endl;
    Registration(path);
    return false;
}

void RemoveBalanceData(const string& path3) {
    ifstream inFile(path3);
    if (!inFile.is_open()) {
        cout << "Ошибка открытия файла." << endl;
        return;
    }

    ofstream outFile("temp.txt"); // Временный файл
    if (!outFile.is_open()) {
        cout << "Ошибка открытия временного файла." << endl;
        return;
    }

    string line;
    bool foundOldBalance = false;
    while (getline(inFile, line)) {
        if (line.find(NAME + " : ") != string::npos) { // Нашли строку с именем и балансом
            foundOldBalance = true; // Отмечаем, что нашли старую запись 
        }
        else {
            outFile << line << endl; // Копируем строку в временный файл
        }
    }

    inFile.close();
    outFile.close();

    if (foundOldBalance) {
        remove(path3.c_str()); // Удаляем исходный файл
        RenameFile("temp.txt", path3.c_str()); // Переименовываем временный файл
    }
    else {
        remove("temp.txt"); // Удаляем временный файл (если старая запись не найдена)
    }
}


bool Balance(const string& path, const string& path2) {
    ToKnowBalance(path3);
    ToKnowCard(path2);
    float balance = BALANCE;
    ToKnowBalance(path3);
    cout << "Ваш баланс: " << balance << endl;
    int choose = 0;
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
        } while (money < 0);

        int choose2;
        if (CardCheck(path3, NumberOfTheCard) != "1") {
            do {
                cout << "Желаете использовать уже привязанную к аккаунту карту?" << endl << "Да (нажмите 1)" << endl << "Нет (нажмите 0): ";
                cin >> choose2;
            } while (choose2 > 1 || choose2 < 0);
            if (choose2) {
                cout << "Отправьте " << money << " рублей на карту СберБанк 2202 2067 0057 7831 с карты: " << endl << GetSavedCard(path2) << endl << "и после обработки транзакции средства поступят на ваш баланс." << endl;
                int choose3 = 0;
                do {
                    cout << "Подтверждаете отправку средств? (нажмите 1 для подтверждения)" << endl;
                    cout << "Чтобы отменить операцию (нажмите 0): ";
                    cin >> choose3;
                } while (choose3 != 0 && choose3 != 1);
                if (choose3 == 1) {
                    balance += money;
                }
                BALANCE = balance;
                // Удаляем старые данные о балансе из файла
                RemoveBalanceData(path3);
                SaveBalance(path3, BALANCE);
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
        int choose3 = 0;
        do {
            cout << "Подтверждаете отправку средств? (нажмите 1 для подтверждения)" << endl;
            cout << "Чтобы отменить операцию (нажмите 0): ";
            cin >> choose3;
        } while (choose3 != 0 && choose3 != 1);
        if (choose3 == 1) {
            balance += money;
            int dataChoice = 0;
            do {
                cout << "Запомнить эту карту?" << endl << "Да (нажмите 1)" << endl << "Нет (нажмите 0): ";
                cin >> dataChoice;
            } while (dataChoice > 1 || dataChoice < 0);
            if (dataChoice == 1) {
                RemeberCard(path2, NumberOfTheCard, CardPeriod, CVC);
            }
        }
        BALANCE = balance;
        // Удаляем старые данные о балансе из файла
        RemoveBalanceData(path3);
        SaveBalance(path3, BALANCE);
    }
    return true;
}
float BalanceCheck(const string& path3) {
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

void ToKnowBalance(const string& path3) {
    int size = 0;
    string* array = ReadFile(path3, size);
    if (array == nullptr) return;
    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(" : ");
        if (pos != string::npos) {
            if (array[i].substr(0, pos) == NAME) {
                BALANCE = stof(array[i].substr(pos + 3));
                delete[] array;
                return;
            }
        }
    }
    delete[] array;
    return;
}

void SaveBalance(const string& path3, float BALANCE) {
    ofstream out(path3, ios::app);
    if (out.is_open()) {
        out << NAME << " : " << BALANCE << endl;
        out.close();
    }
    else {
        cout << "Ошибка записи в файл" << endl;
    }
}

void RemeberCard(string path, string& NumberOfTheCard, string& CardPeriod, string& CVC) {
    ofstream out(path, ios::app);
    if (out.is_open()) {
        out << NAME << " : " << NumberOfTheCard << " : " << CardPeriod << " : " << CVC << endl;
        out.close();
        cout << "Карта сохранена." << endl;
    }
    else {
        cout << "Ошибка записи в файл" << endl;
    }
}

bool ToKnowCard(const string& path2) {
    int size = 0;
    string* array = ReadFile(path2, size);
    if (array == nullptr) return false;
    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(" : ");
        if (pos != string::npos) {
            if (array[i].substr(0, pos) == NAME) {
                CARDNUMBER = array[i].substr(pos + 3);
                delete[] array;
                return true;
            }
        }
    }
    delete[] array;
}

string CardCheck(const string& path2, const string& NumberOfTheCard) {
    int size = 0;
    string* array = ReadFile(path2, size);
    if (array == nullptr) return "";
    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(" : ");
        if (pos != string::npos) {
            if (array[i].substr(pos + 3) == NumberOfTheCard) {
                delete[] array;
                return array[i].substr(0, pos);
            }
        }
    }
    delete[] array;
    return "";
}
string GetSavedCard(const string& path2) {
    string cardData;
    int size = 0;
    string* array = ReadFile(path2, size); //  Чтение данных из файла
    if (array == nullptr) return cardData; //  Обработка ошибки при чтении файла

    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(NAME); //  Поиск имени пользователя
        if (pos != string::npos) {
            //  Проверка pos и размера строки перед извлечением подстроки
            if (pos + NAME.length() + 1 < array[i].length()) {
                cardData = array[i].substr(pos + NAME.length() + 3);
                delete[] array;
                return cardData;
            }
        }
    }
    delete[] array;
    return cardData;
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

// Функция для продавцов
void ForSellers() {
    string category;
    int action;
    do {
        cout << "Выберите действие:" << endl;
        cout << "1. Добавить товар" << endl;
        cout << "2. Удалить товар" << endl;
        cout << "3. Перейти в личный кабинет" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> action;
        cout << endl;
        switch (action) {
        case 1:
            PutAProductUpForSale();
            break;
        case 2: { // Блок для обработки удаления товара
            string productName;
            cout << "Введите название товара: ";
            cin.ignore();
            getline(cin, productName);

            string productCategory;
            cout << "Введите категорию товара: ";
            getline(cin, productCategory);

            // Считываем данные о товарах продавца
            ifstream sellerFile("Sellers/" + NAME + ".txt");
                if (!sellerFile.is_open()) {
                    cout << "Ошибка открытия файла продавца." << endl;
                    break; // Выходим из блока, если не удалось открыть файл
                }

            bool productFound = false;
            string line;
            Product product;

            // Считываем информацию о товаре до разделителя "-----"
            while (getline(sellerFile, line) && line != "-----") {
                // Считываем данные товара
                if (product.seller.empty()) {
                    product.seller = line;
                }
                else if (product.name.empty()) {
                    product.name = line;
                }
                else if (product.producer.empty()) {
                    product.producer = line;
                }
                else if (product.year.empty()) {
                    product.year = line;
                }
                else if (product.description.empty()) {
                    product.description = line;
                }
                else if (product.availableAmount == 0) {
                    product.availableAmount = stoi(line);
                }
                else if (product.price == 0.0f) {
                    product.price = stof(line);
                }
                else {
                    product.category = line; // Считываем категорию
                }

                // Если товар найден, удаляем его
                if (product.name == productName && product.category == productCategory) {
                    DeleteProduct(productCategory, productName);
                    productFound = true;
                    break;
                }
            }

            // Если "-----" встречен, продолжаем поиск
            if (line == "-----") {
                while (getline(sellerFile, line)) {
                    if (line == "-----") {
                        product = Product(); // Очищаем структуру
                        continue; // Пропускаем разделитель
                    }

                    // Считываем данные товара
                    if (product.seller.empty()) {
                        product.seller = line;
                    }
                    else if (product.name.empty()) {
                        product.name = line;
                    }
                    else if (product.producer.empty()) {
                        product.producer = line;
                    }
                    else if (product.year.empty()) {
                        product.year = line;
                    }
                    else if (product.description.empty()) {
                        product.description = line;
                    }
                    else if (product.availableAmount == 0) {
                        product.availableAmount = stoi(line);
                    }
                    else if (product.price == 0.0f) {
                        product.price = stof(line);
                    }
                    else {
                        product.category = line; // Считываем категорию
                    }

                    // Если товар найден, удаляем его
                    if (product.name == productName && product.category == productCategory) {
                        DeleteProduct(productCategory, productName);
                        productFound = true;
                        break;
                    }
                }
            }

            sellerFile.close();

            if (!productFound) {
                cout << "Товар не найден." << endl;
            }
            break; // Выход из блока удаления товара
        }
        case 3:
            PersonalAccount();
            break;
        case 0:
            cout << "Выход..." << endl;
            break;
        default:
            cout << "Некорректный выбор. Попробуйте снова." << endl;
            break;
        }
    } while (action != 0);
}
// Функция для выставки товара на продажу
void PutAProductUpForSale() {
    Product product;

    // Выбор категории
    int categoryChoice;
    cout << "\t*Выберите категорию вашего товара*" << endl;
    cout << "1. Еда" << endl;
    cout << "2. Одежда и обувь" << endl;
    cout << "3. Техника" << endl;
    cout << "4. Медицина" << endl;
    cout << "5. Предметы личной гигиены" << endl;
    cout << "6. Книги" << endl;
    cout << "7. Аксессуары" << endl;
    cout << "8. Предметы роскоши" << endl;
    cout << "9. Строительство и ремонт" << endl;
    cout << "10. Транспорт и запчасти" << endl;
    cout << "11. Зоотовары" << endl;
    cout << "12. Иное" << endl;
    cout << "Ваш выбор: ";
    do {
        cin >> categoryChoice;
    } while (categoryChoice < 1 || categoryChoice > 12);
    cin.ignore(); // Игнорировать оставшуюся строку

    // Получение информации о товаре
    cout << "Введите название товара: ";
    getline(cin, product.name);
    cout << "Введите производителя: ";
    getline(cin, product.producer);
    cout << "Введите год выпуска/производства товара: ";
    getline(cin, product.year);
    cout << "Описание товара: ";
    getline(cin, product.description);
    cout << "Введите количество товара (штук): ";
    cin >> product.availableAmount;
    do {
        cout << "Введите цену товара: ";
        cin >> product.price;
    } while (product.price < 0);

    // Настройка категории
    switch (categoryChoice) {
    case 1: product.category = "Еда"; break;
    case 2: product.category = "Одежда и обувь"; break;
    case 3: product.category = "Техника"; break;
    case 4: product.category = "Медицина"; break;
    case 5: product.category = "Предметы личной гигиены"; break;
    case 6: product.category = "Книги"; break;
    case 7: product.category = "Аксессуары"; break;
    case 8: product.category = "Предметы роскоши"; break;
    case 9: product.category = "Строительство и ремонт"; break;
    case 10: product.category = "Транспорт и запчасти"; break;
    case 11: product.category = "Зоотовары"; break;
    case 12: product.category = "Иное"; break;
    }

    // Запись данных в файл
    product.seller = NAME; // Заполняем продавца

    ofstream outSellers("Sellers/" + NAME + ".txt", ios::app); 
    if (outSellers.is_open()) {
        WriteProductToFile(outSellers, product);
        outSellers << "-----" << endl;
        outSellers.close();
    }
    else {
        cout << "Ошибка при открытии файла для записи!" << endl;
    }

    // Запись в категорию
    ofstream outCategories("Categories/" + product.category + ".txt", ios::app); 
    if (outCategories.is_open()) {
        WriteProductToFile(outCategories, product);
        outCategories << "-----" << endl;
        outCategories.close();
        cout << "Товар успешно сохранён." << endl;
    }
    else {
        cout << "Ошибка при открытии файла для записи!" << endl;
    }
}

// Функция для личного кабинета продавца
void PersonalAccount() {
    string filename = "Sellers/" + NAME + ".txt"; //  NAME должна быть определена ранее
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла продавца." << endl;
        return;
    }

    bool hasProducts = false; // Флаг, чтобы отслеживать, есть ли товары
    string seller;
    string name;
    string producer;
    string year;
    string description;
    int availableAmount = 0;
    float price = 0.0f;
    string category;

    string line;
    while (getline(file, line)) {
        if (line == "-----") {
            if (!name.empty()) { // Проверяем, что товар не пустой
                hasProducts = true; // Устанавливаем флаг, что есть товары
                cout << "-------------------------" << endl;
                cout << "Название: " << name << endl;
                cout << "Производитель: " << producer << endl;
                cout << "Год выпуска: " << year << endl;
                cout << "Описание: " << description << endl;
                cout << "Количество: " << availableAmount << endl;
                cout << "Цена: " << price << endl;
                cout << "Категория: " << category << endl;
            }

            // Очищаем переменные для следующего товара
            seller.clear();
            name.clear();
            producer.clear();
            year.clear();
            description.clear();
            availableAmount = 0;
            price = 0.0f;
            category.clear();

            continue; // Пропускаем разделитель
        }

        if (seller.empty()) { // Считываем продавца только в начале
            seller = line;
        }
        else if (name.empty()) {
            name = line;
        }
        else if (producer.empty()) {
            producer = line;
        }
        else if (year.empty()) {
            year = line;
        }
        else if (description.empty()) {
            description = line;
        }
        else if (availableAmount == 0) { // Считываем количество
            availableAmount = stoi(line);
        }
        else if (price == 0.0f) { // Считываем цену
            price = stof(line);
        }
        else { // Считываем категорию
            category = line;
        }
    }

    if (!name.empty()) { // Проверяем последний товар
        hasProducts = true; // Устанавливаем флаг, что есть товары
        cout << "-------------------------" << endl;
        cout << "Название: " << name << endl;
        cout << "Производитель: " << producer << endl;
        cout << "Год выпуска: " << year << endl;
        cout << "Описание: " << description << endl;
        cout << "Количество: " << availableAmount << endl;
        cout << "Цена: " << price << endl;
        cout << "Категория: " << category << endl;
    }

    file.close();
    if (!hasProducts) {
        cout << "У вас нет товаров." << endl;
    }
}

// Функция для удаления товара
void DeleteProduct(string& category, string productName) {
    // Удаление товара из файла категории
    string categoryFileName = "Categories/" + category + ".txt";
    string tempFile = "temp.txt";
    ifstream inCategories;
    inCategories.open(categoryFileName);
    if (!inCategories.is_open()) {
        cout << "Ошибка открытия файла категории." << endl;
        return;
    }
    ofstream outTemp(tempFile);
    if (!outTemp.is_open()) {
        cout << "Ошибка при создании временного файла." << endl;
        return;
    }

    bool productFound = false;
    string line;
    while (getline(inCategories, line)) {
        if (line == "-----") { // Проверяем разделитель
            outTemp << line << endl;
            continue;
        }
        if (line == productName) { // Проверяем название товара
            productFound = true;
        }
        else {
            outTemp << line << endl; // Записываем строку в временный файл
        }
    }
    inCategories.close();
    outTemp.close();

    if (productFound) {
        remove(categoryFileName.c_str());
        RenameFile(tempFile.c_str(), categoryFileName.c_str());
        cout << "Товар удален из категории." << endl;
    }
    else {
        cout << "Товар не найден в категории." << endl;
    }
    remove(tempFile.c_str());

    // Удаление товара из файла продавца
    string sellerFileName = "Sellers/" + NAME + ".txt";
    tempFile = "temp.txt";
    ifstream inSeller(sellerFileName);
    if (!inSeller.is_open()) {
        cout << "Ошибка открытия файла продавца." << endl;
        return;
    }
    outTemp.open(tempFile);
    if (!outTemp.is_open()) {
        cout << "Ошибка при создании временного файла." << endl;
        return;
    }

    productFound = false;
    while (getline(inSeller, line)) {
        if (line == "-----") { // Проверяем разделитель
            outTemp << line << endl;
            continue;
        }
        if (line == productName) { // Проверяем название товара
            productFound = true;
        }
        else {
            outTemp << line << endl; // Записываем строку в временный файл
        }
    }
    inSeller.close();
    outTemp.close();

    if (productFound) {
        remove(sellerFileName.c_str());
        RenameFile(tempFile.c_str(), sellerFileName.c_str());
        cout << "Товар удален из файла продавца." << endl;
    }
    else {
        cout << "Товар не найден в файле продавца." << endl;
    }
    remove(tempFile.c_str());
}

// Функция для обновления количества товара в файле
void UpdateProductAmount(const string& filename, const Product& product, int amount) {
    ifstream inFile(filename);
    ofstream outFile(filename + ".tmp");
    Product currentProduct;

    if (inFile.is_open() && outFile.is_open()) {
        while (ReadProductFromFile(inFile, currentProduct)) {
            if (currentProduct.name == product.name &&
                currentProduct.producer == product.producer &&
                currentProduct.year == product.year &&
                currentProduct.category == product.category) {
                currentProduct.availableAmount -= amount;
            }
            WriteProductToFile(outFile, currentProduct);
            outFile << "-----" << endl;
        }
        inFile.close();
        outFile.close();
        remove(filename.c_str());
        RenameFile((filename + ".tmp").c_str(), filename.c_str());
    }
    else {
            cout << "Ошибка при открытии файла " << filename << endl;
    }
}


// Функция для покупки товаров
void Shopping() {
    // Выбор категории
    int categoryChoice;
    cout << "\t*Выберите категорию товара*" << endl;
    cout << "1. Еда" << endl;
    cout << "2. Одежда и обувь" << endl;
    cout << "3. Техника" << endl;
    cout << "4. Медицина" << endl;
    cout << "5. Предметы личной гигиены" << endl;
    cout << "6. Книги" << endl;
    cout << "7. Аксессуары" << endl;
    cout << "8. Предметы роскоши" << endl;
    cout << "9. Строительство и ремонт" << endl;
    cout << "10. Транспорт и запчасти" << endl;
    cout << "11. Зоотовары" << endl;
    cout << "12. Иное" << endl;
    cout << "Ваш выбор: ";
    do {
        cin >> categoryChoice;
    } while (categoryChoice < 1 || categoryChoice > 12);
    cin.ignore();

    // Создание имени файла с использованием текстового названия категории
    string categoryFilename;
        switch (categoryChoice) {
        case 1: categoryFilename = "Categories/Еда.txt"; break;
        case 2: categoryFilename = "Categories/Одежда и обувь.txt"; break;
        case 3: categoryFilename = "Categories/Техника.txt"; break;
        case 4: categoryFilename = "Categories/Медицина.txt"; break;
        case 5: categoryFilename = "Categories/Предметы личной гигиены.txt"; break;
        case 6: categoryFilename = "Categories/Книги.txt"; break;
        case 7: categoryFilename = "Categories/Аксессуары.txt"; break;
        case 8: categoryFilename = "Categories/Предметы роскоши.txt"; break;
        case 9: categoryFilename = "Categories/Строительство и ремонт.txt"; break;
        case 10: categoryFilename = "Categories/Транспорт и запчасти.txt"; break;
        case 11: categoryFilename = "Categories/Зоотовары.txt"; break;
        case 12: categoryFilename = "Categories/Иное.txt"; break;
        }

    // Считываем данные о товарах из файла категории
    ifstream categoryFile(categoryFilename);

    if (categoryFile.is_open()) {
        Product product;
        while (ReadProductFromFile(categoryFile, product)) {
            cout << "----------------------------------------" << endl;
            cout << "Название: " << product.name << endl;
            cout << "Производитель: " << product.producer << endl;
            cout << "Продавец: " << product.seller << endl;
            cout << "Год выпуска: " << product.year << endl;
            cout << "Описание: " << product.description << endl;
            cout << "Доступное количество: " << product.availableAmount << endl;
            cout << "Цена: " << product.price << endl;
            cout << "----------------------------------------" << endl;

            int quantity;
            cout << "Введите количество товаров, которые хотите добавить в корзину (0 - пропустить): ";
            cin >> quantity;
            cin.ignore();

            if (quantity > 0 && quantity <= product.availableAmount) {
                // Добавляем товар в корзину
                ofstream basketFile("Baskets/" + NAME + ".txt", ios::app);
                if (basketFile.is_open()) {
                    WriteProductToFile(basketFile, product);
                    basketFile << quantity << endl; // Записываем количество товара
                    basketFile << "-----" << endl; // Записываем разделитель
                    basketFile.close();
                }
                else {
                    cout << "Ошибка при открытии файла корзины!" << endl;
                }

                // Обновляем количество товара в файлах категории и продавца
                UpdateProductAmount(categoryFilename, product, quantity);
                UpdateProductAmount("Sellers/" + product.seller + ".txt", product, quantity);

                cout << "Товар добавлен в корзину!" << endl;
            }
            else if (quantity < 0) {
                
            }
        }
        categoryFile.close();
        cout << "Товары в категории " << categoryChoice << " закончились." << endl;
    }
    else {
        
    }

    // Предлагаем пользователю выбрать другую категорию
    char continueShopping;
    cout << "Хотите посмотреть товары другой категории? (y/n): ";
    cin >> continueShopping;
    if (continueShopping == 'y') {
        Shopping();
    }
    else {
        Basket();
    }
}

// Функция для работы с корзиной
void Basket() {
    string basketFilename = "Baskets/" + NAME + ".txt"; // Замените NAME на фактическое имя переменной
    ifstream basketFile(basketFilename);

    if (basketFile.is_open()) {
        float totalCost = 0.0f; // Суммарная стоимость покупки

        Product2 product;
        string line;
        while (getline(basketFile, line)) {
            if (line != "-----") {
                // Чтение данных о товаре из файла
                    if (ReadProductFromFile2(basketFile, product)) {
                        // Вывод информации о товаре
                        cout << "----------------------------------------" << endl;
                        cout << "Продавец: " << product.seller << endl;
                        cout << "Название: " << product.name << endl;
                        cout << "Производитель: " << product.producer << endl;
                        cout << "Год выпуска: " << product.year << endl;
                        cout << "Описание: " << product.description << endl;
                        cout << "Цена: " << product.price << endl;
                        cout << "Количество: " << product.quantity << endl; // Выводим количество товара из структуры
                        cout << "----------------------------------------" << endl;

                        // Подсчет стоимости товара
                        float cost = product.price * product.quantity; // Используем количество из структуры
                        totalCost += cost;

                        // Предложение оплатить товар
                        char payChoice;
                        cout << "Оплатить этот товар? (y/n): ";
                        cin >> payChoice;
                        cin.ignore();

                        if (payChoice == 'y') {
                            // Оплата товара
                            string city, street, house, entrance, apartment;
                            cout << "Введите город: ";
                            getline(cin, city);
                            cout << "Введите улицу: ";
                            getline(cin, street);
                            cout << "Введите номер дома: ";
                            getline(cin, house);
                            cout << "Введите номер подъезда: ";
                            getline(cin, entrance);
                            cout << "Введите номер квартиры: ";
                            getline(cin, apartment);

                            // Проверка баланса
                            if (BALANCE >= totalCost) { // Замените BALANCE на фактическое имя переменной с балансом
                                // Списание средств с баланса
                                BALANCE -= totalCost;
                                cout << "Оплата прошла успешно!" << endl;
                                cout << "Ваш адрес: " << city << ", " << street << ", " << house << ", " << entrance << ", " << apartment << endl;
                            }
                            else {
                                // Недостаточно средств
                                char choice;
                                cout << "Недостаточно средств на балансе! Пополнить баланс (y/n) или отменить покупку? (y/n): ";
                                cin >> choice;
                                cin.ignore();
                                if (choice == 'y') {
                                    // Вызов функции пополнения баланса
                                    // Balance(username);
                                    // Повтор оплаты
                                    Basket();
                                }
                                else {
                                    cout << "Покупка отменена!" << endl;
                                    return; // Выход из функции Basket
                                }
                            }
                        }

                        // Переход к следующему товару
                        cout << endl; // Добавить пустую строку для разделения
                    }
            }
            else {
                // Следующий товар
                cout << "-----" << endl;
            }
        }
        basketFile.close();
    }
    // Shopping(); // Возможно, вам нужно добавить сюда вызов функции Shopping()
}
