//
// Created by mandriy on 11/15/15.
//

#ifndef BINARY_TREE_TREEGRAPHBUILDER_H
#define BINARY_TREE_TREEGRAPHBUILDER_H

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <sstream>

#include "TreeGraph.h"
#include "Tree.h"

class TreeGraphBuilder {
public:
    template <typename TreeElement>
    TreeGraphBuilder(Tree<TreeElement> tree);

    TreeGraph getGraph() {
        return std::move(TreeGraph(vertexLabels, edges));
    }
private:

    template<typename TreeElement>
    void treeTraverse(typename Tree<TreeElement>::NodePtr& iter_parent,
                      typename Tree<TreeElement>::NodePtr& iter,
                      std::function<void(typename Tree<TreeElement>::NodePtr&,
                                             typename Tree<TreeElement>::NodePtr&)> f);

    template<typename TreeElement>
    void buildEdge(typename Tree<TreeElement>::NodePtr& parent,
                   typename Tree<TreeElement>::NodePtr& child);

    template<typename TreeElement>
    void buildRoot(typename Tree<TreeElement>::NodePtr& node);

    template<typename TreeElement>
    void buildNullNode(typename Tree<TreeElement>::NodePtr& parent);

    template<typename TreeElement>
    void buildOrdinaryEdge(typename Tree<TreeElement>::NodePtr& parent,
                           typename Tree<TreeElement>::NodePtr& child);

    template<typename TreeElement>
    std::string convertNodeToString(const typename  Tree<TreeElement>::NodePtr& node);

    int node_counter;

    std::map<void*, int> node_id_map;
    std::vector<Edge> edges;
    std::vector<std::string> vertexLabels;
};

template<typename TreeElement>
std::string TreeGraphBuilder::convertNodeToString(const typename Tree<TreeElement>::NodePtr& node) {
    std::ostringstream o;
    o << node->getValue();
    return o.str();
}

template<typename TreeElement>
void TreeGraphBuilder::buildOrdinaryEdge(typename Tree<TreeElement>::NodePtr& parent,
                                         typename Tree<TreeElement>::NodePtr& child) {
    node_id_map[(void*)child.get()] = node_counter++;
    vertexLabels.push_back(convertNodeToString<TreeElement>(child));
    edges.push_back(Edge(node_id_map[(void*)parent.get()], node_id_map[(void*)child.get()]));
}

template<typename TreeElement>
void TreeGraphBuilder::buildNullNode(typename Tree<TreeElement>::NodePtr& parent) {
    edges.push_back(Edge(node_id_map[(void*)parent.get()], node_counter++));
    vertexLabels.push_back("NULL");
}

template<typename TreeElement>
void TreeGraphBuilder::buildRoot(typename Tree<TreeElement>::NodePtr& node) {
    if ( node != nullptr ) {
        node_id_map[(void*)node.get()] = node_counter++;
        vertexLabels.push_back(convertNodeToString<TreeElement>(node));
    }
}

template<typename TreeElement>
void TreeGraphBuilder::buildEdge(typename Tree<TreeElement>::NodePtr& parent,
                                 typename Tree<TreeElement>::NodePtr& child) {
    if ( parent != nullptr ) {
        if ( child != nullptr ) {
            buildOrdinaryEdge<TreeElement>(parent, child);
        } else {
            buildNullNode<TreeElement>(parent);
        }
    } else {
        buildRoot<TreeElement>(child);
    }
}

template<typename TreeElement>
void TreeGraphBuilder::treeTraverse(typename Tree<TreeElement>::NodePtr& iter_parent,
                                    typename Tree<TreeElement>::NodePtr& iter,
                                    std::function<void(typename Tree<TreeElement>::NodePtr&,
                                                       typename Tree<TreeElement>::NodePtr&)> f) {
    f(iter_parent, iter);
    if ( iter != nullptr ) {
        treeTraverse<TreeElement>(iter, iter->getLeft(), f);
        treeTraverse<TreeElement>(iter, iter->getRight(), f);
    }
}

template<typename TreeElement>
TreeGraphBuilder::TreeGraphBuilder(Tree<TreeElement> tree) : node_counter(0) {
    typename Tree<TreeElement>::NodePtr fake_parent;
    treeTraverse<TreeElement>(fake_parent, tree.root,
                              [&](typename Tree<TreeElement>::NodePtr& iter_parent,
                                  typename Tree<TreeElement>::NodePtr& iter) {
                                  buildEdge<TreeElement>(iter_parent, iter);
                              });
}


#endif //BINARY_TREE_TREEGRAPHBUILDER_H
