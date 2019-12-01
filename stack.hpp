#pragma once

template<typename T>
class Stack
{
public:
	Stack(int n) : top(-1), max(n)
	{
		items = (T *)malloc(sizeof(T) * n);
	}
	
	~Stack()
	{
		free(items);
		items = nullptr;
		top = max = 0;
	}

	inline bool empty() const { return top == -1; }
	inline bool full() const { return top == max; }

	bool push(T item)
	{
		if (top + 1 >= max) return false;
		items[++top] = item;
		return true;
	}

	bool pop(T &item)
	{
		if (top - 1 <= -1) return false;
		item = items[top--];
		return true;
	}

private:
	T *items;
	int top, max;
};