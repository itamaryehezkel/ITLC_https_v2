#ifndef TOKENS_C
#define TOKENS_C



void add_token(Token ** list, Token new_token, size_t * curr_size){
  Token * new_list = realloc(*list, sizeof(Token)*(*curr_size + 1));
  new_list[*curr_size] = new_token;
  *list  = new_list;
  *curr_size += 1;
}

int check_next(const char * ptr, char c){
  return *(ptr+1) && *(ptr+1) == c;
}

Token * tokenise(const char * line){
  const char * ptr = line;
  
  size_t line_num = 0;
  size_t col_num = 0;

  Token * tokens = malloc(sizeof(Token));
  size_t size = 0;
  while(*ptr){
    while(*ptr && (isspace(*ptr) || ! (*ptr >= '!' && *ptr <= '~'))){

      if(*ptr == '\n'){
        line_num++;
        col_num = 0;
      }else{
        col_num++;
      }
      ptr++;
    } 
  
    switch(*ptr){
      case 0:
        break;
      case '(':
        add_token(&tokens, (Token){.type= TOK_LEFT_PAREN, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case ')': 
        add_token(&tokens, (Token){.type= TOK_RIGHT_PAREN, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case '[': 
        add_token(&tokens, (Token){.type= TOK_LEFT_BRACE, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case ']': 
        add_token(&tokens, (Token){.type= TOK_RIGHT_BRACE, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case '{': 
        add_token(&tokens, (Token){.type= TOK_LEFT_CURLY, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case '}': 
        add_token(&tokens, (Token){.type= TOK_RIGHT_CURLY, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case ',': 
        add_token(&tokens, (Token){.type= TOK_COMMA, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case '.': 
        add_token(&tokens, (Token){.type= TOK_DOT, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case '?': 
        add_token(&tokens, (Token){.type= TOK_QUESTION_MARK, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case ':': 
        add_token(&tokens, (Token){.type= TOK_COLON, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case ';': 
        add_token(&tokens, (Token){.type= TOK_SEMICOLON, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        ptr++;
        col_num++;
        break;
      case '-': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_SUBTRACT_ASSIGN, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else{
          add_token(&tokens, (Token){.type= TOK_MINUS, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '+': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_ADD_ASSIGN, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else{
          add_token(&tokens, (Token){.type= TOK_PLUS, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '/': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_DIVIDE_ASSIGN, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else if(check_next(ptr, '*')){
          while(*ptr && *(ptr+1)){
            
            if(*ptr == '*' && *(ptr+1) == '/'){
              ptr += 2;
              col_num += 2;
              break;
            }else if(!*(ptr+1)){
              add_token(&tokens, (Token){.type= TOK_ERROR_UNTERM_COMMENT, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
              break;
            }else{
              if(*ptr == '\n'){
                line_num++;
                col_num = 0;
              }else{
                col_num++;
              }
              ptr++;

            }
          }
          break;
        }else if(check_next(ptr, '/')){
          while(*ptr && *ptr != '\n'){
            ptr++;
            col_num++;
          }  
          line_num++;
          col_num = 0;

        }else{
          add_token(&tokens, (Token){.type= TOK_SLASH, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '*': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_MULTIPLY_ASSIGN, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else if(check_next(ptr, '/')){
          add_token(&tokens, (Token){.type=TOK_ERROR_UNTERM_COMMENT, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }else{
          add_token(&tokens, (Token){.type= TOK_STAR, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '%': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_MODULO_ASSIGN, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else{
          add_token(&tokens, (Token){.type= TOK_MODULO, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '&': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_BW_AND_ASSIGN, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else if(check_next(ptr, '&')){
          add_token(&tokens, (Token){.type=TOK_LOGIC_AND, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        } else{
          add_token(&tokens, (Token){.type= TOK_BW_AND, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '|': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_BW_OR_ASSIGN, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else if(check_next(ptr, '|')){
          add_token(&tokens, (Token){.type=TOK_LOGIC_OR, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else{
          add_token(&tokens, (Token){.type= TOK_BW_OR, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '^': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_BW_XOR_ASSIGN, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
         col_num++;
        }else{
          add_token(&tokens, (Token){.type= TOK_BW_XOR, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '!': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_NOT_EQUAL, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else{
          add_token(&tokens, (Token){.type=TOK_NOT, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '=': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_EQUAL_EQUAL, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else{
          add_token(&tokens, (Token){.type= TOK_ASSIGN, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '>': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_GREATER_EQUAL, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;  
        }else{
          add_token(&tokens, (Token){.type= TOK_GREATER, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      case '<': 
        if(check_next(ptr, '=')){
          add_token(&tokens, (Token){.type=TOK_LESS_EQUAL, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }else{
          add_token(&tokens, (Token){.type= TOK_LESS, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
        }
        ptr++;
        col_num++;
        break;
      default:
        if(strncmp(ptr, "var ", 4) == 0){ // VAR
          add_token(&tokens, (Token){.type=TOK_VAR, .start=(size_t)(ptr-line), .length=4, .line=line_num , .col=col_num}, &size);
          ptr += 4;
          col_num += 4;
        }else if(strncmp(ptr, "function ", 9) == 0){ // FUNCTION
          add_token(&tokens, (Token){.type=TOK_FUNCTION, .start=(size_t)(ptr-line), .length=9, .line=line_num , .col=col_num}, &size);
          ptr += 9;
          col_num += 9;
        }else if(strncmp(ptr, "class ", 6) == 0){ // FUNCTION
          add_token(&tokens, (Token){.type=TOK_CLASS, .start=(size_t)(ptr-line), .length=6, .line=line_num , .col=col_num}, &size);
          ptr += 6;
          col_num += 9;
        }else if(strncmp(ptr, "if", 2) == 0){ // IF
          add_token(&tokens, (Token){.type=TOK_IF, .start=(size_t)(ptr-line), .length=2, .line=line_num , .col=col_num}, &size);
          ptr += 2;
          col_num += 2;
        }else if(strncmp(ptr, "else", 4) == 0){ // ELSE
          add_token(&tokens, (Token){.type=TOK_ELSE, .start=(size_t)(ptr-line), .length=4, .line=line_num , .col=col_num}, &size);
          ptr += 4;
          col_num += 4;
        }else if(strncmp(ptr, "for", 3) == 0){ // FOR
          add_token(&tokens, (Token){.type=TOK_FOR, .start=(size_t)(ptr-line), .length=3, .line=line_num , .col=col_num}, &size);
          ptr += 3;
          col_num += 3;
        }else if(strncmp(ptr, "while", 5) == 0){ // WHILE
          add_token(&tokens, (Token){.type=TOK_WHILE, .start=(size_t)(ptr-line), .length=5, .line=line_num , .col=col_num}, &size);
          ptr += 5;
          col_num += 5;
        }else if(strncmp(ptr, "break", 5) == 0){ // BREAK
          add_token(&tokens, (Token){.type=TOK_BREAK, .start=(size_t)(ptr-line), .length=5, .line=line_num , .col=col_num}, &size);
          ptr += 5;
          col_num += 5;
        }else if(strncmp(ptr, "continue", 8) == 0){ // CONTINUE
          add_token(&tokens, (Token){.type=TOK_CONTINUE, .start=(size_t)(ptr-line), .length=8, .line=line_num , .col=col_num}, &size);
          ptr += 8;
          col_num += 8;
        }else if(strncmp(ptr, "return", 6) == 0){ // RETURN
          add_token(&tokens, (Token){.type=TOK_RETURN, .start=(size_t)(ptr-line), .length=6, .line=line_num , .col=col_num}, &size);
          ptr += 6;
          col_num += 6;
        }else if(strncmp(ptr, "true", 3) == 0){ // RETURN
          add_token(&tokens, (Token){.type=TOK_TRUE, .start=(size_t)(ptr-line), .length=4, .line=line_num , .col=col_num}, &size);
          ptr += 4;
          col_num += 4;
        }else if(strncmp(ptr, "false", 5) == 0){ // RETURN
          add_token(&tokens, (Token){.type=TOK_FALSE, .start=(size_t)(ptr-line), .length=5, .line=line_num , .col=col_num}, &size);
          ptr += 5;
          col_num += 5;
        }else if(strncmp(ptr, "null", 4) == 0){ // RETURN
          add_token(&tokens, (Token){.type=TOK_NULL, .start=(size_t)(ptr-line), .length=4, .line=line_num , .col=col_num}, &size);
          ptr += 4;
          col_num += 4;
        }else if(strncmp(ptr, "new", 3) == 0){ // RETURN
          add_token(&tokens, (Token){.type=TOK_NEW, .start=(size_t)(ptr-line), .length=3, .line=line_num , .col=col_num}, &size);
          ptr += 3;
          col_num += 3;
        }else if(strncmp(ptr, "this", 4) == 0){ // RETURN
          add_token(&tokens, (Token){.type=TOK_THIS, .start=(size_t)(ptr-line), .length=4, .line=line_num , .col=col_num}, &size);
          ptr += 4;
          col_num += 4;
        }else if(strncmp(ptr, "print", 5) == 0){ // RETURN
          add_token(&tokens, (Token){.type=TOK_PRINT, .start=(size_t)(ptr-line), .length=5, .line=line_num , .col=col_num}, &size);
          ptr += 5;
          col_num += 5;
        }else if(*ptr == '"'){ // STRING_LITERAL
          ptr++;
          col_num++;
          Token tok = {.type=STRING_LITERAL, .start=(size_t)(ptr-line), .length=0, .line=line_num , .col=col_num};
          char buffer[256];
          unsigned int i = 0;
          while(*ptr && *ptr != '"'){ 
            if(*ptr == '\n'){
              line_num++;
              col_num = 0;
            }else{
              col_num++;
            }
            buffer[i++] = *ptr++;
          } 
            buffer[i] = '\0';
          tok.length = i;
          add_token(&tokens, tok, &size);
          if(*ptr == '"'){
            col_num++;
            ptr++;
          } 
        }else if(*ptr == '\''){ // STRING_LITERAL
          ptr++;
          col_num++;
          Token tok = {.type=STRING_LITERAL, .start=(size_t)(ptr-line), .length=0, .line=line_num , .col=col_num};
          char buffer[256];
          unsigned int i = 0;
          while(*ptr && *ptr != '\''){ 
            if(*ptr == '\n'){
              line_num++;
              col_num = 0;
            }else{
              col_num++;
            }
            buffer[i++] = *ptr++;
          } 
            buffer[i] = '\0';
          tok.length = i;
          add_token(&tokens, tok, &size);
          if(*ptr == '\''){
            col_num++;
            ptr++;
          } 
        }else if (isalpha(*ptr)) { // IDENTIFIER only alphabet
          Token tok = {.type=IDENTIFIER, .start=(size_t)(ptr-line), .length=0, .line=line_num , .col=col_num};
          char buffer[64];
          unsigned int i = 0;
          while (isalnum(*ptr)){
           /*if(*ptr == '\n'){
              line_num++;
              col_num = 0;
            }else{
            }*/
              col_num++;
            buffer[i++] = *ptr++;
          } 
          buffer[i] = '\0';
          tok.length = i;
          add_token(&tokens, tok, &size);
        }else if(isdigit(*ptr)){
          Token tok = {.type=NUMBER_LITERAL, .start=(size_t)(ptr-line), .length=0, .line=line_num , .col=col_num};
          char buffer[64];
          unsigned int i = 0;
          while (isdigit(*ptr)){
            if(*ptr == '\n'){
              line_num++;
              col_num = 0;
            }else{
              col_num++;
            }
            buffer[i++] = *ptr++;
          } 
          buffer[i] = '\0';
          tok.length = i;
          add_token(&tokens, tok, &size);
        }else{  // UNKNOWN
          fprintf(stderr, "UNRECOGNIZED TOKEN!!!!   '%d'\n", *ptr);
          add_token(&tokens, (Token){.type=TOK_INVALID, .start=(size_t)(ptr-line), .length=1, .line=line_num , .col=col_num}, &size);
          ptr++;
          col_num++;
        }
    }
  }
  add_token(&tokens, (Token){.type=TOK_EOF, .start=(size_t)(ptr - line), .length=1, .line=line_num , .col=col_num}, &size);
  return tokens;
}


#endif