
#ifndef PRETTY_PRINT_H
#define PRETTY_PRINT_H



#include <stdarg.h>


void sanitize(char *str);
void dump_vars();
char * color(Token token);

#define colors(indent, color, format, ...) colors_(indent, color, format, __VA_ARGS__)
size_t last_indent = 0;

void colors_(int indent, const char * color, const char * format, ...) {

  
  if(indent >= 0){
    size_t num = indent;
    size_t digits = 0;

    while(num){
      digits++;
      num /= 10;
    }

    fprintf(stdout, "%ld|", indent);
    for(size_t i = 0; indent > 0 && ( i < indent * 2); i++){
      if(i >= indent * 2 - 2)
        fprintf(stdout, "-");
      else if((i == indent * 2 - 3) && last_indent > indent)
        fprintf(stdout, " ");
      else
        fprintf(stdout, " ");
    }
    /*
    if(indent > digits){
      for(size_t i = 0; i < indent-digits+2; i++ ){
        if(i == indent-digits+1){
          if(last_indent > (size_t)indent)
            fprintf(stdout, "^ ", color);
          else
            fprintf(stdout, "-", color);
        }else{
          if(i == (size_t)(last_indent-digits+1)) // && last_indent > (size_t)indent) // != (size_t)(indent+1))
            fprintf(stdout, "  ", color);
          else
            fprintf(stdout, "  ", color);
        }  
    }
    }else{
     // if(indent == 1)
     //   fprintf(stdout, " ", color);
      
      for(size_t i = 0; indent > 0 && ( i < indent * 2); i++){
        if( (last_indent == (size_t)indent && i == indent))
            fprintf(stdout, "^", color);
        else if(i >= indent * 2-2){
          if(i == 0)
            fprintf(stdout, "^", color);
          else
            fprintf(stdout, "-", color);
        }else
          fprintf(stdout, " ", color);
      }
      //fprintf(stdout, "-", color);
    }*/
    last_indent = (size_t)indent;
  }

  fprintf(stdout, "%s", color);
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  fprintf(stdout, COLOR_RESET);
  
  return;
    /*char * str = malloc(sizeof(char) * (strlen(format)+strlen(color)+1));
    strcpy(str, format);
    //strcat(str, color);
    
    (stdout, "%s_\n", str);

    va_list args;
    va_start(args, color);

    // Determine required buffer size
    va_list args_copy;
    va_copy(args_copy, args);
    int size = vsnprintf(NULL, 0, str, args_copy);
    va_end(args_copy);

    // Allocate buffer
    char *buffer = malloc(size + 1); // +1 for null terminator
    if (!buffer) {
        va_end(args);
        return NULL; // Allocation failed
    }

    // Format string into buffer
    vsnprintf(buffer, size + 1, str, args);

    //vfprintf(stdout, "%s", args);
   

    va_end(args);
 fprintf(stdout, "%s\n", buffer);
 */
   // return buffer; // Caller must free this
  }
  
//void color_format_(const char * format, ...){
 

//}

























void sanitize(char *str) {
    if (!str) return;

    // Trim leading whitespace
    while (isspace((unsigned char)*str)) str++;

    // Copy sanitized content into a new buffer
    char *src = str;
    char *dst = str;
    while (*src) {
        if (isprint((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';

    // Trim trailing whitespace
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
}

void print_n_char(int n,char c){
  for(int i = 0; i < n; i++){
    fprintf(stdout, "%c", c);
  }
}

void dump_vars(){
  /*fprintf(stdout, "%s_____________________%s\nNAME\tTYPE\tVALUE\n", COLOR_BLACK, COLOR_RESET);
  if(env->vars.size == 0)
    fprintf(stdout, "Empty Set\n");
  else
    for(size_t i = 0; i < env->vars.size; i++){
      int name_len = strlen(env->vars.list[i].name);
      int type_len;
      switch(env->vars.list[i].literal.type){
        case INT:
          type_len = 3;
          fprintf(stdout, "%s%s",BRIGHT_MAGENTA, env->vars.list[i].name);
          print_n_char(PP_FIELD - name_len, ' ');
          fprintf(stdout, "%s%s",BRIGHT_CYAN, "INT");
          print_n_char(PP_FIELD - type_len, ' ');
          fprintf(stdout, "%d%s\n", env->vars.list[i].literal.num, COLOR_RESET);
/*
          fprintf(stdout, "%s%-8s%s%-8s%-8d%s\n",
                  BRIGHT_MAGENTA, vafrs.list[i].name,
                  BRIGHT_CYAN, "INT",
                  (int) vafrs.list[i].literal.value,
                  COLOR_RESET);*/
            //"%-8s%s%s%-8s%-8d%s\n",BRIGHT_MAGENTA, vafrs.list[i].name, BRIGHT_CYAN, "INT", (int) vafrs.list[i].literal.value, COLOR_RESET);
         /* break;
        case STRING:
          type_len = 6;
          fprintf(stdout, "%s%s",BRIGHT_MAGENTA, env->vars.list[i].name);
          print_n_char(PP_FIELD - name_len, ' ');
          fprintf(stdout, "%s%s",BRIGHT_YELLOW, "STRING");
          print_n_char(PP_FIELD - type_len, ' ');
          fprintf(stdout, "\"%s\"%s\n", env->vars.list[i].literal.str, COLOR_RESET);
          break;

        case NILL:
          type_len = 4;
          fprintf(stdout, "%s%s",BRIGHT_MAGENTA, env->vars.list[i].name);
          print_n_char(PP_FIELD - name_len, ' ');
          fprintf(stdout, "%s%s%s\n",BRIGHT_RED, "NULL", COLOR_RESET);
          break;
      }
    }
  //fprintf(stdout, "==========(%d)===========\n", env->vars.size);*/
}

char * var_func_color(Token * token){
  if((token+1)->type == TOK_LEFT_PAREN && (token)->type == IDENTIFIER)
    return "\e[38;2;100;240;100m";//BOLD_BRIGHT_GREEN;
  return color(*token);
}

char * color(Token token){
    switch(token.type){
          // Keywords: Bright Magenta for control flow and important words
        // Keywords: Bright Magenta
    case TOK_IF:
    case TOK_ELSE:
    case TOK_FOR:
    case TOK_WHILE:
    case TOK_NEW:
      return BOLD_BRIGHT_YELLOW;


    case TOK_RETURN:
      return BOLD_BRIGHT_MAGENTA; //BOLD_BRIGHT_MAGENTA;

    case TOK_VAR:
    case TOK_FUNCTION:
    case TOK_CLASS:
    case TOK_SUPER:
    case TOK_THIS:
    case TOK_BREAK:
    case TOK_CONTINUE:
    case TOK_AND:
    case TOK_OR:
      return BOLD_BRIGHT_MAGENTA;

    // Operators and punctuation: Simple White
    case TOK_PLUS:
    case TOK_MINUS:
    case TOK_STAR:
    case TOK_SLASH:
    case TOK_MODULO:
    case TOK_ASSIGN:
    case TOK_EQUAL_EQUAL:
    case TOK_NOT_EQUAL:
    case TOK_GREATER:
    case TOK_LESS:
    case TOK_GREATER_EQUAL:
    case TOK_LESS_EQUAL:
    case TOK_LOGIC_AND:
    case TOK_LOGIC_OR:
    case TOK_BW_AND:
    case TOK_BW_OR:
    case TOK_BW_XOR:
    case TOK_NOT:
    case TOK_COMMA:
    case TOK_DOT:
    case TOK_COLON:
    case TOK_SEMICOLON:
    case TOK_QUESTION_MARK:
    case TOK_BW_AND_ASSIGN:
    case TOK_BW_OR_ASSIGN:
    case TOK_BW_XOR_ASSIGN:
    case TOK_ADD_ASSIGN:
    case TOK_SUBTRACT_ASSIGN:
    case TOK_MULTIPLY_ASSIGN:
    case TOK_DIVIDE_ASSIGN:
    case TOK_MODULO_ASSIGN:
      return BOLD_BRIGHT_WHITE;

    // Grouping symbols: Normal Green
    case TOK_PRINT:
      return BOLD_BRIGHT_GREEN;

    case TOK_LEFT_PAREN:
    case TOK_RIGHT_PAREN:
    case TOK_LEFT_BRACE:
    case TOK_RIGHT_BRACE:
    case TOK_LEFT_CURLY:
    case TOK_RIGHT_CURLY:
      return COLOR_WHITE;

    // Literals: Bright and distinct
    case STRING_LITERAL:
      return BRIGHT_ORANGE; //BRIGHT_YELLOW;

    case NUMBER_LITERAL:
      return "\e[38;5;87m"; //BRIGHT_CYAN;
//#define BRIGHT_LIGHT_CYAN "\e[38;5;87m"

    // Identifiers: Bright Blue
    case IDENTIFIER:
      return "\e[38;5;69m"; //"\e[38;5;75m";


    // Boolean and null literals: Bright Cyan (like constants)
    case TOK_TRUE:
    case TOK_FALSE:
      return BRIGHT_CYAN;

    // Errors and EOF: Bright Red
    case TOK_INVALID:
    case TOK_EOF:
    case TOK_NULL:
      return BRIGHT_RED;

    // Fallback
    default:
      return BOLD_BRIGHT_BLACK;
  }
}

void print_n_tokens(Token * tokens, char * text, size_t start){
  for(size_t i = start; tokens[i].type != TOK_EOF; i++){
    char * value = malloc(sizeof(char)*256);

      strncpy(value, text + tokens[i].start, tokens[i].length);
      //fprintf(stdout, "%s\t%s%s%s%s\n", token_to_str(tokens[i]), strlen(token_to_str(tokens[i])) < 8 ? "\t" : "", color(tokens[i]), value, COLOR_RESET);
      //fprintf(stdout, "%s\t%s%s\n", token_to_str(tokens[i]), strlen(token_to_str(tokens[i])) < 8 ? "\t" : "", value);
      //fprintf(stdout, "%s ", /*color(tokens[i]),*/ token_to_str(tokens[i]));
      
      // = malloc(sizeof(char) * ( + 1)); 
      size_t len = tokens[i+1].type != TOK_EOF ? tokens[i+1].start-(tokens[i].start + tokens[i].length) : 0;
      char * after = malloc(sizeof(char) * ((len) + 1)); 
      

      if(tokens[i].type == STRING_LITERAL){
        strncpy(after, text + (tokens[i].start + tokens[i].length+1), len-1);
        after[len] = '\0';
       fprintf(stdout, "%s\"%s\"%s%s", color(tokens[i]), value, after, COLOR_RESET);
      }else{
        
        if(tokens[i+1].type == STRING_LITERAL){
        strncpy(after, text + (tokens[i].start + tokens[i].length), len-1);
        after[len-1] = '\0';
        }else{
          strncpy(after, text + (tokens[i].start + tokens[i].length), len);
          after[len] = '\0';
        }
        //if(tokens[i+1].type == TOK_SEMICOLON)
          fprintf(stdout, "%s%s%s%s%s", var_func_color(&tokens[i]), value, BOLD_BRIGHT_BLACK, after, COLOR_RESET);
        /*else{
          if(tokens[i].type == TOK_VAR)
            fprintf(stdout, "%s%s%s%s", color(tokens[i]), value, after, COLOR_RESET);
          else
            fprintf(stdout, "%s%s%s%s ", color(tokens[i]), value, after, COLOR_RESET);
        }*/
      }
    
  }
  fprintf(stdout, "\n");
}

void print_color_tokens(Token * tokens, char * text){
  print_n_tokens(tokens, text, (size_t)0);
}
void print_tokens(Token * tokens, char * text){
  print_color_tokens(tokens, text);
}

char * get_line(const char * text, size_t line){
 
  size_t start = 0;
  size_t ptr = 0;
  while(*(text+ptr)){
    if(start == line)
      break;
    if(*(text+ptr) == '\n')
      start++;
    ptr++;
  }
  size_t end = 0;
  for(end = ptr; *(text + end); end++){
    if(*(text + end) == '\n'){
      break;
    }
  }
  size_t len = end - ptr;
  char * ret = malloc(sizeof(char) * (len + 1));
 // (stdout, "+_%d %d__%s\n", ptr,len, text);

  if(text[ptr] == '\n')
    strncpy(ret, text + ptr + 1, len);
  else
    strncpy(ret, text + ptr, len);
  ret[len] = '\0';
  
  return ret;
}

char * node_type(Node node){
  switch(node.type){
    case LITERAL:        return "LITERAL";
    case VARIABLE:       return "VARIABLE";
    case ERROR:          return "ERROR";
    case GROUP:          return "GROUP";
    case UNARY:          return "UNARY";
    case BINARY:         return "BINARY";
    case TERNARY:        return "TERNARY";
    case CALL:           return "CALL";
    case FUNCTION:       return "FUNCTION";
    case INDEX:          return "INDEX";
    case ASSIGNMENT:     return "ASSIGNMENT";
    case STATEMENT:      return "STATEMENT";
    case SEQUENCE:       return "SEQUENCE";
    case CONTROL_FLOW:   return "CONTROL_FLOW";
    case OBJECT:         return "OBJECT";
    case CLASS:          return "CLASS";
  
    default:             return "INVALID";
  }
}



void print_node(Node * node, size_t indent, char * end){
  char * type = node_type(*node);
  
  //strcpy(node_type, );
  //*
  switch(node->type){
    
    case FUNCTION:
      colors(indent, BRIGHTEST_GREEN, "%s", "FUNCTION Definiton ");
      //colors(-1, "\e[38;2;100;240;100m", "%s()", node->function.callee->variable);
      //print_node(new_node(.type=VARIABLE, .variable=node->function.callee->variable), -1, "");
      break;
    case CALL:
      switch(node->call.type){
        case METHOD:
          colors(indent, BRIGHTEST_GREEN, "%s", "Method ");
          print_node(node->call.callee, -1, ".");
          print_node(node->call.method.caller, -1, "");
          break;
        case CONSTRUCT:
          colors(indent, color((Token){.type=TOK_NEW}), "%s", "new ");
          print_node(node->call.callee, -1, "");
          colors(-1, BRIGHTEST_GREEN, "%s", "()");

          break;
        case CALL_FUNCTION:
          colors(indent, BRIGHTEST_GREEN, "%s", "Call ");
         // colors(-1, BRIGHTEST_GREEN, "%s", "");
          colors(-1, BRIGHTEST_GREEN, "%s", node->call.callee->variable);
          //print_node(new_node(.type=VARIABLE, .variable=node->call.callee->variable), -1, "");
          colors(-1, BRIGHTEST_GREEN, "%s", "()");
          break;
      }
    //  colors(indent, BRIGHTEST_GREEN, "%s", "CALL ");
    //  colors(-1, "\e[38;2;100;240;100m", "%s()", node->function.callee->variable);
      //print_node(new_node(.type=VARIABLE, .variable=node->function.callee->variable), -1, "");
      break;
    case CLASS:
    colors(indent, BRIGHTEST_GREEN, "%s", "CLASS Definiton");
      //colors(indent, "\e[38;2;100;240;100m", "CLASS %s", node->class.name);
      //colors(-1, "\e[38;2;100;240;100m", "%s()", );
      break;
    case OBJECT:
    colors(indent, BRIGHTEST_GREEN, "%s", "OBJECT Assignment");
      /*switch(node->object.type){
        case ACCESS:
          colors(indent,  "\033[1;38;5;129m", "%s", "_(");
          //colors(-1, "\e[38;2;100;240;100m", "%s.", node_type(*node->object.callee));
          print_node(node->object.callee, -1, "");
          colors(-1, BOLD_BRIGHT_YELLOW, "%s", "->");
          print_node(node->object.access.caller, -1, "");
          colors(-1,  "\033[1;38;5;129m", "%s", ")");
          //colors(-1, "\033[1;38;5;129m", "%s", node_type(*node->object.caller));
          break;
        case CONSTRUCT:
          colors(indent,  BOLD_BRIGHT_YELLOW, "%s", "new ");
          //colors(-1, "\e[38;2;100;240;100m", "%s.", node_type(*node->object.callee));
          colors(indent,  "\033[1;38;5;129m", "%s()", node->object.callee->variable);

          //print_node(node->object.callee, -1, "");
          //colors(-1, BOLD_BRIGHT_YELLOW, "%s", "->");
         // print_node(node->object.construct.name, -1, "");
          //colors(-1,  "\033[1;38;5;129m", "%s", ")");
          //colors(-1, "\033[1;38;5;129m", "%s", node_type(*node->object.caller));
          break;
      }*/
    
      break;

    case LITERAL:
      switch(node->literal.type){
        case STRING:
          colors(indent, color((Token){STRING_LITERAL}), "%s \"%s\"",literal_type(&node->literal) ,node->literal.str);
          break;
        case INT:
          colors(indent, color((Token){NUMBER_LITERAL}), "%s %ld",literal_type(&node->literal), (long int)node->literal.num);
          break;
        case NILL:
          colors(indent, color((Token){TOK_NULL}), "%s", literal_type(&node->literal));
          break;
        default:
          colors(indent, color((Token){TOK_INVALID}), "%s", literal_type(&node->literal)); 
          break;
      }
      break;
    case VARIABLE:
      colors(indent, color((Token){IDENTIFIER}), "%s", node->variable);
      break;
    case SEQUENCE:
      colors(indent, BOLD_BRIGHT_WHITE, "%s", "(");
      //Node * res;
      for(size_t i = 0; i < node->sequence.count; i++){
        print_node(node->sequence.exp[i], -1, i == node->sequence.count-1 ? "" : ", "); 
      }
      colors(indent, BOLD_BRIGHT_WHITE, "%s", ")");
      break;
    case UNARY:
      colors(indent-1, color((Token){TOK_ASSIGN}), "%s %s", "UNARY", op_to_str(node->binary.op));
      break;
    case BINARY:
      colors(indent-1, color((Token){TOK_ASSIGN}), "%s %s", "BINARY", op_to_str(node->binary.op));
      break;
    case TERNARY:
      colors(indent, color((Token){TOK_ASSIGN}), "%s", "TERNARY");
      break;
    case ASSIGNMENT:
      colors(indent, color((Token){TOK_ASSIGN}), "%s", "ASSIGN ");
      print_node(node->assignment.target, -1, "");
      //print_node(node->assignment.target, -1, "");
      colors(-1, color((Token){TOK_ASSIGN}), "%s", " = ");
      print_node(node->assignment.value, -1, "");
      break;
    case GROUP:  //TOK_LEFT_PAREN
      colors(indent, color((Token){TOK_LEFT_PAREN}), "%s", "(");
      print_node(node->group.exp, 0, "");
      colors(-1, color((Token){TOK_RIGHT_PAREN}), "%s", ")");
      break;
    case STATEMENT:
      switch(node->statement.type){
        case DECLARATION:
          colors(indent, BOLD_BRIGHT_BLACK, "%s ","DECLARING");
          switch(node->statement.declaration.type){
            case DT_VAR:
              print_node(new_node(.type=VARIABLE, .variable=node->statement.declaration.name), -1, "");
              break;
            case DT_FUNCTION:
              colors(-1, "\e[38;2;100;240;100m", "%s()", node->statement.declaration.name);
              break;
            case DT_CLASS:
              //colors(-1, BOLD_BRIGHT_MAGENTA, "%s", node->statement.declaration.name);
              colors(-1, color((Token){IDENTIFIER}), "%s", node->statement.declaration.name);
              colors(-1, "\e[38;2;100;240;100m", "%s", "()");
              break;
          }
          
          break;

        case BLOCK:
          colors(indent, BOLD_BRIGHT_MAGENTA, "%s", stmt_type(node->statement.type));
          break;
        case PRINT:
          colors(indent, color((Token){TOK_PRINT}), "%s ", stmt_type(node->statement.type));
          print_node(node->statement.print.exp, -1, "");
          break;

        default:
          colors(indent, color((Token){TOK_IF}), "%s", stmt_type(node->statement.type));
      }
      break;
    case INDEX:
      colors(indent, BOLD_BRIGHT_MAGENTA, "%s", node_type);
      break;
    case CONTROL_FLOW:
      colors(indent, BOLD_BRIGHT_BLACK, "%s", ctrl_type(node->control_flow.type));
      break;
    case ERROR:
      colors(indent, color((Token){TOK_INVALID}), "%s", error_to_str(node->error.type));
      break;
    default:
      colors(indent, BOLD_BRIGHT_RED, "FATAL: N/A %s\nExiting...\n", node_type);
      exit(0);
      break;
  }//*/
  fprintf(stdout, "%s", end);
}


char * signal_to_str(int signal) {
    switch(signal) {
        case 1: return "SIGHUP";
        case 2: return "SIGINT";
        case 3: return "SIGQUIT";
        case 4: return "SIGILL";
        case 5: return "SIGTRAP";
        case 6: return "SIGABRT";
        case 7: return "SIGBUS";
        case 8: return "SIGFPE";
        case 9: return "SIGKILL";
        case 10: return "SIGUSR1";
        case 11: return "SIGSEGV";
        case 12: return "SIGUSR2";
        case 13: return "SIGPIPE";
        case 14: return "SIGALRM";
        case 15: return "SIGTERM";
        case 16: return "SIGSTKFLT";
        case 17: return "SIGCHLD";
        case 18: return "SIGCONT";
        case 19: return "SIGSTOP";
        case 20: return "SIGTSTP";
        case 21: return "SIGTTIN";
        case 22: return "SIGTTOU";
        case 23: return "SIGURG";
        case 24: return "SIGXCPU";
        case 25: return "SIGXFSZ";
        case 26: return "SIGVTALRM";
        case 27: return "SIGPROF";
        case 28: return "SIGWINCH";
        case 29: return "SIGIO";
        case 30: return "SIGPWR";
        case 31: return "SIGSYS";
        // Real-time signals (may vary slightly by system)
        case 32: return "SIGRTMIN";
        case 33: return "SIGRTMIN+1";
        case 34: return "SIGRTMIN+2";
        case 35: return "SIGRTMIN+3";
        case 36: return "SIGRTMIN+4";
        case 37: return "SIGRTMIN+5";
        case 38: return "SIGRTMIN+6";
        case 39: return "SIGRTMIN+7";
        case 40: return "SIGRTMIN+8";
        case 41: return "SIGRTMIN+9";
        case 42: return "SIGRTMIN+10";
        case 43: return "SIGRTMIN+11";
        case 44: return "SIGRTMIN+12";
        case 45: return "SIGRTMIN+13";
        case 46: return "SIGRTMIN+14";
        case 47: return "SIGRTMIN+15";
        case 48: return "SIGRTMAX-15";
        case 49: return "SIGRTMAX-14";
        case 50: return "SIGRTMAX-13";
        case 51: return "SIGRTMAX-12";
        case 52: return "SIGRTMAX-11";
        case 53: return "SIGRTMAX-10";
        case 54: return "SIGRTMAX-9";
        case 55: return "SIGRTMAX-8";
        case 56: return "SIGRTMAX-7";
        case 57: return "SIGRTMAX-6";
        case 58: return "SIGRTMAX-5";
        case 59: return "SIGRTMAX-4";
        case 60: return "SIGRTMAX-3";
        case 61: return "SIGRTMAX-2";
        case 62: return "SIGRTMAX-1";
        case 63: return "SIGRTMAX";
        default: return "UNKNOWN";
    }
}


void print_error(Node * err, char * line){
  Node * curr = err;
  char buff[1024];
  sprintf(buff, "|ERROR: %s at line %d, col %d\n", error_to_str(curr->error.type), curr->error.token.line, curr->error.token.col);
  //printf(buff);
  fprintf(stdout, "%s\n", COLOR_RED);
  for(int i = 0; i < strlen(buff); i++)
    fprintf(stdout, "_");
  fprintf(stdout, "%s\n", COLOR_RED, COLOR_RESET);
  while(curr && curr->type == ERROR){
    //if(!curr) break;
    //fprintf(stdout, "______\n");_______________________\n
    
    fprintf(stdout, "%s|ERROR: %s%s%s at line %d, col %d, len %d\n", BRIGHT_RED, BRIGHT_MAGENTA, error_to_str(curr->error.type), COLOR_RESET, curr->error.token.line, curr->error.token.col, curr->error.token.length );
    
    //("%s\n", curr->error.token.line, curr->error.token.col);
    //fprintf(stdout, "%.*s\n", length, get_line(line, curr->error.token.line));
    char * error_line = get_line(line, curr->error.token.line);
    //fprintf(stdout, "TOK: col: %d len: %d, line_len: %d\n", stmt->error.token.col, stmt->error.token.length, strlen(error_line));
    fprintf(stdout, "%s\n", error_line);
    for(int i = 0; i <= strlen(error_line) + 1; i++){
      if(i >= curr->error.token.col && i < curr->error.token.col + (curr->error.token.length ? curr->error.token.length : 1))
        fprintf(stdout, "%s^%s", COLOR_YELLOW, COLOR_RESET);
      else
        fprintf(stdout, "%s-%s", BRIGHT_BLACK, COLOR_RESET);
    }
    fprintf(stdout, "\n");
    free(error_line);
    curr = curr->error.inner;
  }
}
/*
  fprintf(stdout, "_____________________\nToken\t\tValue\n");
  for(size_t i = 0; tokens[i].type != _EOF; i++){
    char * value = malloc(sizeof(char)*256);
    strncpy(value, text + tokens[i].start, tokens[i].length);
    fprintf(stdout, "%s\t%s%s%s%s\n", token_to_str(tokens[i]), strlen(token_to_str(tokens[i])) < 8 ? "\t" : "", color(tokens[i]), value, COLOR_RESET);
  }
  fprintf(stdout, "_____________________\n");
  */




/*
//var s = substring(s, 0, 2) + "hello"
  assign("s", (Literal){.type=STRING, .value=(void *)"012345"} );
  dump_vars();
  Node *node_assign = new_node(.type=ASSIGNMENT, .assignment.target="s", .assignment.value = NULL);
  Node *node_cat = new_node(.type=BINARY_OP, .binary.left=NULL, .binary.right=NULL, .binary.op=ADD);
  node_assign->assignment.value = node_cat;
  //start left block
  Node ** args = malloc(sizeof(Node *) * 3);
  args[0] = new_node(.type=VARIABLE, .variable="s");
  args[1] = new_node(.type=LITERAL, .literal.value=(void *)0, .literal.type=INT);
  args[2] = new_node(.type=LITERAL, .literal.value=(void *)2, .literal.type=INT);
  Node *substring = new_node(.type=FUNCTION_CALL, .function.name="substring", .function.argc=3 , .function.args=args);
  node_cat->binary.left=substring;
  //start right block
  Node *literal_hello = new_node(.type=LITERAL, .literal.value="hello", .literal.type=STRING);
  node_cat->binary.right = literal_hello;
  traverse(node_assign);
  dump_vars();
*/
#endif