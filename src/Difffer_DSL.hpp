#define CONST(num__) new_node(Node_type::CONST, {.num = num__}, NULL, NULL, NULL)

#define OPERATOR(operator__) new_node(Node_type::OPERATOR, {.opr = operator__}, NULL, NULL, NULL)

#define DIFF_CONST(node) node->type = Node_type::CONST; node->data.num = 0; return node;

#define DIFF_VAR(node)  {if (node->data.variable == var) { \
                            node->data.num = 1;            \
                        } else {                            \
                            node->data.num = 0;              \
                        }                                     \
                        node->type = Node_type::CONST;         \
                        return node;}                       

#define DIFF_ADD(node)  {if (node->left == NULL || node->right == NULL)\
                            return NULL;                               \
                        node->left  = differ_node(tree, node->left, var);     \
                        node->right = differ_node(tree, node->right, var);     \
                        return node;}

#define DIFF_SUB(node)  {if (node->left == NULL || node->right == NULL)\
                            return NULL;                               \
                        node->left  = differ_node(tree, node->left, var);     \
                        node->right = differ_node(tree, node->right, var);     \
                        return node;} 

#define DIFF_MUL(node) {if (node->left == NULL || node->right == NULL)   \
                            return NULL;                                  \
                        node->data.opr = Operation::ADD;                   \
                        Tree_node* left = node->left;                       \
                        Tree_node* right = node->right;                      \
                        node->left = OPERATOR(Operation::MUL);                \
                        node->right = OPERATOR(Operation::MUL);                \
                        node->left->left   = left;                              \
                        node->left->right  = right;                              \
                        node->right->left  = (left);                     \
                        node->right->right = (right);                     \
                        differ_node(tree, node->left->left, var);                         \
                        differ_node(tree, node->right->right, var);                        \
                        return node;}                  


#define DIFF_DIV(node) {if (node->left == NULL || node->right == NULL)   \
                            return NULL;                                  \
                        Tree_node* left = node->left;                      \
                        Tree_node* right = node->right;                     \
                        node->left = OPERATOR(Operation::SUB);               \
                        node->right = OPERATOR(Operation::POW);               \
                        node->data.opr = Operation::MUL;                       \
                        node->right->right = CONST(-2);                         \
                        node->right->left = (right);                    \
                                                                                  \
                        node->left->left   = OPERATOR(Operation::MUL);             \
                        node->left->right  = OPERATOR(Operation::MUL);            \
                                                                                 \
                        node->left->left->left = left;                          \
                        node->left->left->right = (right);            \
                                                                              \
                        node->left->right->right = right;                    \
                        node->left->right->left = (left);          \
                                                                           \
                        differ_node(tree, node->left->left->left, var);         \
                        differ_node(tree, node->left->right->right, var);      \
                        return node;}                  

#define DIFF_POW(node) {if (node->left == NULL || node->right == NULL)   \
                            return NULL;                                  \
                        bool left_var = node_contains_var(node->left, var);     \
                        bool right_var = node_contains_var(node->right, var);    \
                        Tree_node* left = node->left;                        \
                        Tree_node* right = node->right;                       \
                                                                               \
                        if (left_var && right_var) {\
                            node->left = new_node(Node_type::OPERATOR, {.opr = Operation::POW}, node, left, right);\
                            node->data.opr = Operation::MUL;                        \
                            node->right = new_node(Node_type::OPERATOR, {.opr = Operation::MUL}, node, (right), OPERATOR(Operation::LOG));\
                            node->right->right->left = (left);                                                                      \
                            differ_node(tree, node->right, var);    \
                            return node;                                                                      \
                        }\
                        if (!left_var && !right_var) { \
                            delete_node(left);          \
                            delete_node(right);          \
                            node->left = node->right = NULL;\
                            node->type = Node_type::CONST;\
                            printf("diff consttttttttt \n");\
                            node->data.num = 0;            \
                            return node;                    \
                        }                                    \
                        if (!left_var && right_var) {                                                                    \
                            node->left = new_node(Node_type::OPERATOR, {.opr = Operation::POW}, node , left, right);\
                            node->type = Node_type::OPERATOR;                                                      \
                            node->data.opr = Operation::POW;                                                      \
                            node->right = OPERATOR(Operation::MUL);                                              \
                            node->right->right = (right);                                              \
                            node->right->left = OPERATOR(Operation::LOG);                                      \
                            node->right->left->left = (left);                                                   \
                            differ_node(tree, node->right->right, var);                                            \
                            return node;                                                                    \
                        }                                                                                  \
                        if (left_var && !right_var) {                                                                    \
                            node->right = OPERATOR(Operation::MUL);\
                            node->right->left = (left);\
                            node->right->right = (right);\
                            node->left = OPERATOR(Operation::POW);\
                            node->left->left = left;\
                            node->left->right = OPERATOR(Operation::SUB);\
                            node->left->right->left = right;\
                            node->left->right->right = CONST(1);\
                            node->type = Node_type::OPERATOR; \
                            node->data.opr = Operation::MUL; \
                            differ_node(tree, node->right->left, var);\
                            return node;                                                                                     \
                        }                                                                                                   \
                        return NULL;                                                                                       \
    };

#define DIFF_SIN(node) {if (node->left == NULL || node->right != NULL)\
                            return NULL;                               \
                        Tree_node* left = node->left;                   \
                        node->data.opr = Operation::MUL;                 \
                        node->left = OPERATOR(Operation::COS);            \
                        node->left->left = left;                           \
                        node->right = (left);                      \
                        differ_node(tree, node->right, var);                       \
                        return node;}

#define DIFF_COS(node) {if (node->left == NULL || node->right != NULL)\
                            return NULL;                               \
                        Tree_node* left = node->left;                   \
                        node->data.opr = Operation::MUL;                 \
                        node->left = OPERATOR(Operation::MUL);            \
                        node->left->left = CONST(-1);                      \
                        node->left->right = OPERATOR(Operation::SIN);       \
                        node->left->right->left = left;                      \
                        node->right = (left);                        \
                        differ_node(tree, node->right, var);                         \
                        return node;}

#define DIFF_TAN(node) {if (node->left == NULL || node->right != NULL)\
                            return NULL;                               \
                        Tree_node* left = node->left;                   \
                        node->data.opr = Operation::MUL;                 \
                        node->left = OPERATOR(Operation::POW);            \
                        node->left->left = OPERATOR(Operation::COS);       \
                        node->left->left->left = left;                      \
                        node->left->right = CONST(-2);                       \
                        node->right = (left);                        \
                        differ_node(tree, node->right, var);                         \
                        return node;}

#define DIFF_COT(node) {if (node->left == NULL || node->right != NULL)\
                            return NULL;                               \
                        Tree_node* left = node->left;                   \
                        node->data.opr = Operation::MUL;                 \
                        node->left = OPERATOR(Operation::POW);            \
                        node->left->left = OPERATOR(Operation::SIN);       \
                        node->left->left->left = left;                      \
                        node->left->right = CONST(-2);                       \
                        node->right = OPERATOR(Operation::MUL);               \
                        node->right->left = CONST(-1);                         \
                        node->right->right = (left);                   \
                        differ_node(tree, node->right->right, var);                    \
                        return node;}

#define DIFF_LOG(node) {if (node->left == NULL || node->right != NULL)\
                            return NULL;                               \
                        Tree_node* left = node->left;                   \
                        node->data.opr = Operation::MUL;                 \
                        node->left = OPERATOR(Operation::POW);            \
                        node->left->left = left;                           \
                        node->left->right = CONST(-1);                      \
                        node->right = (left);                       \
                        differ_node(tree, node->right, var);                        \
                        return node;}
  