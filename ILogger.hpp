#pragma once
#include <string>

namespace wv
{
	class OperationLogger;

	/*
	 * Интерфейс для логирования действий
	 */
	class ILogger
	{
	public:
		virtual ~ILogger() = default;

		/*
		 * Записывает строку в лог
		 */
		virtual void Log(const std::string& message) const = 0;

		/*
		 * Записывает строку в лог, добавляя в конец символ конца строки
		 */
		virtual void LogLine(const std::string& message) const = 0;

		/*
		 * Записывает целое число в лог
		 */
		virtual void Log(int value) const = 0;

		/*
		 * Логирует длительность операции
		 */
		virtual OperationLogger LogOperation(const std::string& message) const = 0;
	};
}
