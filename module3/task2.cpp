#include <iostream>
#include <vector>
#include <queue>
#include <cassert>

/*
Задача 2. Количество различных путей (3 балла)

Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами. Найдите количество различных кратчайших путей между заданными вершинами.
Требования: сложность O(V+E).
Формат ввода.
v: кол-во вершин (макс. 50000),
n: кол-во ребер (макс. 200000),
n пар реберных вершин,
пара вершин u, w для запроса.
Формат вывода.
Количество кратчайших путей от u к w.

 */

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
public:
    explicit ListGraph(size_t size);
    explicit ListGraph(const IGraph& other);

    ~ListGraph();

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    std::vector<std::vector<int>> graph;
};

ListGraph::ListGraph(size_t size) : graph(size) {}


ListGraph::ListGraph(const IGraph &other)
{
    for (int i = 0; i < other.VerticesCount(); ++i)
    {
        graph[i] = other.GetNextVertices(i);
    }
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < graph.size());
    assert(0 <= to && to < graph.size());
    graph[from].push_back(to);
}

int ListGraph::VerticesCount() const { return (int)graph.size(); }

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < graph.size());
    return graph[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const
{
    assert(0 <= vertex && vertex < graph.size());
    std::vector<int> prevVertices;

    for (int from = 0; from < graph.size(); ++from)
    {
        for (int to: graph[from])
        {
            if (to == vertex)
                prevVertices.push_back(from);
        }
    }
    return prevVertices;
}



int findShortestWay(IGraph& graph, int from, int to) {
    std::vector<int> pathCount(graph.VerticesCount(), 0);
    pathCount[from] = 1;
    std::vector<int> distance(graph.VerticesCount(), 0);

    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> q;
    int vertex = from;
    visited[vertex] = true;
    q.push(vertex);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        std::vector<int> children = graph.GetNextVertices(v);
        for (auto& next : children) {
            if ((distance[next] == 0) || (distance[next] > distance[v] + 1)) {
                distance[next] = distance[v] + 1;
                pathCount[next] = pathCount[v];
            }
            else if (distance[next] == distance[v] + 1) {
                pathCount[next] += pathCount[v];
            }
            if (!visited[next]) {
                visited[next] = true;
                q.push(next);
            }
        }
    }
    return pathCount[to];
}



int main() {
    int v, n;
    std::cin >> v >> n;
    IGraph* graph = new ListGraph(v);
    int u, w;
    for (int i = 0; i < n; ++i) {
        std::cin >> u >> w;
        graph->AddEdge(u, w);
        graph->AddEdge(w, u);
    }

    std::cin >> u >> w;

    std::cout << findShortestWay(*graph, u, w);

    delete graph;
    return 0;
}
