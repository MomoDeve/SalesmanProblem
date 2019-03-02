#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <cstdlib>
#include <ctime>
#include <string>

class IAnnealingSimulation
{
protected:
	float T, deltaT;

	virtual long long iteration() = 0;

	virtual void beginIterations() = 0;

	virtual double calculateProbability(long long working, long long current) = 0;

public:

	virtual void start() = 0;
};

class SalesmanProblem : public IAnnealingSimulation
{
	using Graph = std::vector<std::vector<std::pair<int, int> > >;
private:
	Graph graph;
	std::set<int>used;
	std::vector<int> path;

	std::vector<int> workingSolution;
	long long workingResult;

	std::vector<int> bestSolution;
	long long bestResult;

	int random(int upperBound)
	{
		if(upperBound < 2) return 0;
		else return rand() % upperBound;
	}
protected:
	virtual long long dfs(int node, long long dist = 0)
	{
		used.insert(node);
		if (used.size() == graph.size() && node == 0) return dist;
		int dir = random(graph[node].size());
		path.push_back(dir);
		return dfs(graph[node][dir].first, dist + graph[node][dir].second);
	}

	virtual long long iteration() override
	{
		long long dist = 0;
		int slice = random(path.size() - 1);
		path.erase(path.begin() + slice, path.end());
		int cur = 0;
		for (int i : path)
		{
			dist += graph[cur][i].second; //????
			used.insert(cur);
			cur = graph[cur][i].first;
		}
		dist += dfs(cur);
		return dist;
	}

	virtual void beginIterations() override
	{
		while (T > 1.0)
		{
			long long current = iteration();
			int probability = int(100.0 * calculateProbability(workingResult, current));
			int rnd = random(101);
			if (rnd <= probability)
			{
				workingResult = current;
				workingSolution = path;
				if (workingResult <= bestResult)
				{
					bestResult = workingResult;
					bestSolution = workingSolution;
				}
			}
			else
			{
				path = workingSolution;
			}
			T *= deltaT;
			used.clear();
		}
	}

	virtual double calculateProbability(long long working, long long current) override
	{
		if (current < working) return 1.0;
		else return exp(-(current - working / T));
	}
public:
	SalesmanProblem(double T, double deltaT, Graph& graph)
	{
		this->T = T;
		this->deltaT = deltaT;
		this->graph = std::move(graph);
	}

	virtual void start() override
	{
		//srand(time(NULL));
		workingSolution.reserve(2 * graph.size());
		bestResult = workingResult = dfs(0);
		bestSolution = workingSolution = path;
		used.clear();
		beginIterations();
	}

	virtual std::pair<long long, std::string> getResult()
	{
		std::string solutionString = "0 ";
		int cur = 0;
		for (int i : bestSolution)
		{
			cur = graph[cur][i].first;
			solutionString += std::to_string(cur) + " ";
		}
		return { bestResult, solutionString };
	}
};

int main() {
	using Graph = std::vector<std::vector<std::pair<int, int> > >;
	int N, M, v, u, dist;
	std::cin >> N >> M;
	Graph g(N);
	for (int i = 0; i < M; i++)
	{
		std::cin >> v >> u >> dist;
		g[v].push_back({ u, dist });
		g[u].push_back({ v, dist });
	}
	SalesmanProblem NP(1000.0, 0.99, g);
	NP.start();
	auto res = NP.getResult();
	std::cout << res.first << std::endl << res.second << std::endl;
	return 0;
	return 0;
}



