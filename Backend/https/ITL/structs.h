#ifndef STRUCTS_H
#define STRUCTS_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>





#define COLOR_BLACK   "\e[0;30m"
#define COLOR_RED     "\e[0;31m"
#define COLOR_GREEN   "\e[0;32m"
#define COLOR_YELLOW  "\e[0;33m"
#define COLOR_BLUE    "\e[0;34m"
#define COLOR_MAGENTA "\e[0;35m"
#define COLOR_CYAN    "\e[0;36m"
#define COLOR_WHITE   "\e[0;37m"


#define BRIGHT_BLACK   "\e[0;90m"
#define BRIGHT_RED     "\e[0;91m"
#define BRIGHT_GREEN   "\e[0;92m"
#define BRIGHT_YELLOW  "\e[0;93m"
#define BRIGHT_BLUE    "\e[0;94m"
#define BRIGHT_MAGENTA "\e[0;95m"
#define BRIGHT_CYAN    "\e[0;96m"
#define BRIGHT_WHITE   "\e[0;97m"


#define BOLD_BLACK         "\e[1m\e[30m"
#define BOLD_RED           "\e[1m\e[31m"
#define BOLD_GREEN         "\e[1m\e[32m"
#define BOLD_YELLOW        "\e[1m\e[33m"
#define BOLD_BLUE          "\e[1m\e[34m"
#define BOLD_MAGENTA       "\e[1m\e[35m"
#define BOLD_CYAN          "\e[1m\e[36m"
#define BOLD_WHITE         "\e[1m\e[37m"

#define BOLD_BRIGHT_BLACK   "\e[1m\e[90m"
#define BOLD_BRIGHT_RED     "\e[1m\e[91m"
#define BOLD_BRIGHT_GREEN   "\e[1m\e[92m"
#define BOLD_BRIGHT_YELLOW  "\e[1m\e[93m"
#define BOLD_BRIGHT_BLUE    "\e[1m\e[94m"
#define BOLD_BRIGHT_MAGENTA "\e[1m\e[95m"
#define BOLD_BRIGHT_CYAN    "\e[1m\e[96m"
#define BOLD_BRIGHT_WHITE   "\e[1m\e[97m"

#define BRIGHT_ORANGE "\e[38;5;208m"
#define BRIGHTEST_GREEN "\e[38;2;0;255;0m"


#define COLOR_RESET "\e[0m"
#define BOLD        "\e[1m"
#define UNDERLINE   "\e[4m"
#define INVERSE     "\e[7m"

#define PP_FIELD 8


//______________________________DON'T USE THESE FUNCTIONS!!!______________________________________________________________________________________
//XXH64_hash_t * create_lookup_table(size_t size);


//char ** create_map(size_t size);

//void set_value(char * key, char * value, XXH64_hash_t * lookup_table, char ** hash_map, size_t size);

//char * get_value(char * key, XXH64_hash_t * lookup_table, char ** hash_map, size_t size);
/*

typedef struct {
  size_t size;
  XXH64_hash_t * lookup_table;
  char ** hash_map;
} HashMap;
//___________________USE THE NEXT ONES INSTEAD!_____________________________________________________________________________________________________



HashMap create_hashmap(size_t size){
  return (HashMap){.size=size, .lookup_table=create_lookup_table(size), .hash_map=create_map(size)};
}

void set(HashMap map, char * key, char * value){
  set_value(key, value, map.lookup_table, map.hash_map, map.size);
}

char * get(HashMap map, char * key){
  return get_value(key, map.lookup_table, map.hash_map, map.size);
}

long int get_index(HashMap map, char * key){  // check existance of key. -1 if doesn't exist. valid index if does.
  XXH64_hash_t _hash  = XXH3_64bits(key, strlen(key));
  size_t index = _hash % map.size;
  if(map.lookup_table[index] == _hash)
    return index;
  return -1;
}

char * get_value_from_index(HashMap map, long int index){
  return map.hash_map[index];
}
*/

//_________________________________________________________________________________________________________






//_________________________________________________________________________________________________________

typedef enum {
  TOK_LEFT_PAREN,        // (
  TOK_RIGHT_PAREN,       // )
  TOK_LEFT_BRACE,        // [
  TOK_RIGHT_BRACE,       // ]
  TOK_LEFT_CURLY,        // {
  TOK_RIGHT_CURLY,       // }
  TOK_COMMA,             // ,
  TOK_DOT,               // .
  TOK_MINUS,             // -
  TOK_PLUS,              // +
  TOK_SLASH,             // /
  TOK_STAR,              // *
  TOK_MODULO,            // %
  TOK_QUESTION_MARK,     // ?
  TOK_COLON,             // :
  TOK_SEMICOLON,         // ;
  TOK_BW_AND,            // &
  TOK_BW_OR,             // |
  TOK_BW_XOR,            // ^
  TOK_NOT,               // !
  TOK_ASSIGN,            // =
  TOK_GREATER,           // >
  TOK_LESS,              // <
  TOK_NOT_EQUAL,         // !=
  TOK_EQUAL_EQUAL,       // ==
  TOK_GREATER_EQUAL,     // >=
  TOK_LESS_EQUAL,        // <=
  TOK_BW_AND_ASSIGN,     // &=
  TOK_BW_OR_ASSIGN,      // |=
  TOK_BW_XOR_ASSIGN,     // ^=
  TOK_ADD_ASSIGN,        // +=
  TOK_SUBTRACT_ASSIGN,   // -=
  TOK_MULTIPLY_ASSIGN,   // *=
  TOK_DIVIDE_ASSIGN,     // /=
  TOK_MODULO_ASSIGN,     // %=
  TOK_LOGIC_AND,         // &&
  TOK_LOGIC_OR,          // ||
  IDENTIFIER, STRING_LITERAL, NUMBER_LITERAL,
  TOK_AND, TOK_ELSE, TOK_FALSE, TOK_FUNCTION, TOK_FOR, TOK_IF, TOK_NULL, TOK_OR,
  TOK_PRINT, TOK_RETURN, TOK_TRUE, TOK_VAR, TOK_WHILE, TOK_INVALID, TOK_EOF,
  TOK_BREAK, TOK_CONTINUE,

  TOK_ERROR_UNTERM_COMMENT,
  
  TOK_CLASS, TOK_SUPER, TOK_THIS, TOK_NEW
} TokenType;

typedef struct {
    TokenType type;
    size_t start, length, line, col;
} Token;


typedef enum {
  LITERAL,
  VARIABLE,
  SEQUENCE,
  UNARY,
  BINARY,
  TERNARY,
  ASSIGNMENT,
  GROUP,
  STATEMENT,
  INDEX,
  CONTROL_FLOW,
  ERROR,
  CALL,
  FUNCTION,
  CLASS,
  OBJECT,
  DELETE_ME
} NodeType;

typedef enum {
  OP_ADD,  // +
  OP_SUBTRACT, // -
  OP_MULTIPLY, // *
  OP_DIVISION,  // /
  OP_MODULO,  // %
  OP_EQUAL,  //==
  OP_NOT_EQUAL, // !=
  OP_LESS,  // <
  OP_LESS_EQUAL,  // <=
  OP_GREATER,  // >
  OP_GREATER_EQUAL, // >=
  OP_LOGIC_OR,   // ||
  OP_LOGIC_AND,  // &&
  OP_BITWISE_OR, // |
  OP_BITWISE_XOR,// ^
  OP_BITWISE_AND,// &
  OP_UNARY_MINUS, // -()
  OP_UNARY_NOT, // !()
  NOOP,
  OP_ADD_AFTER,
  OP_SUBTRACT_AFTER
} Operation;


typedef enum {
  NO_SEMICOLON,
  NO_RIGHT_PAREN,
  NO_LEFT_PAREN,
  MISSING_COLON,
  INVALID_ASSIGNMENT_TARGET,
  NO_RIGHT_BRACE,
  TYPE_MISMATCH,
  UNDECLARED_IDENTIFIER,
  NOT_IMPLEMENTED,
  TOKEN_SYNC_ERROR,
  UNMATCHED_RIGHT_CURLY_BRACE,
  INVALID_ENUMERABLE,
  INDEX_OUT_OF_BOUNDS,
  BREAK_NOT_IN_LOOP,
  CONTINUE_NOT_IN_LOOP,
  PARSER_ERROR,
  UNTERMINATED_COMMENT_BLOCK,
  UNTERMINATED_BLOCK,
  EXPECTED_LITERAL,
  EXPECTED_PRIMARY,
  EXPECTED_INDEX,
  EXPECTED_CALL,
  EXPECTED_POSTFIX,
  EXPECTED_UNARY,
  EXPECTED_FACTOR,
  EXPECTED_TERM,
  INVALID_LEFT_HAND,
  INVALID_RIGHT_HAND,
  EXPECTED_EXP,
  EXPECTED_BLOCK,
  EXPECTED_VAR_NAME,
  EXPECTED_FUNCTION_NAME,
  EXPECTED_IDENTIFIER,
  UNEXPECTED_LITERAL,
  EXPECTED_STATEMENT,
  ERROR_NAN,
  EXPECTED_NUMBER,
  SYNTAX_ERROR,
  ZERO_DIVISION,
  TOO_MANY_ARGS,
  MISSING_ARGS,
  MISSING_CONSTRUCTOR,
  UNEXPECTED_EOI,
} Error;

typedef enum {
    INT,
    STRING,
    NILL
} DataType;

typedef enum {
    DT_VAR,
    DT_FUNCTION,
    DT_CLASS
} DeclarationType;

typedef struct {
  DataType type;
  union{
    long double num;
    char * str;
  };
} Literal;





typedef enum {
    EXPRESSION, 
    BLOCK, 
    IF_STMT, 
    WHILE_STMT, 
    FOR_STMT, 
    RETURN_STMT,
    DECLARATION,
    PRINT
} StatementType;

typedef enum {
  CF_BREAK,
  CF_CONTINUE,
  CF_RETURN,
  CF_ERROR,
  CF_DONE
} ControlFlowType;

typedef enum {
  METHOD,
  CONSTRUCT,
  CALL_FUNCTION
} CallType;

typedef struct NodeMap NodeMap;

typedef struct Node Node;
//Abstract Syntax Tree
//typedef struct Scope Scope;

typedef struct{
  Node * type;
  NodeMap * fields;
} Object;

struct Node {
    NodeType type;
    Token token;

    union {
        
        Literal literal;
        char * variable;
        Object object;

        struct {
          CallType type;
          Node * callee;  // method callee | function name | class type constructor
          union{
            struct {
              Node * caller;
            } method;
            struct {
              Node ** args;
              size_t argc;
            } function;
            struct {
              Node ** args;
              size_t argc;
            } construct;
          };
        } call;
        
        struct {
            char * name;
            Node** params;
            size_t arity;
            Node * body;
        } function_definiton;  //definition
        
         struct {
            char * name;  
            Node * constructor;  //construct method
            Node ** methods;  
            size_t method_count;
        } class_definiton;  //definition and constructor
        

        //new_node(.type=CLASS, .class.name=class_name, .class.methods=methods, .class.method_count = count);
        struct {
          Node * exp;
          Token token_start;
          Token token_end;
        } group;
        
        struct {
          Node ** exp;
          size_t count;
          Token token;//_start;
          //Token token_end;
        } sequence;

        struct {
          ControlFlowType type;
          Node * carry_back;
        } control_flow;

        struct {
            Node* right;
            Operation op;
            Token token_right;
            Token token_prefix;
        } unary;

        struct {
            Node* left;
            Node* right;
            Operation op;

            Token token_left;
            Token token_right;
            Token token_op;
        } binary;

        struct {
            Node* condition;
            Node* true_branch;
            Node* false_branch;
            
            Token token_condition;
            Token token_true_block;
            Token token_false_block;
        } ternary;
      
        struct {
          StatementType type;
          Token token_statement;
          union{
            Node* exp;           // For expression statements

            struct {
                Node ** statements;
                size_t count;
            } block;

            struct {
                Node* condition;
                Node* then_branch;
                Node* else_branch;
                Token token_condition;
                Token token_true_block;
                Token token_false_block;
            } if_statement;

            struct {
                Node* condition;
                Node* body;
                Token token_condition;
            } while_statement;

            struct {
                Node* initializer;
                Node* condition;
                Node* increment;
                Node* body;
                Token token_initializer;
                Token token_condition;
                Token token_increment;
                Token token_body;
            } for_statement;

            struct {
                Node* value;
                Token token_return_value;
            } return_statement;
            
            struct {
                Node* exp;
                Token token_exp;
            } print;

            struct {
                DeclarationType type;
                char* name;
                Node* initializer;
                Token token_target;
                Token token_initializer;
            } declaration;

            

          };
        } statement;      

        struct {
            Node * target;
            Node * value;
            Operation op;
            Token token_target;
            Token token_value;
        } assignment;
        
        struct {
            Node * target;
            Node * index;
            Token token_target;
            Token token_index;
        } index;

        struct {
          Error type;  // if p_() == ERROR   return new_node(error, inner=p_(), token from field)
          Node * inner;
          Token token;
        } error;
    };
};



struct NodeMap {
  size_t size;
  size_t capacity;
  XXH64_hash_t * lookup_table;
  Node ** hash_map;
} ;


void destroy_nodemap(NodeMap * map){
  free(map->lookup_table);
  for(size_t i = 0; i < map->capacity; i++){
    if(map->hash_map[i] != NULL)
      free(map->hash_map[i]);
  }
  free(map->hash_map);
  map->size=0;
  map->capacity=0;
  free(map);
}

typedef struct Scope Scope;
struct Scope{

    struct{
      NodeMap * classes;
    } global;

    struct{
      Scope * parent;
      NodeMap * vars; 
      NodeMap * functions;
    }current;
};


#define new_node(...) new_node_((Node){__VA_ARGS__})
Node *new_node_(Node x) {
   Node *res = malloc(sizeof(Node));
   *res = x;
   return res;
}

#define DEFAULT_CAPACITY 32

NodeMap * create_node_map(size_t capacity);

NodeMap * create_new_node_map(){
  return create_node_map(DEFAULT_CAPACITY);
}

NodeMap * create_node_map(size_t capacity){
  NodeMap * map = malloc(sizeof(NodeMap));
  map->size=0;
  map->capacity=capacity;
  map->lookup_table=(XXH64_hash_t *) calloc(capacity,(sizeof(XXH64_hash_t)));
  map->hash_map=(Node **) calloc(capacity,(sizeof(Node *)));
  return map;
}

void expand_node_map(NodeMap * map){
  NodeMap * copy = create_node_map(map->capacity* 2);  //needs free
  //XXH64_hash_t lut = *map->lookup_table;
 // Node ** hm 

  //map->size=copy.size;
  //map->capacity= copy.capacity * 2;
  //free(map->lookup_table);
  //map->lookup_table=(XXH64_hash_t *) realloc(map->lookup_table,(map->capacity)*(sizeof(XXH64_hash_t)));
  //free(map->hash_map);
  //map->hash_map=(Node **) realloc(map->hash_map,(map->capacity)*(sizeof(Node *)));

  //map = create_node_map(map->capacity);
  //map->size = copy.size;

  for(size_t i = 0; i < map->capacity; i++){
    if(map->hash_map[i] != NULL){
      //XXH64_hash_t _hash  = XXH3_64bits(map->lookup_table[i], strlen(map->hash_map[i]));
      size_t new_index = map->lookup_table[i] % copy->capacity;
      //size_t new_index = map->lookup_table[i] % new_capacity;
      //("hash: %llu\n", map->lookup_table[i]);
      copy->lookup_table[new_index] = map->lookup_table[i];
      copy->hash_map[new_index] = map->hash_map[i];
      //free(map->hash_map[i]);
    }
  }
  free(map->lookup_table);
  free(map->hash_map);
  map->lookup_table = copy->lookup_table;
  map->hash_map = copy->hash_map;
  map->capacity = copy->capacity;
  
 // map
  //free(map->lookup_table);
  //free(map->hash_map);
 // *map = *new_map;
}


Node * get_node(NodeMap * map, char * key){
  XXH64_hash_t _hash  = XXH3_64bits(key, strlen(key));
  size_t index = _hash % map->capacity;
  if(map->lookup_table[index] == _hash)
    return map->hash_map[index];
  return NULL;
}


long int get_node_index(NodeMap * map, char * key){
  XXH64_hash_t _hash  = XXH3_64bits(key, strlen(key));
  size_t index = _hash % map->capacity;
  if(map->lookup_table[index] == _hash)
    return index;
  return -1;
}

void set_node(NodeMap * map, char * name, Node * node){
  if(get_node_index(map, name) == -1){  // new entry
    
    if(map->size+1 > map->capacity){
      //("EXPANDING\n");
      expand_node_map(map);
      //("EXPANDed %ld %ld\n", map->size, map->capacity);
      //set_node(map, name, node);

      //return;
      
    }
    map->size++;
    XXH64_hash_t _hash  = XXH3_64bits(name, strlen(name));
    //("insert: %ld/%ld %llu\n--", map->size, map->capacity, _hash);
    
    size_t index = _hash % map->capacity;
    map->lookup_table[index] = _hash;
    map->hash_map[index] = node;
   // for(size_t i = 0; i < map->capacity; i++){
      //(" %llu ", map->lookup_table[i]);
    //}
    //("\n");
  }else{
    XXH64_hash_t _hash  = XXH3_64bits(name, strlen(name));
    size_t index = _hash % map->capacity;
    if(map->lookup_table[index] == _hash){
      map->hash_map[index] = node;
    }else{
      fprintf(stderr, "%s%s%s", BRIGHT_RED, "TACTICAL NUKE INCOMING!\nThe day has come... hashing collisions.\nTried to assign a value to a key whose hash is found however lookup table hash doesn't match\nExiting now\n", COLOR_RESET);
      exit(0);
    }
      
    return;
  }
 // (stder, "%llu\n", map->lookup_table[0]);
}

void destroy_current_scope(Scope * scope){
  destroy_nodemap(scope->current.vars);
  destroy_nodemap(scope->current.functions);
  free(scope);
}


void destroy_scope(Scope * scope){
  if(!scope)
    return;
  // if(scope->current.vars)
  if(scope->current.parent)
    printf("parent alive\n");
  //   destroy_nodemap(scope->current.vars);
  // if(scope->current.functions)
  //   destroy_nodemap(scope->current.functions);
  // if(scope->global.classes)
  //   destroy_nodemap(scope->global.classes);
  free(scope);
}
 
// check if exists
// (if doesnt exist, and size<capacity)insert new, size++, or update if exists, or realloc
// check size
// get
// resize by factor of 2




XXH64_hash_t * create_lookup_table(size_t size){
  return (XXH64_hash_t *) malloc(size*(sizeof(XXH64_hash_t)));
}

Node ** create_map(size_t size){
  return (Node **) malloc(size*(sizeof(Node *)));
}

void set_value(char * key, Node * value, XXH64_hash_t * lookup_table, Node ** hash_map, size_t size){
  XXH64_hash_t _hash  = XXH3_64bits(key, strlen(key));
  size_t index = _hash % size;
  lookup_table[index] = _hash;
  hash_map[index] = value;
}

Node * get_value(char * key, XXH64_hash_t * lookup_table, Node ** hash_map, size_t size){
  XXH64_hash_t _hash  = XXH3_64bits(key, strlen(key));
  size_t index = _hash % size;
  if(lookup_table[index] == _hash)
    return hash_map[index];
  return NULL;
}












char * literal_type(Literal * literal);
char * ctrl_type(ControlFlowType type);
char * stmt_type(StatementType type);
char *op_to_str(Operation op);
char *error_to_str(Error err );
char *token_to_str(Token tok);


char * literal_type(Literal * literal){
  switch(literal->type){
        case STRING:   return "STRING";
        case INT:      return "INT";
        case NILL:     return "NULL";
        default:       return "UNKNOWN TYPE";
      }
}


char * ctrl_type(ControlFlowType type){
  switch(type){
    case CF_BREAK:     return "CF_BREAK";
    case CF_CONTINUE:  return "CF_CONTINUE";
    case CF_RETURN:    return "CF_RETURN";
    case CF_ERROR:     return "CF_ERROR";
    case CF_DONE:      return "CF_DONE";
    default:           return "CF_UNKNOWN";
  }
}

char * stmt_type(StatementType type){
  switch(type){
    case EXPRESSION:    return "EXPRESSION";
    case BLOCK:         return "BLOCK";
    case IF_STMT:       return "IF_STMT";
    case WHILE_STMT:    return "WHILE_STMT";
    case FOR_STMT:      return "FOR_STMT";
    case RETURN_STMT:   return "RETURN_STMT";
    case DECLARATION:   return "DECLARATION";
    case PRINT:         return "PRINT";
    default:            return "UNKNOWN_STATEMENT";
  }
}



char *op_to_str(Operation op) {
  switch (op) {
    case OP_ADD:             return "ADD";             // +
    case OP_SUBTRACT:        return "SUBTRACT";        // -
    case OP_MULTIPLY:        return "MULTIPLY";        // *
    case OP_DIVISION:        return "DIVISION";        // /
    case OP_MODULO:          return "MODULO";          // %
    case OP_EQUAL:           return "EQUAL";           // ==
    case OP_NOT_EQUAL:       return "NOT_EQUAL";       // !=
    case OP_LESS:            return "LESS";            // <
    case OP_LESS_EQUAL:      return "LESS_EQUAL";      // <=
    case OP_GREATER:         return "GREATER";         // >
    case OP_GREATER_EQUAL:   return "GREATER_EQUAL";   // >=
    case OP_LOGIC_OR:        return "LOGIC_OR";        // ||
    case OP_LOGIC_AND:       return "LOGIC_AND";       // &&
    case OP_BITWISE_OR:      return "BITWISE_OR";      // |
    case OP_BITWISE_XOR:     return "BITWISE_XOR";     // ^
    case OP_BITWISE_AND:     return "BITWISE_AND";     // &
    case OP_UNARY_MINUS:     return "UNARY_MINUS";     // -()
    case OP_UNARY_NOT:       return "UNARY_NOT";       // !()
    case NOOP:               return "NOOP";            // no operation
    case OP_ADD_AFTER:       return "OP_ADD_AFTER";
    case OP_SUBTRACT_AFTER:  return "OP_SUBTRACT_AFTER";
    default:                 return "UNKNOWN_OP";
  }
}


char *error_to_str(Error err ){
  switch(err){
    case NO_SEMICOLON:                  return "NO_SEMICOLON";
    case NO_RIGHT_PAREN:                return "NO_RIGHT_PAREN";
    case MISSING_COLON:                 return "MISSING_COLON";
    case INVALID_ASSIGNMENT_TARGET:     return "INVALID_ASSIGNMENT_TARGET";
    case NO_RIGHT_BRACE:                return "NO_RIGHT_BRACE";
    case TYPE_MISMATCH:                 return "TYPE_MISMATCH";
    case UNDECLARED_IDENTIFIER:         return "UNDECLARED_IDENTIFIER";
    case NOT_IMPLEMENTED:               return "NOT_IMPLEMENTED";
    case TOKEN_SYNC_ERROR:              return "TOKEN_SYNC_ERROR";
    case UNMATCHED_RIGHT_CURLY_BRACE:   return "UNMATCHED_RIGHT_CURLY_BRACE";
    case INVALID_ENUMERABLE:            return "INVALID_ENUMERABLE";
    case INDEX_OUT_OF_BOUNDS:           return "INDEX_OUT_OF_BOUNDS";
    case NO_LEFT_PAREN:                 return "NO_LEFT_PAREN";
    case BREAK_NOT_IN_LOOP:             return "BREAK_NOT_IN_LOOP";
    case CONTINUE_NOT_IN_LOOP:          return "CONTINUE_NOT_IN_LOOP";
    case PARSER_ERROR:                  return "PARSER_ERROR";
    case UNTERMINATED_COMMENT_BLOCK:    return "UNTERMINATED_COMMENT_BLOCK";
    case EXPECTED_LITERAL:              return "EXPECTED_LITERAL";
    case EXPECTED_PRIMARY:              return "EXPECTED_PRIMARY";
    case EXPECTED_INDEX:                return "EXPECTED_INDEX";
    case EXPECTED_CALL:                 return "EXPECTED_CALL";
    case EXPECTED_POSTFIX:              return "EXPECTED_POSTFIX";
    case EXPECTED_UNARY:                return "EXPECTED_UNARY";
    case EXPECTED_FACTOR:               return "EXPECTED_FACTOR";
    case EXPECTED_TERM:                 return "EXPECTED_TERM";
    case INVALID_LEFT_HAND:             return "INVALID_LEFT_HAND";
    case INVALID_RIGHT_HAND:            return "INVALID_RIGHT_HAND";
    case EXPECTED_EXP:                  return "EXPECTED_EXP";
    case EXPECTED_BLOCK:                return "EXPECTED_BLOCK";
    case EXPECTED_VAR_NAME:             return "EXPECTED_VAR_NAME";
    case EXPECTED_FUNCTION_NAME:        return "EXPECTED_FUNCTION_NAME";
    case UNEXPECTED_LITERAL:            return "UNEXPECTED_LITERAL";
    case UNEXPECTED_EOI:                return "UNEXPECTED_EOI";
    case SYNTAX_ERROR:                  return "SYNTAX_ERROR";
    case EXPECTED_STATEMENT:            return "EXPECTED_STATEMENT";
    case ERROR_NAN:                     return "ERROR_NAN";
    case UNTERMINATED_BLOCK:            return "UNTERMINATED_BLOCK";
    case ZERO_DIVISION:                 return "ZERO_DIVISION";
    case EXPECTED_NUMBER:               return "EXPECTED_NUMBER";
    case TOO_MANY_ARGS:                 return "TOO_MANY_ARGS";
    case MISSING_ARGS:                  return "MISSING_ARGS";
    case EXPECTED_IDENTIFIER:           return "EXPECTED_IDENTIFIER";
    case MISSING_CONSTRUCTOR:           return "MISSING_CONSTRUCTOR";
    default:                            return "UNKNOWN_ERROR"; 
  }

}

char * token_to_str(Token tok) {
  switch (tok.type) {
    case TOK_LEFT_PAREN:            return "TOK_LEFT_PAREN";         // (
    case TOK_RIGHT_PAREN:           return "TOK_RIGHT_PAREN";        // )
    case TOK_LEFT_BRACE:            return "TOK_LEFT_BRACE";         // [
    case TOK_RIGHT_BRACE:           return "TOK_RIGHT_BRACE";        // ]
    case TOK_LEFT_CURLY:            return "TOK_LEFT_CURLY";         // {
    case TOK_RIGHT_CURLY:           return "TOK_RIGHT_CURLY";        // }
    case TOK_COMMA:                 return "TOK_COMMA";              // ,
    case TOK_DOT:                   return "TOK_DOT";                // .
    case TOK_MINUS:                 return "TOK_MINUS";              // -
    case TOK_PLUS:                  return "TOK_PLUS";               // +
    case TOK_SLASH:                 return "TOK_SLASH";              // /
    case TOK_STAR:                  return "TOK_STAR";               // *
    case TOK_MODULO:                return "TOK_MODULO";             // %
    case TOK_QUESTION_MARK:         return "TOK_QUESTION_MARK";      // ?
    case TOK_COLON:                 return "TOK_COLON";              // :
    case TOK_SEMICOLON:             return "TOK_SEMICOLON";          // ;
    case TOK_BW_AND:                return "TOK_BW_AND";             // &
    case TOK_BW_OR:                 return "TOK_BW_OR";              // |
    case TOK_BW_XOR:                return "TOK_BW_XOR";             // ^
    case TOK_NOT:                   return "TOK_NOT";                // !
    case TOK_ASSIGN:                return "TOK_ASSIGN";             // =
    case TOK_GREATER:               return "TOK_GREATER";            // >
    case TOK_LESS:                  return "TOK_LESS";               // <
    case TOK_NOT_EQUAL:             return "TOK_NOT_EQUAL";          // !=
    case TOK_EQUAL_EQUAL:           return "TOK_EQUAL_EQUAL";        // ==
    case TOK_GREATER_EQUAL:         return "TOK_GREATER_EQUAL";      // >=
    case TOK_LESS_EQUAL:            return "TOK_LESS_EQUAL";         // <=
    case TOK_BW_AND_ASSIGN:         return "TOK_BW_AND_ASSIGN";      // &=
    case TOK_BW_OR_ASSIGN:          return "TOK_BW_OR_ASSIGN";       // |=
    case TOK_BW_XOR_ASSIGN:         return "TOK_BW_XOR_ASSIGN";      // ^=
    case TOK_ADD_ASSIGN:            return "TOK_ADD_ASSIGN";         // +=
    case TOK_SUBTRACT_ASSIGN:       return "TOK_SUBTRACT_ASSIGN";    // -=
    case TOK_MULTIPLY_ASSIGN:       return "TOK_MULTIPLY_ASSIGN";    // *=
    case TOK_DIVIDE_ASSIGN:         return "TOK_DIVIDE_ASSIGN";      // /=
    case TOK_MODULO_ASSIGN:         return "TOK_MODULO_ASSIGN";      // %=
    case TOK_LOGIC_AND:             return "TOK_LOGIC_AND";          // &&
    case TOK_LOGIC_OR:              return "TOK_LOGIC_OR";           // ||
    case IDENTIFIER:                return "IDENTIFIER";             // identifier
    case STRING_LITERAL:            return "STRING_LITERAL";         // string
    case NUMBER_LITERAL:            return "NUMBER_LITERAL";         // number
    case TOK_AND:                   return "TOK_AND";                // and
    case TOK_ELSE:                  return "TOK_ELSE";               // else
    case TOK_FALSE:                 return "TOK_FALSE";              // false
    case TOK_FUNCTION:              return "TOK_FUNCTION";           // function
    case TOK_FOR:                   return "TOK_FOR";                // for
    case TOK_IF:                    return "TOK_IF";                 // if
    case TOK_NULL:                  return "TOK_NULL";               // null
    case TOK_OR:                    return "TOK_OR";                 // or
    case TOK_PRINT:                 return "TOK_PRINT";              // print
    case TOK_RETURN:                return "TOK_RETURN";             // return
    case TOK_TRUE:                  return "TOK_TRUE";               // true
    case TOK_VAR:                   return "TOK_VAR";                // var
    case TOK_WHILE:                 return "TOK_WHILE";              // while
    case TOK_BREAK:                 return "TOK_BREAK";              // break
    case TOK_CONTINUE:              return "TOK_CONTINUE";           // continue
    case TOK_CLASS:                 return "TOK_CLASS";              // class
    case TOK_SUPER:                 return "TOK_SUPER";              // super
    case TOK_THIS:                  return "TOK_THIS";               // this
    case TOK_NEW:                   return "TOK_NEW";               // new
    case TOK_INVALID:               return "TOK_INVALID";            // invalid
    case TOK_EOF:                   return "TOK_EOF";                // end of file
    case TOK_ERROR_UNTERM_COMMENT:  return "TOK_ERROR_UNTERM_COMMENT";                // end of file
    default:                        return "UNKNOWN_TOKEN";
  }
}


Scope * scope;

//____________________INTEGRATION BEGINS. GOOD LUCK___________________________


char *methods[9] = {"OPTIONS", "DELETE", "CONNECT", "GET", "PUT", "PATCH", "TRACE", "HEAD", "POST"};
int methods_hash[9] = {-2052162639,-1417066568,-415991569,193456677,193467006,231455573,236789492,2089138423,2089437419};

typedef struct {
    const char *extension;
    const char *mime_type;
} MimeType;







static const MimeType mime_map[] = {
    { "aac", "audio/aac" },
    { "abw", "application/x-abiword" },
    { "arc", "application/x-freearc" },
    { "avi", "video/x-msvideo" },
    { "azw", "application/vnd.amazon.ebook" },
    { "bin", "application/octet-stream" },
    { "bmp", "image/bmp" },
    { "bz", "application/x-bzip" },
    { "bz2", "application/x-bzip2" },
    { "csh", "application/x-csh" },
    { "css", "text/css" },
    { "csv", "text/csv" },
    { "doc", "application/msword" },
    { "docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document" },
    { "eot", "application/vnd.ms-fontobject" },
    { "epub", "application/epub+zip" },
    { "gz", "application/gzip" },
    { "gif", "image/gif" },
    { "htm", "text/html" },
    { "html", "text/html" },
    { "ico", "image/vnd.microsoft.icon" },
    { "ics", "text/calendar" },
    { "jar", "application/java-archive" },
    { "jpeg", "image/jpeg" },
    { "jpg", "image/jpeg" },
    { "js", "application/javascript" },
    { "json", "application/json" },
    { "jsonld", "application/ld+json" },
    { "mid", "audio/midi" },
    { "midi", "audio/midi" },
    { "mjs", "text/javascript" },
    { "mp3", "audio/mpeg" },
    { "mp4", "video/mp4" },
    { "mpeg", "video/mpeg" },
    { "mpkg", "application/vnd.apple.installer+xml" },
    { "odp", "application/vnd.oasis.opendocument.presentation" },
    { "ods", "application/vnd.oasis.opendocument.spreadsheet" },
    { "odt", "application/vnd.oasis.opendocument.text" },
    { "oga", "audio/ogg" },
    { "ogv", "video/ogg" },
    { "ogx", "application/ogg" },
    { "opus", "audio/opus" },
    { "otf", "font/otf" },
    { "png", "image/png" },
    { "pdf", "application/pdf" },
    { "php", "application/x-httpd-php" },
    { "ppt", "application/vnd.ms-powerpoint" },
    { "pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation" },
    { "rar", "application/vnd.rar" },
    { "rtf", "application/rtf" },
    { "sh", "application/x-sh" },
    { "svg", "image/svg+xml" },
    { "tar", "application/x-tar" },
    { "tif", "image/tiff" },
    { "tiff", "image/tiff" },
    { "ts", "video/mp2t" },
    { "ttf", "font/ttf" },
    { "txt", "text/plain" },
    { "vsd", "application/vnd.visio" },
    { "wav", "audio/wav" },
    { "weba", "audio/webm" },
    { "webm", "video/webm" },
    { "webp", "image/webp" },
    { "woff", "font/woff" },
    { "woff2", "font/woff2" },
    { "xhtml", "application/xhtml+xml" },
    { "xls", "application/vnd.ms-excel" },
    { "xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" },
    { "xml", "application/xml" },
    { "xul", "application/vnd.mozilla.xul+xml" },
    { "zip", "application/zip" },
    { "3gp", "video/3gpp" },
    { "3g2", "video/3gpp2" },
    { "7z", "application/x-7z-compressed" },
    { NULL, NULL }
};


#endif

/*
expression     → literal
               | unary
               | binary
               | grouping ;

literal        → NUMBER | STRING | "true" | "false" | "nil" ;
grouping       → "(" expression ")" ;
unary          → ( "-" | "!" ) expression ;
binary         → expression operator expression ;
operator       → "==" | "!=" | "<" | "<=" | ">" | ">="
               | "+"  | "-"  | "*" | "/" ;





program        → statement* EOF ;

statement      → exprStmt
               | printStmt ;

exprStmt       → expression ";" ;
printStmt      → "print" expression ";" ;








program        → declaration* EOF ;

declaration    → varDecl
               | statement ;

statement      → exprStmt
               | printStmt ;






varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;

primary        → "true" | "false" | "nil"
               | NUMBER | STRING
               | "(" expression ")"
               | IDENTIFIER ;



____________________________________________________________________________________________________
%APPDATA%\Code\User\settings.json
____________________________________________________________________________________________________
{
    "security.workspace.trust.untrustedFiles": "open",
    "editor.minimap.enabled": false,

    "terminal.integrated.defaultProfile.windows": "PowerShell",
    "terminal.integrated.profiles.windows": {
      "PowerShell": {
        "path": "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe"
      },
      "SSH to VM": {
        "path": "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe",
        "args": [ "-Command", "ssh vm"] //"-NoExit",
      }
    },
    "terminal.integrated.defaultProfile.windows": "SSH to VM",
    "files.autoSave": "afterDelay"

}
_______________________________________________________________________________________________________________________________________________________
alias r='gcc -o /home/tc/itlc/parser/r /home/tc/itlc/parser/itl.c /home/tc/itlc/parser/xxhash.c && cat /home/tc/itlc/parser/t | /home/tc/itlc/parser/r'
_______________________________________________________________________________________________________________________________________________________




_______________________________________________________________________________________________________________________________________________________
SSHD_CONFIG  /usr/local/etc/ssh/sshd_config
_______________________________________________________________________________________________________________________________________________________
#       $OpenBSD: sshd_config,v 1.104 2021/07/02 05:11:21 dtucker Exp $

# This is the sshd server system-wide configuration file.  See
# sshd_config(5) for more information.

# This sshd was compiled with PATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin

# The strategy used for options in the default sshd_config shipped with
# OpenSSH is to specify options with their default value where
# possible, but leave them commented.  Uncommented options override the
# default value.

Port 22
#AddressFamily any
ListenAddress 0.0.0.0
#ListenAddress ::

HostKey /usr/local/etc/ssh/ssh_host_rsa_key
#HostKey /usr/local/etc/ssh/ssh_host_ecdsa_key
#HostKey /usr/local/etc/ssh/ssh_host_ed25519_key

# Ciphers and keying
#RekeyLimit default none

# Logging
#SyslogFacility AUTH
#LogLevel INFO

# Authentication:

#LoginGraceTime 2m
#PermitRootLogin prohibit-password
#StrictModes yes
#MaxAuthTries 6
#MaxSessions 10

PubkeyAuthentication yes

# The default is to check both .ssh/authorized_keys and .ssh/authorized_keys2
# but this is overridden so installations will only check .ssh/authorized_keys
AuthorizedKeysFile      .ssh/authorized_keys

#AuthorizedPrincipalsFile none

#AuthorizedKeysCommand none
#AuthorizedKeysCommandUser nobody

# For this to work you will also need host keys in /usr/local/etc/ssh/ssh_known_hosts
#HostbasedAuthentication no
# Change to yes if you don't trust ~/.ssh/known_hosts for
# HostbasedAuthentication
#IgnoreUserKnownHosts no
# Don't read the user's ~/.rhosts and ~/.shosts files
#IgnoreRhosts yes

# To disable tunneled clear text passwords, change to no here!
PasswordAuthentication yes
#PermitEmptyPasswords no

# Change to no to disable s/key passwords
#KbdInteractiveAuthentication yes

# Kerberos options
#KerberosAuthentication no
#KerberosOrLocalPasswd yes
#KerberosTicketCleanup yes
#KerberosGetAFSToken no

# GSSAPI options
#GSSAPIAuthentication no
#GSSAPICleanupCredentials yes

# Set this to 'yes' to enable PAM authentication, account processing,
# and session processing. If this is enabled, PAM authentication will
# be allowed through the KbdInteractiveAuthentication and
# PasswordAuthentication.  Depending on your PAM configuration,
# PAM authentication via KbdInteractiveAuthentication may bypass
# the setting of "PermitRootLogin prohibit-password".
# If you just want the PAM account and session checks to run without
# PAM authentication, then enable this but set PasswordAuthentication
# and KbdInteractiveAuthentication to 'no'.
#UsePAM no

#AllowAgentForwarding yes
#AllowTcpForwarding yes
#GatewayPorts no
#X11Forwarding no
#X11DisplayOffset 10
#X11UseLocalhost yes
#PermitTTY yes
#PrintMotd yes
#PrintLastLog yes
#TCPKeepAlive yes
#PermitUserEnvironment no
#Compression delayed
#ClientAliveInterval 0
#ClientAliveCountMax 3
#UseDNS no
#PidFile /var/run/sshd.pid
#MaxStartups 10:30:100
#PermitTunnel no
#ChrootDirectory none
#VersionAddendum none

# no default banner path
#Banner none

# override default of no subsystems
Subsystem       sftp    /usr/local/lib/openssh/sftp-server

# Example of overriding settings on a per-user basis
#Match User anoncvs
#       X11Forwarding no
#       AllowTcpForwarding no
#       PermitTTY no
#       ForceCommand cvs server

___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________
.ssh/authorized_keys
___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAACAQC3it8SkeTqqjmym5HrYHqVXL2ZvUBA6AxDJw4d5Ob/CTBMM3DwrrtNaVC6uDAf7vyyoJm/+hGEr72IDFQ9mcx3l2PM4d7Le+K9dNdi/iTWy4ErBe7Z6onFFYNKxbFTgx1xWrkEqxdZf+Y0/gzVkO//k1s2xL3Au9VHAYmHaprQn24Ox6BOYhp75V/W3utSLEtohgKq8RfSrWWBVz866CnTOr1gQ6X85sfM+uROa2jJswizGI5Gk9xm5UYY9eozdAwCY9Z43rUNwk78cskkjvYoD6ZERes/ox93picffMtB92d3daKlNevaGY74u0xPfY8qXquW+sU/eEuaU0duD+E4caltnZCUvuFQZdsuQ8eTX9k8tYCqosBm1WEoc8u0qANM7jkEVK1zuA/T0slKIneX/sWmuk3CyrMxdacrUhC9+RmNlvYVxhfNgnb08eZYty+xFDSgw3E3PXNTiVE8TLslF+Apnkvh4WUHbVilRYH+a0danXiE44ojjHSKsjrKaDMgwDPVNbnjq+gZ4n9aR+ACxTgjTDYCdJTVklGZDD0nba8guq71217Hh1YQ+/x2wzC5ro3ceY2caS3IBQrLR9exhdftVnqVwpnoCodT6p9ccHacU42Lo4BKDq2Ud9j5SJe3wfAAMYUdak33jKBIZcZ0rtwsuKzbx0ZHCvG8fu1PiQ== itamar
___________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________
*/