#include <iostream>
#include <vector>
#include <stack>

class Graph
{
public:
    Graph(int vertices) : V(vertices)
    {
        adj.resize(V);
    }

    void addEdge(int v, int w)
    {
        adj[v].push_back(w);
    }

    bool hasCycle()
    {
        std::vector<bool> visited(V, false);
        std::vector<bool> recStack(V, false);

        for (int i = 0; i < V; ++i)
        {
            if (!visited[i] && isCyclicUtil(i, visited, recStack))
            {
                return true;
            }
        }

        return false;
    }

private:
    int V;
    std::vector<std::vector<int>> adj;

    bool isCyclicUtil(int v, std::vector<bool> &visited, std::vector<bool> &recStack)
    {
        visited[v] = true;
        recStack[v] = true;

        for (const auto &neighbor : adj[v])
        {
            if (!visited[neighbor] && isCyclicUtil(neighbor, visited, recStack))
            {
                return true;
            }
            else if (recStack[neighbor])
            {
                return true;
            }
        }

        recStack[v] = false;
        return false;
    }
};

int main()
{
    // 构建有向图
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);

    // 判断是否存在回路
    if (g.hasCycle())
    {
        std::cout << "The graph contains a cycle." << std::endl;
    }
    else
    {
        std::cout << "The graph does not contain a cycle." << std::endl;
    }

    return 0;
}
