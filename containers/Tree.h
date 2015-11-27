//
// Created by mandriy on 10/14/15.
//

#ifndef BINARY_TREE_TREE_H
#define BINARY_TREE_TREE_H

#include <memory>
#include <functional>

template<typename Element>
class Tree {
public:
    typedef std::function<void(Element &)> ElementsTraverseFunc;
    typedef std::function<bool(const Element &)> ElementPredicate;

    friend class TreeGraphBuilder;

#define NEGATIVE_PREDICATE [](const Element &) -> bool { return false; }

    Tree() : number_of_elements(0), root(nullptr) { }
    void insert(const Element &el);
    bool isMember(const Element &el) const;
    unsigned int removeAll(const Element &el);
    unsigned int removeAll(ElementPredicate);
    unsigned int remove(const Element &el, unsigned int count = 1);
    unsigned int countElements(const Element &el) const;
    unsigned int countElements(ElementPredicate) const;
    unsigned int size() const {
        return number_of_elements;
    }
    void clear() {
        number_of_elements = 0;
        root = nullptr;
    }

    Tree makeElementsSubtree(ElementPredicate filterFunc) const;
    Tree getSubtreeFromElement(const Element &) const;
    Tree getSubtreeFromElement(ElementPredicate) const;

    void preLeftTraverse(ElementsTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;
    void postLeftTraverse(ElementsTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;
    void preRightTraverse(ElementsTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;
    void postRightTraverse(ElementsTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;
    void inOrderTraverse(ElementsTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;
    void inOppositeOrderTraverse(ElementsTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;

private:

    class Node;
    class ConditionWrapper;

    typedef std::shared_ptr<Node> NodePtr;
    typedef std::function<void(NodePtr &)> NodesTraverseFunc;

    Tree(NodePtr root) : root(root), number_of_elements(0) {
        inOrderNodesTraverse([&](const NodePtr &node) {
            number_of_elements++;
        });
    }

    void insertNode(NodePtr parent_node, NodePtr node_to_insert);

    NodePtr findParentForNodeInsertion(const NodePtr& starting_node, const NodePtr& node_for_insertion) const;
    NodePtr findElement(ElementPredicate) const;
    NodePtr findElement(const Element &value) const;
    NodePtr iterStepByValue(const Element &node_value, NodePtr current_iter_pos) const;

    void removeRoot();
    void removeRightNode(NodePtr node_to_remove, NodePtr parent_node);
    void removeLeftNode(NodePtr node_to_remove, NodePtr parent_node);
    void removeNode(NodePtr node_to_remove, NodePtr parent_node);
    void traverseWithParent(NodePtr& iter,
                            std::function<void(NodePtr&, NodePtr&)>);

    void preLeftNodesTraverse(NodesTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;
    void postLeftNodesTraverse(NodesTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;
    void preRightNodesTraverse(NodesTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;
    void postRightNodesTraverse(NodesTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;
    void inOrderNodesTraverse(NodesTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;
    void inOppositeOrderNodesTraverse(NodesTraverseFunc, ElementPredicate=NEGATIVE_PREDICATE) const;

    void preLeftTraverseInner(NodePtr, NodesTraverseFunc, ConditionWrapper&) const;
    void postLeftTraverseInner(NodePtr, NodesTraverseFunc, ConditionWrapper&) const;
    void preRightTraverseInner(NodePtr, NodesTraverseFunc, ConditionWrapper&) const;
    void postRightTraverseInner(NodePtr, NodesTraverseFunc, ConditionWrapper&) const;
    void inOrderTraverseInner(NodePtr, NodesTraverseFunc, ConditionWrapper&) const;
    void inOppositeOrderTraverseInner(NodePtr, NodesTraverseFunc, ConditionWrapper&) const;

    class ConditionWrapper {
    public:
        ConditionWrapper(ElementPredicate predicate) : predicate(predicate), evaluated(false) { }

        bool operator()(Element& el) {
            return evaluated || (evaluated = predicate(el));
        }

        bool isAlreadyStopped() {
            return  evaluated;
        }
    private:
        ElementPredicate predicate;
        bool evaluated;
    };

    class Node {
    public:
        Node() : left(nullptr), right(nullptr) {}
        Node(NodePtr left, NodePtr right) : left(left), right(right) {}

        // set right child to provided Node
        void operator>>(NodePtr new_right) {
            right = new_right;
        }

        // set left child to provided Node
        void operator<<(NodePtr new_left) {
            left = new_left;
        }

        NodePtr& getLeft() {
            return left;
        }

        NodePtr& getRight() {
            return right;
        }

        virtual Element& getValue() {
            throw std::string("Trying to get element of base node.");
        }

    private:
        NodePtr left;
        NodePtr right;
    };

    class ElementNode : public Node {
    public:
        ElementNode(Element el) : Node(), el(el) { }
        ElementNode(Element el, NodePtr left, NodePtr right) : Node(left, right), el(el) {}

        virtual Element& getValue() {
            return el;
        }

    private:
        Element el;
    };

    unsigned int number_of_elements;
    NodePtr root;
};

template<typename Element>
void Tree<Element>::insert(const Element &element_to_insert) {
    NodePtr inserted_node(new ElementNode(element_to_insert));
    if (root != nullptr) {
        insertNode(root, inserted_node);
    } else {
        root = inserted_node;
    }
    number_of_elements++;
}

template<typename Element>
void Tree<Element>::insertNode(NodePtr parent_node, NodePtr node_to_insert) {
    if ( node_to_insert != nullptr ) {
        auto insert_node = findParentForNodeInsertion(parent_node, node_to_insert);
        if (node_to_insert->getValue() > insert_node->getValue()) {
            *insert_node >> node_to_insert;
        } else {
            *insert_node << node_to_insert;
        }
    }
}

template<typename Element>
typename Tree<Element>::NodePtr Tree<Element>::findElement(ElementPredicate test_func) const {
    NodePtr found;
    preLeftNodesTraverse([&](NodePtr& node) {
        if ( test_func(node->getValue()) ) {
            found = node;
        }
    }, [&](const Element& el) {
        return found != nullptr;
    });
    return found;
}

template<typename Element>
typename Tree<Element>::NodePtr Tree<Element>::findElement(const Element &value) const {
    auto iter = root;
    while (iter != nullptr && iter->getValue() != value) {
        iter = iterStepByValue(value, iter);
    }
    return iter;
}

template<typename Element>
typename Tree<Element>::NodePtr Tree<Element>::findParentForNodeInsertion(
        const NodePtr& starting_node,
        const NodePtr& node_for_insertion
) const {
    auto tree_iterator = starting_node;
    NodePtr prevParent = nullptr;
    while (tree_iterator != nullptr) {
        prevParent = tree_iterator;
        tree_iterator = iterStepByValue(node_for_insertion->getValue(), tree_iterator);
    }
    return prevParent;
}

template<typename Element>
typename Tree<Element>::NodePtr Tree<Element>::iterStepByValue(const Element &node_value,
                                                               NodePtr current_iter_pos) const {
    if (node_value > current_iter_pos->getValue()) {
        return current_iter_pos->getRight();
    }
    return current_iter_pos->getLeft();
}

template<typename Element>
bool Tree<Element>::isMember(const Element &el) const {
    return findElement(el) != nullptr;
}

template<typename Element>
unsigned int Tree<Element>::removeAll(ElementPredicate func) {
    unsigned int removed = 0;
    NodePtr imaginary_root(new Node(nullptr, root));
    traverseWithParent(imaginary_root, [&](NodePtr& p, NodePtr& c) {
        if ( func(c->getValue()) ) {
            removeNode(c, p);
            removed++;
        }
    });
    root = imaginary_root->getRight();
    return removed;
}

template<typename Element>
void Tree<Element>::traverseWithParent(NodePtr& iter,
                                       std::function<void(NodePtr&, NodePtr&)> func) {
    if ( iter ) {
        if ( iter->getLeft() ) {
            traverseWithParent(iter->getLeft(), func);
            func(iter, iter->getLeft());
        }
        if ( iter->getRight() ) {
            traverseWithParent(iter->getRight(), func);
            func(iter, iter->getRight());
        }
    }
}

template<typename Element>
unsigned int Tree<Element>::removeAll(const Element &el_to_remove) {
    return removeAll([&](const Element &test_el) -> bool {
        return el_to_remove == test_el;
    });
}

template<typename Element>
unsigned int Tree<Element>::remove(const Element &el, unsigned int count) {
    try {
        unsigned int removed = 0;
        return removeAll([&](const Element &test_el) -> bool {
            if ( removed == count )
                throw removed;
            if ( test_el == el ) {
                removed++;
                return true;
            }
            return false;
        });
    } catch (unsigned int i) {
        return i;
    }
}

template<typename Element>
unsigned int Tree<Element>::countElements(ElementPredicate test_func) const {
    unsigned int i = 0;
    inOrderNodesTraverse([&](const NodePtr &node) {
        if ( test_func(node->getValue()) ) {
            i++;
        }
    });
    return i;
}

template<typename Element>
unsigned int Tree<Element>::countElements(const Element &el) const {
    return countElements([&](const Element& test_el) {
        return test_el == el;
    });
}

template<typename Element>
Tree<Element> Tree<Element>::makeElementsSubtree(ElementPredicate filterFunc) const {
    Tree<Element> new_tree;
    preLeftNodesTraverse([&](const NodePtr &node) {
        if ( filterFunc(node->getValue()) ) {
            new_tree.insert(node->getValue());
        }
    });
    return std::move(new_tree);
}

template<typename Element>
Tree<Element> Tree<Element>::getSubtreeFromElement(const Element &el) const {
    return Tree<Element>(findElement(el));
}

template<typename Element>
Tree<Element> Tree<Element>::getSubtreeFromElement(ElementPredicate func) const {
    return Tree<Element>(findElement(func));
}

/// Traversals

template<typename Element>
void Tree<Element>::preLeftTraverse(ElementsTraverseFunc func, ElementPredicate stopCondition) const {
    preLeftNodesTraverse([&](NodePtr &node) {
        func(node->getValue());
    }, stopCondition);
}

template<typename Element>
void Tree<Element>::postLeftTraverse(ElementsTraverseFunc func, ElementPredicate stopCondition) const {
    postLeftNodesTraverse([&](NodePtr &node) {
        func(node->getValue());
    }, stopCondition);
}

template<typename Element>
void Tree<Element>::preRightTraverse(ElementsTraverseFunc func, ElementPredicate stopCondition) const {
    preRightNodesTraverse([&](NodePtr &node) {
        func(node->getValue());
    }, stopCondition);
}

template<typename Element>
void Tree<Element>::postRightTraverse(ElementsTraverseFunc func, ElementPredicate stopCondition) const {
    postRightNodesTraverse([&](NodePtr &node) {
        func(node->getValue());
    }, stopCondition);
}

template<typename Element>
void Tree<Element>::preLeftNodesTraverse(NodesTraverseFunc func, ElementPredicate stopCondition) const {
    ConditionWrapper condition(stopCondition);
    preLeftTraverseInner(root, func, condition);
}

template<typename Element>
void Tree<Element>::postLeftNodesTraverse(NodesTraverseFunc func, ElementPredicate stopCondition) const {
    ConditionWrapper condition(stopCondition);
    postLeftTraverseInner(root, func, condition);
}

template<typename Element>
void Tree<Element>::preRightNodesTraverse(NodesTraverseFunc func, ElementPredicate stopCondition) const {
    ConditionWrapper condition(stopCondition);
    preRightTraverseInner(root, func, condition);
}

template<typename Element>
void Tree<Element>::postRightNodesTraverse(NodesTraverseFunc func, ElementPredicate stopCondition) const {
    ConditionWrapper condition(stopCondition);
    postRightTraverseInner(root, func, condition);
}

template<typename Element>
void Tree<Element>::preLeftTraverseInner(NodePtr currentNode,
                                         NodesTraverseFunc func,
                                         ConditionWrapper& stopCondition) const {
    if (currentNode != nullptr) {
        func(currentNode);
        if ( !stopCondition(currentNode->getValue()) ) {
            preLeftTraverseInner(currentNode->getLeft(), func, stopCondition);
            preLeftTraverseInner(currentNode->getRight(), func, stopCondition);
        }
    }
}

template<typename Element>
void Tree<Element>::postLeftTraverseInner(NodePtr currentNode,
                                          NodesTraverseFunc func,
                                          ConditionWrapper& stopCondition) const {
    if (currentNode != nullptr) {
        if ( !stopCondition.isAlreadyStopped() ) {
            postLeftTraverseInner(currentNode->getLeft(), func, stopCondition);
            postLeftTraverseInner(currentNode->getRight(), func, stopCondition);
            stopCondition(currentNode->getValue());
        }
        func(currentNode);
    }
}

template<typename Element>
void Tree<Element>::preRightTraverseInner(NodePtr currentNode,
                                          NodesTraverseFunc func,
                                          ConditionWrapper& stopCondition) const {
    if (currentNode != nullptr) {
        func(currentNode);
        if ( !stopCondition(currentNode->getValue()) ) {
            preRightTraverseInner(currentNode->getRight(), func, stopCondition);
            preRightTraverseInner(currentNode->getLeft(), func, stopCondition);
        }
    }
}

template<typename Element>
void Tree<Element>::postRightTraverseInner(NodePtr currentNode,
                                           NodesTraverseFunc func,
                                           ConditionWrapper& stopCondition) const {
    if (currentNode != nullptr) {
        if ( !stopCondition.isAlreadyStopped() ) {
            postRightTraverseInner(currentNode->getRight(), func, stopCondition);
            postRightTraverseInner(currentNode->getLeft(), func, stopCondition);
            stopCondition(currentNode->getValue());
        }
        func(currentNode);
    }
}

template<typename Element>
void Tree<Element>::inOrderTraverse(
        ElementsTraverseFunc func,
        ElementPredicate stopCondition
) const {
    inOrderNodesTraverse([&](NodePtr &node) {
        func(node->getValue());
    }, stopCondition);
}

template<typename Element>
void Tree<Element>::inOppositeOrderTraverse(
        ElementsTraverseFunc func,
        ElementPredicate stopCondition
) const {
    inOppositeOrderNodesTraverse([&](NodePtr &node) {
        func(node->getValue());
    }, stopCondition);
}

template<typename Element>
void Tree<Element>::inOrderNodesTraverse(
        NodesTraverseFunc func,
        ElementPredicate stopCondition
) const {
    ConditionWrapper condition(stopCondition);
    inOrderTraverseInner(root, func, condition);
}

template<typename Element>
void Tree<Element>::inOppositeOrderNodesTraverse(
        NodesTraverseFunc func,
        ElementPredicate stopCondition
) const {
    ConditionWrapper condition(stopCondition);
    inOppositeOrderTraverseInner(root, func, condition);
}

template<typename Element>
void Tree<Element>::inOrderTraverseInner(
        NodePtr currentNode,
        NodesTraverseFunc func,
        ConditionWrapper& stopCondition
) const {
    if (currentNode != nullptr) {
        if ( !stopCondition.isAlreadyStopped() ) {
            inOrderTraverseInner(currentNode->getLeft(), func, stopCondition);
        }
        if ( !stopCondition(currentNode->getValue()) ) {
            func(currentNode);
            inOrderTraverseInner(currentNode->getRight(), func, stopCondition);
        }
    }
}

template<typename Element>
void Tree<Element>::inOppositeOrderTraverseInner(
        NodePtr currentNode,
        NodesTraverseFunc func,
        ConditionWrapper& stopCondition
) const {
    if (currentNode != nullptr) {
        if ( !stopCondition(currentNode->getValue()) ) {
            inOppositeOrderTraverseInner(currentNode->getRight(), func, stopCondition);
        }
        if ( !stopCondition.isAlreadyStopped() ) {
            func(currentNode);
            inOppositeOrderTraverseInner(currentNode->getLeft(), func, stopCondition);
        }
    }
}

template<typename Element>
void Tree<Element>::removeNode(NodePtr node_to_remove, NodePtr parent_node) {
    if ( parent_node == nullptr ) {
        removeRoot();
    }
    if ( node_to_remove == parent_node->getRight()) {
        removeRightNode(node_to_remove, parent_node);
    } else {
        removeLeftNode(node_to_remove, parent_node);
    }
}

template<typename Element>
void Tree<Element>::removeRightNode(NodePtr node_to_remove, NodePtr parent_node) {
    if ( node_to_remove->getRight() != nullptr ) {
        *parent_node >> node_to_remove->getRight();
        insertNode(parent_node->getRight(), node_to_remove->getLeft());
    } else {
        *parent_node >> node_to_remove->getLeft();
    }
}

template<typename Element>
void Tree<Element>::removeLeftNode(NodePtr node_to_remove, NodePtr parent_node) {
    if ( node_to_remove->getLeft() != nullptr ) {
        *parent_node << node_to_remove->getLeft();
        insertNode(parent_node->getLeft(), node_to_remove->getRight());
    } else {
        *parent_node << node_to_remove->getRight();
    }
}


template<typename Element>
void Tree<Element>::removeRoot() {
    if ( root->getRight() == nullptr ) {
        root = root->getLeft();
    } else {
        insertNode(root, root->getLeft());
        root = root->getRight();
    }
}

#endif //BINARY_TREE_TREE_H

