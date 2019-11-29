#include <iostream>
#include <cstdlib>
#include <functional>

template<typename T>
inline void exch(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}

class UnionFind
{
public:
	UnionFind(int n);
	~UnionFind();

	const int find(int a) const;
	void connect(int a, int b);

private:
	int *items;
	int *sizes;
	int count;
};

template<typename T>
class Heap
{
public:
	using PriorityFnc = std::function<const int(T)>;
	Heap(const int n, PriorityFnc priority) : count(0), max(n + 1), priority(priority)
	{
		items = (T *)malloc(sizeof(T) * (n + 1));
	}

	~Heap()
	{
		free(items);
		count = max = 0;
		priority = nullptr;
		items = nullptr;
	}

	bool add(T item)
	{
		int top = ++count;
		if (top > max)
		{
			--count;
			return false;
		}
		items[top] = item;
		FixUp(top);
		return true;
	}

	T pop(void)
	{
		--count;
		if (count < 0)
		{
			count++;
			return items[0];
		}
		T first = items[1];
		exch(items[1], items[count + 1]);
		FixDown(1);
		return first;
	}

private:
	void FixUp(int pos)
	{
		while (pos > 1 && priority(items[pos / 2]) > priority(items[pos]))
		{
			exch(items[pos / 2], items[pos]);
			pos /= 2;
		}
	}
	
	void FixDown(int pos)
	{
		while (2 * pos < count)
		{
			int j = 2 * pos;
			if (j < count && priority(items[j]) > priority(items[j + 1])) ++j;
			if (priority(items[pos]) <= priority(items[j])) break;
			exch(items[pos], items[j]);
			pos = j;
		}
	}

	PriorityFnc priority;
	int count, max;
	T *items;
};

class Graph
{
public:
	using edge = int;

	Graph(int n, int m);
	~Graph();

	edge set(int u, int v, int w);

	inline const int start(edge e) const { return data.start[e]; }
	inline const int end(edge e) const { return data.end[e]; }
	inline const int weight(edge e) const { return data.weight[e]; }

	const edge *mst(void) const;

private:
	struct EdgeData
	{
		int *start;
		int *end;
		int *weight;
	};
	EdgeData data;

	int n, m, count;
};

int main(int argc, char *argv[])
{
	int n, m;

	std::cin >> n >> m;

	Graph graph(n, m);

	for (int i = 0; i < m; ++i)
	{
		int u, v, w;
		std::cin >> u >> v >> w;
		graph.set(u, v, w);
	}

	const Graph::edge *mst = graph.mst();

	int result = 0;
	for (int i = 0; i < (n - 1); ++i)
	{
		result += graph.weight(mst[i]);
	}

	std::cout << result << std::endl;

	return 0;
}

UnionFind::UnionFind(int n) : count(n)
{
	items = (int *)malloc(sizeof(int) * n);
	sizes = (int *)malloc(sizeof(int) * n);
	for (int i = 0; i < count; ++i)
	{
		items[i] = i;
		sizes[i] = 1;
	}
}

UnionFind::~UnionFind()
{
	free(items);
	items = nullptr;
	count = 0;
}

const int UnionFind::find(int a) const
{
	while (a != items[a]) a = items[a];
	return a;
}

void UnionFind::connect(int a, int b)
{
	int i = find(a);
	int j = find(b);

	if (i == j) return;

	if (sizes[i] < sizes[j])
	{
		items[i] = j;
		sizes[j] += sizes[i];
	}
	else
	{
		items[j] = i;
		sizes[i] += sizes[j];
	}
}

Graph::Graph(int n, int m) : n(n), m(m), count(0)
{
	data.start = (int *)malloc(sizeof(int) * m);
	data.end = (int *)malloc(sizeof(int) * m);
	data.weight = (int *)malloc(sizeof(int) * m);
}

Graph::~Graph()
{
	free(data.start);
	free(data.end);
	free(data.weight);
	data.end = data.start = data.weight = nullptr;
	n = m = 0;
}

Graph::edge Graph::set(int u, int v, int w)
{
	if (count >= m) return -1;
	data.start[count] = u;
	data.end[count] = v;
	data.weight[count] = w;
	return count++;
}

const Graph::edge *Graph::mst(void) const
{
	edge *result = (edge *)malloc(sizeof(edge) * (n - 1));
	Heap<edge> heap(m, [this](edge e)
	{
		return weight(e);
	});
	UnionFind uf(n);
	for (int i = 0; i < m; ++i) heap.add(i);
	int count = 0;
	for (int i = 0; count < (n - 1); ++i)
	{
		edge e = heap.pop();
		if (uf.find(start(e)) != uf.find(end(e)))
		{
			result[count] = e;
			++count;
			uf.connect(start(e), end(e));
		}
	}
	return result;
}
