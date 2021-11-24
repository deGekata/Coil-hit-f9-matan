#include "Diff_tree.hpp"

//----------------------------------------Tools begin ----------------------------------------

const size_t Max_out_file_len = 150;
const size_t Max_cmd_len  = 149;

const char* Img_dump_dir = "./dump/";

const char* Next_link_color = "blue";
const char* Prev_link_color = "red";
const char* Free_link_color = "orange";

bool resolve_errors(Node_tree_status status, char* msg) {
    switch (status) {
    case Node_tree_status::OK:
        break;
    case Node_tree_status::NOT_INITIALIZED:
        fprintf(stderr, "%s", msg);
        return 1;
        break;
    case Node_tree_status::NULL_PTR:
        fprintf(stderr, "%s %s", msg, " Null ptr");
        break;
    
    default:

        break;
    }
    return true;
}

int64_t get_file_size (FILE* inp) {
    assert(inp);
    fseek(inp, 0L, SEEK_END);
    int64_t size = ftell(inp);
    fseek(inp, 0L, SEEK_SET);

    return size;
}

bool is_oper_sym(char c) {
    return isalpha(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '^'; 
}


//----------------------------------------Tools end ----------------------------------------



//----------------------------------------Node begin ----------------------------------------

Tree_node* new_node (Node_type type, Node_data data, Tree_node* prev, Tree_node* left, Tree_node* right) {
    Tree_node* node = (Tree_node*) calloc(1, sizeof(Tree_node));
    node->prev = prev;
    node->left = left;
    node->right = right;

    node->type = type;
    node->data = data;

    return node;
}

Node_tree_status delete_node (Tree_node* node) {
    ASSERT_EQUAL_RET(node, NULL, Node_tree_status::NULL_PTR);

    if (node->left != NULL) ASSERT_NOT_EQUAL_RET(delete_node(node->left), Node_tree_status::OK, Node_tree_status::ERROR_IN_CALLED_FUNC);
    if (node->right != NULL) ASSERT_NOT_EQUAL_RET(delete_node(node->left), Node_tree_status::OK, Node_tree_status::ERROR_IN_CALLED_FUNC);
    node->prev = node->left = node->right = NULL;
    node->type = Node_type::NONE;

    return Node_tree_status::OK;
}


bool is_node_const (Tree_node* node) {}

bool is_node_variable (Tree_node* node) {}

bool is_node_operation (Tree_node* node) {}

Node_tree_status node_write_const (Tree_node* node, FILE* output) {
    ASSERT_EQUAL_RET(node, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(output, NULL, Node_tree_status::NULL_PTR);
    ASSERT_NOT_EQUAL_RET(node->type, Node_type::CONST, Node_tree_status::TYPE_NOT_MATCHING);
    // printf("check\n");
    fprintf(output, "\"%d\", shape=\"egg\", color=\"grey\", thickness=4, fillcolor=\"darkolivegreen2\"", node->data.num);
    return Node_tree_status::OK;
}

Node_tree_status node_write_variable (Tree_node* node, FILE* output) {
    ASSERT_EQUAL_RET(node, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(output, NULL, Node_tree_status::NULL_PTR);
    ASSERT_NOT_EQUAL_RET(node->type, Node_type::VARIABLE, Node_tree_status::TYPE_NOT_MATCHING);

    fprintf(output, "\"%c\", shape=\"egg\", color=\"grey\", thickness=4, fillcolor=\"cornflowerblue\"", node->data.variable);

    return Node_tree_status::OK;
}

#define DEF_OPER(NAME, VAL, code, arg_cnt) case Operation::NAME : code; break;
Node_tree_status node_write_operator (Tree_node* node, FILE* output) {
    ASSERT_EQUAL_RET(node, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(output, NULL, Node_tree_status::NULL_PTR);
    ASSERT_NOT_EQUAL_RET(node->type, Node_type::OPERATOR, Node_tree_status::TYPE_NOT_MATCHING);

    switch (node->data.opr) {

        #include "OPERATOR_DEF.hpp"

    default:
        break;
    }

    return Node_tree_status::OK;
}
#undef DEF_OPER


Node_tree_status node_write (Tree_node* node, FILE* output) {
    ASSERT_EQUAL_RET(node, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(output, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(node->type, Node_type::NONE, Node_tree_status::NOT_INITIALIZED);
    
    switch (node->type) {
    
    case Node_type::CONST:
        ASSERT_NOT_EQUAL_RET(node_write_const(node, output), Node_tree_status::OK, Node_tree_status::ERROR_IN_CALLED_FUNC); 
        break;
    
    case Node_type::VARIABLE:
        ASSERT_NOT_EQUAL_RET(node_write_variable(node, output), Node_tree_status::OK, Node_tree_status::ERROR_IN_CALLED_FUNC); 
        break;
    case Node_type::OPERATOR:
        ASSERT_NOT_EQUAL_RET(node_write_operator(node, output), Node_tree_status::OK, Node_tree_status::ERROR_IN_CALLED_FUNC); 
        break;
    
    default:

        break;
    }
    
    return Node_tree_status::OK;

}

//----------------------------------------Node end ----------------------------------------


//----------------------------------------Tree begin ----------------------------------------
Node_tree_status tree_init(Tree* tree) {
    ASSERT_EQUAL_RET(tree, NULL, Node_tree_status::NULL_PTR);
    ASSERT_NOT_EQUAL_RET(tree->root, NULL, Node_tree_status::NULL_PTR);
    
    tree->root = new_node(Node_type::NONE, {.num = 0}, NULL, NULL, NULL);

    return Node_tree_status::OK;
}

Node_tree_status tree_read(Tree* tree, FILE* input) {
    ASSERT_EQUAL_RET(input, NULL, Node_tree_status::NULL_PTR);

    if (tree->root != NULL) delete_node(tree->root);

    tree->root = new_node(Node_type::NONE, {}, NULL, NULL, NULL);
    Tree_node* cur_node = tree->root;

    Tokenizer tokenizer = {};
    tokenizer_init(&tokenizer, input);

    Token token = {};
    get_token(&tokenizer, &token);
    

    while (token.type != Token_type::EPSILON && token.type != Token_type::UNKNOWN) {
        switch (token.type) {

        case Token_type::EPSILON:
            printf("Epsilon\n");
            if (cur_node->prev != NULL) RAISE_ERR(Node_tree_status::TOKEN_PARSE_ERR);
            break;
        
        case Token_type::CLOSING_BRACKET:
            ASSERT_EQUAL_RET(cur_node->prev, NULL, Node_tree_status::NULL_PTR);
            cur_node = cur_node->prev;
            printf("CLOSING_BRACKET\n");
            break;
        
        case Token_type::OPEN_BRACKET:
            printf("OPEN_BRACKET\n");
            if (cur_node->left == NULL) {
                cur_node->left = new_node(Node_type::NONE, {}, cur_node, NULL, NULL);
                cur_node = cur_node->left;
            } else if (cur_node->right == NULL) {
                cur_node->right = new_node(Node_type::NONE, {}, cur_node, NULL, NULL);
                cur_node = cur_node->right;
            } else {
                RAISE_ERR(Node_tree_status::TOKEN_PARSE_ERR);
                return Node_tree_status::TOKEN_PARSE_ERR;
            }
            
            break;
        
        case Token_type::UNKNOWN:
            printf("UNKNOWN\n");
            RAISE_ERR(Node_tree_status::TOKEN_PARSE_ERR);
            break;
        
        case Token_type::STANDART:
            printf("standart\n");
            cur_node->type = token.node_type;
            cur_node->data = token.data;
            break;

        default:
            break;
        }
        get_token(&tokenizer, &token);
    }

    if (token.type == Token_type::UNKNOWN) {
        ASSERT_NOT_EQUAL(token.type, Token_type::UNKNOWN, Node_tree_status::ERROR_IN_CALLED_FUNC);
        return Node_tree_status::ERROR_IN_CALLED_FUNC;
    }

    return Node_tree_status::OK;
}

Node_tree_status graph_node(Tree_node* node, FILE* output) {
    ASSERT_EQUAL_RET(node, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(output, NULL, Node_tree_status::NULL_PTR);

    fprintf(output, "   L%lu[label=", node);
    node_write(node, output);

    fprintf(output, " style=filled];\n");

    if (node->left != NULL)  { 
        graph_node(node->left,  output);
        fprintf(output, "    L%lu->L%lu[color=\"black\"];\n", node, node->left);
    }
    if (node->right != NULL) { 
        graph_node(node->right,  output);
        fprintf(output, "    L%lu->L%lu[color=\"black\"];\n", node, node->right);
    }

    return Node_tree_status::OK;
}

Node_tree_status fgraph_tree(Tree* tree) {
    ASSERT_EQUAL_RET(tree, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(tree->root, NULL, Node_tree_status::NULL_PTR);

    static int dumpNumber = 0;

    char filename[Max_out_file_len];
    sprintf(filename, "%sLIST_DMP_№%d.dot", Img_dump_dir, dumpNumber);
    
    FILE* file = fopen(filename, "w");
    assert(file && "cant open file");

    fprintf(file,   "digraph G{\n");
    fprintf(file,   "   nodesep=1;\n");
    
    graph_node(tree->root, file);

    fprintf(file, "}");
    fclose(file);

    char command[Max_cmd_len] = {};

    sprintf(command, "dot %sLIST_DMP_№%d.dot -T png -o %sLIST_DMP_№%d.png", Img_dump_dir, dumpNumber, Img_dump_dir, dumpNumber);
    system(command);
    ++dumpNumber;
    return Node_tree_status::OK;
}

//----------------------------------------Tree end ----------------------------------------

//----------------------------------------Lexer begin ----------------------------------------

Node_tree_status tokenizer_init (Tokenizer* tokenizer, FILE* input) {
    ASSERT_EQUAL_RET(tokenizer, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(input, NULL, Node_tree_status::NULL_PTR);
    
    tokenizer->size = get_file_size(input) + 2;
    tokenizer->offset = 0;
    tokenizer->data = (char*) calloc(tokenizer->size, sizeof(char));

    fgets(tokenizer->data, tokenizer->size, input);

    return Node_tree_status::OK;
}

Node_tree_status tokenizer_destruct (Tokenizer* tokenizer) {
    ASSERT_EQUAL_RET(tokenizer, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(tokenizer->data, NULL, Node_tree_status::NULL_PTR);
    
    free(tokenizer->data);
    tokenizer->offset = tokenizer->size = -1;

    return Node_tree_status::OK;
}

Node_tree_status get_token_try_const(Tokenizer* tokenizer, Token* token) {
    ASSERT_EQUAL_RET(tokenizer, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(token, NULL, Node_tree_status::NULL_PTR);

    num_t cur_num = 0;
    size_t prev_offset = tokenizer->offset;
    
    while (tokenizer->offset < tokenizer->size && isdigit(tokenizer->data[tokenizer->offset])) {
        cur_num *= 10;
        cur_num += tokenizer->data[tokenizer->offset] - '0';

        ++tokenizer->offset;
    }

    if (prev_offset == tokenizer->offset) {
        token->type = Token_type::ERR;
        return Node_tree_status::TOKEN_PARSE_ERR;
    }
    
    token->type = Token_type::STANDART;
    
    token->data.num = cur_num;
    token->node_type = Node_type::CONST;

    return Node_tree_status::OK;
}

#define DEF_OPER(NAME, VAL, code, arg_cnt) case VAL: {                                        \
        token->type = Token_type::STANDART;                                                    \
        token->node_type = Node_type::OPERATOR;                                                 \
        token->data.opr=Operation::NAME;                                                         \
    }                                                                                             \
    return Node_tree_status::OK;                                                                   \
    break;

Node_tree_status get_token_try_operation(Tokenizer* tokenizer, Token* token) {
    ASSERT_EQUAL_RET(tokenizer, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(token, NULL, Node_tree_status::NULL_PTR);
    
    size_t max_oper_len = 4;

    num_t cur_num = 0;
    size_t prev_offset = tokenizer->offset;

    while (max_oper_len > 0 && tokenizer->offset < tokenizer->size && is_oper_sym(tokenizer->data[tokenizer->offset])) {
        cur_num *= 256;
        cur_num += tokenizer->data[tokenizer->offset];
        // printf("sym = %c\n", tokenizer->data[tokenizer->offset]);
        ++tokenizer->offset;
        --max_oper_len;
    }  
    // printf("cur_num = %d\n", cur_num);

    switch (cur_num) {
        
        #include "OPERATOR_DEF.hpp"
        
        default:
            token->type = Token_type::ERR;
            tokenizer->offset = prev_offset;
            return Node_tree_status::TOKEN_PARSE_ERR;
            break;
    }

    return Node_tree_status::TOKEN_PARSE_ERR;
}
#undef DEFO_OPER

Node_tree_status get_token_try_variable(Tokenizer* tokenizer, Token* token) {
    ASSERT_EQUAL_RET(tokenizer, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(token, NULL, Node_tree_status::NULL_PTR);

    if (tokenizer->offset < tokenizer->size && isalpha(tokenizer->data[tokenizer->offset])) {
        token->type = Token_type::STANDART;
        
        token->data.variable = tokenizer->data[tokenizer->offset]; 
        token->node_type = Node_type::VARIABLE;

        ++tokenizer->offset;
        return Node_tree_status::OK;
    } 

    return Node_tree_status::TOKEN_PARSE_ERR;
}

Node_tree_status get_token_try_bracket(Tokenizer* tokenizer, Token* token) {
    ASSERT_EQUAL_RET(tokenizer, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(token, NULL, Node_tree_status::NULL_PTR);

    if (tokenizer->offset < tokenizer->size) {
        if (tokenizer->data[tokenizer->offset] == '(') {
            token->type = Token_type::OPEN_BRACKET;
            ++tokenizer->offset;
            return Node_tree_status::OK;

        } else if (tokenizer->data[tokenizer->offset] == ')') {
            token->type = Token_type::CLOSING_BRACKET;
            ++tokenizer->offset;
            return Node_tree_status::OK;

        } else {
            token->type = Token_type::ERR;
            return Node_tree_status::TOKEN_PARSE_ERR;
        }
    } 

    printf("variable todo\n");
    return Node_tree_status::TOKEN_PARSE_ERR;
}

Node_tree_status get_token(Tokenizer* tokenizer, Token* token) {
    ASSERT_EQUAL_RET(tokenizer, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(token, NULL, Node_tree_status::NULL_PTR);

    if (tokenizer->offset == tokenizer->size || tokenizer->data[tokenizer->offset] == '\n' || tokenizer->data[tokenizer->offset] == '\r' || tokenizer->data[tokenizer->offset] == '\0') {
        token->type = Token_type::EPSILON;
        return Node_tree_status::OK;
    }

    size_t cur_offset = tokenizer->offset;

    get_token_try_operation(tokenizer, token);
    if (token->type != Token_type::ERR) {
        return Node_tree_status::OK;
    }

    get_token_try_const(tokenizer, token);
    if (token->type != Token_type::ERR) {
        return Node_tree_status::OK;
    }
    
    get_token_try_variable(tokenizer, token);
    if (token->type != Token_type::ERR) {
        return Node_tree_status::OK;
    }

    get_token_try_bracket(tokenizer, token);
    if (token->type != Token_type::ERR) {
        return Node_tree_status::OK;
    }

    printf("cur sym = %d  offset %d  size %d\n", tokenizer->data[tokenizer->offset], tokenizer->offset, tokenizer->size);
    // int a  = 1 / 0;
    token->type = Token_type::UNKNOWN;
    fprintf(stderr, "\033[1;31;40mTokenizer offset: %d\033[0m\n", tokenizer->offset);
    RAISE_ERR(Node_tree_status::TOKEN_PARSE_ERR);
    return Node_tree_status::ERROR_IN_CALLED_FUNC;
}

//----------------------------------------Lexer end ----------------------------------------


//----------------------------------------Differ begin ----------------------------------------
Tree_node* node_copy(Tree_node* node) {
    assert(node && "node must not be NULL");

    Tree_node* n_node = (Tree_node*) calloc(1, sizeof(Tree_node));
    *n_node = *node;
    if (node->left != NULL) {
        n_node->left = node_copy(node->left);
    }
    if (node->right != NULL) {
        n_node->right = node_copy(node->right);
    }

    return n_node;

}

Tree_node* differ_node(Tree_node* node, char var) {
    assert(node && "node must not be NULL");

    if (node->type == Node_type::VARIABLE) {
       DIFF_VAR(node);
    } 

    if (node->type == Node_type::CONST) {
        printf("diff const\n");
        DIFF_CONST(node);
    }

    if (node->type == Node_type::OPERATOR) {

        switch (node->data.opr) {
        case Operation::ADD: DIFF_ADD(node); break;
        
        case Operation::SUB: DIFF_SUB(node); break;
        
        case Operation::MUL: DIFF_MUL(node); break;
        
        case Operation::DIV: DIFF_DIV(node); break;
        
        case Operation::POW: DIFF_POW(node); break;
        
        case Operation::SIN: DIFF_SIN(node); break;
        
        case Operation::COS: DIFF_COS(node); break;
        
        case Operation::TAN: DIFF_TAN(node); break;
        
        case Operation::COT: DIFF_COT(node); break;
        
        case Operation::LOG: DIFF_LOG(node); break; 
        
        default:
            ASSERT_EQUAL(Node_tree_status::OK, Node_tree_status::RAISE_ERR, Node_tree_status::RAISE_ERR);
            return NULL;
            break;
        }

    }

    return NULL;
}

Tree* differ(Tree* tree, char var) {
    assert(tree && "tree ptr must not be null");
    Tree* diff_tree = (Tree*) calloc(1, sizeof(Tree));

    diff_tree->root = node_copy(tree->root);
    if (diff_tree->root == NULL) {
        printf("copy not ok \n");
    } else {
        printf("copy ok \n");
    }
    diff_tree->root = differ_node(diff_tree->root, var);

    return diff_tree;
}

//----------------------------------------Differ end ----------------------------------------