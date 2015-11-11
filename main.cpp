
#include <iostream>
#include "Tree.h"

using namespace std;

int main() {
    Tree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(1);

    cout << tree.countElements(1) << endl;
    cout << tree.size() << endl;
    tree.inOrderTraverse([](int& n) {
        cout << n << " ";
    });
    cout << endl;

    cout << tree.removeAll(1) << endl;

    cout << "Hello, world!!!" << endl;
    return 0;
}