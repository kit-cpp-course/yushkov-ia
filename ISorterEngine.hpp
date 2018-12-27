#pragma once
#include <algorithm>
#include "ParallelQuickSorter.hpp"
#include "IDataProvider.hpp"
#include "ILogger.hpp"
#include "OperationLogger.hpp"

namespace wv
{
	/*
	 * ѕредоставл€ет пользователю интерфейс дл€ сортировки данных
	 */
	template<typename T>
	class ISorterEngine
	{
	public:
		virtual ~ISorterEngine() = default;

		/*
		 * ќсуществл€ет сортировку данных, предоставл€емых IDataProvider 
		 */
		virtual void Sort(const IDataProvider<T>& provider) const = 0;
	};

	template<typename T>
	class SingleThreadSorter : public ISorterEngine<T>
	{
	private:
		const ILogger * _logger;

	public:
		SingleThreadSorter(const ILogger* logger);
		void Sort(const IDataProvider<T>& provider) const override;
	};


	template<typename T>
	class MultiThreadSorter : public ISorterEngine<T>
	{
	private:
		const ILogger * _logger;

	public:
		MultiThreadSorter(const ILogger* logger);
		void Sort(const IDataProvider<T>& provider) const override;
	};

	template <typename T>
	SingleThreadSorter<T>::SingleThreadSorter(const ILogger* logger)
		: _logger(logger)
	{}

	template <typename T>
	void SingleThreadSorter<T>::Sort(const IDataProvider<T>& provider) const
	{
		auto rawData = provider.GetData();
		auto data = std::vector<T>(rawData.begin(), rawData.end());
		{
			auto op = _logger->LogOperation("Single threaded sort begins");
			std::sort(data.begin(), data.end());
		}
		provider.PutData({data.begin(), data.end()});
	}

	template <typename T>
	MultiThreadSorter<T>::MultiThreadSorter(const ILogger* logger)
		: _logger(logger)
	{}

	template <typename T>
	void MultiThreadSorter<T>::Sort(const IDataProvider<T>& provider) const
	{
		auto data = provider.GetData();
		{
			auto op = _logger->LogOperation("Multi threaded sort begins");
			data = wv::ParallelQuickSort(data);
		}
		provider.PutData(data);
	}
}
