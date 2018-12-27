#pragma once
#include <future>
#include <list>
#include "ThreadSafeStack.hpp"

namespace wv
{
	/*
	 * Алгоритм параллельного алгоритма Quicksort
	 */
	template<typename T>
	class ParallelQuickSorter
	{
		struct ChunkToSort
		{
			std::list<T> _data;
			std::promise<std::list<T>> _promise;
		};

		ThreadSafeStack<ChunkToSort> _chuncks;
		std::vector<std::thread> _threads;
		unsigned const _maxThreadCount;
		std::atomic<bool> _endOfData;

	public:
		ParallelQuickSorter();
		~ParallelQuickSorter();
		void TrySortChunk();
		std::list<T> DoSort(std::list<T>& chunkData);
		template<typename U>
		void SortChunk(U chunk);
		void SortThread();
	};

	template <typename T>
	ParallelQuickSorter<T>::ParallelQuickSorter()
		: _maxThreadCount(std::thread::hardware_concurrency() - 1),
		_endOfData(false)
	{}

	template <typename T>
	ParallelQuickSorter<T>::~ParallelQuickSorter()
	{
		_endOfData = true;
		for (auto& thread : _threads)
		{
			thread.join();
		}
	}

	template <typename T>
	void ParallelQuickSorter<T>::TrySortChunk()
	{
		auto chunk = _chuncks.Pop();
		if (chunk) SortChunk(chunk);
	}

	template <typename T>
	std::list<T> ParallelQuickSorter<T>::DoSort(std::list<T>& chunkData)
	{
		if (chunkData.empty()) return chunkData;

		std::list<T> result;
		result.splice(result.begin(), chunkData, chunkData.begin());
		T const& patitionVal = *result.begin();

		auto dividePoint =
			std::partition(
				chunkData.begin(), chunkData.end(),
				[&](T const& value) { return value < patitionVal; }
		);

		ChunkToSort newLowerChunk{};
		newLowerChunk._data.splice(
			newLowerChunk._data.end(),
			chunkData,
			chunkData.begin(),
			dividePoint
		);

		std::future<std::list<T>> newLower
			= newLowerChunk._promise.get_future();
		_chuncks.Push(std::move(newLowerChunk));

		if (_threads.size() < _maxThreadCount)
		{
			_threads.push_back(std::thread(&ParallelQuickSorter<T>::SortThread, this));
		}

		std::list<T> newHigher(DoSort(chunkData));

		result.splice(result.end(), newHigher);
		while (newLower.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
		{
			TrySortChunk();
		}

		result.splice(result.begin(), newLower.get());
		return result;
	}

	template<typename T>
	template<typename U>
	void ParallelQuickSorter<T>::SortChunk(U chunk)
	{
		chunk->_promise.set_value(DoSort(chunk->_data));
	}

	template <typename T>
	void ParallelQuickSorter<T>::SortThread()
	{
		while (!_endOfData)
		{
			TrySortChunk();
			std::this_thread::yield();
		}
	}

	template<typename T>
	std::list<T> ParallelQuickSort(std::list<T> input)
	{
		return (input.empty()) ? input : ParallelQuickSorter<T>{}.DoSort(input);
	}
}
