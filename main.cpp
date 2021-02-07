//
// Created by nikita on 2/6/21.
//

# include <Server.hpp>
# include <iostream>

enum arg_index {
	host,
	port,
	logfile,
	threads
};

char**	parse_argv(int argc, char** argv) {
	char**		parsing_argv = new char*[4];

	for (int i = 1; i < argc; ++i) {
		if (!strcmp(argv[i], "-h"))
			parsing_argv[host] = argv[i];
		else if (!strcmp(argv[i], "-p"))
			parsing_argv[port] = argv[i];
		else if (!strcmp(argv[i], "-logfile"))
			parsing_argv[logfile] = argv[i];
		else if (!strcmp(argv[i], "-threads"))
			parsing_argv[threads] = argv[i];
	}
	return (argv);
}

int		main(int argc, char** argv) {
	char**	parsing_argv;

	try {
		Server server("172.17.0.2", "3306", "default.log");
		server.runServer();
	}
	catch (Exception& e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}
