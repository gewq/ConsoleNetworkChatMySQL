#### О программе
---
- Программа представляет собой консольный сетевой чат, имеющий клиент-серверную архитектуру
- Клиентская часть расположена в папке `client` - отвечает за пользовательский интерфейс и отправляет запросы серверу
- Серверная часть расположена в папке `server` - отвечает за работу с Базой данных пользователей и их сообщений
- Клиентская и серверная части компилируются отдельно друг от друга
- База данных реализована посредством **MySQL**
- Сетевые настройки сервера и базы данных загружаются из файлов (XML). Для серверной и клиентской части файлы задаются разные. Имена файлов задаются через командную строку.

#### Клиент
---
- регистрация пользователей - логин, пароль, имя
- вход в чат по логину и паролю
- отправка сообщений конкретному пользователю или всем сразу
- выход из чата 
- удаление аккаунта пользователя
- отправка запросов серверу


#### Сервер
---
- обработка запросов клиентов
- обращение к Базе данных


#### База данных
---
- База содержит данные пользователей и их сообщений друг другу
```sql
CREATE TABLE users(
	id INT AUTO_INCREMENT PRIMARY KEY,
	#Логин пользователя
	login VARCHAR(20) NOT NULL,
	#Ник пользователя
	name VARCHAR(20) NOT NULL,
	#Хэш пароля учётной записи
	hash VARCHAR(200) NOT NULL
);
```

```sql
CREATE TABLE messages(\
	id INT AUTO_INCREMENT PRIMARY KEY,
	#Ник отправителя
	sender VARCHAR(20) NOT NULL,
	#Ник получателя
	receiver VARCHAR(20) NOT NULL,
	#Текст сообщения
	text VARCHAR(1000) NOT NULL
);
```


#### Описание выбранной идеи решения

##### Клиент
---
- Основная логика работы чата реализуется в виде конечного автомата и выносится в отдельный класс `Chat`.
    - Количество состояний автомата = 12. Переходы между состояниями осуществляются достаточно редко ~ секунд. Исходя из этого для реализации класса `Chat` применён паттерн **State**.
    - Поскольку чат в программе может быть только один - значит и объект класса `Chat` может быть только один. Поэтому помимо паттерна **State** также применён паттерн **Singleton**.
- Информация о конкретном пользователе инкапсулируется в отдельный класс `User`. У каждого зарегистрированного пользователя есть:
    - Ник - имя по которому он известен остальным пользователям
    - Логин
    - Хеш пароля
- Информация о конкретном сообщении инкапсулируется в отдельный класс `Message`.
- Запросы к серверу отправляет модуль `Server`

##### Сервер
---
- Формирует запросы к базе данных:
    - проверить зарегистрирован ли пользователь с заданным Логином
    - проверить зарегистрирован ли пользователь с заданным Ником
    - проверить соответствует ли Пароль заданному Логину
    - добавить в базу сообщение от одного пользователя другому
    - загрузить сообщения, адресованные заданному пользователю
    - добавить в базу нового пользователя
    - удалить заданного пользователя из базы
    - вернуть Ник пользователя по заданному Логину
    - вернуть количество зарегистрированных пользователей
    - вернуть Ники зарегистрированных пользователей
- Работа с сетью осуществляется посредством модуля `Network`
- Обработку входящих запросов выполняет модуль `Handler`


#### Установка и запуск (Linux)
---
Программа не кросс-платформенная - поддерживает только Linux.
Данная инструкция применима только для Linux


##### Установка - делается один раз
---
- Установить библиотеку взаимодействия C++ с MySQL
```
sudo apt-get update
sudo apt-get install libmysqlcppconn-dev
```

- Установить `mysql`
```linux
sudo apt-get update
sudo apt-get install mysql-server
```

- Создать в `mysql` пользователя с ==именем== (`newuser`) и ==паролем== (`password`), соответствующим ==текущей учётной записи!!!== Linux
```sql
CREATE USER 'newuser'@'localhost' IDENTIFIED BY 'password'; 
```

> [!important] Внимание!
> `newuser`, `password` - это нужно вводить в файле входных данных серверной части!

- Дать вновь созданному пользователю (`newuser`) все права
```sql
GRANT ALL PRIVILEGES ON *.* TO 'newuser'@'localhost'; 
```

- (Опционально) Создать тестовую базу данных `testdb` (Если не запускать тесты можно это не делать)
```sql
CREATE DATABASE testdb;
```

- Создать базу данных чата `chatdb`
> [!important] Внимание!
> `chatdb` - это название нужно вводить в файле входных данных серверной части!

```sql
CREATE DATABASE chatdb;
```

- Выбрать созданную базу данных
```sql
USE chatdb;
```

- Создать таблицы пользователей `users` и сообщений `messages`
```sql
CREATE TABLE users(
	id INT AUTO_INCREMENT PRIMARY KEY,
	#Логин пользователя
	login VARCHAR(20) NOT NULL,
	#Ник пользователя
	name VARCHAR(20) NOT NULL,
	#Хэш пароля учётной записи
	hash VARCHAR(200) NOT NULL
);
```

```sql
CREATE TABLE messages(\
	id INT AUTO_INCREMENT PRIMARY KEY,
	sender VARCHAR(20) NOT NULL,
	receiver VARCHAR(20) NOT NULL,
	text VARCHAR(1000) NOT NULL
);
```


##### Запуск
---
- Находясь в папке `server` ввести в терминале `./server <входной файл>`
- Дождаться появления надписи `Server is listening to new connections...`
- Находясь в папке `client` ввести в терминале `./client <входной файл>`


#### Платформа
---
- Linux
