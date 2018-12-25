#pragma once
#include <string>

namespace wv
{
	class OperationLogger;

	class ILogger
	{
	public:
		virtual ~ILogger() = default;
		virtual OperationLogger LogOperation(const std::string& message) const = 0;
		virtual void Log(const std::string& message) const = 0;
		virtual void LogLine(const std::string& message) const = 0;
		virtual void Log(int value) const = 0;
	};
}
