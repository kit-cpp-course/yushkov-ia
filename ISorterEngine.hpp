#pragma once
#include <algorithm>
#include "ParallelQuickSorter.hpp"
#include "IDataProvider.hpp"
#include "ILogger.hpp"
#include "OperationLogger.hpp"

namespace wv
{
	template<typename T>
	class ISorterEngine
	{
	public:
		virtual ~ISorterEngine() = default;
		virtual void Sort(const IDataProvider<T>& provider) const = 0;
	};

	template<typename T>
	class SingleThreadSorter : public ISorterEngine<T>
	{
	private:
		ILogger * _logger;
	public:
		SingleThreadSorter(ILogger * logger)
			: _logger(logger) {}

		void Sort(const IDataProvider<T>& provider) const override
		{
			auto rawData = provider.GetData();
			auto data = std::vector<T>(rawData.begin(), rawData.end());
			{
				auto op = _logger->LogOperation("Single threaded sort begins");
				std::sort(data.begin(), data.end());
			}
			provider.PutData({ data.begin(), data.end() });
		}
	};

	template<typename T>
	class MultiThreadSorter : public ISorterEngine<T>
	{
	private:
		ILogger * _logger;
	public:
		MultiThreadSorter(ILogger * logger)
			: _logger(logger) {}

		void Sort(const IDataProvider<T>& provider) const override
		{
			auto data = provider.GetData();
			{
				auto op = _logger->LogOperation("Multi threaded sort begins");
				data = wv::ParallelQuikSort(data);
			}
			provider.PutData(data);
		}
	};
}
