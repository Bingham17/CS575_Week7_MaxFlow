# CS575_Week7_MaxFlow

Like Dijkstra and Prim, we'll have a simple file input format -- but this time EDGES ARE DIRECTED.  An edge from 0 to 1 might have a capacity (where we used length before), but THERE IS NOT an edge from 1 to 0, unless it's given in the input file.

Run the Edmonds-Karp algorithm, to find the maximum flow from vertex 0 to vertex (num_vertices - 1)

Here's a simple case -- three vertices, two edges. The first edge has a capacity of 7, the second has a capacity of 9. The maximum flow from vertex 0 to vertex 2 is 7.

3 2
0 1 7
1 2 9
The expected flow (going from vertex 0 to the highest numbered vertex, V-1) for test cases 1 thorugh 6 are 7, 200000, 2, 80, 10000, and 18
