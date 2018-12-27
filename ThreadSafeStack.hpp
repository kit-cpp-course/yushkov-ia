#pragma once
#include <exception>
#include <stack>
#include <mutex>

namespace wv
{
	/*
	 * ����������, ��������� � ������, ���� ������������ ������ �������� ������ �� ������� �����
	 */
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
		/*
		 * ����������� �� ���������, ������� ������ ����
		 */
		ThreadSafeStack() = default;

		/*
		 * ����������� �����������
		 */
		ThreadSafeStack(const ThreadSafeStack& other);

		/*
		 * ���������
		 */
		~ThreadSafeStack() = default;

		/*
		 * �������� �����������
		 */
		ThreadSafeStack& operator = (const ThreadSafeStack&) = delete;

		/*
		 * ���������� ������ � ����
		 */
		void Push(T newValue);

		/*
		 * ������� ������ �� �����, ��������� ������������ ��������� �� ����
		 */
		std::shared_ptr<T> Pop();

		/*
		 * ������� ������ �� �����, ��������� ��� � ������, ��������������� �������������
		 */
		void Pop(T& value);

		/*
		 * ���������, ������ �� ����
		 */
		bool Empty() const;
	};

	template <typename T>
	ThreadSafeStack<T>::ThreadSafeStack(const ThreadSafeStack& other)
	{
		std::lock_guard<std::mutex> lock(other._mutex);
		_data = other._data;
	}

	template <typename T>
	ThreadSafeStack<T>::~ThreadSafeStack() = default;

	template <typename T>
	void ThreadSafeStack<T>::Push(T newValue)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_data.push(std::move(newValue));
	}

	template <typename T>
	std::shared_ptr<T> ThreadSafeStack<T>::Pop()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_data.empty()) return nullptr;
		std::shared_ptr<T> const result(std::make_shared<T>(std::move(_data.top())));
		_data.pop();
		return result;
	}

	template <typename T>
	void ThreadSafeStack<T>::Pop(T& value)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_data.empty()) throw EmptyStack();
		value = std::move(_data.top());
		_data.pop();
	}

	template <typename T>
	bool ThreadSafeStack<T>::Empty() const
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _data.empty();
	}
}
