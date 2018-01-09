#pragma once

#include <string>
#include <iostream>

#define LOG_ERROR_BIT   (1 << 0)
#define LOG_DEBUG_BIT   (1 << 1)
#define LOG_WARNING_BIT (1 << 2)


namespace Log
{
	void whatToShow(int flags);
	void showOutput(bool show);
	void showError(bool show);
	void showDebug(bool show);
	void showWarning(bool show);

	void print(const std::string& msg);
	void error(const std::string& msg);
	void debug(const std::string& msg);
	void warning(const std::string& msg);

	void println(const std::string& msg);
	void errorln(const std::string& msg);
	void debugln(const std::string& msg);
	void warningln(const std::string& msg);
}