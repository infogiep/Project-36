#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "iTables.h"
#include <memory>
#include <vector>

#include <iomanip>
#include <ctime>

#include "mySQL_db.h"

class db_Table : virtual public iTables
{
protected:
	bool _statusCreateDB = false;
	//shared_ptr<mySQL_db> connect;
	mySQL_db _connect;
	//std::unique_ptr<mySQL_db> _connect;

	std::string _request;
	///bool _flagCreateTable;
	MYSQL_RES* res;
	MYSQL_ROW row;

	bool add_in_table(std::string lits_variables[]) override;

	std::vector<std::string> show_tables(); // формирует список существующих таблиц

	bool checking_existence_table(const std::string tableName); //Проверка существования таблицы

	bool request_insert_from_Table(std::string& request) override;

	std::vector<std::string> request_select_from_Table(std::string& request) override; //Запрос на получение данных из таблицы БД

	std::string get_id_user(const std::string& variable) override; // Получение id пользователя


public:
	//db_Table(const bool& statusCreateDB, mySQL_db& connect); // изначально
	db_Table(const bool& statusCreateDB, mySQL_db& connect);
    db_Table();
	~db_Table() = default;

	bool getStatusCreateTable() override;
	/// 
	///bool checking_existence_table() override;
};

class db_Table_Client : public db_Table
{
private:
	bool _createTable_Clients;
	std::string get_id_user(const std::string& variable) override;
	char _mode; // режим в котором работае класс: s - server; c - client
	
	//int get_id_user(string& login);
	//int get_id_user(string& login, string& h_pass);
	
	//string get_id_clients(const string& login); // Получить id пользователя по логину
public:
	db_Table_Client(const bool& statusCreateDB, mySQL_db& connect);
	~db_Table_Client() = default;

	bool getStatusCreateTable() override;

	bool add_in_table(std::string lits_variables[]) override;
};

class db_Table_hPassword : public db_Table
{
private:
	std::string get_id_user(const std::string& variable) override;
	bool _createTable_hp;
	char _mode; // режим в котором работае класс: s - server; c - client
public:
	// mode: s - server; c - client
	//db_Table_hPassword(const bool& statusCreateDB, mySQL_db& connect, char mode); - изначально
	db_Table_hPassword(const bool& statusCreateDB, mySQL_db& connect);
    db_Table_hPassword();
	~db_Table_hPassword() = default;

	bool getStatusCreateTable() override;
	bool add_in_table(std::string lits_variables[]) override;
};

class db_Table_Messages : public db_Table
{
private:
	//bool _createTable_1, _createTable_2;
	bool _createTable_Messages;
	char _mode; // режим в котором работае класс: s - server; c - client

	std::string get_current_Time_Date(); /// текущая дата и время
	std::vector<std::string> request_select_from_Table(std::string& request) override; //Запрос на получение данных из таблицы БД
	std::string get_id_user(const std::string& variable) override; // по id получить login пользователя

public:
	// mode: s - server; c - client
	//db_Table_Messages(const bool& statusCreateDB, mySQL_db& connect, char mode); - изначально
	db_Table_Messages(const bool& statusCreateDB, mySQL_db& connect);
    db_Table_Messages();
	~db_Table_Messages() = default;
	bool getStatusCreateTable() override;

	bool add_in_table(std::string lits_variables[]) override; // сформировать сообщение

	//bool checking_existence_table() override;
};
