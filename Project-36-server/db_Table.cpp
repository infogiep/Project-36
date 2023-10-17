#include "db_Table.h"

bool db_Table::checking_existence_table(const std::string tableName)
{
	/*---Первое исполнение метода---*/
	/*bool result = false;
	this->_request.clear();
	this->_request = "select count(*) from " + tableName;

	bool flag_requst = false;
	flag_requst = !mysql_query(_connect.getMYSQL(), this->_request.c_str());
	if (flag_requst)
		return true;*/

	std::vector<std::string> list_show_tables = this->show_tables();

	for (auto& line : list_show_tables)
		if (line == tableName)
			return true;

	return false;
}

std::vector<std::string> db_Table::request_select_from_Table(std::string& request)
{
	std::vector<std::string> result;
	result.clear();
	//bool flag = !mysql_query(_connect.getMYSQL(), request.c_str()); // изначально
	bool flag = !mysql_query(_connect.getMYSQL(), request.c_str());
	if (flag)
	{
		//if (res = mysql_store_result(_connect.getMYSQL())) // изначально
		if (res = mysql_store_result(_connect.getMYSQL()))
		{
			while (row = mysql_fetch_row(res))
			{
				for (int i = 0; i < mysql_num_fields(res); i++)
				{
					//cout << row[i] << "  ";
					result.push_back(row[i]);
				}
				///cout << endl;
			}
		}
	}
	return result;
}

std::string db_Table::get_id_user(const std::string& variable)
{
	return 0;
}

bool db_Table::add_in_table(std::string lits_variables[])
{
	return false;
}

std::vector<std::string> db_Table::show_tables()
{
	this->_request = " ";
	this->_request = "show tables";
	std::vector<std::string> result;
	result.clear();
	//bool flag = !mysql_query(_connect.getMYSQL(), this->_request.c_str()); // изначально
	bool flag = !mysql_query(_connect.getMYSQL(), this->_request.c_str());
	if (flag)
	{
		//if (res = mysql_store_result(_connect.getMYSQL())) // изначально
		if (res = mysql_store_result(_connect.getMYSQL()))
		{
			while (row = mysql_fetch_row(res)) 
			{
				for (int i = 0; i < mysql_num_fields(res); i++) 
				{
					//cout << row[i] << "  ";
					result.push_back(row[i]);
				}
				///cout << endl;
			}
		}
	}
	return result;
}

db_Table::db_Table(const bool& statusCreateDB, mySQL_db& connect) : _statusCreateDB(statusCreateDB), _connect(connect)
{}

db_Table::db_Table() {}

bool db_Table::request_insert_from_Table(std::string & request)
{
	if (this->getStatusCreateTable() == true)
	{
		//bool flag = !mysql_query(_connect.getMYSQL(), request.c_str()); // изначально
		bool flag = !mysql_query(_connect.getMYSQL(), request.c_str());
		if (flag == true)
			return true;
	}

	return false;
}

bool db_Table::getStatusCreateTable()
{
	return false;
}


std::string db_Table_Client::get_id_user(const std::string& variable)
{
	/*string result = "";
	this->_request = "select id from clients where id in "
		"(select clients_id from password_hash where hash = '" + lits_variables[1] + "') and login = '" + lits_variables[0] + "'";

	cout << this->_request << endl;

	auto list = this->request_select_from_Table(this->_request);

	for (auto& item : list)
		result += item;

	return result;*/

	std::string result = "";
	//cout << variable << endl;
	this->_request = "select clients.id from clients where login = '" + variable + "'";

	auto list = this->request_select_from_Table(this->_request);

	if (list.size() > 0)
	{
		for (auto& item : list)
			result += item;
	}
	else
		result = "-1";

	return result;
}

//db_Table_Client::db_Table_Client(const bool& statusCreateDB, mySQL_db& connect, char mode) : db_Table(statusCreateDB, connect) - изначально
db_Table_Client::db_Table_Client(const bool& statusCreateDB, mySQL_db& connect) 
	: db_Table(statusCreateDB, connect)
{
	if (_statusCreateDB == true)
	{
		/*---clients---*/
		bool existence_table_clients = this->checking_existence_table("clients");
		///bool existence_table = checking_existence_table();
		if (existence_table_clients == true)
		{
			//this->_flagCreateTable = true;
			this->_createTable_Clients = true;
		}
		else
		{
			this->_request = "CREATE TABLE  `clients` ( `id` int NOT NULL AUTO_INCREMENT,"
				" `last_name` varchar(255) NOT NULL," 
				"`name` varchar(255) NOT NULL,"
				"`patronymic` varchar(255) NOT NULL,"
				"`email` text,"
				"`login` varchar(255) NOT NULL,"
				"PRIMARY KEY(`id`))";

			bool flag_clients = !mysql_query(_connect.getMYSQL(), this->_request.c_str());

			if (flag_clients)
				this->_createTable_Clients = true;
			
			if (flag_clients == false && existence_table_clients == false)
				this->_createTable_Clients = false;
		}
	}
}

bool db_Table_Client::getStatusCreateTable()
{
	return this->_createTable_Clients;
}


bool db_Table_Client::add_in_table(std::string lits_variables[])
{																		/// 0       1       2          3     4
    this->_request = "INSERT INTO " + _connect.get_dbName() + ".clients(last_name, name, patronymic, email, login)" +
		"VALUES('" + lits_variables[0] // last_name
		+ "', '" + lits_variables[1] // name
		+ "', '" + lits_variables[2] ///patronymic
		+ "', '" + lits_variables[3] //email
		+ "', '" + lits_variables[4] //login
		+ "')";

	if (this->request_insert_from_Table(this->_request))
	{
		return true;
	}

	return false;
}


std::string db_Table_Messages::get_current_Time_Date()
{
	/*auto current_time = chrono::system_clock::now();
	time_t current_time_t = chrono::system_clock::to_time_t(current_time);
	return ctime(&current_time_t);*/
	/*auto time = std::time(nullptr);

	put_time(localtime(&time), "%F / %T");*/

	time_t rawtime;
	char buffer[80];
	time(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y", localtime(&rawtime));
	std::string str(buffer);

	return str;
}

std::vector<std::string> db_Table_Messages::request_select_from_Table(std::string& request)
{
	std::vector<std::string> result;
	result.clear();
	bool flag = !mysql_query(_connect.getMYSQL(), request.c_str());
	if (flag)
	{
		if (res = mysql_store_result(_connect.getMYSQL()))
		{
			while (row = mysql_fetch_row(res))
			{
				std::string v_0 = row[0], v_1 = row[1], v_2 = row[2], v_3 = row[3];

				result.push_back("От: " + v_0 + " | Кому: " + v_1 + " | Сообщение: " + v_2 + " | Дата: " + v_3);
			}
		}
	}
	return result;
}

std::string db_Table_Messages::get_id_user(const std::string& variable)
{
	std::string result = "";
	//cout << variable << endl;
	this->_request = "select clients.login from clients where id = '" + variable + "'";

	auto list = this->request_select_from_Table(this->_request);

	if (list.size() > 0)
	{
		for (auto& item : list)
			result += item;
	}
	else
		result = "-1";

	return result;
}

/*db_Table_Messages::db_Table_Messages(const bool& statusCreateDB, mySQL_db& connect, char mode) -- изначально 
	: db_Table(statusCreateDB, connect), _connect(connect)*/
db_Table_Messages::db_Table_Messages(const bool& statusCreateDB, mySQL_db& connect) 
	: db_Table(statusCreateDB, connect)
{
	if (_statusCreateDB == true)
	{
		/*---messages---*/
		bool existence_table_1 = checking_existence_table("messages");
		if (existence_table_1 == true)
		{
			//this->_flagCreateTable = true;
			this->_createTable_Messages = true;
		}
		else
		{
			this->_request = "CREATE TABLE `messages`"
				"(`id` int NOT NULL AUTO_INCREMENT, "
				"`sender_login` varchar(255) NOT NULL,"
				"`recipient_login` varchar(255) NOT NULL, "
				"`text_messag` text, "
				"`date_send` varchar(255) NOT NULL, "
				"PRIMARY KEY (`id`))";


			bool flag_1 = !mysql_query(_connect.getMYSQL(), this->_request.c_str());

			if (flag_1)
				this->_createTable_Messages = true;

			if (flag_1 == false && existence_table_1 == false)
				this->_createTable_Messages = false;
        }
    }
}

db_Table_Messages::db_Table_Messages()
{}

bool db_Table_Messages::getStatusCreateTable()
{
	return this->_createTable_Messages;
}

bool db_Table_Messages::add_in_table(std::string lits_variables[])
{
	//sender - отправитель
	//recipient - получатель

	std::string default_status = "не прочитано"; // статус по умолчанию
	std::string current_date = this->get_current_Time_Date();

	this->_request = "INSERT INTO " + _connect.get_dbName() + ".messages(`sender_login`, `recipient_login`, `text_messag`, `date_send`)" +
		"VALUES('" + lits_variables[0] // логин отправителя
		+ "', '" + lits_variables[1] // логин получателя
		+ "', '" + lits_variables[2] // текст сообщения
		+ "', '" + current_date
		+ "')";

	if (this->request_insert_from_Table(this->_request))
	{
		return true;
	}
	return false;
}

std::string db_Table_hPassword::get_id_user(const std::string& variable)
{
	//select password_hash.clients_id from password_hash where hash = '439'
	std::string result = "";
	this->_request = "select password_hash.clients_id from password_hash where hash = '" + variable + "'";

	auto list = this->request_select_from_Table(this->_request);

	if (list.size() > 0)
	{
		for (auto& item : list)
			result += item;
	}
	else
	{
		result = "-1";
	}

	return result;
}

//db_Table_hPassword::db_Table_hPassword(const bool& statusCreateDB, mySQL_db& connect, char mode) : db_Table(statusCreateDB, connect) - изначально
db_Table_hPassword::db_Table_hPassword(const bool& statusCreateDB, mySQL_db& connect) 
	: db_Table(statusCreateDB, connect)
{
	/*---password_hash---*/
	bool existence_table_hp = this->checking_existence_table("password_hash"); //проверка существования таблицы

	if (existence_table_hp == true)
	{
		//this->_flagCreateTable = true;
		this->_createTable_hp = true;
	}
	else
	{
		this->_request = "CREATE TABLE `password_hash`(`clients_id` int DEFAULT NULL, `hash` varchar(32) DEFAULT NULL)";

		bool flag_hp = !mysql_query(_connect.getMYSQL(), this->_request.c_str());

		if (flag_hp)
			this->_createTable_hp = true;

		if (existence_table_hp == false && flag_hp == false)
            this->_createTable_hp = false;
    }
}

db_Table_hPassword::db_Table_hPassword()
{}

bool db_Table_hPassword::getStatusCreateTable()
{
	return this->_createTable_hp;
}

bool db_Table_hPassword::add_in_table(std::string lits_variables[])
{
	//this->get_id_user(lits_variables[0])
	///string id_login = lits_variables[0];

	this->_request = "INSERT INTO " + _connect.get_dbName() + ".password_hash(clients_id, hash)" +
		"VALUES('" + lits_variables[0] //id_login
		+ "', '" + lits_variables[1] //email
		+ "')";

	//cout << this->_request << endl;

	if (this->request_insert_from_Table(this->_request))
        return true;

    return false;
}
