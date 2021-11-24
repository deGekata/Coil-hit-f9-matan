#include "stdlib.h"
#include "stdio.h"
#include "Diff_tree.hpp"

int main() {
    bool choose = true;
    // (choose  && ({printf("truthy\n"); true;})) || (({printf("falsy\n"); true;}));
    Tree_node node = {};
    node.type = Node_type::VARIABLE;
    node.data.variable= 'x';

    Tree_node node1 = {};
    node1.type = Node_type::CONST;
    node1.data.num = 15;

    Tree_node node2 = {};
    node2.type = Node_type::OPERATOR;
    node2.data.opr = Operation::ADD;

    node.left = &node1;
    node.right = &node2;

    Tree tree;
    tree.root = &node;
    // FILE* file = fopen("Temp")
    fgraph_tree(&tree);
    FILE* file = fopen("test.txt", "r");
    printf("%d\n", __LINE__);

    tree_read(&tree, file);
    printf("%d\n", __LINE__);

    fgraph_tree(&tree);
    printf("%d\n", __LINE__);
    Tree* diff_tree = differ(&tree, 'x');

    fgraph_tree(diff_tree);
    system("rm ./dump/*.png");
    // write_node(&node, stdout);
    return 0;
}