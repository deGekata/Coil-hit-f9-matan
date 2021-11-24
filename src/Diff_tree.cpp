#include "Diff_tree.hpp"

//----------------------------------------Tools begin ----------------------------------------

const size_t Max_out_file_len = 49;
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
    printf("check\n");
    ASSERT_NOT_EQUAL_RET(node->type, Node_type::CONST, Node_tree_status::TYPE_NOT_MATCHING);
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

#define DEF_OPER(NAME, VAL, code) case Operation::NAME : code; break;
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

    
}


Node_tree_status tree_read(Tree* tree, FILE* input) {
    ASSERT_EQUAL_RET(input, NULL, Node_tree_status::NULL_PTR);

    if (tree->root != NULL) delete_node(tree->root);

    tree->root = new_node(Node_type::NONE, {}, NULL, NULL, NULL);

    Tokenizer tokenizer = {};
    tokenizer_init(&tokenizer, input);

    Token token = {};
    get_token(&tokenizer, &token);
    
    // while (token->type != Token_type::EPSILON || token->type != Token_type::UNKNOWN) {
        
    // }

    // if (token == Token_type::UNKNOWN) {
    //     ASSERT_NOT_EQUAL(token->type, Token_type::UNKNOWN, Node_tree_status::ERROR_IN_CALLED_FUNC);
    //     return Node_tree_status::ERROR_IN_CALLED_FUNC;
    // }

    switch (token.type)
    {
    case Token_type::EPSILON:
        printf("Epsilon\n");
        break;
    
    case Token_type::CLOSING_BRACKET:
        printf("CLOSING_BRACKET\n");
        break;
    
    case Token_type::OPEN_BRACKET:
        printf("OPEN_BRACKET\n");
        break;
    
    case Token_type::UNKNOWN:
        printf("UNKNOWN\n");
        break;
    
    case Token_type::STANDART: {
            switch (token.node->type)
            {
            case Node_type::CONST :
                printf("Const %d\n", token.node->data.num);
                break;

            case Node_type::VARIABLE :
                printf("Variable %c\n", token.node->data.variable);
                break;

            case Node_type::OPERATOR :
                printf("Operator\n");
                break;
            
            default:
                break;
            }

        }
        break;

    default:
        break;
    }
    printf("tokenizer ret: %d\n", token.node->data.num);

    return Node_tree_status::OK;
}

Node_tree_status graph_tree(Tree* tree) {
    return Node_tree_status::NOT_INITIALIZED;
}

Node_tree_status graph_node(Tree_node* node, FILE* output) {
    ASSERT_EQUAL_RET(node, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(output, NULL, Node_tree_status::NULL_PTR);

    fprintf(output, "   L%lu[label=",
                 node);
    node_write(node, output);
    //  node, list->data[node].prev, node, list->data[node].next);
    fprintf(output, " style=filled];\n");

    if (node->left != NULL)  { 
        graph_node(node->left,  output);
        fprintf(output, "L%lu->L%lu[color=\"black\"];\n", node, node->left);
    }
    if (node->right != NULL) { 
        graph_node(node->right,  output);
        fprintf(output, "L%lu->L%lu[color=\"black\"];\n", node, node->right);
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
    // fprintf(file,   "   rankdir=LR;\n");
    // fprintf(file,   "   splines=ortho;\n");
    fprintf(file,   "   nodesep=1;\n");
    // fprintf(file,   "   F[shape=\"circle\", color=\"blue\", label=\"Free\"];\n");
    

    graph_node(tree->root, file);


    fprintf(file, "}");
    fclose(file);

    char command[Max_cmd_len] = {};

    sprintf(command, "dot %sLIST_DMP_№%d.dot -T png -o %sLIST_DMP_№%d.png", Img_dump_dir, dumpNumber, Img_dump_dir, dumpNumber);
    system(command);


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
    
    token->node = new_node(Node_type::CONST, {.num = cur_num}, NULL, NULL, NULL);

    return Node_tree_status::OK;
}

Node_tree_status get_token_try_operation(Tokenizer* tokenizer, Token* token) {
    ASSERT_EQUAL_RET(tokenizer, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(token, NULL, Node_tree_status::NULL_PTR);
    
    size_t max_oper_len = 4;

    num_t cur_num = 0;
    size_t prev_offset = tokenizer->offset;
    
        

    printf("operation todo\n");
    return Node_tree_status::TOKEN_PARSE_ERR;
}

Node_tree_status get_token_try_variable(Tokenizer* tokenizer, Token* token) {
    ASSERT_EQUAL_RET(tokenizer, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(token, NULL, Node_tree_status::NULL_PTR);

    if (tokenizer->offset < tokenizer->size && isalpha(tokenizer->data[tokenizer->offset])) {
        token->type = Token_type::STANDART;
        
        Node_data data;
        data.variable = tokenizer->data[tokenizer->offset]; 
        
        token->node = new_node(Node_type::VARIABLE, data, NULL, NULL, NULL);

        ++tokenizer->offset;
        return Node_tree_status::OK;
    } 

    printf("variable todo\n");
    return Node_tree_status::TOKEN_PARSE_ERR;
}

Node_tree_status get_token(Tokenizer* tokenizer, Token* token) {
    ASSERT_EQUAL_RET(tokenizer, NULL, Node_tree_status::NULL_PTR);
    ASSERT_EQUAL_RET(token, NULL, Node_tree_status::NULL_PTR);

    if (tokenizer->offset == tokenizer->size || tokenizer->data[tokenizer->offset] == '\n' || tokenizer->data[tokenizer->offset] == '\r') {
        token->type = Token_type::EPSILON;
        return Node_tree_status::OK;
    }

    size_t cur_offset = tokenizer->offset;

    get_token_try_const(tokenizer, token);
    if (token->type != Token_type::ERR) {
        return Node_tree_status::OK;
    }
    
    get_token_try_variable(tokenizer, token);
    if (token->type != Token_type::ERR) {
        return Node_tree_status::OK;
    }

    get_token_try_operation(tokenizer, token);
    if (token->type != Token_type::ERR) {
        return Node_tree_status::OK;
    }

    RAISE_ERR(Node_tree_status::TOKEN_PARSE_ERR);

    return Node_tree_status::ERROR_IN_CALLED_FUNC;
}

//----------------------------------------Lexer end ----------------------------------------