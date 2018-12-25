#pragma once
#include <future>
#include <list>
#include "ThreadSafeStack.hpp"

namespace wv
{
	template<typename T>
	struct ParallelQuickSorter
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

		ParallelQuickSorter()
			: _maxThreadCount(std::thread::hardware_concurrency() - 1),
				_endOfData(false)
		{}

		~ParallelQuickSorter()
		{
			_endOfData = true;
			for(auto& thread : _threads)
			{
				thread.join();
			}
		}

		void TrySortChunk()
		{
			auto chunk = _chuncks.Pop();
			if(chunk)
			{
				SortChunk(chunk);
			}
		}

		std::list<T> DoSort(std::list<T>& chunkData)
		{
			if(chunkData.empty())
			{
				return chunkData;
			}

			std::list<T> result;
			result.splice(result.begin(), chunkData, chunkData.begin());
			T const& patitionVal = *result.begin();

			auto dividePoint =
				std::partition(
					chunkData.begin(), chunkData.end(),
					[&](T const& value) {return value < patitionVal; }
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

			if(_threads.size() < _maxThreadCount)
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
		void SortChunk(T chunk)
		{
			chunk->_promise.set_value(DoSort(chunk->_data));
		}

		void SortThread()
		{
			while(!_endOfData)
			{
				TrySortChunk();
				std::this_thread::yield();
			}
		}
	};

	template<typename T>
	std::list<T> ParallelQuikSort(std::list<T> input)
	{
		if(input.empty())
		{
			return input;
		}
		ParallelQuickSorter<T> s;

		return s.DoSort(input);
	}
}
