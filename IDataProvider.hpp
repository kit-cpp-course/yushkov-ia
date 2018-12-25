#pragma once
#include <list>
#include <iostream>

namespace wv
{
	template<typename T>
	class IDataProvider
	{
	public:
		virtual ~IDataProvider() = default;
		virtual std::list<T> GetData() const = 0;
		virtual void PutData(const std::list<T> data) const = 0;
	};
}
