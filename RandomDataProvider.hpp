#pragma once
#include "IDataProvider.hpp"
#include <random>

namespace wv
{
	template<typename T>
	class RandomDataProvider : public IDataProvider<T>
	{
	private:
		const size_t _count;
		const T _minValue;
		const T _maxValue;
	public:
		RandomDataProvider(size_t count, T minValue, T maxValue)
			: _count(count), _minValue(minValue), _maxValue(maxValue)
		{
		}

		std::list<T> GetData() const override
		{
			auto randomEngine = std::mt19937(_count);
			auto distribution = std::uniform_int_distribution<T>(_minValue, _maxValue);
			std::list<T> data{};
			std::generate_n(std::back_inserter(data), _count, [&distribution, &randomEngine]() { return distribution(randomEngine); });
			return data;
		}

		void PutData(const std::list<T> data) const override
		{
			for (const auto& d : data)
			{
				std::cout << d << " ";
			}
			std::cout << std::endl;
		}
	};
}

