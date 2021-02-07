//
// Created by nikita on 2/6/21.
//

# include <Server.hpp>
# include <iostream>

int		main(int argc, char** argv) {
	try {
		Server server("172.17.0.2", "3306", "default.log");
		server.runServer();
	}
	catch (Exception& e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}
