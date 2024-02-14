#include <unistd.h>
#include <iostream>
#include <vector>

int main() {
	// Prepare the argument vector
	std::vector<char *> arguments = {
			const_cast<char *>("/usr/bin/node"),
			const_cast<char *>("../site/test1.js"),
			nullptr
	};

	// Prepare the environment variable vector (empty for simplicity)
	std::vector<const char*> environment = {
			nullptr
	};

	// Execute the interpreter with the provided arguments and environment
	if (execve(arguments[0], const_cast<char **>(arguments.data()), nullptr) == -1) {
		// execve failed
		std::cerr << "Failed to execute script. errno: " << errno << std::endl;
		return EXIT_FAILURE;
	}

	// This code will only be reached if execve fails
	return EXIT_FAILURE;
}