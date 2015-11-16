//
// Created by mandriy on 11/15/15.
//

#ifndef BINARY_TREE_TREEGRAPH_H
#define BINARY_TREE_TREEGRAPH_H

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <string>
#include <vector>

typedef std::pair<int, int> Edge;

class TreeGraph {
private:
    friend class TreeGraphBuilder;
    typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::directedS,
            boost::property<boost::vertex_color_t, boost::default_color_type>,
            boost::property<boost::edge_weight_t, int>> LibGraph;


    TreeGraph(std::vector<std::string> vertexNames, std::vector<Edge> edges) : vertexNames(vertexNames) {
        int weights[edges.size()];
        std::fill(weights, weights + edges.size(), 1);
        g = LibGraph(begin(edges), end(edges), weights, 0);
    }

    LibGraph g;
    std::vector<std::string> vertexNames;

    class VertexLabelWriter {
    public:
        VertexLabelWriter(const TreeGraph *g) : g(g) { }
        void operator()(std::ostream &os, unsigned const int vertex) {
            os << "[label=\"" << g->vertexNames.at(vertex) << "\"]";
        }
    private:
        const TreeGraph *g;
    };

public:
    friend std::ostream &operator<<(std::ostream &os, const TreeGraph &graph) {
        boost::write_graphviz(os, graph.g, VertexLabelWriter(&graph));
        return os;
    }
};




#endif //BINARY_TREE_TREEGRAPH_H
