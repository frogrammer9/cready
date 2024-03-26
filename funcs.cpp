#include "funcs.hpp"
#include "langData.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;
///////////////////////////////////////////////////////////////////
//// Helper funcions
///////////////////////////////////////////////////////////////////
int cmakeMinVer = 0;
std::string to_lower(const std::string& str) {
	std::string out = "";
	for(auto ch : str) {
		if(ch > 64 && ch < 91) out += (ch | 0x20);
		else out += ch;
	}
	return out;
}

std::string to_upper(const std::string& str) {
	std::string out = "";
	for(auto ch : str) {
		if(ch > 96 && ch < 123) out += (ch & 0xDF);
		else out += ch;
	}
	return out;
}

bool endsWith(std::string str, const std::vector<std::string>& vec) {
	for(const auto& a : vec) {
		if(str.ends_with(a)) return true;
	}
	return false;
}

bool hasspace(const std::string& str) {
	for(auto a : str) {
		if(isspace(a)) return true;
	}
	return false;
}

bool vectorContains(const std::vector<std::string>& vec, std::string elem) {
	for(int i = 0; i < vec.size(); ++i) {
		if(vec[i] == elem) return true;
	}
	return false;
}

std::string getDirName() {
	std::string path = fs::current_path();
	path = path.substr(path.find_last_of("/") + 1);
	return path;
}

///////////////////////////////////////////////////////////////////
//// Project helper funcions
///////////////////////////////////////////////////////////////////
bool getCmakeVersion(std::string lang, std::string ver) {
	if(ld::cmakeLangVersions.find({lang, ver}) == ld::cmakeLangVersions.end()) {
		if(lang == "c") return 0;
		else if(lang == "cpp") return 1;
	}
	return ld::cmakeLangVersions.at({lang, ver});
}

void logOptions(const std::vector<std::string>& opt) {
	std::cout << "(";
	for(const auto& a : opt) {
		std::cout << "[" << a << "]";
	}
	std::cout << "): ";
}

void gencmake(const fs::path& pathToDir) {
	////TODO	
}

fs::path getProjectRootPath() {
	fs::path currentPath = fs::current_path();
	while(currentPath != (currentPath.root_name() / "/")) {
		fs::path filePath = currentPath / ".crdy";
		if(fs::exists(filePath) && fs::is_regular_file(filePath)) {return currentPath;}
		currentPath = currentPath.parent_path();
	}
	std::cout << "Not inside a valid crdy project\n";
	exit(EXIT_FAILURE);
}

std::string getParam(std::string param) {
	std::ifstream in(getProjectRootPath() / ".crdy");
	std::string line;
	std::string data;
	while(std::getline(in, line)) {
		auto paramLoc = line.find(param + "=");
		if(paramLoc != std::string::npos) {
			data = line.substr(paramLoc + param.size() + 1);
			return data;
		}
	}
	std::cout << "Parameter \"" << param <<"\" is not defined\n";
	exit(EXIT_FAILURE);
}

///////////////////////////////////////////////////////////////////
//// Crdy funcions
///////////////////////////////////////////////////////////////////

void crdy::inti(std::string path) {
  std::string projName = "";
  std::string lang;
  std::string langVer;
  //// Project name
  std::cout << "Project name: ";
  std::getline(std::cin, projName);
  while (projName == "" || hasspace(projName)) {
    std::cout << "Invalid project name\n";
    std::cout << "Project name: ";
	std::getline(std::cin, projName);
  }
  //// Language
	std::cout << "Language ";
	logOptions(ld::languages);
	std::getline(std::cin, lang);
  while (!vectorContains(ld::languages, lang)) {
    std::cout << "Language \""<< lang <<"\" not supported by crdy\n";
    std::cout << "Language ";
	logOptions(ld::languages);
	std::getline(std::cin, lang);
  }
  //// Language version
	std::cout << "Language version ";
	logOptions(ld::versions.at(lang));
	std::getline(std::cin, langVer);
	if(langVer == "") langVer = ld::defaultVersions.at(lang);
	while(!vectorContains(ld::versions.at(lang), langVer)) {
		std::cout << langVer << " is not a viable version for " << lang <<" language\n";
		std::cout << "Language version ";
		logOptions(ld::versions.at(lang));
		std::getline(std::cin, langVer);
		if(langVer == "") langVer = ld::defaultVersions.at(lang);
  }
	int cmakeVer = getCmakeVersion(lang, langVer);
	if(cmakeMinVer < cmakeVer) cmakeMinVer = cmakeVer;

	fs::create_directory(path + "src");
	fs::create_directory(path + "lib");
	fs::create_directory(path + "vendor");
	fs::create_directory(path + "assets");

	std::ofstream out;
	out.open("CMakeLists.txt");
	out << "cmake_minimum_required(VERSION 3." << cmakeMinVer <<")\n\n";
	out << "project(" << projName << ")\n\n";
	out << ((lang == "cpp") ? "set(CMAKE_CXX_STANDARD " : "set(CMAKE_C_STANDARD");
	out << langVer << ")\n";
	out << ((lang == "cpp") ? "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -std=c++" : "set(CMAKE_C_FLAGS \"${CMAKE_C_FLAGS} -std=c");
	out << langVer << ((lang == "cpp") ? " -03\")\n\n" : "\")\n\n");
	out << "include_directories(src)\ninclude_directories(lib)\ninclude_directories(vendor)\n\n";
	out << "add_subdirectory(src)\nadd_subdirectory(lib)\nadd_subdirectory(vendor)\n\n";
	out.close();
	out.open(".crdy");
	out << "LANG=" << lang << "\n";
	out.close();
}


void crdy::addToSrc() {
	std::string lang = getParam("LANG");
	std::ifstream in;
	in.open("CMakeLists.txt");
	std::vector<std::string> dataBfSetSrc;
	std::vector<std::string> setSrcData;
	std::vector<std::string> dataAfSetSrc;
	std::string line;
	int setSrcPos = 0;
	while(std::getline(in, line)) {
		if(setSrcPos == 0) dataBfSetSrc.push_back(line + "\n");
		else if(setSrcPos == 1) setSrcData.push_back(line + "\n");
		else if(setSrcPos == 2) dataAfSetSrc.push_back(line + "\n"); 
		if(setSrcPos == 0 && line.find("set(") != std::string::npos && line.find("_SOURCE") != std::string::npos) setSrcPos = 1;
		if(setSrcPos == 1 && line.find(")") != std::string::npos) setSrcPos = 2;
	}
	in.close();

	setSrcData.erase(setSrcData.end());
	for(const auto& p : fs::recursive_directory_iterator(fs::current_path())) {
		if (fs::is_regular_file(p) && endsWith(p.path(), ld::langFileExtenction.at(lang))) {
			std::string path = fs::relative(p, getProjectRootPath());
			bool alreadyIs = false;
			for(auto str : setSrcData) {
				if(str.find(path) != std::string::npos) { alreadyIs = true; break; }
			}
			if(!alreadyIs) setSrcData.push_back(path + "\n");
		}
	}
	setSrcData.push_back(")\n");
	std::ofstream out;
	out.open("CMakeLists.txt");
	for(const auto& str : dataBfSetSrc) out << str;
	for(const auto& str : setSrcData) out << str;
	for(const auto& str : dataAfSetSrc) out << str;
	out.close();
}

void crdy::initCmakeLib() {
//TODO this implementation if just for debugging another funcion
	std::ofstream out;
	out.open("CMakeLists.txt");
	out << "set("<< to_upper(getDirName()) << "_SOURCE\n)";
	out.close();
}

void crdy::initCmakeSrc() {
//TODO this implementation if just for debugging another funcion
	std::ofstream out;
	out.open("CMakeLists.txt");
	out << "set(SRC_SOURCE\n)";
	out.close();
}


void crdy::run() {}

void crdy::build() {}
