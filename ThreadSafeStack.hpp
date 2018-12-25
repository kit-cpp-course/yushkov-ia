#pragma once
#include <exception>
#include <stack>
#include <mutex>

namespace wv
{
	struct EmptyStack : std::exception
	{
		const char * what() const override
		{
			return "Stack is empty";
		}
	};

	template<typename T>
	class ThreadSafeStack
	{
	private:
		std::stack<T> _data;
		mutable std::mutex _mutex;

	public:
		ThreadSafeStack() : _data{} {}

		ThreadSafeStack(const ThreadSafeStack& other)
		{
			std::lock_guard<std::mutex> lock(other._mutex);
			_data = other._data;
		}

		~ThreadSafeStack() = default;

		ThreadSafeStack& operator = (const ThreadSafeStack&) = delete;

		void Push(T newValue)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_data.push(std::move(newValue));
		}

		std::shared_ptr<T> Pop()
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_data.empty()) return nullptr;
			std::shared_ptr<T> const result(std::make_shared<T>(std::move(_data.top())));
			_data.pop();
			return result;
		}

		void Pop(T& value)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_data.empty()) throw EmptyStack();
			value = std::move(_data.top());
			_data.pop();
		}

		bool Empty() const
		{
			std::lock_guard<std::mutex> lock(_mutex);
			return _data.empty();
		}
	};
}
