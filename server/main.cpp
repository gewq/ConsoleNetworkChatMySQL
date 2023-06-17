#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "mysql_connection.h"

#include "Initialization/Initialization.h"
#include "Network/Network.h"
#include "Handler/Handler.h"



int main(int argc, char** argv)
{
  try{
    initialization(argc, argv);
      
    while(true){
      network::startListen();
      std::string message = "";
      network::receive(&message);
      handler::handle(message);
      message.clear();
    }
    network::disconnect();
  }
  catch (sql::SQLException &e) {
    std::cout << "# ERR: SQLException in " << __FILE__;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what();
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
  }
	catch (std::exception& error) {
		std::cerr << error.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Undefined exception" << std::endl;
	}
  return EXIT_SUCCESS;
}