#pragma once
#include "ILogger.hpp"
#include <chrono>

namespace wv
{
	/*
	 * Содержит необходимые данные для логирования длительности операции
	 */
	class OperationLogger
	{
		const ILogger& _logger;
		const std::chrono::system_clock::time_point _begin;

	public:
		/*
		 * Конструктор, принимает объект, куда будет осуществляться логирование
		 */
		OperationLogger(const ILogger& logger);
		~OperationLogger();
	};
}
