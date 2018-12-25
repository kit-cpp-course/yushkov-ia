#include "OperationLogger.hpp"
#include "ILogger.hpp"

namespace wv
{
	OperationLogger::OperationLogger(const ILogger * logger)
		: _logger(logger), _begin(std::chrono::system_clock::now())
	{
	}

	OperationLogger::~OperationLogger()
	{
		auto end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - _begin).count();

		_logger->Log("Operation compleded in: ");
		_logger->Log(elapsed);
		_logger->LogLine("millisecond");
	}
}
