#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "mysql.h"

//using namespace std;

class mySQL_db
{
public:
	mySQL_db(const std::string &host, const std::string &user, const std::string &password, const std::string& dbName);
    mySQL_db();
	//~mySQL_db();
	~mySQL_db() = default;
	bool connect_DB(const std::string& dbName); // Подключение к существующей БД

	void disconnect_DB(); //Разорвать соединение с БД

	bool create_DB(const std::string& dbName); // Создание новой БД
	void connection_new_DB(const std::string& dbName); // Подключение к новой БД
	const std::string get_dbName() const; // Получить имя созданной БД
	const bool getConnectStatus() const;

	MYSQL* getMYSQL() { return &this->mysql; }


private:
	std::string _host;
	std::string _user;
	std::string _password;
	std::string _dbName;

	MYSQL mysql;

	bool _connectStatus;

};

