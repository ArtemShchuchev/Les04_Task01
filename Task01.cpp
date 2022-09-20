#include <iostream> // консоль - cout
#include <fstream>  // работа с файлами

class Adr
{
private:
    std::string city;
    std::string street;
    unsigned short house;
    unsigned short apartment;
public:
    Adr()
    {
        //setHouse(1);        // мне кажется - это не правильно
        //setApartment(1);    // ну с какой стати номер 1?
        // пусть будет так:
        house = 0;
        apartment = 0;
    }
    bool setCity(std::string str)
    {
        // по моему, в данном случае не особо много смысла в этой проверке?!
        if (str == "") return false;
        city = str;
        return true;
    }
    bool setStreet(std::string str)
    {
        if (str == "") return false;
        street = str;
        return true;
    }
    bool setHouse(short num)
    {
        if (num <= 0) return false;
        house = static_cast<unsigned short>(num);
        return true;
    }
    bool setApartment(short num)
    {
        if (num <= 0) return false;
        apartment = static_cast<unsigned short>(num);
        return true;
    }
    std::string getCity() { return city; }
    std::string getStreet() { return street; }
    unsigned short getHouse() { return house; }
    unsigned short getApartment() { return apartment; }
};

enum Err {NO_ERR, CANT_OP, FILE_ERR};

void memFree(Adr*&);

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");   // задаём русский текст
    system("chcp 1251");            // настраиваем кодировку консоли
    std::system("cls");

    std::cout << "Задача 1. Адреса\n";
    std::cout << "----------------\n" << std::endl;

    const std::string IN_FILE = "in.txt";
    const std::string OUT_FILE = "out.txt";

    Adr* adress = nullptr;          // указатель на структуру адресов
    unsigned int numAdr = 0;        // адресов в базе
    Err err = NO_ERR;               // ошибок чтения - нет
    
    // читаю из файла
    std::ifstream file(IN_FILE);
    if (file.is_open())
    {
        int temp; // временная переменная, т.к. проверка на отриц. значение
        if ((file >> temp) && temp > 0)
        {
            numAdr = temp;

            adress = new Adr[numAdr];    // создаю базу адресов в локальной памяти
            for (unsigned int i = 0; i < numAdr; ++i)
            {
                std::string str;    // без временных переменных можно?
                short temp;         // без временных переменных можно?
                if (!(file >> str && adress[i].setCity(str) &&
                    file >> str && adress[i].setStreet(str) &&
                    file >> temp && adress[i].setHouse(temp) &&
                    file >> temp && adress[i].setApartment(temp)))
                {
                    memFree(adress);
                    err = FILE_ERR;
                    break;
                }
            }
        }
        else err = FILE_ERR;

        file.close();
    }
    else err = CANT_OP;


    // пишу в файл
    std::ofstream file_wr(OUT_FILE);
    if (file_wr.is_open())
    {
        if (adress)
        {
            file_wr << numAdr << std::endl;
            for (int i = numAdr-1; i >= 0; --i)
            {
                file_wr << adress[i].getCity() << ", "
                    << adress[i].getStreet() << ", "
                    << adress[i].getHouse() << ", "
                    << adress[i].getApartment() << std::endl;
            }
            std::cout << "Смотри файл: " << OUT_FILE << std::endl;
        }
        else // если входной файл не считан, выходной создается пустым/очищается
        {
            switch (err)
            {
            case CANT_OP:
                std::cout << "Не удалось открыть файл: " << IN_FILE;
                break;
            case FILE_ERR:
                std::cout << "Ошибка! Файл повреждён!";
                break;
            default:
                std::cout << "Ошибка!";
            }
            std::cout << std::endl;
        }
        
        file_wr.close();
    }
    else std::cout << "Не удалось соранить файл: " << OUT_FILE << std::endl;

    memFree(adress);
    return 0;
}

void memFree(Adr*& memPtr)
{
    if (memPtr)
    {
        //std::cout << "Освобождаю память..." << std::endl;
        delete[] memPtr;
        memPtr = nullptr;
    }
    //else std::cout << "Нечего освобождать!" << std::endl;
}

/*
Пример работы программы
in.txt
3
Москва
Строителей
34
12
Омск
Пушкина
2
13
Новосибирск
Мира
85
64



out.txt
3
Новосибирск, Мира, 85, 64
Омск, Пушкина, 2, 13
Москва, Строителей, 34, 12
*/