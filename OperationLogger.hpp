#pragma once
#include "ILogger.hpp"
#include <chrono>

namespace wv
{
	class OperationLogger
	{
		const ILogger * _logger;

		const std::chrono::system_clock::time_point _begin;
	public:
		OperationLogger(const ILogger * logger);

		~OperationLogger();
	};
}
