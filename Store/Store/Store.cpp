#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector> // Добавляем vector для хранения товаров

using namespace std;

string NAME = "";
float BALANCE;
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
    int availableAmount; // Доступное количество
    float price; // Цена
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
}

// Функция для чтения данных о товаре из файла
Product ReadProductFromFile(ifstream& file) {
    Product product;
    getline(file, product.seller);
    getline(file, product.name);
    getline(file, product.producer);
    getline(file, product.year);
    getline(file, product.description);
    getline(file, product.availableAmount);
    product.availableAmount = stoi(product.availableAmount);
    getline(file, product.price);
    product.price = stof(product.price);
    return product;
}

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
void ForSellers();
void PutAProductUpForSale();
void PersonalAccount();
void DeleteProduct();
void Shopping();
void viewBasket();

int main() {
    // Русский язык
 setlocale(LC_ALL, "RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

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
    //viewBasket();
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

void Registration(const string& path) {
    string Name, Password;
    cout << "Введите имя: ";
    cin >> Name;
    cout << "Введите пароль: ";
    cin >> Password;
 ofstream out(path, ios::app);
    if (out.is_open()) {
        out << Name << " : " << Password << endl;
        out.close();
        cout << "Регистрация завершена" << endl;
    }
    else {
        cout << "Ошибка записи в файл" << endl;
    }
}

bool LogIn(const string& path) {
    string Name, Password;
    cout << "Введите имя: ";
    cin >> Name;
    cout << "Введите пароль: ";
    cin >> Password;
    int size = 0;
    string* array = ReadFile(path, size);
    if (array == nullptr) return false;
    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(" : ");
        if (pos != string::npos) {
            if (array[i].substr(0, pos) == Name && array[i].substr(pos + 3) == Password) {
                NAME = Name;
                delete[] array;
                return true;
            }
        }
    }
    delete[] array;
    return false;
}

bool Balance(const string& path, const string& path2) {
    int choose;
    cout << "\t\tG A Z O N" << endl;
    do {
        cout << "Пополнить баланс?" << endl << "Нажмите 1 чтобы пополнить баланс" << endl << "Нажмите 0 чтобы продолжить: ";
        cin >> choose;
    } while (choose > 1 || choose < 0);
    if (choose) {
        if (!ToKnowCard(path2, NAME)) return false;
        ToKnowBalance(path3, NAME);
        float sum;
        cout << "Введите сумму: ";
        cin >> sum;
        BALANCE += sum;
        SaveBalance(path3, NAME, BALANCE);
        return true;
}
    return true;
}

void ToKnowBalance(const string& path3, string& NAME) {
    int size = 0;
    string* array = ReadFile(path3, size);
    if (array == nullptr) return;
    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(" : ");
        if (pos != string::npos) {
            if (array[i].substr(0, pos) == NAME) {
                BALANCE = stof(array[i].substr(pos + 3));
                cout << "Баланс: " << BALANCE << endl;
                delete[] array;
                return;
            }
        }
    }
    delete[] array;
    return;
}

void SaveBalance(const string& path3, const string& NAME, float BALANCE) {
    ofstream out(path3);
    if (out.is_open()) {
        out << NAME << " : " << BALANCE << endl;
        out.close();
    }
    else {
        cout << "Ошибка записи в файл" << endl;
    }
}

void RemeberCard(string path, string& NumberOfTheCard, string& CardPeriod, string& CVC) {
    cout << "Введите номер карты: ";
    cin >> NumberOfTheCard;
    cout << "Введите срок действия: ";
    cin >> CardPeriod;
    cout << "Введите CVC: ";
    cin >> CVC;
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

void ToKnowCard(const string& path2, string& NAME) {
    int size = 0;
    string* array = ReadFile(path2, size);
    if (array == nullptr) return;
    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(" : ");
        if (pos != string::npos) {
            if (array[i].substr(0, pos) == NAME) {
                CARDNUMBER = array[i].substr(pos + 3);
                cout << "Номер карты: " << CARDNUMBER << endl;
                delete[] array;
                return;
            }
        }
    }
    delete[] array;
    return;
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

string GetSavedCard(const string& path2, const string& NAME) {
    int size = 0;
    string* array = ReadFile(path2, size);
    if (array == nullptr) return "";
    for (int i = 0; i < size; i++) {
        size_t pos = array[i].find(" : ");
        if (pos != string::npos) {
            if (array[i].substr(0, pos) == NAME) {
                delete[] array;
                return array[i].substr(pos + 3);
            }
        }
    }
    delete[] array;
    return "";
}

void RenameFile(const string& oldName, const string& newName) {
    if (MoveFileEx(oldName.c_str(), newName.c_str(), MOVEFILE_REPLACE_EXISTING)) {
        cout << "Файл переименован" << endl;
    }
    else {
        cout << "Ошибка переименования файла" << endl;
    }
}

// Функция для управления товарами продавца
void ForSellers() {
    int choose;
    do {
        cout << "Выберите действие:" << endl
            << "1. Выставить товар на продажу" << endl
            << "2. Личный кабинет" << endl
            << "3. Удалить товар" << endl
            << "4. Вернуться назад" << endl
            << "Ваш выбор: ";
        cin >> choose;
    } while (choose < 1 || choose > 4);

    switch (choose) {
    case 1:
        PutAProductUpForSale();
break;
    case 2:
        PersonalAccount();
        break;
    case 3:
        DeleteProduct();
        break;
    case 4:
        break;
    }
}

// Функция для выставки товара на продажу
void PutAProductUpForSale() {
    Product product;
    cout << "Введите информацию о товаре:" << endl;
    cout << "Имя продавца: ";
    cin >> product.seller;
    cout << "Название товара: ";
    cin >> product.name;
    cout << "Производитель: ";
    cin >> product.producer;
    cout << "Год выпуска: ";
    cin >> product.year;
    cout << "Описание: ";
    cin.ignore(); // Считываем остаток символа новой строки из буфера
    getline(cin, product.description);
    cout << "Доступное количество: ";
    cin >> product.availableAmount;
    cout << "Цена: ";
    cin >> product.price;

    // Записываем товар в файл категории
    string categoryFileName = "Categories/" + product.name + ".txt";
    ofstream outCategories(categoryFileName, ios::app);
    if (outCategories.is_open()) {
        WriteProductToFile(outCategories, product);
        outCategories.close();
        cout << "Товар добавлен в категорию." << endl;
    } else {
        cout << "Ошибка открытия файла категории." << endl;
    }

    // Записываем товар в файл продавца
    string sellerFileName = "Sellers/" + product.seller + ".txt";
    ofstream outSellers(sellerFileName, ios::app);
    if (outSellers.is_open()) {
        WriteProductToFile(outSellers, product); // Используем функцию WriteProductToFile
        outSellers.close();
        cout << "Товар добавлен в файл продавца." << endl;
    } else {
        cout << "Ошибка открытия файла продавца." << endl;
    }
}

// Функция для личного кабинета продавца
void PersonalAccount() {
    // Реализуйте функциональность личного кабинета
    cout << "Личный кабинет продавца" << endl;
}

// Функция для удаления товара
void DeleteProduct() {
    string productName;
    string sellerName;

    cout << "Введите имя продавца: ";
    cin >> sellerName;
    cout << "Введите название товара: ";
    cin >> productName;

    // Удаление товара из файла категории
    string categoryFileName = "Categories/" + productName + ".txt";
    string tempFile = "temp.txt";

    ifstream inCategories(categoryFileName);
    if (!inCategories.is_open()) {
        cout << "Ошибка открытия файла категории." << endl;
        return;
    }

    ofstream outTemp(tempFile);
    if (!outTemp.is_open()) {
        cout << "Ошибка при создании временного файла." << endl;
        return;
    }

    Product product;
    bool productFound = false;
    while (getline(inCategories, line)) {
        if (line.empty() || line == "-----") {
            outTemp << line << endl;
            continue;
        }
        product = ReadProductFromFile(inCategories);
        if (product.name == productName && product.seller == sellerName) {
            productFound = true;
        } else {
            WriteProductToFile(outTemp, product);
        }
    }

    inCategories.close();
    outTemp.close();

    if (productFound) {
        remove(categoryFileName.c_str());
        rename(tempFile.c_str(), categoryFileName.c_str());
        cout << "Товар удален из категории." << endl;
    } else {
        cout << "Товар не найден в категории." << endl;
    }
    remove(tempFile.c_str());

    // Удаление товара из файла продавца
    string sellerFileName = "Sellers/" + sellerName + ".txt";
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
if (line.empty() || line == "-----") {
            outTemp << line << endl;
            continue;
        }
        product = ReadProductFromFile(inSeller);
        if (product.name == productName && product.seller == sellerName) {
            productFound = true;
        } else {
            WriteProductToFile(outTemp, product);
        }
    }

    inSeller.close();
    outTemp.close();

    if (productFound) {
        remove(sellerFileName.c_str());
        rename(tempFile.c_str(), sellerFileName.c_str());
        cout << "Товар удален из файла продавца." << endl;
    } else {
        cout << "Товар не найден в файле продавца." << endl;
    }
    remove(tempFile.c_str());
}

// Функция для покупок
void Shopping() {
    int choose;
    cout << "\t*Выберите категорию*" << endl
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

    string category;
    switch (choose) {
        case 1: category = "Еда"; break;
        case 2: category = "Одежда и обувь"; break;
        case 3: category = "Техника"; break;
        case 4: category = "Медицина"; break;
        case 5: category = "Предметы личной гигиены"; break;
        case 6: category = "Книги"; break;
        case 7: category = "Аксессуары"; break;
        case 8: category = "Предметы роскоши"; break;
        case 9: category = "Строительство и ремонт"; break;
        case 10: category = "Транспорт и запчасти"; break;
        case 11: category = "Зоотовары"; break;
        case 12: category = "Иное"; break;
    }

    string categoryFileName = "Categories/" + category + ".txt";
    ifstream inCategories(categoryFileName);
    if (!inCategories.is_open()) {
        cout << "Ошибка открытия файла категории." << endl;
        return;
    }

    string line;
    while (getline(inCategories, line)) {
        if (line.empty() || line == "-----") continue;

        Product product = ReadProductFromFile(inCategories);

        cout << "\nНазвание товара: " << product.name << endl
            << "Продавец: " << product.seller << endl
            << "Производитель: " << product.producer << endl
            << "Год выпуска: " << product.year << endl
            << "Описание: " << product.description << endl
            << "Доступно: " << product.availableAmount << ", Цена: " << product.price << endl;

        int amountToAdd;
        int choise;
        do {
            cout << "Добавить товар в корзину?" << endl << "Да [нажмите 1]" << endl << "Нет [нажмите 0]: ";
            cin >> choise;
        } while (choise > 1 || choise < 0);

        if (choise) {
            cout << "Введите количество для добавления в корзину: ";
            cin >> amountToAdd;

            if (amountToAdd <= product.availableAmount) {
                product.availableAmount -= amountToAdd;
                bool productRemoved = (product.availableAmount == 0); // Флаг, чтобы проверить, нужно ли удалить продукт
                // Обновляем файл категории
                string tempFile = "temp.txt";
                ofstream outTemp(tempFile);
                if (!outTemp.is_open()) {
                    cout << "Ошибка при создании временного файла." << endl;
                    return;
                }

                inCategories.clear();
                inCategories.seekg(
0);

                while (getline(inCategories, line)) {
                    if (line.empty() || line == "-----") {
                        outTemp << line << endl;
                        continue;
                    }
                    Product product = ReadProductFromFile(inCategories);
                    WriteProductToFile(outTemp, product);
                }

                inCategories.close();
                outTemp.close();
                remove(categoryFileName.c_str());
                rename(tempFile.c_str(), categoryFileName.c_str());

                // Запись в корзину
                ofstream outBasket("Baskets/" + NAME + ".txt", ios::app);
                if (outBasket.is_open()) {
                    float totalCost = amountToAdd * product.price;
                    outBasket << "Название товара: " << product.name << endl
                        << "Количество: " << amountToAdd << endl
                        << "Цена: " << product.price << endl
                        << "Итоговая стоимость: " << totalCost << endl
                        << "-------------" << endl;
                    outBasket.close();
                    cout << "Товар добавлен в корзину." << endl;
                }

                // Обновляем файл продавца
                string sellerFile = "Sellers/" + product.seller + ".txt";
                ifstream inSeller(sellerFile);
                if (!inSeller.is_open()) {
                    cout << "Ошибка открытия файла продавца." << endl;
                    return;
                }

                string tempSellerFile = "temp_seller.txt";
                ofstream outTempSeller(tempSellerFile);
                if (!outTempSeller.is_open()) {
                    cout << "Ошибка при создании временного файла продавца." << endl;
                    return;
                }

                while (getline(inSeller, line)) {
                    if (line.empty() || line == "-----") {
                        outTempSeller << line << endl;
                        continue;
                    }

                    Product product = ReadProductFromFile(inSeller);
                    if (product.name == productName && product.seller == sellerName) {
                        product.availableAmount = availableAmountInt;
                        WriteProductToFile(outTempSeller, product);
                    } else {
                        WriteProductToFile(outTempSeller, product);
                    }
                }

                inSeller.close();
                outTempSeller.close();
                remove(sellerFile.c_str());
                rename(tempSellerFile.c_str(), sellerFile.c_str());
                remove(tempFile.c_str());
                remove(tempSellerFile.c_str());
            } else {
                cout << "Недостаточно товара на складе." << endl;
            }
        }
        cout << "\n\n";
    }

    inCategories.close();
}

void viewBasket() {
    // Реализуйте функциональность просмотра корзины
    cout << "Просмотр корзины" << endl;
}

// ... (Остальной код)
