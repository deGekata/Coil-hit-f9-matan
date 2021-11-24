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
                        node->left  = differ_node(node->left, var);     \
                        node->right = differ_node(node->right, var);     \
                        return node;}

#define DIFF_SUB(node)  {if (node->left == NULL || node->right == NULL)\
                            return NULL;                               \
                        node->left  = differ_node(node->left, var);     \
                        node->right = differ_node(node->right, var);     \
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
                        node->right->left  = node_copy(left);                     \
                        node->right->right = node_copy(right);                     \
                        differ_node(node->left->left, var);                         \
                        differ_node(node->right->right, var);                        \
                        return node;}                  


#define DIFF_DIV(node) {if (node->left == NULL || node->right == NULL)   \
                            return NULL;                                  \
                        Tree_node* left = node->left;                      \
                        Tree_node* right = node->right;                     \
                        node->left = OPERATOR(Operation::SUB);               \
                        node->right = OPERATOR(Operation::POW);               \
                        node->data.opr = Operation::MUL;                       \
                        node->right->right = CONST(-2);                         \
                        node->right->left = node_copy(right);                    \
                                                                                  \
                        node->left->left   = OPERATOR(Operation::MUL);             \
                        node->left->right  = OPERATOR(Operation::MUL);            \
                                                                                 \
                        node->left->left->left = left;                          \
                        node->left->left->right = node_copy(right);            \
                                                                              \
                        node->left->right->right = right;                    \
                        node->left->right->left = node_copy(left);          \
                                                                           \
                        differ_node(node->left->left->left, var);         \
                        differ_node(node->left->right->right, var);      \
                        return node;}                  

#define DIFF_POW(node) {};

#define DIFF_SIN(node) {if (node->left == NULL || node->right != NULL)\
                            return NULL;                               \
                        Tree_node* left = node->left;                   \
                        node->data.opr = Operation::MUL;                 \
                        node->left = OPERATOR(Operation::COS);            \
                        node->left->left = left;                           \
                        node->right = node_copy(left);                      \
                        differ_node(node->right, var);                       \
                        return node;}

#define DIFF_COS(node) {if (node->left == NULL || node->right != NULL)\
                            return NULL;                               \
                        Tree_node* left = node->left;                   \
                        node->data.opr = Operation::MUL;                 \
                        node->left = OPERATOR(Operation::MUL);            \
                        node->left->left = CONST(-1);                      \
                        node->left->right = OPERATOR(Operation::SIN);       \
                        node->left->right->left = left;                      \
                        node->right = node_copy(left);                        \
                        differ_node(node->right, var);                         \
                        return node;}

#define DIFF_TAN(node) {if (node->left == NULL || node->right != NULL)\
                            return NULL;                               \
                        Tree_node* left = node->left;                   \
                        node->data.opr = Operation::MUL;                 \
                        node->left = OPERATOR(Operation::POW);            \
                        node->left->left = OPERATOR(Operation::COS);       \
                        node->left->left->left = left;                      \
                        node->left->right = CONST(-2);                       \
                        node->right = node_copy(left);                        \
                        differ_node(node->right, var);                         \
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
                        node->right->right = node_copy(left);                   \
                        differ_node(node->right->right, var);                    \
                        return node;}

#define DIFF_LOG(node) {if (node->left == NULL || node->right != NULL)\
                            return NULL;                               \
                        Tree_node* left = node->left;                   \
                        node->data.opr = Operation::MUL;                 \
                        node->left = OPERATOR(Operation::POW);            \
                        node->left->left = left;                           \
                        node->left->right = CONST(-1);                      \
                        node->right = node_copy(left);                       \
                        differ_node(node->right, var);                        \
                        return node;}
  