/*
3. Города

Требуется отыскать самый выгодный маршрут между городами. 
Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
Формат входных данных.
Первая строка содержит число N – количество городов.
Вторая строка содержит число M - количество дорог.
Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Последняя строка содержит маршрут (откуда и куда нужно доехать).
Формат выходных данных.
Вывести длину самого выгодного маршрута.

*/


#include <iostream>
#include <vector>
#include <cassert>
#include <set>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
public:
    ListGraph(int count);

    virtual void AddEdge(int from, int to, int weight) override;
    virtual int VerticesCount() const override;
    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override;

private:
    std::vector<std::vector<std::pair<int, int>>> graph;
};

ListGraph::ListGraph(int count) {
    graph.resize(count);
}

void ListGraph::AddEdge(int from, int to, int weight) {
    assert(from >= 0 && from < int(graph.size()));
    assert(to >= 0 && to < int(graph.size()));
    graph[from].emplace_back(weight, to);
}

int ListGraph::VerticesCount() const {
    return graph.size();
}

std::vector<std::pair<int, int>> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < int(graph.size()));
    return graph[vertex];
}

int findShortestWay(const IGraph& graph, int start, int end) {
    std::vector<int> minDistances(graph.VerticesCount(), -1);
    minDistances[start] = 0;
    std::set<std::pair<int, int>> q;
    q.emplace(0, start);

    while (!q.empty()) {
        auto current = *q.begin();
        if (current.second == end) {
            break;
        }
        q.erase(q.begin());

        for (const auto& elem : graph.GetNextVertices(current.second)) {
            int newDist = current.first + elem.first;
            if (minDistances[elem.second] == -1 || newDist < minDistances[elem.second]) {
                if (minDistances[elem.second] != -1) {
                    q.erase({minDistances[elem.second], elem.second});
                }
                minDistances[elem.second] = newDist;
                q.emplace(newDist, elem.second);
            }
        }
    }
    return minDistances[end];
}

int main() {
    size_t numVertices = 0;
    size_t numEdges = 0;
    std::cin >> numVertices >> numEdges;

    assert(numVertices > 0 && numEdges > 0);

    IGraph* graph = new ListGraph(numVertices);
    for (size_t i = 0; i < numEdges; ++i) {
        int from = 0;
        int to = 0;
        int weight = 0;
        std::cin >> from >> to >> weight;
        assert(weight >= 0);
        graph->AddEdge(from, to, weight);
        graph->AddEdge(to, from, weight);
    }

    int startVertex, endVertex;
    std::cin >> startVertex >> endVertex;

    std::cout << findShortestWay(*graph, startVertex, endVertex);

    delete graph;
    return 0;
}
