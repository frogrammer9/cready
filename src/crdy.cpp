#include "funcions.hpp"
#include "askInterface.hpp"

#include <iostream>

int main (int argc, char *argv[]) {
	std::string a;
	try {
		a = crdy::internal::ask("Specify language", {"cpp", "c"}, {"cc"}, true);
	}
	catch(const crdy::internal::error& error) {
		error.log();
	}

	std::cout << a << "\n";

	return 0;
}
