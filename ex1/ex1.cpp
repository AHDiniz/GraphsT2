#include <cstdlib>
#include <climits>
#include <iostream>
#include <functional>

#define INFINITY (INT_MAX - 1000)

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

	inline const bool empty() const { return count <= 0; }

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
	using vertex = int;
	using edge = int;

	Graph(int n, int m);
	~Graph();

	void set(int u, int v, int w);
	
	const int dijkstra(vertex start, vertex end) const;

private:
	struct EdgeData
	{
		vertex *start;
		vertex *end;
		int *weight;
	};
	EdgeData edges;

	struct VertexData
	{
		int *distance;
		vertex *previous;
	};
	VertexData vertices;

	int n, m, eCount;
};

int main()
{
	for (int i = 0; !std::cin.eof(); ++i)
	{
		std::cout << "Instancia " << i + 1 << std::endl;

		int n, m;

		std::cin >> n >> m;

		Graph graph(n, m);

		for (int i = 0; i < m; ++i)
		{
			int u, v, w;
			std::cin >> u >> v >> w;
			graph.set(u - 1, v - 1, w);
		}

		int d, k;
		std::cin >> d >> k;
		if (d < k)
		{
			std::cout << "impossivel" << std::endl << std::endl;
			continue;
		}

		std::cout << graph.dijkstra(0, n - 1) << std::endl << std::endl;
	}

	return 0;
}

Graph::Graph(int n, int m) : n(n), m(m), eCount(0)
{
	vertices.distance = (int *)malloc(sizeof(int) * n);
	for (int i = 0; i < n; ++i)
	{
		vertices.distance[i] = INFINITY;
		vertices.previous[i] = -1;
	}
	edges.start = (vertex *)malloc(sizeof(vertex) * m);
	edges.end = (vertex *)malloc(sizeof(vertex) * m);
	edges.weight = (int *)malloc(sizeof(int) * m);
}

Graph::~Graph()
{
	free(vertices.distance);
	free(vertices.previous);
	free(edges.start);
	free(edges.end);
	free(edges.weight);
}

void Graph::set(int u, int v, int w)
{
	edges.start[eCount] = u;
	edges.end[eCount] = v;
	edges.weight[eCount] = w;
	++eCount;
}

const int Graph::dijkstra(vertex start, vertex end) const
{
	vertices.distance[start] = 0;
	Heap<vertex> queue(n, [this](vertex v){
		return vertices.distance[v];
	});
	for (vertex i = 0; i < n; ++i) queue.add(i);
	while (!queue.empty())
	{
		vertex u = queue.pop();
		for (edge e = 0; e < m; ++e)
		{
			if (edges.start[e] == u || edges.end[e] == u)
			{
				edge v = (edges.start[e] == u) ? edges.end[e] : edges.start[e];
				int cost = vertices.distance[u] + edges.weight[e];
				vertices.distance[v] = (cost < vertices.distance[v]) ? cost : vertices.distance[v];
			}
		}
	}
	return vertices.distance[end];
}
