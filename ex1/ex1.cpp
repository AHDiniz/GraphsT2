#include <iostream>
#include <cstdlib>
#include <cstring>
#include <climits>

#define INFINITY (INT_MAX / 2)
#define ERROR -1

template<typename T>
inline void exch(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

template<typename T>
class Heap
{
public:
	Heap(int n) : count(0), max(n)
	{
		data = (T *)malloc(sizeof(T) * n);
		priority = (int *)malloc(sizeof(int) * n);
		memset(&data[0], 0, sizeof(T));
	}

	~Heap()
	{
		free(data);
		free(priority);
		count = max = 0;
		data = priority = nullptr;
	}

	bool push(T item, int priority)
	{
		int top = ++count;
		if (top > max)
		{
			--count;
			return false;
		}
		data[top] = item;
		this->priority[top] = priority;
		FixUp(top);
		return true;
	}

	T pop(void)
	{
		--count;
		if (count < 0)
		{
			count++;
			return data[0];
		}
		T first = data[1];
		exch(data[1], data[count + 1]);
		exch(priority[1], priority[count + 1]);
		FixDown(1);
		return first;
	}

	int get(T item)
	{
		for (int i = 0; i < count; ++i)
			if (data[i] == item) return priority[i];
		return -1;
	}

	void reset(T item, int priority)
	{
		for (int i = 0; i < count; ++i)
		{
			if (data[i] == item)
			{
				int prevPrio = this->priority[i];
				if (prevPrio > priority) FixDown(i);
				else FixUp(i);
				break;
			}
		}
	}

	inline bool empty() const { return count == 0; }

	bool contains(T item)
	{
		for (int i = 0; i < count; ++i)
			if (data[i] == item) return true;
		return false;
	}

private:
	void FixUp(int pos)
	{
		while (pos > 2 && priority[pos / 2] > priority[pos])
		{
			exch(data[pos / 2], data[pos]);
			exch(priority[pos / 2], priority[pos]);
			pos /= 2;
		}
	}

	void FixDown(int pos)
	{
		while (2 * pos < count)
		{
			int j = 2 * pos;
			if (j < count && priority[j] > priority[j + 1]) ++j;
			if (priority[pos] <= priority[j]) break;
			exch(data[pos], data[j]);
			exch(priority[pos], priority[j]);
			pos = j;
		}
	}

	T *data;
	int *priority;
	int count, max;
};

class Graph
{
public:
	Graph(int n, int m);
	~Graph();

	inline void set(int u, int v, int w) { adj[u * n + v] = w; }
	int get(int u, int v) const { return adj[u * n + v]; }

	int *mst(void);
	int maxFlow(int resources, int flow);

private:
	int *adj;
	int n, m;
};

int main()
{
	for (int instance = 1; !std::cin.eof(); ++instance)
	{
		std::cout << "Instancia " << instance << std::endl;

		int n, m;
		std::cin >> n >> m;

		Graph graph(n, m);

		for (int edge = 0; edge < m; ++edge)
		{
			int u, v, w;
			std::cin >> u >> v >> w;
			graph.set(u - 1, v - 1, w);
		}

		int resources, flow;
		std::cin >> resources >> flow;

		int result = graph.maxFlow(resources, flow);
		if (result == ERROR) std::cout << "impossivel" << std::endl << std::endl;
		else std::cout << result << std::endl << std::endl;
	}

	return 0;
}

Graph::Graph(int n, int m) : n(n), m(m)
{
	adj = (int *)malloc(sizeof(int) * n * n);
	memset(adj, 0, sizeof(int) * n * n);
}

Graph::~Graph()
{
	free(adj);
	n = m = 0;
	adj = nullptr;
}

int *Graph::mst(void)
{
	int *previous = (int *)malloc(sizeof(int) * n);
	for (int i = 0; i < n; ++i) previous[i] = ERROR;

	Heap<int> queue(n);
	for (int i = 0; i < n; ++i) queue.push(i, INFINITY);

	while (!queue.empty())
	{
		int u = queue.pop();

		for (int i = 0; i < n; ++i)
		{
			if (get(u, i) && queue.contains(i) && get(u, i) < queue.get(i))
			{
				previous[i] = u;
				std::cout << "(" << previous[i] << ", " << i << ")" << std::endl;
				queue.reset(i, get(u, i));
			}
		}
	}

	return previous;
}

int Graph::maxFlow(int resources, int flow)
{
	int result = 0;

	int *previous = mst();
	if (previous[n - 1] == ERROR) return ERROR;

	if (resources < flow)
	{
		for (int i = n - 1; i > 0; i = previous[i])
			result += get(previous[i], i);
	}
	else
	{
		while (resources > 0 && previous[n - 1] != ERROR)
		{
			for (int i = n - 1; i > 0; i = previous[i])
			{
				result += get(previous[i], i);
				set(previous[i], i, 0);
			}
			free(previous);
			previous = mst();
		}
		if (resources > 0) resources = ERROR;
	}
	
	free(previous);
	return result;
}
