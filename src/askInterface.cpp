#include "askInterface.hpp"

#include <sstream>
#include <iostream>

void tolower(std::string& str) {
	for(size_t i = 0; i < str.size(); ++i) {
		str[i] = std::tolower(str[i]);
	}
}

bool isValid(std::string check, std::vector<std::string>& valids, bool ignoreCase) {
	if(ignoreCase) { tolower(check); }
	for(auto a : valids) {
		if(ignoreCase) tolower(a);
		if(a == check) return true;
	}
	return false;
}

std::string read(const crdy::internal::Optional<std::string>& dv) {
	std::string in;
	std::getline(std::cin, in);
	if(in.empty() && dv.isValid()) return dv.getValue();
	return in;
}

std::string crdy::internal::ask(std::string question, std::vector<std::string> validOpts,
	  crdy::internal::Optional<std::string> defVal, bool caseSensitive) {
	std::stringstream out;
	std::string in;
	out << question;
	size_t definx = -1;
	if(defVal.isValid()) { for(size_t i = 0; i < validOpts.size(); ++i) { if(validOpts[i] == defVal.getValue()) { definx = i; } } }
	if(!validOpts.empty()) {
		out << " - Valid entries are: ";
		for(size_t i = 0; i < validOpts.size(); ++i) {
			out << "[";
			if(i == definx) out << "|";
			out << validOpts[i];
			if(i == definx) out << "|";
			out << "]";
		}
	}
	if(defVal.isValid() && definx == -1) throw INTERNAL_ERROR("Default value specified in crdy::internal::ask isn't a valid entry\n");
	out << ": ";
	std::cout << out.str();
	in = read(defVal);
	if(!validOpts.empty()) {
		while(!isValid(in, validOpts, !caseSensitive)) {
			std::cout << "This response isn't valid. Please specify a valid option: ";
			in = read(defVal);
		}
	}
	if(!caseSensitive) tolower(in);
	return in;
}
