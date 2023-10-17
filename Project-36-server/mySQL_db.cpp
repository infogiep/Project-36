#include "mySQL_db.h"

mySQL_db::mySQL_db(const std::string& host, const std::string& user, const std::string& password, const std::string& dbName):
    _host(host), _user(user), _password(password), _dbName(dbName)
{}

mySQL_db::mySQL_db()
{}

bool mySQL_db::connect_DB(const std::string& dbName)
{
	_dbName = dbName;

	// Получаем дескриптор соединения
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		std::cout << "Error: can't create MySQL-descriptor" << std::endl;

		_connectStatus = false;
		return _connectStatus;
	}

	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql, _host.c_str(), _user.c_str(), _password.c_str(), _dbName.c_str(), NULL, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		std::cout << "Error: can't connect to database " << mysql_error(&mysql) << std::endl;

		_connectStatus = false;
		return _connectStatus;
	}
	else {
		// Если соединение успешно установлено
		///cout << "Success!" << endl;

		if (!_dbName.empty())
			std::cout << "Connected to " << _dbName << std::endl;

		_connectStatus = true;

		mysql_set_character_set(&mysql, "utf8mb4");
	}

	//return true;
	return _connectStatus;
}


void mySQL_db::disconnect_DB()
{
	mysql_close(&this->mysql);
}


bool mySQL_db::create_DB(const std::string& dbName)
{
	std::string request = "CREATE DATABASE IF NOT EXISTS " + dbName;

	if (connect_DB(""))
	{
		if (mysql_query(&mysql, request.c_str()))
			return _connectStatus = true;
	}
	
	return _connectStatus = false;
}

void mySQL_db::connection_new_DB(const std::string& dbName)
{
	create_DB(dbName);
	connect_DB(dbName);
}


const std::string mySQL_db::get_dbName() const
{
	return _dbName;
}

const bool mySQL_db::getConnectStatus() const
{
	return _connectStatus;
}
