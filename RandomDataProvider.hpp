#pragma once
#include "IDataProvider.hpp"
#include <random>

namespace wv
{
	/*
	 * Осуществляет рандомную генерацию данных для сотировки
	 */
	template<typename T>
	class RandomDataProvider : public IDataProvider<T>
	{
	private:
		const size_t _count;
		const T _minValue;
		const T _maxValue;
	public:
		RandomDataProvider(size_t count, T minValue, T maxValue);
		std::list<T> GetData() const override;
		void PutData(const std::list<T> data) const override;
	};

	template <typename T>
	RandomDataProvider<T>::RandomDataProvider(size_t count, T minValue, T maxValue)
		: _count(count), _minValue(minValue), _maxValue(maxValue)
	{}

	template <typename T>
	std::list<T> RandomDataProvider<T>::GetData() const
	{
		auto randomEngine = std::mt19937(_count);
		auto distribution = std::uniform_int_distribution<T>(_minValue, _maxValue);
		std::list<T> data{};
		std::generate_n(std::back_inserter(data), _count, [&distribution, &randomEngine]()
		{
			return distribution(randomEngine);
		});
		return data;
	}

	template <typename T>
	void RandomDataProvider<T>::PutData(const std::list<T> data) const
	{
		for (const auto& d : data)
		{
			std::cout << d << " ";
		}
		std::cout << std::endl;
	}
}

