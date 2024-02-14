#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>

#define PATH "../site/"

void setupSocketAndAddr(int &sockfd, sockaddr_in &sockaddr) {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cout << "Failed to create a new socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(8080); // htons is necessary to convert a number to network byte order
	std::cout << "Starting the Socket binding.." << std::endl;
	while (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		sleep(1);
	}
	std::cout << "Socket bind" << std::endl;

	if (listen(sockfd, 10) < 0) {
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

int connectionAccept(int sockfd, struct sockaddr *sockaddr, socklen_t *addrLen, int &type) {
	std::cout << "Server waiting for a Client connection" << std::endl;
	int connection = accept(sockfd, sockaddr, (socklen_t *) addrLen);
	if (connection < 0) {
		std::cout << "Failed to grab connection. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Client connected" << std::endl;

	std::string buffer;
	std::string ex;
	buffer.resize(1024);
	read(connection, &buffer[0], buffer.size());
	ex = buffer.substr(buffer.find_first_of('.') + 1, 3);
	type = 0;
	if (ex == "css")
		type = 1;
	else if (ex == "js ")
		type = 2;
	std::cout << "Client wanted type of file: " << ex << std::endl;
	return connection;
}

void responseBuilder(std::string path, std::string header, int connection) {
	std::fstream  fs;
	std::string line;
	std::string fileStr;
	std::string response;

	fs.open(path, std::fstream::in);
	while (fs.good()) {
		std::getline(fs, line);
		fileStr.append(line);
	}
	response.append(header);
	response.append(fileStr);
	send(connection, response.c_str(), response.size(), 0);
	std::cout << response << std::endl;
	fs.close();
}

//CGItest
void execJS() {
	std::vector<char *> arguments = {
			const_cast<char *>("/usr/bin/node"),
			const_cast<char *>((std::string(PATH) + "test.js").c_str()),
			nullptr
	};

	// Execute the interpreter with the provided arguments and environment
	if (execve(arguments[0], const_cast<char **>(arguments.data()), nullptr) == -1) {
		// execve failed
		std::cerr << "Failed to execute script. errno: " << errno << std::endl;
	}
}

int main() {
	std::cout << "Starting server.." << std::endl;

	int sockfd = -1;
	int fd[2];
	sockaddr_in sockaddr;
	setupSocketAndAddr(sockfd, sockaddr);
	auto addrLen = sizeof(sockaddr);

	int connection;
	int type;
	while (1) {
		connection = connectionAccept(sockfd, (struct sockaddr *) &sockaddr, (socklen_t *) &addrLen, type);
		if (type == 0) {
			responseBuilder(std::string(PATH) + "index.html", "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 377\r\n\r\n", connection);
		} else if (type == 1) {
			responseBuilder(std::string(PATH) + "styles.css", "HTTP/1.1 200 OK\nContent-Type: text/css\nContent-Length: 183\r\n\r\n", connection);
		} else if (type == 2) {
			responseBuilder(std::string(PATH) + "test.js", "HTTP/1.1 200 OK\nContent-Type: text/javascript\nContent-Length: 183\r\n\r\n", connection);
		}
		close(connection);
	}
	close(sockfd);
}