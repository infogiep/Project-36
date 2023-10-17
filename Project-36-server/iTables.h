#pragma once
#include <iostream>
#include <vector>
#include <string>
//using namespace std;

class iTables
{
public:
	virtual ~iTables() = default;
	virtual bool getStatusCreateTable() = 0;
	virtual std::vector<std::string> request_select_from_Table(std::string& request) = 0; //Запрос на получение данных из таблицы БД
	virtual bool request_insert_from_Table(std::string &request) = 0; //Запрос на добавление данных в таблицу БД
	virtual bool add_in_table(std::string lits_variables[]) = 0;

	virtual std::string get_id_user(const std::string& variable) = 0; /// получение id пользователя
};
