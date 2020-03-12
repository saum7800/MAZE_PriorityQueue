# MAZE_PriorityQueue
Maze solving algorithm implemented in C using priority queue. The goal is to find the path with the least cost and then the shortest
of the paths having that minimum cost. In each iteration, each current path is incremented by 1 move in all
possible directions. If there is an increase in the cost of the path, the path is moved to the next priority. Once we find a 
possible solution, we limit our updations to only the priorities above the priority of the solution path. Soon, this algorithm
converges to the minimum cost path.
