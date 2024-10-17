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
const int MAX_PRODUCTS = 100; // Максимальное количество товаров
const int PRODUCT_INFO_LINES = 7; // Количество линий информации о товаре

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
    setlocale(LC_ALL, "RU");
 
    // Попытка войти в систему, если не удается - регистрация
    StartOfTheProgramm(path);
    system("cls");
    Balance(path, path2);
    system("cls");
    int choose;
    do {
        cout << "Перейти в управление товарами?" << endl << "Да [нажмите 1]" <<endl << "Нет [нажмите 0]: ";
        cin >> choose;
       if(choose) ForSellers();
    } while (choose > 1 || choose < 0);
    const int MAX_PRODUCTS = 100;
    system("cls");
    Shopping();
    system("cls");
    viewBasket();
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

void ForSellers() {
    int action;
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
            DeleteProduct();
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
    string category;
    string nameOfProduct;
    string producer;
    string time;
    string description;
    float price;
    int amount;

    // Настройка категории
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

    // Получение информации о товаре
    cout << "Введите название товара: ";
    getline(cin, nameOfProduct);
    cout << "Введите производителя: ";
    getline(cin, producer);
    cout << "Введите год выпуска/производства товара: ";
    getline(cin, time);
    cout << "Описание товара: ";
    getline(cin, description);
    cout << "Введите количество товара (штук): ";
    cin >> amount;

    do {
        cout << "Введите цену товара: ";
        cin >> price;
    } while (price < 0);

    // Запись данных в файл
    ofstream outSellers("Sellers/" + NAME + ".txt", ios::app);
    if (outSellers.is_open()) {
        outSellers << nameOfProduct << endl << category << endl << producer << endl
            << time << endl << description << endl << amount << endl << price << endl << "-----" << endl;
        outSellers.close();
    }
    else {
        cout << "Ошибка при открытии файла для записи!" << endl;
    }

    // Запись в категорию
    ofstream outCategories("Categories/" + category + ".txt", ios::app);
    if (outCategories.is_open()) {
        outCategories << NAME << endl << nameOfProduct << endl << producer << endl
            << time << endl << description << endl << amount << endl << price << endl << "-----" << endl;
        outCategories.close();
        cout << "Товар успешно сохранён." << endl;
    }
    else {
        cout << "Ошибка при открытии файла для записи!" << endl;
    }
}
void PersonalAccount() {
    cout << "\t\t**Личный кабинет**" << endl;
    cout << "Имя: " << NAME << endl;
    cout << "Товары: " << endl;

    // Чтение товаров пользователя из файла
    ifstream inSellers("Sellers/" + NAME + ".txt");
    if (inSellers.is_open()) {
        string line;
        while (getline(inSellers, line)) {
            cout << line << endl; // Выводим каждую строку
        }
        inSellers.close();
    }
    else {
       cout << "Товары не обнаружены." << endl;
    }
    cout << endl;
}

void DeleteProduct() {
    string nameOfProductToDelete;
    cout << "Введите название товара, который хотите удалить: ";
    cin.ignore();
    getline(cin, nameOfProductToDelete);

    string sellerFile = "Sellers/" + NAME + ".txt";
    ifstream inSellers(sellerFile);
    ofstream outSellers("temp.txt");

    if (inSellers.is_open()) {
        string line;
        string category;
        bool found = false;

        while (getline(inSellers, line)) {
            if (line == nameOfProductToDelete) {
                found = true;
                getline(inSellers, category); // сохранение категории
                // Пропустить данные товара
                for (int i = 0; i < 6; i++) getline(inSellers, line);
                continue; // пропустить добавление в temp.txt
            }
            outSellers << line << endl;
        }

        inSellers.close();
        outSellers.close();
        remove(sellerFile.c_str());
        RenameFile("temp.txt", sellerFile.c_str());

        // Удаление из файла категории
        string categoryFile = "Categories/" + category + ".txt";
        ifstream inCategories(categoryFile);
        ofstream outCategories("temp_category.txt");

        if (inCategories.is_open()) {
            while (getline(inCategories, line)) {
                if (line == NAME) {
                    for (int i = 0; i < 7; i++) getline(inCategories, line); // пропускаем данные товара
                    continue;
                }
                outCategories << line << endl;
            }
            inCategories.close();
            outCategories.close();
            remove(categoryFile.c_str());
            RenameFile("temp_category.txt", categoryFile.c_str());
        }
    }
    else {
        cout << "Ошибка при открытии файла продавца." << endl;
    }
    remove("temp.txt");
    remove("temp_category.txt");
}

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

    ifstream inCategories("Categories/" + category + ".txt");
    if (!inCategories.is_open()) {
        cout << "Ошибка открытия файла категории." << endl;
        return;
    }

    string line;
    while (getline(inCategories, line)) {
        if (line.empty() || line == "-----") continue;

        string producer = line;
        string productName;
        getline(inCategories, productName);
        string produсer2;
        getline(inCategories, produсer2);
        string year;
        getline(inCategories, year);
        string description;
        getline(inCategories, description);
        string availableAmount;
        getline(inCategories, availableAmount);
        string priceStr;
        getline(inCategories, priceStr);

        float price = stof(priceStr);
        int availableAmountInt = stoi(availableAmount);

        cout << "\nНазвание товара: " << productName << endl
            << "Продавец: " << producer << endl
            << "Производитель: " << produсer2 << endl
            << "Год выпуска: " << year << endl
            << "Описание: " << description << endl
            << "Доступно: " << availableAmountInt << ", Цена: " << price << endl;

        int amountToAdd;
        int choise;
        do {
            cout << "Добавить товар в корзину?" << endl << "Да [нажмите 1]" << endl << "Нет [нажмите 0]: ";
            cin >> choise;
        } while (choise > 1 || choise < 0);
        if (choise) {
            cout << "Введите количество для добавления в корзину: ";
            cin >> amountToAdd;

            if (amountToAdd <= availableAmountInt) {
                availableAmountInt -= amountToAdd;
                bool productRemoved = (availableAmountInt == 0); // Флаг, чтобы проверить, нужно ли удалить продукт

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
                    outTemp << line << endl;
                    getline(inCategories, productName);
                    outTemp << productName << endl;
                    getline(inCategories, produсer2);
                    outTemp << produсer2 << endl;
                    getline(inCategories, year);
                    outTemp << year << endl;
                    getline(inCategories, description);
                    outTemp << description << endl;

                    string currentAmount;
                    getline(inCategories, currentAmount);
                    if (line == producer) {
                        outTemp << availableAmountInt << endl; // Обновляем количество только для нужного товара
                    }
                    else {
                        outTemp << currentAmount << endl; // Записываем текущее количество
                    }
                    getline(inCategories, priceStr);
                    outTemp << priceStr << endl;
                }

                inCategories.close();
                outTemp.close();
                remove(("Categories/" + category + ".txt").c_str());
                RenameFile(tempFile.c_str(), ("Categories/" + category + ".txt").c_str());

                // Запись в корзину
                ofstream outBasket("Baskets/" + NAME + ".txt", ios::app);
                if (outBasket.is_open()) {
                    float totalCost = amountToAdd * price;
                    outBasket << "Название товара: " << productName << endl
                        << "Количество: " << amountToAdd << endl
                        << "Цена: " << price << endl
                        << "Итоговая стоимость: " << totalCost << endl
                        << "-------------" << endl;
                    outBasket.close();
                    cout << "Товар добавлен в корзину." << endl;
                }

                // Обновляем файл продавца
                string sellerFile = "Sellers/" + producer + ".txt";
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
                    outTempSeller << line << endl;
                    getline(inSeller, category);
                    outTempSeller << category << endl;
                    getline(inSeller, produсer2);
                    outTempSeller << produсer2 << endl;
                    getline(inSeller, year);
                    outTempSeller << year << endl;
                    getline(inSeller, description);
                    outTempSeller << description << endl;

                    string currentAmount;
                    getline(inSeller, currentAmount);
                    if (line == producer) {
                        outTempSeller << (productRemoved ? "0" : to_string(availableAmountInt)) << endl;  // Обновляем количество
                    }
                    else {
                        outTempSeller << currentAmount << endl;
                    }
                    getline(inSeller, priceStr);
                    outTempSeller << priceStr << endl;
                }

                inSeller.close();
                outTempSeller.close();
                remove(sellerFile.c_str());
                RenameFile(tempSellerFile.c_str(), sellerFile.c_str());
                remove(tempFile.c_str());
                remove(tempSellerFile.c_str());

            }
            else {
                cout << "Недостаточно товара на складе." << endl;
            }
        }
    }
    inCategories.close();
}
void viewBasket() {
    string basketFileName = "Baskets/" + NAME + ".txt";
    ifstream basketFile(basketFileName);

    if (!basketFile.is_open()) {
        cout << "Ошибка: не удалось открыть корзину." << endl;
        return;
    }

    string selectedProducts[MAX_PRODUCTS][PRODUCT_INFO_LINES]; // Массив для хранения информации о товарах
    int productCount = 0; // Чтение содержимого корзины

    // Чтение содержимого корзины
    while (productCount < MAX_PRODUCTS && getline(basketFile, selectedProducts[productCount][0])) {
        if (selectedProducts[productCount][0].empty()) continue; // Проверка на пустую строку
        for (int i = 1; i < PRODUCT_INFO_LINES; ++i) {
            getline(basketFile, selectedProducts[productCount][i]);
        }
        productCount++;
    }

    basketFile.close();

    // Если корзина пустая
    if (productCount == 0) {
        cout << "Ваша корзина пуста." << endl;
        return;
    }

    // Получение текущего баланса
    ToKnowBalance(path3, NAME);

    // Вывод информации о товарах в корзине с предложением оплаты
    for (int i = 0; i < productCount; ) {
        cout << "Товар " << (i + 1) << ":" << endl;
        cout << selectedProducts[i][0] << endl; // Название товара
        cout << selectedProducts[i][1] << endl; // Количество
        cout << selectedProducts[i][2] << endl; // Цена
        cout << "Итоговая стоимость: " << selectedProducts[i][3] << endl;

        cout << "Оплатить товар? (1 - Да, 0 - Нет): ";
        int payChoice;
        cin >> payChoice;

        if (payChoice == 1) {
            float itemPrice = stof(selectedProducts[i][3]); // Получаем стоимость товара

            // Проверка на наличие достаточных средств
            if (BALANCE >= itemPrice) {
                // Запрашиваем адрес доставки
                string address;
                cout << "Введите адрес доставки (город, улица, дом): ";
                cin.ignore(); // Игнорируем оставшийся символ новой строки
                getline(cin, address);

                cout << "Товар успешно оплачен. Адрес доставки: " << address << endl;
                BALANCE -= itemPrice; // Уменьшаем баланс на стоимость товара

                // Удаление товара из корзины
                for (int j = i; j < productCount - 1; ++j) {
                    for (int k = 0; k < PRODUCT_INFO_LINES; ++k) {
                        selectedProducts[j][k] = selectedProducts[j + 1][k];
                    }
                }
                productCount--; // Уменьшаем счетчик товаров на 1
                // Не увеличиваем i, так как мы уже проверили i-ый товар
            }
            else {
                cout << "Недостаточно средств на балансе! Пополните баланс." << endl;
                Balance(path, path2);
            }
        }
        else {
            i++; // Переход к следующему товару
        }

        cout << "-------------" << endl;
    }

    // Перезапись корзины с обновлённым содержимым
    ofstream outBasket(basketFileName);
    if (outBasket.is_open()) {
        for (int i = 0; i < productCount; ++i) {
            outBasket << selectedProducts[i][0] << endl;
            outBasket << selectedProducts[i][1] << endl;
            outBasket << selectedProducts[i][2] << endl;
            outBasket << selectedProducts[i][3] << endl;
            outBasket << "-------------" << endl;
        }
        outBasket.close();
    }
    else {
        cout << "Ошибка: не удалось обновить корзину." << endl;
    }
}