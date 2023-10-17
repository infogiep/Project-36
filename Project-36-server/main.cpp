#include "mainwindow.h"
#include <fstream>

#include <QApplication>

template <typename T> std::vector<T> get_config_file(const std::string fileName);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto list = get_config_file<std::string>("cofig_file.txt"); //Запуск шаблонной функции для считывания данных из файла на этапе компиляции

    MainWindow w(list);
    w.show();
    return a.exec();
}


template<typename T>
std::vector<T> get_config_file(const std::string fileName)
{
    std::ifstream file = std::ifstream(fileName);
    std::vector<T> result;
    T item;

    if (file.is_open())
    {
        char ch;
        while (file.get(ch))
        {
            if (ch == '\n')
            {
                result.push_back(item);
                item = "";
                continue;
            }
            item += ch;
        }

        result.push_back(item);
        item = "";
    }

    file.close();

    return result;
}
