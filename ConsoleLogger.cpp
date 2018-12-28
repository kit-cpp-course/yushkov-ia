#include <iostream>
#include "ConsoleLogger.hpp"
#include "OperationLogger.hpp"

namespace wv
{
	void ConsoleLogger::Log(const int value) const
	{
		std::clog << value;
	}

	void ConsoleLogger::Log(const std::string& message) const
	{
		std::clog << message;
	}

	void ConsoleLogger::LogLine(const std::string& message) const
	{
		std::clog << message << '\n';
	}

	OperationLogger ConsoleLogger::LogOperation(const std::string& message) const
	{
		LogLine(message);
		return OperationLogger(*this);
	}
}
