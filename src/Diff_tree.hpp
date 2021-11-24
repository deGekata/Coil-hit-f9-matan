#ifndef INC_DIFF_TREE
#define INC_DIFF_TREE

#include "assert.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <stack>
#include <cctype>
#include "Difffer_DSL.hpp"

static std::stack<char*> stack_trace;


//----------------------------------------Tools begin ----------------------------------------
#define ASSERT_EQUAL(left, right, status_code) (((left) == (right)) && ({ fprintf(stderr, "\033[1;31;40m %s is %s  \033[0m", #left, #status_code); 1;})) || ({0;})

#define ASSERT_NOT_EQUAL(left, right, status_code) (((left) != (right)) && ({ fprintf(stderr, "\033[1;31;40m %s is %s  \033[0m", #left, #status_code); 1;})) || ({0;})

#define RAISE_ERR(status_code)  {                                          \
        fprintf(stderr,                                                     \
            "\033[1;31;40mstatus:%s in function '%s' on line %d \033[0m\n",  \
            #status_code, __PRETTY_FUNCTION__, __LINE__);                     \
        return status_code;                                                    \
    }


#define ASSERT_EQUAL_RET(left, right, status_code)                        \
    if((left) == (right)) {                                                \
        fprintf(stderr,                                                     \
            "\033[1;31;40m%s is %s,\033[0m \n"                               \
            "\033[1;31;40mstatus:%s in function '%s' on line %d \033[0m\n",   \
            #left, #right, #status_code, __PRETTY_FUNCTION__, __LINE__);       \
        return status_code;                                                     \
    }

#define ASSERT_NOT_EQUAL_RET(left, right, status_code)                    \
    if((left) != (right)) {                                                \
        fprintf(stderr,                                                     \
            "\033[1;31;40m%s is %s,\033[0m \n"                               \
            "\033[1;31;40mstatus:%s in function '%s' on line %d \033[0m\n",   \
            #left, #right, #status_code, __PRETTY_FUNCTION__, __LINE__);       \
        return status_code;                                                     \
    }


#define PUSH_STACK_TRACE() stack_trace.push(__PRETTY_FUNCTION__);

#define POP_STACK_TRACE

enum class Node_tree_status {
    OK,
    NOT_INITIALIZED,
    NULL_PTR,
    NOT_NULL_PTR,
    TYPE_NOT_MATCHING,
    ERROR_IN_CALLED_FUNC,
    TOKEN_PARSE_ERR,
    RAISE_ERR,
};

bool resolve_errors(Node_tree_status status, char* msg);

int64_t get_file_size (FILE* inp);

//----------------------------------------Tools end ----------------------------------------

//----------------------------------------Node begin ----------------------------------------

enum class Node_type {
    NONE,
    CONST,
    VARIABLE,
    OPERATOR
};

#define DEF_OPER(NAME, VAL, code, arg_cnt) NAME = VAL,
enum class Operation {
    #include "OPERATOR_DEF.hpp"
};
#undef DEF_OPER

typedef Operation opr_t;
typedef int       num_t;
typedef char      var_t;

union Node_data {
    opr_t opr;
    num_t num;
    var_t variable;
};

struct Tree_node {
    Tree_node* prev;
    Tree_node* left;
    Tree_node* right;

    Node_type type = Node_type::NONE;
    Node_data data = {};

};

Tree_node* new_node (Node_type type, Node_data data, Tree_node* prev, Tree_node* left, Tree_node* right);


bool is_node_const (Tree_node* node);

bool is_node_variable (Tree_node* node);

bool is_node_operation (Tree_node* node);

Node_tree_status node_write_const (Tree_node* node, FILE* ouput);

Node_tree_status node_write_variable (Tree_node* node, FILE* ouput);

Node_tree_status node_write_operator (Tree_node* node, FILE* ouput);

Node_tree_status node_write (Tree_node* node, FILE* ouput);

//----------------------------------------Node end ----------------------------------------

//----------------------------------------Tree begin ----------------------------------------
struct Tree {
    Tree_node* root;
};

Node_tree_status tree_init(Tree* tree);

Node_tree_status tree_read(Tree* tree, FILE* input);

Node_tree_status fgraph_tree(Tree* tree);

//----------------------------------------Tree end ----------------------------------------

//----------------------------------------Lexer begin ----------------------------------------
enum class Token_type {
    UNKNOWN,
    EPSILON,
    OPEN_BRACKET,
    CLOSING_BRACKET,
    STANDART,
    ERR,
};

struct Token {
    Token_type type;
    Node_type node_type;
    Node_data data;
};

struct Tokenizer {
    char* data;
    size_t size;
    size_t offset;
};

Node_tree_status tokenizer_init (Tokenizer* tokenizer, FILE* input);

Node_tree_status tokenizer_destruct (Tokenizer* tokenizer);

Node_tree_status get_token_try_bracket(Tokenizer* tokenizer, Token* token);

Node_tree_status get_token_try_const(Tokenizer* tokenizer, Token* token);

Node_tree_status get_token_try_operation(Tokenizer* tokenizer, Token* token);

Node_tree_status get_token_try_variable(Tokenizer* tokenizer, Token* token);

Node_tree_status get_token(Tokenizer* tokenizer, Token* token);

//----------------------------------------Lexer end ----------------------------------------

//----------------------------------------Differ begin ----------------------------------------
Tree_node* differ_node(Tree_node* node, char var);

Tree* differ(Tree* tree, char var);

//----------------------------------------Differ end ----------------------------------------


#endif