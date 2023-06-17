#include "DataBase.h"
#include <assert.h>


namespace {
	//Параметры для входа в базу данных MySQL
	std::string DATABASE_ADDRESS = "";
  std::string DATABASE_USER = "";
  std::string DATABASE_PASSWORD = "";
  std::string DATABASE_NAME = "";

	//Имя адресата чтобы отправить сообщение всем
	const std::string MSG_TO_ALL = "all";
}



void database::initialize(const std::string &url,
                          const std::string &user,
                          const std::string &password,
                          const std::string &database)
{
  DATABASE_ADDRESS = url;
  DATABASE_USER = user;
  DATABASE_PASSWORD = password;
  DATABASE_NAME = database;
}


// Подключиться к базе данных
static sql::Connection *connectToDatabase();


bool database::isLoginRegistered(const std::string &login)
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Отправить запрос
  const std::string query =
  "SELECT\
    name\
  FROM\
    users\
  WHERE\
    login = '" + login + "';";
  sql::ResultSet *response = statement->executeQuery(query);

  bool result = false;
  if (response->next() != false){
    result = true;
  }

  delete response;
  delete statement;
  delete connection;

  return result;
}



bool database::isPasswordRight(const std::string &login,
                               const std::string &passwordHash)
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query =
  "SELECT\
    hash\
  FROM\
    users\
  WHERE\
    login = '" + login + "';";

  // Отправить запрос
  sql::ResultSet *response = statement->executeQuery(query);

  bool result = false;
  if (response->next() != false){
    if (response->getString("hash") == passwordHash){
      result = true;
    }
  }

  delete response;
  delete statement;
  delete connection;

  return result;
}



bool database::isNicknameRegistered(const std::string &name)
{
  // Зарезервированное слово
  if (name == MSG_TO_ALL){
    return true;
  }

  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query =
  "SELECT\
    login\
  FROM\
    users\
  WHERE\
    name = '" + name + "';";

  // Отправить запрос
  sql::ResultSet *response = statement->executeQuery(query);

  bool result = false;
  if (response->next() != false){
    result = true;
  }

  delete response;
  delete statement;
  delete connection;

  return result;
}



std::string database::getNickname(const std::string &login)
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query =
  "SELECT\
    name\
  FROM\
    users\
  WHERE\
    login = '" + login + "';";

  // Отправить запрос
  sql::ResultSet *response = statement->executeQuery(query);

  std::string result = "";
  if (response->next() != false){
    result = response->getString("name");
  }

  delete response;
  delete statement;
  delete connection;

  return result;
}



size_t database::getNumberUsers()
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query =
  "SELECT\
    count(*)\
  FROM\
    users;";

  // Отправить запрос
  sql::ResultSet *response = statement->executeQuery(query);

  size_t result = 0;
  if (response->next() != false){
    result = std::stoi(response->getString(1));
  }

  delete response;
  delete statement;
  delete connection;

  return result;
}



void database::loadUserNames(std::shared_ptr<std::vector<std::string>> userNames)
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query =
  "SELECT\
    name\
  FROM\
    users;";

  // Отправить запрос
  sql::ResultSet *response = statement->executeQuery(query);

  userNames->clear();
  while (response->next()){
    userNames->push_back(response->getString("name"));
  }

  delete response;
  delete statement;
  delete connection;
}



void database::loadMessages(const std::string &login,
                            std::shared_ptr<std::list<Message> > &messages)
{
  // Получатель НЕ зарегистрирован
  if (!isLoginRegistered(login)){
    return;
  }
  
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  const std::string receiver = getNickname(login);

  // Сформировать запрос
  const std::string query =
  "SELECT\
    sender, text\
  FROM\
    messages\
  WHERE\
    receiver = '" + receiver + "' OR\
    receiver = '" + MSG_TO_ALL + "';";

  // Отправить запрос
  sql::ResultSet *response = statement->executeQuery(query);

  while (response->next()){
    std::string sender = response->getString("sender");
    std::string text = response->getString("text");
    messages->emplace_front(Message(sender, text));
  }

  delete response;
  delete statement;
  delete connection;
}



void database::addUser(const std::string &name,
                       const std::string &login,
                       const std::string &hash)
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query =
  "INSERT INTO users (login, name, hash)\
  VALUES ('" + login + "','" + name + "','" + hash + "');";

  // Отправить запрос
  statement->execute(query);

  delete statement;
  delete connection;
}



void database::addMessage(const std::string &sender,
                          const std::string &receiver,
                          const std::string &text)
{
  // Получатель НЕ зарегистрирован
  if (!isNicknameRegistered(receiver)){
    return;
  }

  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query =
  "INSERT INTO messages (sender, receiver, text)\
  VALUES ('" + sender + "','" + receiver + "','" + text + "');";

  // Отправить запрос
  statement->execute(query);

  delete statement;
  delete connection;
}

//Удалить сообщения адресованные пользователю с ником name
static void deleteMessages(const std::string& name);
//Удалить пользователя с ником name
static void deleteUser(const std::string& name);


void database::removeUser(const std::string& login)
{
  // Получатель НЕ зарегистрирован
  if (!isLoginRegistered(login)){
    return;
  }
  const std::string name = getNickname(login);
  deleteMessages(name); //Удалить сообщения для этого пользователя
  deleteUser(name);     //Удалить самого пользователя
}



//-----------------------------------------------------------------------------
// Задать настройки для работы с тестовой базой даных
static void initializeTestDatabase(const std::string& url,
                                    const std::string& user,
                                    const std::string& password);

static void testIsLoginRegistered();
static void testIsPasswordRight();
static void testIsNicknameRegistered();
static void testGetNickname();
static void testGetNumberUsers();
static void testLoadUserNames();
static void testLoadMessages();
static void testAddUser();
static void testAddMessage();
static void testRemoveUser();


void database::test(const std::string& url,
                    const std::string& user,
                    const std::string& password)
{
  initializeTestDatabase(url, user, password);

  testIsLoginRegistered();
  testIsPasswordRight();
  testIsNicknameRegistered();
  testGetNickname();
  testGetNumberUsers();
  testLoadUserNames();
  testLoadMessages();
  testAddUser();
  testAddMessage();
  testRemoveUser();
}
//=============================================================================
// Создать таблицу users
static void createTableUsers();

// Создать таблицу messages
static void createTableMessages();

// Удалить таблицу users
static void dropTableUsers();

// Удалить таблицу messages
static void dropTableMessages();



static void testIsLoginRegistered()
{
  createTableUsers();

  const std::string login = "Test_Login";
  const std::string name = "Test_Name";
  const std::string hash = "Test_Hash";

  // Пользователя нет в базе
  assert(database::isLoginRegistered(login) == false);

  database::addUser(name, login, hash);

  assert(database::isLoginRegistered(login) == true);

  dropTableUsers();
}



static void testIsPasswordRight()
{
  createTableUsers();

  const std::string login = "Test_Login";
  const std::string name = "Test_Name";
  const std::string hash = "Test_Hash";

  // Пользователя нет в базе
  assert(database::isPasswordRight(login, hash) == false);

  database::addUser(name, login, hash);

  assert(database::isPasswordRight(login, "not correct hash") == false);
  assert(database::isPasswordRight("not exist", hash) == false);
  assert(database::isPasswordRight(login, hash) == true);

  dropTableUsers();
}



static void testIsNicknameRegistered()
{
  createTableUsers();

  const std::string login = "Test_Login";
  const std::string name = "Test_Name";
  const std::string hash = "Test_Hash";

  // Пользователя нет в базе
  assert(database::isNicknameRegistered(name) == false);

  database::addUser(name, login, hash);

  assert(database::isNicknameRegistered(name) == true);

  dropTableUsers();
}



static void testGetNickname()
{
  createTableUsers();

  const std::string login = "Test_Login";
  const std::string name = "Test_Name";
  const std::string hash = "Test_Hash";

  // Пользователя нет в базе
  assert((database::getNickname(login) == name) == false);

  database::addUser(name, login, hash);

  assert((database::getNickname(login) == name) == true);

  dropTableUsers();
}



static void testGetNumberUsers()
{
  createTableUsers();

  // Таблица пользователей пуста
  assert(database::getNumberUsers() == 0);

  // Добавить тестовые значения
  const std::string login_1 = "testLogin_1";
  const std::string name_1 = "testName_1";
  const std::string hash_1 = "testHash_1";

  const std::string login_2 = "testLogin_2";
  const std::string name_2 = "testName_2";
  const std::string hash_2 = "testHash_2";

  database::addUser(login_1, name_1, hash_1);
  assert(database::getNumberUsers() == 1);

  database::addUser(login_2, name_2, hash_2);
  assert(database::getNumberUsers() == 2);

  dropTableUsers();
}



static void testLoadUserNames()
{
  createTableUsers();

  auto nicknames = std::make_shared<std::vector<std::string> >();
  database::loadUserNames(nicknames);

  // Таблица пользователей пуста
  assert(nicknames->empty() == true);

  // Добавить тестовые значения
  const std::string login_1 = "testLogin_1";
  const std::string name_1 = "testName_1";
  const std::string hash_1 = "testHash_1";

  const std::string login_2 = "testLogin_2";
  const std::string name_2 = "testName_2";
  const std::string hash_2 = "testHash_2";

  database::addUser(name_1, login_1, hash_1);
  database::addUser(name_2, login_2, hash_2);

  database::loadUserNames(nicknames);

  assert(nicknames->size() == 2);
  assert(nicknames->at(0) == name_1);
  assert(nicknames->at(1) == name_2);

  dropTableUsers();
}



static void testLoadMessages()
{
  createTableUsers();
  createTableMessages();

  // Добавить тестовое значение
  const std::string login_1 = "testLogin_1";
  const std::string name_1 = "testName_1";
  const std::string hash_1 = "testHash_1";
  database::addUser(name_1, login_1, hash_1);

  const std::string login_2 = "testLogin_2";
  const std::string name_2 = "testName_2";
  const std::string hash_2 = "testHash_2";
  database::addUser(name_2, login_2, hash_2);

  const std::string text_12 = "testText_1";
  database::addMessage(name_1, name_2, text_12);

  const std::string text_21 = "testText_2";
  database::addMessage(name_2, name_1, text_21);

  const std::string text_to_ALL = "testText_ALL";
  database::addMessage(name_1, MSG_TO_ALL, text_to_ALL);

  // Указатель на сообщения конкретному пользователю
  auto messagesToUser_1 = std::make_shared<std::list<Message> >();
  auto messagesToUser_2 = std::make_shared<std::list<Message> >();
  database::loadMessages(login_1, messagesToUser_1);
  database::loadMessages(login_2, messagesToUser_2);

  // Собщение от user_2 to user_1
  assert(messagesToUser_1->back().getNameFrom() == name_2);
  assert(messagesToUser_1->back().getText() == text_21);
  // Собщение всем от user_1
  assert(messagesToUser_1->front().getNameFrom() == name_1);
  assert(messagesToUser_1->front().getText() == text_to_ALL);
  assert(messagesToUser_1->size() == 2);

  // Собщение от user_1 to user_2
  assert(messagesToUser_2->back().getNameFrom() == name_1);
  assert(messagesToUser_2->back().getText() == text_12);
  // Собщение всем от user_1
  assert(messagesToUser_2->front().getNameFrom() == name_1);
  assert(messagesToUser_2->front().getText() == text_to_ALL);
  assert(messagesToUser_2->size() == 2);

  dropTableMessages();
  dropTableUsers();
}



static void testAddUser()
{
  createTableUsers();

  // Таблица пользователей пуста
  assert(database::getNumberUsers() == 0);

  // Добавить тестовое значение
  const std::string login = "Test_Login";
  const std::string name = "Test_Name";
  const std::string hash = "Test_Hash";
  database::addUser(name, login, hash);

  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();
  sql::ResultSet *result = statement->executeQuery("SELECT * from users;");
  result->next();
  assert(database::getNumberUsers() == 1);
  assert(result->getString("login") == login);
  assert(result->getString("name") == name);
  assert(result->getString("hash") == hash);

  delete result;
  delete statement;
  delete connection;

  dropTableUsers();
}



static void testAddMessage()
{
  createTableUsers();
  createTableMessages();

  // Чтобы добавить сообщение должны быть пользователи
  const std::string login_1 = "testLogin_1";
  const std::string name_1 = "testName_1";
  const std::string hash_1 = "testHash_1";
  database::addUser(name_1, login_1, hash_1);
  const std::string login_2 = "testLogin_2";
  const std::string name_2 = "testName_2";
  const std::string hash_2 = "testHash_2";
  database::addUser(name_2, login_2, hash_2);

  const std::string sender = name_1;
  const std::string receiver = name_2;
  const std::string text = "testText";
  database::addMessage(sender, receiver, text);

  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();
  sql::ResultSet *result = statement->executeQuery("SELECT * from messages;");
  result->next();
  assert(result->getString("sender") == sender);
  assert(result->getString("receiver") == receiver);
  assert(result->getString("text") == text);

  delete result;
  delete statement;
  delete connection;

  dropTableMessages();
  dropTableUsers();
}



static void testRemoveUser()
{
  createTableUsers();
  createTableMessages();

  // Добавить тестовое значение
  const std::string login_1 = "testLogin_1";
  const std::string name_1 = "testName_1";
  const std::string hash_1 = "testHash_1";
  database::addUser(name_1, login_1, hash_1);

  const std::string login_2 = "testLogin_2";
  const std::string name_2 = "testName_2";
  const std::string hash_2 = "testHash_2";
  database::addUser(name_2, login_2, hash_2);

  const std::string text_12 = "testText_1";
  database::addMessage(name_1, name_2, text_12);

  const std::string text_21 = "testText_2";
  database::addMessage(name_2, name_1, text_21);

  const std::string text_to_ALL = "testText_ALL";
  database::addMessage(name_1, MSG_TO_ALL, text_to_ALL);

  //При удалении пользователя удаляются сообщения, адресованные только ему
  database::removeUser(login_1);

  auto messagesToUser_1 = std::make_shared<std::list<Message> >();
  database::loadMessages(login_1, messagesToUser_1);

  assert(messagesToUser_1->empty() == true);

  dropTableMessages();
  dropTableUsers();
}



static void initializeTestDatabase(const std::string& url,
                                  const std::string& user,
                                  const std::string& password)
{
  database::initialize(url, user, password, "testdb");
}



static void createTableUsers()
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query =
  "CREATE TABLE users(\
    id INT AUTO_INCREMENT PRIMARY KEY,\
    login VARCHAR(20) NOT NULL,\
    name VARCHAR(20) NOT NULL,\
    hash VARCHAR(200) NOT NULL\
  );";

  // Отправить запрос
  statement->execute(query);

  delete statement;
  delete connection;
}



static void createTableMessages()
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query =
  "CREATE TABLE messages(\
    id INT AUTO_INCREMENT PRIMARY KEY,\
    sender VARCHAR(20) NOT NULL,\
    receiver VARCHAR(20) NOT NULL,\
    text VARCHAR(1000) NOT NULL\
  );";

  // Отправить запрос
  statement->execute(query);

  delete statement;
  delete connection;
}



static void dropTableUsers()
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query = "DROP TABLE users;";

  // Отправить запрос
  statement->execute(query);

  delete statement;
  delete connection;
}



static void dropTableMessages()
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Сформировать запрос
  const std::string query = "DROP TABLE messages;";

  // Отправить запрос
  statement->execute(query);

  delete statement;
  delete connection;
}



static void deleteMessages(const std::string& name)
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Удалить сообщения пользователю
  const std::string query =
  "DELETE FROM messages\
  WHERE receiver = '" + name + "';";

  // Отправить запрос
  statement->execute(query);

  delete statement;
  delete connection;
}



static void deleteUser(const std::string& name)
{
  // Подключиться к базе данных
  sql::Connection *connection = connectToDatabase();
  sql::Statement *statement = connection->createStatement();

  // Удалить сообщения пользователю
  const std::string query =
  "DELETE FROM users\
  WHERE name = '" + name + "';";

  // Отправить запрос
  statement->execute(query);

  delete statement;
  delete connection;
}



static sql::Connection *connectToDatabase()
{
  //Создать соединение
  sql::Driver *driver = get_driver_instance();
  sql::Connection *connection = driver->connect(DATABASE_ADDRESS,
                                                DATABASE_USER,
                                                DATABASE_PASSWORD);
  //Подключиться к базе данных
  connection->setSchema(DATABASE_NAME);
  return connection;
}