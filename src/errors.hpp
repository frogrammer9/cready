#ifndef _SRC_ERRORS_
#define _SRC_ERRORS_

#include <iostream>
#include <stdexcept>

//TODO: Make it so the file and line is of the location a function was called on instead of where the throw statment is
#define INTERNAL_ERROR(msg) crdy::internal::internal_error(msg, __FILE__, __LINE__)

namespace crdy::internal {

class error : public std::exception {
public:
	virtual void log() const = 0;
private:
	std::string m_errorMsg;
};

class internal_error : public error {
public:
	internal_error(std::string msg, std::string file, int line) : m_errorMsg(msg), m_fileName(file), m_line(line) {}
	void log() const override { std::cerr << "[INTERNAL][" << m_fileName << " line: " << m_line << "] " << m_errorMsg << std::endl; }
private:
	std::string m_errorMsg;
	std::string m_fileName;
	int m_line;
};

class external_error : public error {
public:
	external_error(std::string msg) : m_errorMsg(msg) {}
	void log() const override { std::cerr << m_errorMsg << std::endl; }
private:
	std::string m_errorMsg;
};

}
#endif // !_SRC_ERRORS_
