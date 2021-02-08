//
// Created by nikita on 2/6/21.
//

# include <Server.hpp>
# include <iostream>
# include <csignal>

enum arg_index {
	host,
	port,
	logfile,
	threads
};

void	close_server(int signum) {
	if (signum == SIGINT || signum == SIGTERM) {
		std::cout << "See you soon!" << std::endl;
		exit(0);
	}
}

char**	parse_argv(int argc, char** argv) {
	char**		parsing_argv = new char*[4];

	parsing_argv[host] = nullptr;
	parsing_argv[port] = strdup("3306");
	parsing_argv[logfile] = strdup("default.log");
	parsing_argv[threads] = strdup("4");
	for (int i = 1; i < argc; ++i) {
		if (!strcmp(argv[i], "-h"))
			parsing_argv[host] = argv[++i];
		else if (!strcmp(argv[i], "-p")) {
			free(parsing_argv[port]);
			parsing_argv[port] = argv[++i];
		}
		else if (!strcmp(argv[i], "-logfile")) {
			free(parsing_argv[logfile]);
			parsing_argv[logfile] = argv[++i];
		}
		else if (!strcmp(argv[i], "-threads")) {
			free(parsing_argv[threads]);
			parsing_argv[threads] = argv[++i];
		}
	}
	return (parsing_argv);
}

int		main(int argc, char** argv) {
	char**	parsing_argv;

	signal(SIGTERM, close_server);
	signal(SIGTERM, close_server);
	try {
		parsing_argv = parse_argv(argc, argv);
		if (!parsing_argv[host])
			throw Exception("Host required!");
		Server server(parsing_argv[host], parsing_argv[port], parsing_argv[logfile], std::stol(parsing_argv[threads]));
		server.runServer();
	}
	catch (Exception& e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}
