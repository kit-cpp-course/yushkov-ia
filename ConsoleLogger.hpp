#pragma once
#include "ILogger.hpp"
#include <string>

namespace wv
{
	class ConsoleLogger : public ILogger
	{
	public:
		void Log(int value) const override;

		void Log(const std::string& message) const override;

		void LogLine(const std::string& message) const override;

		OperationLogger LogOperation(const std::string& message) const override;
	};
}
