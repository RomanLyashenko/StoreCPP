#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

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
    float price = 0.0f; // Цена
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
Product ReadProductFromFile(const string& filename) {
    Product product;
    ifstream file(filename);
    if (file.is_open()) {
        getline(file, product.seller);
        getline(file, product.name);
        getline(file, product.producer);
        file >> product.year;
        file.ignore(); // Пропускаем символ новой строки
        getline(file, product.description);
        file >> product.availableAmount;
        file.ignore(); // Пропускаем символ новой строки
        file >> product.price;
        file.close();
    }
    else {
        cout << "Ошибка открытия файла: " << filename << endl;
    }
    return product;
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
void DeleteProduct(const string& sellerName, const string& category);
void Shopping();

int main() {
    SetConsoleTitleA("\t\tG A Z O N");
    // Русский язык
    setlocale(LC_ALL, "RU");
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
    int attempts = 3;
    while (attempts > 0) {
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
        cout << "Неверный пароль. Попробуйте снова. Осталось попыток: " << attempts << endl;
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

// Функция для управления товарами продавца
void ForSellers() {
    string sellerName, category;
    int action;
    cout << "Введите имя продавца: ";
    cin >> sellerName;
    cout << "Введите категорию товара: ";
    cin >> category;
    do {
        cout << "Выберите действие:" << endl;
        cout << "1. Добавить товар" << endl;
        cout << "2. Удалить товар" << endl;
        cout << "3. Перейти в личный кабинет" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> action;
        switch (action) {
        case 1:
            PutAProductUpForSale();
            break;
        case 2:
            DeleteProduct(NAME, category);
            break;
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
    }
    else {
        cout << "Ошибка открытия файла категории." << endl;
    }

    // Записываем товар в файл продавца
    string sellerFileName = "Sellers/" + product.seller + ".txt";
    ofstream outSellers(sellerFileName, ios::app);
    if (outSellers.is_open()) {
        WriteProductToFile(outSellers, product); // Используем функцию WriteProductToFile
        outSellers.close();
        cout << "Товар добавлен в файл продавца." << endl;
    }
    else {
        cout << "Ошибка открытия файла продавца." << endl;
    }
}

// Функция для личного кабинета продавца
void PersonalAccount() {
    Product product = ReadProductFromFile("Sellers(" + NAME + ").txt");
    if (!product.name.empty()) {
        cout << "Ваши товары:" << endl;
        cout << "Название: " << product.name << endl;
        cout << "Производитель: " << product.producer << endl;
        cout << "Год выпуска: " << product.year << endl;
        cout << "Описание: " << product.description << endl;
        cout << "Количество на складе: " << product.availableAmount << endl;
        cout << "Цена: " << product.price << endl;
    }
    else {
        cout << "У вас нет товаров." << endl;
    }
}

// Функция для удаления товара
void DeleteProduct(const string& sellerName, const string& category) {
    string productName;
    cout << "Введите название товара: ";
    cin.ignore();
    getline(cin, productName);

    // Удаление товара из файла категории
    string categoryFileName = "Categories(" + category + ").txt";
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
    string sellerFileName = "Sellers(" + sellerName + ").txt";
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
            outTemp << line
                << endl; // Записываем строку в временный файл
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
// Функция для покупок
void Shopping() {
    int choose;
    cout << "\t*Выберите категорию*" << endl << "1. Еда" << endl
        << "2. Одежда и обувь" << endl << "3. Техника" << endl
        << "4. Медицина" << endl << "5. Предметы личной гигиены" << endl
        << "6. Книги" << endl << "7. Аксессуары" << endl
        << "8. Предметы роскоши" << endl << "9. Строительство и ремонт" << endl
        << "10. Транспорт и запчасти" << endl << "11. Зоотовары" << endl
        << "12. Иное" << endl << "Ваш выбор: ";
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
    string categoryFileName = "Categories(" + category + ").txt";
    ifstream inCategories(categoryFileName);
    if (!inCategories.is_open()) {
        cout << "Ошибка открытия файла категории." << endl;
        return;
    }
    string line;
    while (getline(inCategories, line)) {
        if (line.empty() || line == "-----") continue;
        Product product = ReadProductFromFile(categoryFileName);
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
                inCategories.seekg(0);
                while (getline(inCategories, line)) {
                    if (line.empty() || line == "-----") {
                        outTemp << line << endl;
                        continue;
                    }
                    product = ReadProductFromFile(categoryFileName); // Считываем продукт из файла
                    WriteProductToFile(outTemp, product); // Записываем обновленный продукт
                }
                inCategories.close();
                outTemp.close();
                remove(categoryFileName.c_str());
                RenameFile(tempFile.c_str(), categoryFileName.c_str());
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
                }
                else {
                    cout << "Ошибка открытия файла корзины." << endl;
                }
                if (productRemoved) {
                    // Удаляем товар из файла продавца, если количество равно 0
                    DeleteProduct(product.seller, category);
                }
            }
            else {
                cout << "Недостаточно товара на складе." << endl;
            }
        }
    }
    inCategories.close();
}
