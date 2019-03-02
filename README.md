# SalesmanProblem
Attempt to solve NP-problem using annealing algorithm 
- use class constructor to set initial temperature and trmperature delta
- class returns result as a distance number and string of whole path
- probability of accepting worse solution is exp(-(current - working) / temperature)
- input graph is moved instead of copying
- use std::vector<std::vector<std::pair<int, int> > > to set a graph,
where graph[i] are paths from node i, graph[i][j].first - node from the other side of path, graph[i][j].second - distance between cities
- salesman should start from city with index 0 and return to that city at the end
