#include <string>
#include <vector>
#include <map>

namespace ld {

const std::vector<std::string> languages = {"c", "cpp"};

const std::map<std::string, std::string> defaultVersions = {{"cpp", "14"}, {"c", "99"}};

const std::map<std::string, std::vector<std::string>> versions = {
	{"c", {"89", "90", "99", "11", "17", "23"}},
	{"cpp", {"98", "11", "14", "17", "20", "23"}}
};

const std::map<std::pair<std::string, std::string>, int> cmakeLangVersions = {
	{{"c", "17"}, 21},
	{{"c", "23"}, 21},
	{{"cpp", "17"}, 8},
	{{"cpp", "20"}, 12},
	{{"cpp", "23"}, 20}
};

const std::map<std::string, std::vector<std::string>> langFileExtenction = {
	{"c", {".c", ".h"}},
	{"cpp", {".cpp", ".hpp", ".h", ".c"}}
};

}

