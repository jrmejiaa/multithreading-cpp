#include <chrono>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

using vertex_type = size_t;
using distance_type = unsigned int;

struct VertexDistancePair {
	vertex_type vertex_index;
	distance_type distance;
};

template <>
struct std::greater<VertexDistancePair> {
	bool operator()(const VertexDistancePair& lhs, const VertexDistancePair& rhs) const {
		return lhs.distance > rhs.distance;
	}
};

struct DistancesAndFurthestVertex {
	std::vector<distance_type> all_pairs_shortest_paths;
	std::vector<VertexDistancePair> furthest_reaching_vertex;
};

/**
 * @brief Reads the connectivity in the specified path. The file format must be:
 *      number_vertices : size_t
 *      (source_index : size_t, target_index : size_t, weight : unsigned int)*
 *      and no index must be larger or equal to the number of vertices
 * @param path The path to the file that specifies the connectivity
 * @return The connectivity as an adjacency list, i.e.,
 *      for vertices i, j, and the weight of their edge k,
 *      <return>[i][j] = k
 *      if <return>[i] does not contain j, then k == 0
 */
std::vector<std::map<vertex_type, distance_type>> read_connectivity(const std::filesystem::path& path) {
	std::ifstream file(path);
	std::string line{};

	std::getline(file, line);

	std::stringstream sstream_number_vertices(line);

	vertex_type number_vertices = 0;
	sstream_number_vertices >> number_vertices;

	std::vector<std::map<vertex_type, distance_type>> connectivity(number_vertices);

	while (std::getline(file, line)) {
		std::stringstream sstream_connectivity(line);

		vertex_type source_idx = 0;
		vertex_type target_idx = 0;
		distance_type weight = 0;

		const auto success =
			(sstream_connectivity >> source_idx) &&
			(sstream_connectivity >> target_idx) &&
			(sstream_connectivity >> weight);

		if (!success) {
			continue;
		}

		if (source_idx >= number_vertices || target_idx >= number_vertices) {
			continue;
		}

		connectivity[source_idx][target_idx] += weight;
	}

	return connectivity;
}

/**
 * @brief Calculated the shortest paths from the specified source vertex using the connectivity
 * @param connectivity The adjacency list of the graph
 * @param source_vertex_id The index of the source vertex from which the shortest path to all vertices should be calculated
 * @return The shortest paths, i.e.,
 *		for all vertices i, the shortest path source_vertex_id--->i has the distance k,
 *		<return>[i] = k
 */
std::vector<distance_type> dijkstra_shortest_paths(const std::vector<std::map<vertex_type, distance_type>>& connectivity, vertex_type source_vertex_id) {
	const auto number_vertices = connectivity.size();
	std::vector<distance_type> distances(number_vertices, std::numeric_limits<distance_type>::max());

	std::priority_queue<VertexDistancePair, std::vector<VertexDistancePair>, std::greater<VertexDistancePair>> shortest_paths_queue{};

	distances[source_vertex_id] = 0;
	shortest_paths_queue.emplace(source_vertex_id, 0);

	while (!shortest_paths_queue.empty()) {
		const auto current_distance = shortest_paths_queue.top().distance;
		const auto current_vertex_id = shortest_paths_queue.top().vertex_index;

		shortest_paths_queue.pop();

		for (const auto& [vertex_id, edge_weight] : connectivity[current_vertex_id]) {
			const auto new_distance = current_distance + edge_weight;
			if (new_distance < distances[vertex_id]) {
				distances[vertex_id] = new_distance;
				shortest_paths_queue.emplace(vertex_id, new_distance);
			}
		}
	}

	return distances;
}

/**
 * @brief Calculates the all-pairs shortest-paths algorithm for the given adjacency list
 * @param connectivity The adjacency list for the graph
 * @return For all pairs for vertices i, j, the shortest path between them, i.e.,
 *		<return>[i * number_vertices + j] = k
 *		indicates that the shortest path i--->j has distance k
 */
std::vector<distance_type> all_pairs_shortest_paths(const std::vector<std::map<vertex_type, distance_type>>& connectivity) {
	const auto number_vertices = connectivity.size();

	std::vector<distance_type> all_distances(number_vertices * number_vertices, std::numeric_limits<distance_type>::max());
	for (vertex_type source_vertex_id = 0; source_vertex_id < number_vertices; source_vertex_id++) {
		const auto& local_distances = dijkstra_shortest_paths(connectivity, source_vertex_id);
		const auto offset = source_vertex_id * number_vertices;

		for (vertex_type target_vertex_id = 0; target_vertex_id < number_vertices; target_vertex_id++) {
			const auto current_distance = local_distances[target_vertex_id];
			all_distances[offset + target_vertex_id] = current_distance;
		}
	}

	return all_distances;
}

/**
 * @brief Calculates for each vertex, the largest smallest-path from another vertex to the first one
 * @param all_distance The all-pairs shortest-paths distance matrix
 * @param number_vertices The number of vertices
 * @return <return>[i] = (j, k) indicates that
 *		from all shortest paths to i, j has the longest, and its distance is k
 */
std::vector<VertexDistancePair> calculate_largest_smallest_path(const std::vector<distance_type>& all_distance, vertex_type number_vertices) {
	std::vector<VertexDistancePair> furthest_reaching_vertex(number_vertices);

	for (vertex_type vertex_id = 0; vertex_id < number_vertices; vertex_id++) {
		furthest_reaching_vertex[vertex_id] = { vertex_id, 0 };
	}

	for (vertex_type source_vertex_id = 0; source_vertex_id < number_vertices; source_vertex_id++) {
		const auto offset = source_vertex_id * number_vertices;

		for (vertex_type target_vertex_id = 0; target_vertex_id < number_vertices; target_vertex_id++) {
			const auto current_distance = all_distance[offset + target_vertex_id];

			const auto current_furthest_distance = furthest_reaching_vertex[target_vertex_id].distance;
			if (current_distance > current_furthest_distance) {
				furthest_reaching_vertex[target_vertex_id] = { source_vertex_id, current_distance };
			}
		}
	}

	return furthest_reaching_vertex;
}

/**
 * @brief Calculated all pairs shortest paths by repeatedly applying Dijkstra's algorithm,
 *		and also which nodes have the largest shortest-path to each other node
 * @param connectivity The adjacency list of the graph
 * @return
 *		(1)	The shortest paths between all vertices, i.e.,
 *			for vertices i, j, the shortest path i--->j has the distance k,
 *			<return.all_pairs_shortest_paths>[i * offset + j] = k
 *			where offset is the number of vertices in the graph
 *		(2) The index and distance of the node, that has the
 *			longest shortest-distance to the other node, i.e.,
 *			<return.furthest_reaching_vertex>[i] = (j, w)
 *			expresses that the shortest path j--->i has distance w
 *			and for all other vertices k != j with shortest path k--->i with distance w'
 *			w' < w
 */
DistancesAndFurthestVertex do_work_serial(const std::vector<std::map<vertex_type, distance_type>>& connectivity) {
	const auto number_vertices = connectivity.size();

	const auto& all_distances = all_pairs_shortest_paths(connectivity);
	const auto& furthest_reaching_vertices = calculate_largest_smallest_path(all_distances, number_vertices);

	return { all_distances, furthest_reaching_vertices };
}

void measure_execution_time(const std::vector<std::map<vertex_type, distance_type>>& connectivity,
	std::function< DistancesAndFurthestVertex(const std::vector<std::map<vertex_type, distance_type>>&)> function) {

	const auto before_calculation = std::chrono::high_resolution_clock::now();
	const auto& [distances, furthest_points] = function(connectivity);
	const auto after_calculation = std::chrono::high_resolution_clock::now();

	std::map<distance_type, vertex_type> histogram{};

	for (const auto& [vertex_index, distance] : furthest_points) {
		histogram[distance]++;
	}

	const auto time = (after_calculation - before_calculation).count();

	std::cout << "The calculation took: " << time << " ns.\n";

	for (const auto& [distance, number_vertices] : histogram) {
		std::cout << "There are " << number_vertices << " vertices that have a largest smallest-path to them of: " << distance << '\n';
	}
}

int main() {
	std::vector<std::map<vertex_type, distance_type>> manual_connectivity =
	{
		{ {1, 3}, {2, 8}},
		{ {0, 2}, {2, 1}},
		{ {0, 2}, {1, 3}}
	};

	std::vector<std::map<vertex_type, distance_type>> file_connectivity =
		read_connectivity("./graph.txt");

	measure_execution_time(file_connectivity, do_work_serial);

	std::cout << std::flush;

	return 0;
}
