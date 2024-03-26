#include "funcs.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
	std::string path = std::filesystem::current_path();
	path += "/";
	crdy::inti(path);
	//crdy::addToSrc();
  return 0;
}
