#ifndef PARSER_C
#define PARSER_C

#include "parser.h"


Node * parse_literal(Token * tokens, char * text, size_t *idx){
  char * value_unary;
  Token tok = tokens[*idx];
  switch(tokens[*idx].type){
    case STRING_LITERAL:
      value_unary = malloc(sizeof(char) * (tokens[*idx].length + 1) );
      strncpy(value_unary, text + tokens[*idx].start, tokens[*idx].length);
      value_unary[tokens[*idx].length] = '\0'; 
      
      ++*idx;

      if(tokens[*idx].type == STRING_LITERAL || tokens[*idx].type == NUMBER_LITERAL || tokens[*idx].type == IDENTIFIER)
        return new_node(.type=ERROR, .error.type=UNEXPECTED_LITERAL, .error.token=tokens[*idx]);
      return new_node(.type=LITERAL, .literal.str=value_unary, .literal.type=STRING, .token=tok);
    case NUMBER_LITERAL:
      value_unary = malloc(sizeof(char) * (tokens[*idx].length + 1) );
      strncpy(value_unary, text + tokens[*idx].start, tokens[*idx].length);
      value_unary[tokens[*idx].length] = '\0'; 
      long double val = strtod(value_unary, NULL);
      ++*idx;
      
      if(tokens[*idx].type == STRING_LITERAL || tokens[*idx].type == NUMBER_LITERAL || tokens[*idx].type == IDENTIFIER)
        return new_node(.type=ERROR, .error.type=UNEXPECTED_LITERAL, .error.token=tokens[*idx]);
      return new_node(.type=LITERAL, .literal.num=val, .literal.type=INT, .token=tok);

    case TOK_NULL:
      ++*idx;
      return new_node(.type=LITERAL, .literal.type=NILL, .token=tok);

    case TOK_TRUE:
      ++*idx;
      return new_node(.type=LITERAL, .literal.num=1, .literal.type=INT, .token=tok);

    case TOK_FALSE:
      ++*idx;
      return new_node(.type=LITERAL, .literal.num=0, .literal.type=INT, .token=tok);
    
    case TOK_ERROR_UNTERM_COMMENT:
      return new_node(.type=ERROR, .error.type=UNTERMINATED_COMMENT_BLOCK, .error.token=tokens[*idx]);

    default:
    
    if(tokens[*idx].type == TOK_EOF)
    return new_node(.type=ERROR, .error.type=UNEXPECTED_EOI, 
         .error.token.line=tokens[*idx-1].line,
         .error.token.col=tokens[*idx-1].col+1,
         .error.token.length=1
      );

    return new_node(.type=ERROR, .error.type=EXPECTED_LITERAL, 
         .error.token.line=tokens[*idx].line,
         .error.token.col=tokens[*idx].col,
         .error.token.length=tokens[*idx].length
      );
  }
}

/*
primary        → _"true"_ | _"false"_ | _"nil"_ 
               | _NUMBER_ | _STRING_ | _IDENTIFIER_ | _"(" expression ")"_
               | "super" "." IDENTIFIER ;| "this"

               
*/
Node * parse_primary(Token * tokens, char * text, size_t *idx){
  switch(tokens[*idx].type){
    case TOK_LEFT_PAREN:
      return parse_group(tokens, text, idx);
    
    case TOK_SUPER:  // | "super" "." IDENTIFIER ;
      ++*idx;  //consume super
      //check if next is dot
      ++*idx; //consume dot
      return new_node(.type=LITERAL, .literal.num=0, .literal.type=INT);

    case TOK_THIS:  //"this"
      ++*idx;
      return new_node(.type=OBJECT, .object.type=NULL, .object.fields=NULL);

    case IDENTIFIER:
      char * value_unary = malloc(sizeof(char) * (tokens[*idx].length + 1) );
      strncpy(value_unary, text + tokens[*idx].start, tokens[*idx].length);
      value_unary[tokens[*idx].length] = '\0';
 
      Token tok = tokens[*idx];
      ++*idx;
      if(tokens[*idx].type == STRING_LITERAL || tokens[*idx].type == NUMBER_LITERAL || tokens[*idx].type == IDENTIFIER)
        return new_node(.type=ERROR, .error.type=UNEXPECTED_LITERAL, .error.token=tokens[*idx]);
      return new_node(.type=VARIABLE, .variable=value_unary, .token=tok);


    default:
      return parse_literal(tokens, text, idx);
  }
}

Node * parse_index(Node * callee,Token * tokens, char * text, size_t *idx){

  ++*idx; // consume '['

  Node *index_expr = parse_exp(tokens, text, idx);
  
  if (index_expr->type == ERROR || index_expr->type == CONTROL_FLOW){
    if(index_expr->type == ERROR)
      return new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx], .error.inner=index_expr);
    
  } 

  if (tokens[*idx].type != TOK_RIGHT_BRACE)
    return new_node(.type=ERROR, .error.type=NO_RIGHT_BRACE, .error.token=tokens[*idx]);

  ++*idx; // consume ']'

  return new_node(.type = INDEX, .index.target=callee, .index.index=index_expr);
}


Node * parse_function(Token * tokens, char * text, size_t *idx){    
  //function       → IDENTIFIER "(" parameters? ")" block ;
            Node * callee; //literal string (func name)
            Node * body; //block
            //Node** args;  //unneeded, function declaration will store name arity and body
            //size_t arg_count; //arity

  if(tokens[*idx].type != IDENTIFIER) //check function decl name validity
    return new_node(.type=ERROR, .error.type=EXPECTED_FUNCTION_NAME, .error.token=tokens[*idx]);
  
    char * name = malloc( sizeof(char) * (tokens[*idx].length + 1) ); //copy to here the identifier name
  strncpy(name, text + tokens[*idx].start, tokens[*idx].length);
  name[tokens[*idx].length] = '\0';
  //callee = new_node(.type=LITERAL, .literal.type=STRING, .literal.str=name);
  ++*idx;  //consume identifier

  //name = parse_litferal(tokens, text, idx);  //also check validity here
  
  if(tokens[*idx].type != TOK_LEFT_PAREN)
    return new_node(.type=ERROR, .error.type=NO_LEFT_PAREN, .error.token=tokens[*idx]);
  ++*idx;

  Node * params = parse_parameters(tokens, text, idx);  //implement later
  if(params->type == ERROR)
    return params; //new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx], .error.inner=params);
    
  if(tokens[*idx].type != TOK_RIGHT_PAREN)
    return new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx]);
  ++*idx;
  
  if(tokens[*idx].type != TOK_LEFT_CURLY)
    return new_node(.type=ERROR, .error.type=EXPECTED_BLOCK, .error.token=tokens[*idx]);
    
  body = parse_block(tokens, text, idx);
  if(body->type == ERROR)
    return body;

  return new_node(.type=FUNCTION,
                  .function_definiton.name=name,
                  .function_definiton.body=body,
                  .function_definiton.arity=params->function_definiton.arity,
                  .function_definiton.params=params->function_definiton.params,
                  );
}

Node * parse_parameters(Token * tokens, char * text, size_t *idx){  //parameters     → IDENTIFIER ( "," IDENTIFIER )* ;
  Node ** params = NULL;
  size_t arity = 0;
  while(tokens[*idx].type == IDENTIFIER){
   
    Node * param = parse_primary(tokens, text, idx);
    if(param->type == ERROR)
      return param;

    arity++;
    Node ** temp_list = realloc(params, sizeof(Node *) * arity);
  
    params = temp_list;
    params[arity-1] = param;

    if(tokens[*idx].type == TOK_COMMA){
    ++*idx;
    }
  }

  return new_node(.type=FUNCTION, .function_definiton.arity=arity, .function_definiton.params=params); 
}



Node * parse_arguments(Node * callee, Token * tokens, char * text, size_t *idx){  //arguments      → expression ( "," expression )* ;
  //Node **args = NULL;
  size_t arg_count = 0;

  if (tokens[*idx].type != TOK_RIGHT_PAREN) {
    Node *arg = parse_exp(tokens, text, idx);
  

    if (arg->type == ERROR ||
        arg->type == CONTROL_FLOW)
        //(arg->type != VARIABLE && arg->type != SEQUENCE) )
    return arg;


      if(arg->type != SEQUENCE){
        Node ** single_arg = malloc(sizeof(Node *));
        single_arg[0] = arg;
        return new_node(.type = CALL, 
                .call.callee=callee,
                .call.function.args=single_arg,
                .call.function.argc = 1,
                .token=arg->token
              );
      }else{  //sequence
        return new_node(.type = CALL, 
              .call.callee=callee,
              .call.function.args=arg->sequence.exp,
              .call.function.argc = arg->sequence.count,
              .token=arg->sequence.token
            );
      }
     
 
  //  if(tokens[*idx].type == TOK_EOF)
   //   return new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx], .error.inner=arg);
  }

   return new_node(.type = CALL, 
                         .call.callee=callee,
                         .call.function.args=NULL,
                         .call.function.argc = 0,
                        .token=(Token){.type=callee->token.type , .line=callee->token.line, .col=callee->token.col, .length=callee->token.col, .start= callee->token.start}
                        );
}

Node * parse_call(Node * callee,Token * tokens, char * text, size_t *idx){  //call  → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
  
  Node * caller;

  switch(tokens[*idx].type){
    case TOK_LEFT_PAREN:
      ++*idx;
      caller = parse_arguments(callee, tokens, text, idx);
      if(caller->type == ERROR)
        return caller;//new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token=tokens[*idx], .error.inner=func_call);

      if (tokens[*idx].type != TOK_RIGHT_PAREN)
        return new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx]);
      ++*idx;

      return new_node(.type = CALL,
                      .call.type=CALL_FUNCTION,
                      .call.callee=callee,
                      .call.function.args=caller->call.function.args,
                      .call.function.argc=caller->call.function.argc,
                      .token=caller->token
                    );
    case TOK_DOT:  //class access or methods
      ++*idx;
      caller = parse_primary(tokens, text, idx); //e_primary
      if(caller->type == ERROR)
        return caller;//new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token=tokens[*idx], .error.inner=func_call);
      return new_node(.type=CALL, .call.type=METHOD, .token = tokens[*idx], .call.callee=callee, .call.method.caller=caller);

    default:
      return new_node(.type=ERROR, .error.type=PARSER_ERROR, .error.token = tokens[*idx]);
  }
  
   

  // consume ')'
  /*
  Node *node = new_node(.type = FUNCTION_CALL);
  node->function.callee = callee;
  node->function.args = args;
  node->function.arg_count = arg_count;
________________________________________________________________________________
call           → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
primary        → "true" | "false" | "nil" | "this"
               | NUMBER | STRING | IDENTIFIER | "(" expression ")"
               | "super" "." IDENTIFIER ;
________________________________________________________________________________
call           -> "(" [expression ("," expression)*] ")"                       
call           → primary ( "(" arguments? ")" )* ;


call           → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;


  Node * params = parse_parameters(tokens, text, idx);  //implement later
  if(params->type == ERROR)
    return params; //new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx], .error.inner=params);
    .function.arg_count=params->function.arg_count,
                  .function.args=params->function.args,
  */
 //.function.callee=callee,
                        // .function.args=args,
                   
  return caller;
}

//unary post inc/dec
Node *parse_postfix(Token * tokens, char * text, size_t *idx) {
  size_t rem = *idx;
   
  Node *expr;// = parse_primary(tokens, text, idx);
  if(tokens[*idx].type == IDENTIFIER && tokens[*idx+1].type == TOK_PLUS && tokens[*idx+2].type == TOK_PLUS && (tokens[*idx+1].start - tokens[*idx].start == 1) && (tokens[*idx+2].start - tokens[*idx+1].start == 1)){
    //if(tokens[*idx].type == TOK_PLUS && tokens[*idx+1].type == TOK_PLUS && (tokens[*idx+1].start - tokens[*idx].start == 1) && (tokens[*idx+2].start - tokens[*idx+1].start == 1) && tokens[*idx+2].type == IDENTIFIER){
  
    //("ADD?\n");
    expr = parse_primary(tokens, text, idx);
    //("ADD!\n");
    if(expr->type == ERROR)
      return expr;
    //("ADD!!!\n");
    ++*idx;
    ++*idx;
    //++*idx;
    //Node * right = parse_postfix(tokens, text, idx);
    return new_node(
        .type = ASSIGNMENT,
        .assignment.target = expr,
        .assignment.op = OP_ADD_AFTER,
        .assignment.value = new_node(.type = BINARY, .binary.left = expr, .binary.right = new_node(.type=LITERAL, .literal.type=INT, .literal.num=1), .binary.op = OP_ADD_AFTER)
    );
  }else if(tokens[*idx].type == IDENTIFIER && tokens[*idx+1].type == TOK_MINUS && tokens[*idx+2].type == TOK_MINUS && (tokens[*idx+1].start - tokens[*idx].start == 1) && (tokens[*idx+2].start - tokens[*idx+1].start == 1)){
    
    expr = parse_primary(tokens, text, idx);
    if(expr->type == ERROR)
      return expr;
    ++*idx;
    ++*idx;
    //("POST DEC\n");
    //Node * right = parse_postfix(tokens, text, idx);
    return new_node(
        .type = ASSIGNMENT,
        .assignment.target = expr,
        .assignment.op = OP_SUBTRACT_AFTER,
        .assignment.value = new_node(.type = BINARY, .binary.left = expr, .binary.right = new_node(.type=LITERAL, .literal.type=INT, .literal.num=1), .binary.op = OP_SUBTRACT_AFTER)
    );
  }
  Token tok = tokens[*idx-1];
  if(tokens[*idx].type != TOK_NEW){
    expr = parse_primary(tokens, text, idx);
    if(expr->type == ERROR)
      return expr;
  }
    

  


  if(tokens[*idx].type == STRING_LITERAL || tokens[*idx].type == NUMBER_LITERAL)
    return new_node(.type=ERROR, .error.type=UNEXPECTED_LITERAL, .error.token=tokens[*idx]);  // prevent bs like <num num num;>
  //call           → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
  Token aft = tokens[*idx];
  size_t idd = *idx;
  while (tokens[*idx].type == TOK_LEFT_PAREN || tokens[*idx].type == TOK_LEFT_BRACE || tokens[*idx].type == TOK_DOT || tokens[*idx].type == TOK_NEW) {
    aft = tokens[*idx];
    idd = *idx;
    switch(tokens[*idx].type){
      case TOK_LEFT_PAREN:
      case TOK_DOT: // gets "(" and consumes ")"  | gets "." and consumes ".", then expects a primary
        expr = parse_call(expr, tokens, text, idx);
        break;

      case TOK_NEW:  //for now handling NEW inside postfix. not inside call. deal wiv it
        ++*idx; // consume "new"
        Node * callee = parse_primary(tokens, text, idx);
        if(callee->type == ERROR)
          return new_node(.type=ERROR, .error.type=EXPECTED_IDENTIFIER, .error.token=tokens[*idx], .error.inner=callee);
        if(callee->type != VARIABLE)
          return new_node(.type=ERROR, .error.type=EXPECTED_IDENTIFIER, .error.token=tokens[*idx]);
        if (tokens[*idx].type != TOK_LEFT_PAREN)
          return new_node(.type=ERROR, .error.type=NO_LEFT_PAREN, .error.token=tokens[*idx]);
        ++*idx;

        Node * args = parse_arguments(callee, tokens, text, idx);
        if(args->type == ERROR)
          return args;

        if (tokens[*idx].type != TOK_RIGHT_PAREN)
          return new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx]);
        ++*idx;

        expr = new_node(.type=CALL, .call.type=CONSTRUCT,
                .call.callee=callee,  
                .call.construct.args=args->call.construct.args,
                .call.construct.argc=args->call.construct.argc);
          
        break;

      case TOK_LEFT_BRACE:
        expr = parse_index(expr, tokens, text, idx); // index gets "[" and consumes "]"
        break;
    }
  }
 // ("___%ld %ld\n", tokens[*idx].start, tokens[idd+1].start + tokens[idd+1].length);
  if(//tokens[*idx].type == IDENTIFIER && 
     ((tokens[*idx].type == TOK_PLUS && tokens[*idx+1].type == TOK_PLUS) || 
     (tokens[*idx].type == TOK_MINUS && tokens[*idx+1].type == TOK_MINUS)) &&
    (tokens[*idx].start == (tokens[idd+1].start + tokens[idd+1].length)) && 
    (tokens[*idx+1].start - tokens[*idx].start == 1)){
    //  ("add after!!!\n");
    Operation op_after = tokens[*idx].type == TOK_PLUS ? OP_ADD_AFTER : OP_SUBTRACT_AFTER;
      ++*idx;
    ++*idx;
      return new_node(
        .type = ASSIGNMENT,
        .assignment.target = expr,
        .assignment.op = op_after,
        .assignment.value = new_node(.type = BINARY, .binary.left = expr, .binary.right = new_node(.type=LITERAL, .literal.type=INT, .literal.num=1), .binary.op = op_after)
    );
  }
  return expr;
}


Node * parse_unary(Token * tokens, char * text, size_t *idx){
  //TRACE_PARSER ("      %sunary%s ", COLOR_BLACK, COLOR_RESET);
  TRACE_PARSER print_n_tokens(tokens, text, *idx);
  //("UNARY\n");
  Token tok = tokens[*idx];
  Token before = tok;
  //Node * expr = parse_postfix(tokens, text, idx);  //deeper
  if (tokens[*idx].type == TOK_MINUS || tokens[*idx].type == TOK_PLUS || tokens[*idx].type == TOK_NOT) {
    Operation op = tokens[*idx].type == TOK_MINUS ? OP_UNARY_MINUS : tokens[*idx].type == TOK_NOT ? OP_UNARY_NOT : NOOP;
    //*
    //("HIT  WHILE\n");
    if(tokens[*idx].type == TOK_PLUS && tokens[*idx+1].type == TOK_PLUS && (tokens[*idx+1].start - tokens[*idx].start == 1) && (tokens[*idx+2].start - tokens[*idx+1].start == 1) && tokens[*idx+2].type == IDENTIFIER){
      ++*idx;
      ++*idx;
      Node * right = parse_postfix(tokens, text, idx);
     // ("UNARY INC\n");
      return new_node(
          .type = ASSIGNMENT,
          .assignment.target = right,
          .assignment.op=NOOP,
          .assignment.value = new_node(.type = BINARY, .binary.left = right, .binary.right = new_node(.type=LITERAL, .literal.type=INT, .literal.num=1), .binary.op = OP_ADD)
      );
    }else if(tokens[*idx].type == TOK_MINUS && tokens[*idx+1].type == TOK_MINUS && (tokens[*idx+1].start - tokens[*idx].start == 1) && (tokens[*idx+2].start - tokens[*idx+1].start == 1) && tokens[*idx+2].type == IDENTIFIER){
      ++*idx;
      ++*idx;
      Node * right = parse_postfix(tokens, text, idx);
      return new_node(
          .type = ASSIGNMENT,
          .assignment.target = right,
          .assignment.op=NOOP,
          .assignment.value = new_node(.type = BINARY, .binary.left = right, .binary.right = new_node(.type=LITERAL, .literal.type=INT, .literal.num=1), .binary.op = OP_SUBTRACT)
      );
    }
    //*/
    ++*idx;
    if(op == NOOP){
      tok = tokens[*idx];
      Node * rh = parse_unary(tokens, text, idx);
      if(rh->type == ERROR){
        return new_node(.type=ERROR, .error.type=INVALID_RIGHT_HAND,.error.token.line=before.line,
          .error.token.col=before.col+1,
          .error.token.length= (tokens[*idx].start+tokens[*idx].length)-before.start, //(before.col+1+tokens[*idx].length)-tokens[*idx].col,
          .error.inner=rh);
        //return new_node(.type=ERROR, .error.type=INVALID_RIGHT_HAND,.error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=rh);
      }
      return rh;
    }
    Node * rhs = parse_unary(tokens, text, idx);
    if(rhs->type == ERROR){  //add pointers and shit here. for now assume it means nothing to the right.    +""+0  !0  -0
      if(op != NOOP && rhs->error.type == EXPECTED_LITERAL){   // !-  expect a number
        return new_node(.type=ERROR, .error.type=ERROR_NAN,
           .error.token.col=tok.col,
            .error.token.line=tok.line,
             .error.token.length=tokens[*idx].col-tok.col, .error.inner=rhs);
//tokens[*idx].type == TOK_MINUS ? OP_UNARY_MINUS : tokens[*idx].type == TOK_NOT ? OP_UNARY_NOT : NOOP;  NAN
      }//TOK_MINUS  OP_UNARY_NOT    NAN         NAN+NOLITERAL
      
      return rhs;
    }
    return new_node( .type= UNARY, .unary.right = rhs, .unary.op=op);
  }
 // ("unary->post %s\n", token_to_str(tokens[*idx]));
  //("unary->postfix %s\n", token_to_str(tokens[*idx]));
  return parse_postfix(tokens, text, idx);
}

Node * parse_factor(Token *tokens, char * text, size_t *idx) {
  //TRACE_PARSER "      %sfactor%s ", COLOR_MAGENTA, COLOR_RESET);
  TRACE_PARSER print_n_tokens(tokens, text, *idx);
  
  Token tok = tokens[*idx];
 

  Node * expr = parse_unary(tokens, text, idx);  //deeper
  //("UNAR\n");
  Token before = tokens[*idx];

  while (tokens[*idx].type == TOK_STAR || tokens[*idx].type == TOK_SLASH) {
    if(expr->type == ERROR){
      if(expr->error.type == EXPECTED_LITERAL){
      return new_node(.type=ERROR, .error.type=INVALID_LEFT_HAND, .error.token.line=tokens[*idx].line,
         .error.token.col=tokens[*idx].col-1,
         .error.token.length=tokens[*idx].col-tok.col,
        // .error.inner=//new_node(.type=ERROR, .error.type=ERRfOR_NAN,.error.token.line=tokens[*idx].line,
            //.error.token.col=tokens[*idx].col-1,
            //.error.token.length= tokens[*idx].col-tok.col,
            .error.inner=new_node(.type=ERROR, .error.type=EXPECTED_NUMBER, .error.token=expr->error.token)
          //  ) 
          );
      }else if(expr->error.type == UNEXPECTED_EOI){
      return new_node(.type=ERROR, .error.type=INVALID_LEFT_HAND, .error.token.line=tokens[*idx].line,
         .error.token.col=tokens[*idx].col-1,
         .error.token.length=tokens[*idx].col-tok.col,
         .error.inner=expr
            
          );
      }else{
        //"NOT EXPECTED LITERAL\n");
        return expr; /*new_node(.type=ERROR, .error.type=INVALID_LEFT_HAND,.error.token.line=tokens[*idx].line,
            .error.token.col=tokens[*idx].col-1,
            .error.token.length= tokens[*idx].col-tok.col,
            .error.inner=expr
            );*/
      }
    }
      
    Token _tok = tokens[*idx];
    Operation op = (tokens[*(idx)].type == TOK_STAR ? OP_MULTIPLY : OP_DIVISION);
    ++*idx;
    tok = tokens[*idx];
    Node * rhs = parse_unary(tokens, text, idx);  //exp
    if(rhs->type == ERROR){
      if(rhs->error.type != EXPECTED_LITERAL && rhs->error.type != UNEXPECTED_EOI){  //fail safe
        int ll = tokens[*idx].col-before.col;
        //tokens[*idx+1].col - before.col;

        if(ll <= 0)
          // ll = tokens[*idx+1].col - before.col;
          ll = tokens[*idx].start - before.start;

        return new_node(.type=ERROR, .error.type=INVALID_RIGHT_HAND,
          .error.token.line=before.line,
          .error.token.col=tok.col+1,
          .error.token.length= ll, //(before.col+1+tokens[*idx].length)-tokens[*idx].col,
          //.error.inner=new_node(.type=ERROR, .error.type=ERROhR_NAN,
          //  .error.token.col=before.col, .error.token.line=before.line,
           // .error.token.length= tok.col-before.col,
            .error.inner=rhs//)
          );

      }else if(rhs->error.type == UNEXPECTED_EOI){
        return new_node(.type=ERROR, .error.type=INVALID_RIGHT_HAND,.error.token.line=before.line,
            .error.token.col=tok.col+1,
            .error.token.length= tokens[*idx].col-before.col+tokens[*idx].length, //(before.col+1+tokens[*idx].length)-tokens[*idx].col,
            .error.inner=rhs
            );
      }else{ // most errors go here.
        int ll = tokens[*idx].col-before.col + tokens[*idx].length-1;

        if(ll <= 0)
          ll = tokens[*idx].start - before.start + tokens[*idx].start-1;
        //
         // rhs->error.type == ERRfOR_NAN;
        if(rhs->error.type == EXPECTED_LITERAL){
          return new_node(.type=ERROR, .error.type=INVALID_RIGHT_HAND,.error.token.line=before.line,
            .error.token.col=before.col+1,
            .error.token.length= (size_t)ll , 
            .error.inner=new_node(.type=ERROR, .error.type=ERROR_NAN, .error.token=rhs->error.token)
          );
        }else{
          return new_node(.type=ERROR, .error.type=INVALID_RIGHT_HAND,.error.token.line=before.line,
                  .error.token.col=before.col+1,
                  .error.token.length= (size_t)ll , 
                  .error.inner=rhs
            );
        }
        
      }
    }
    Token tr = (Token){
      .type=tok.type,
      .line=tok.line,
      .col=_tok.col,
      .start=tok.start,
      .length=tok.col-_tok.col
    };
    expr = new_node( .type= BINARY, .binary.left = expr, .binary.right = rhs, .binary.op=op, .binary.token_left=expr->token, .binary.token_right=tr, .binary.token_op=_tok);
  }

  return expr;
}


Node * parse_term(Token *tokens, char * text, size_t *idx) {
  //TRACE_PARSER "    %sterm%s ", COLOR_MAGENTA, COLOR_RESET);
  TRACE_PARSER print_n_tokens(tokens, text, *idx);
  
  Token tok = tokens[*idx];
  Node * expr = parse_factor(tokens, text, idx);  
  //"FACTOR\n");
  Token before = tokens[*idx];
  while (tokens[*idx].type == TOK_PLUS  || tokens[*idx].type == TOK_MINUS) {
        
    Operation op = (tokens[*(idx)].type == TOK_PLUS ? OP_ADD : OP_SUBTRACT);
    ++*idx;
    Node * rhs = parse_factor(tokens, text, idx);
    
    if(rhs->type == ERROR){
      if((expr->error.type != UNEXPECTED_EOI && rhs->error.type == UNEXPECTED_EOI) && rhs->error.type == EXPECTED_LITERAL){
        return new_node(.type=ERROR, .error.type=INVALID_RIGHT_HAND,
          .error.token.line=tok.line,
          .error.token.col=before.col + 1,
          .error.token.length=tok.col,
          .error.inner = rhs//new_node(.type=ERROR, .error.type=ERROfR_NAN, 
           // .error.token.line=tok.line,
           // .error.token.col=before.col + 1,
            //.error.token.length=tok.col,)
           );
      }else if( expr->error.type == UNEXPECTED_EOI){
        return new_node(.type=ERROR, .error.type=INVALID_RIGHT_HAND,
          .error.token.line=tok.line,
          .error.token.col=before.col + 1,
          .error.token.length=tok.col,
          .error.inner = expr //new_node(.type=ERROR, .error.type=UNEXPECTED_EOI, 
          //  .error.token.line=tok.line,
           // .error.token.col=before.col + 1,
          //  .error.token.length=tok.col,)
           );
      }else{
        return new_node(.type=ERROR, .error.type=INVALID_RIGHT_HAND,
          .error.token.line=tok.line,
        .error.token.col=before.col + 1,
         .error.token.length=tok.length,
          .error.inner=rhs);
      }
      
    }

    expr = new_node( .type= BINARY, .binary.left = expr, .binary.right = rhs, .binary.op=op);
  }
  /**/
  if(expr->type == ERROR){
      //if(expr->error.type == EXPECTED_LITERAL){
        return expr; /*new_node(.type=ERROR, .error.type=EXPECTED_EXP,
          .error.token.line=tok.line,
          .error.token.col=tok.col,
          .error.token.length=tok.length);*/
     // }
  }
  return expr;
}

Node * parse_group(Token * tokens, char * text, size_t *idx){
  //TRACE_PARSER "  %sgroup%s ", COLOR_YELLOW, COLOR_RESET);
  TRACE_PARSER print_n_tokens(tokens, text, *idx);
  ++*idx;
  Token tok = tokens[*idx];
  //"99%s\n", token_to_str(tok));
  //if(tok.type == TOK_RIGHT_PAREN)
  //  return new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=1);

  Node * group = parse_exp(tokens, text, idx);
  if(group->type == ERROR)
    return group; //new_node(.type=ERROR, .error.type=SYNTAfX_ERROR, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=group);

  if(tokens[*idx].type == TOK_RIGHT_PAREN){
    ++*idx;
  }else{
    return new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx]);
  }
  return new_node(.type=GROUP, .group.exp=group);
}

//--------------------------------------------------------------------------------------------------------------
Node * parse_comparison(Token *tokens, char * text, size_t *idx){ //comparison     -> term  ((">" | ">=" | "<" | "<=") term)*
  Token tok = tokens[*idx];
  Node *expr = parse_term(tokens, text, idx);
  
    while (tokens[*idx].type == TOK_GREATER ||
           tokens[*idx].type == TOK_GREATER_EQUAL ||
           tokens[*idx].type == TOK_LESS ||
           tokens[*idx].type == TOK_LESS_EQUAL) {

        Operation op = (tokens[*idx].type == TOK_GREATER)       ? OP_GREATER :
                       (tokens[*idx].type == TOK_GREATER_EQUAL) ? OP_GREATER_EQUAL :
                       (tokens[*idx].type == TOK_LESS)          ? OP_LESS :
                                                              OP_LESS_EQUAL;
        tok = tokens[*idx];
        ++*idx;
        Node *rhs = parse_term(tokens, text, idx);
        
        if(rhs->type == ERROR)
          return rhs; //new_node(.type=ERROR, .error.type=EXPECTED_TERM, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].start-tok.start, .error.inner=rhs);
    
        expr = new_node(.type = BINARY, .binary.left = expr, .binary.right = rhs, .binary.op = op);
    }

  return expr;
}




Node * parse_equality(Token *tokens, char * text, size_t *idx){  // equality       -> comparison (("!=" | "==") comparison)*

  Node *expr = parse_comparison(tokens, text, idx);
  while (tokens[*idx].type == TOK_EQUAL_EQUAL ||
           tokens[*idx].type == TOK_NOT_EQUAL) {
    
    Operation op = (tokens[*idx].type == TOK_EQUAL_EQUAL) ? OP_EQUAL : OP_NOT_EQUAL;
    ++*idx;
    Node *rhs = parse_comparison(tokens, text, idx);
    expr = new_node(.type = BINARY, .binary.left = expr, .binary.right = rhs, .binary.op = op);
  }

  return expr;
  //return parse_comparison(tokens, text, idx);
}


Node * parse_bitwise_and(Token *tokens, char * text, size_t *idx){  //logic_and      -> equality ("&&" equality)*
  
    Node *expr = parse_equality(tokens, text, idx);
    while (tokens[*idx].type == TOK_BW_AND) {
        ++*idx;
        Node *rhs = parse_equality(tokens, text, idx);
        expr = new_node(.type = BINARY, .binary.left = expr, .binary.right = rhs, .binary.op = OP_BITWISE_AND);
    }

  return expr;
}

Node * parse_bitwise_xor(Token *tokens, char * text, size_t *idx){  //logic_and      -> equality ("&&" equality)*
  
    Node *expr = parse_bitwise_and(tokens, text, idx);
    while (tokens[*idx].type == TOK_BW_XOR) {
        ++*idx;
        Node *rhs = parse_bitwise_and(tokens, text, idx);
        expr = new_node(.type = BINARY, .binary.left = expr, .binary.right = rhs, .binary.op = OP_BITWISE_XOR);
    }

  return expr;
}

Node * parse_bitwise_or(Token *tokens, char * text, size_t *idx){  //logic_and      -> equality ("&&" equality)*
  
    Node *expr = parse_bitwise_xor(tokens, text, idx);
    while (tokens[*idx].type == TOK_BW_OR) {
        ++*idx;
        Node *rhs = parse_bitwise_xor(tokens, text, idx);
        expr = new_node(.type = BINARY, .binary.left = expr, .binary.right = rhs, .binary.op = OP_BITWISE_OR);
    }

  return expr;
}

Node * parse_logic_and(Token *tokens, char * text, size_t *idx){  //logic_and      -> equality ("&&" equality)*
  
    Node *expr = parse_bitwise_or(tokens, text, idx);
    while (tokens[*idx].type == TOK_LOGIC_AND) {
        ++*idx;
        Node *rhs = parse_bitwise_or(tokens, text, idx);
        expr = new_node(.type = BINARY, .binary.left = expr, .binary.right = rhs, .binary.op = OP_LOGIC_AND);
    }

  return expr;
}


Node * parse_logic_or(Token *tokens, char * text, size_t *idx){  //logic_or       -> logic_and ("||" logic_and)*

  Node *expr = parse_logic_and(tokens, text, idx);
    while (tokens[*idx].type == TOK_LOGIC_OR) {
        ++*idx;
        Node *rhs = parse_logic_and(tokens, text, idx);
        expr = new_node(.type = BINARY, .binary.left = expr, .binary.right = rhs, .binary.op = OP_LOGIC_OR);
    }

  return expr;

}
//--------------------------------------------------------------------------------------------------------------
// assignment     → ( e_call "." )? IDENTIFIER "=" assignment | logic_or ;
Node * parse_assignment(Token *tokens, char * text, size_t *idx){  //assignment     -> identifier "=" assignment | logic_or
  Token tok = tokens[*idx];
  Token bef = tokens[*idx];
   //("ASSIGN %s\n", token_to_str(tokens[*idx]));
  Node *left = parse_logic_or(tokens, text, idx);  // returns from unary(identifier) or call
  
  //"logic or good\n");
  Operation op;
  switch(tokens[*idx].type){
    case TOK_BW_AND_ASSIGN:     // &=
      op = OP_BITWISE_AND;
      break;
    case TOK_BW_OR_ASSIGN:      // |=
      op = OP_BITWISE_OR;
      break;
    case TOK_BW_XOR_ASSIGN:     // ^=
      op = OP_BITWISE_XOR;
      break;
    case TOK_ADD_ASSIGN:        // +=
      op = OP_ADD;
      break;
    case TOK_SUBTRACT_ASSIGN:   // -=
      op = OP_SUBTRACT;
      break;
    case TOK_MULTIPLY_ASSIGN:   // *=
      op = OP_MULTIPLY;
      break;
    case TOK_DIVIDE_ASSIGN:     // /=
      op = OP_DIVISION;
      break;
    case TOK_MODULO_ASSIGN:     // %=
      op = OP_MODULO;
      break;
    default:  
      op = NOOP;
  }
  if (tokens[*idx].type == TOK_ASSIGN) {
      
      
    //____________________________________________________________________________________________
    if(0 && tokens[*idx+1].type == TOK_NEW){
      ++*idx;
      ++*idx;
      //_FUNCTION
      Node * callee = parse_primary(tokens, text, idx);
      if(callee->type == ERROR)
        return new_node(.type=ERROR, .error.type=EXPECTED_IDENTIFIER, .error.token=tokens[*idx], .error.inner=callee);

      if(callee->type != VARIABLE)
        return new_node(.type=ERROR, .error.type=EXPECTED_IDENTIFIER, .error.token=tokens[*idx]);
      
      //________________________________________________________________________________________________________________________________________________________        
      if (tokens[*idx].type != TOK_LEFT_PAREN)
        return new_node(.type=ERROR, .error.type=NO_LEFT_PAREN, .error.token=tokens[*idx]);
      ++*idx;

      Node * args = parse_arguments(callee, tokens, text, idx);
      if(args->type == ERROR)
        return args;//new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token=tokens[*idx], .error.inner=func_call);

      if (tokens[*idx].type != TOK_RIGHT_PAREN)
        return new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx]);
      ++*idx;

        /*return new_node(.type = FUNCTION_CALL,
                        .function.callee=callee,
                        .function.args=args->function.args,
                        .function.arg_count=args->function.arg_count,
                        .function.token_args=args->function.token_args
                      );*/
      return new_node(.type = ASSIGNMENT,
        .assignment.target = left,
        .assignment.op=NOOP,
        .assignment.value = new_node(.type=CALL, .call.type=CONSTRUCT,
              .call.callee=callee,  //= new class name|type
              .call.construct.args=args->call.construct.args,
              .call.construct.argc=args->call.construct.argc));
          
    }
    //____________________________________________________________________________________________
    if (left->type != VARIABLE && left->type != CALL) {
      fprintf(stdout, "new %s\n", node_type(*left));
        return new_node(.type=ERROR, .error.type=INVALID_ASSIGNMENT_TARGET, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col);
    }
    //if()
     // ("new %s\n", left->call.callee->variable);
    ++*idx;  // consume '='
    tok = tokens[*idx];
    Node *right = parse_assignment(tokens, text, idx);  // right-associative
    if(right->type == ERROR){
      /* if(right->error.type == EXPECTED_LITERAL){
        return new_node(.type=ERROR, .error.type=SYNTfAX_ERROR,
          .error.token.col=tok.col,
          .error.token.line=tok.line,
            .error.token.length=tokens[*idx].col-tok.col,
            .error.inner=right);
      }else{*/
        return new_node(.type=ERROR, .error.type=INVALID_RIGHT_HAND,
          .error.token.col=tok.col,
          .error.token.line=tok.line,
            .error.token.length=tokens[*idx].col-tok.col,
            .error.inner=right);
      //}
      
    }
      

    //expr = new_node(.type = BINARY, .binary.left = expr, .binary.right = rhs, .binary.op = OP_LOGIC_OR);
    return new_node(
        .type = ASSIGNMENT,
        .assignment.target = left,
        .assignment.op=NOOP,
        .assignment.value = right,
        .assignment.token_target=bef
    );

  }else if(op != NOOP){
    if (left->type != VARIABLE && left->type != CALL) {
          return new_node(.type=ERROR, .error.type=INVALID_ASSIGNMENT_TARGET, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col);
      }

      ++*idx;  // consume '='
      Node *right = parse_assignment(tokens, text, idx);  // right-associative
      if(right->type == ERROR)
        return new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=right);

      return new_node(
          .type = ASSIGNMENT,
          .assignment.target = left,
          .assignment.op=NOOP,
          .assignment.value = new_node(.type = BINARY, .binary.left = left, .binary.right = right, .binary.op = op)
      );
  }else{
    if(left->type == ERROR)
    return left; //new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].length, .error.inner=left);

  }

  return left;
}
//*
Node * parse_comma(Token *tokens, char *text, size_t *idx) {
    
//*
    Node **items = malloc(sizeof(Node *));
    size_t count = 0;
    
    //Node *expr = parse_assignment(tokens, text, idx);
    //if (expr->type == ERROR) return expr;

    //items[0] = expr;
    Node *next;
    Token tok = tokens[*idx];
    int itr = 0;
    while (tokens[*idx].type != TOK_SEMICOLON && tokens[*idx].type != TOK_RIGHT_PAREN) {
        itr++;
        next = parse_assignment(tokens, text, idx); 
      
        if(next->type == ERROR){
          return next;
        }
        //tok = tokens[*idx];

        if(tokens[*idx].type == TOK_EOF)  //  (tokens[*idx].start-tok.start-tok.length)
          return new_node(.type=ERROR, .error.type=NO_SEMICOLON,
             .error.token.col=tokens[*idx].line > tok.line ? tokens[*idx-1].col+tokens[*idx-1].length : tokens[*idx].col, .error.token.line=tok.line,
              .error.token.length=1);
        

        items = realloc(items, sizeof(Node *) * (count + 1));
        items[count] = next;

        if(tokens[*idx].type == TOK_COMMA){
          ++*idx;
          ++count;
          //tok = tokens[*idx];
        }
    }
    //for(size_t i = 0; i <= count; i++){
      //"COUNT: %d\n", i);
    //}
    //"COUNT: %d  ITR: %d\n", count, itr);

    // If only one expression, return it directly
    if (count == 0) {
        free(items);
        return next;
    }else if(itr == 0){
      return new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token=tok);
      //"UNEXPECTED SEMI OR CLOSING BRACKET %d\n", count);
    }

    // Build SEQUENCE node
    return new_node(.type = SEQUENCE, .sequence.exp = items, .sequence.count = count+1, 
      .sequence.token=(Token){.type=tok.type, .line=tok.line, .col=tok.col, .length=tokens[*idx].col-tok.col, .start=tok.start});
    //*/
    //return parse_assignment(tokens, text, idx);
}

/*
Node * parse_comma(Token * tokens, char * text, size_t *idx){
//parse_comma → parse_assignment → parse_equality
//ternary        -> assignment ("?" expression ":" assignment)?
  Node * assign = parse_assignment(tokens, text, idx);
  Node * seq = new_node(.type=SEQUENCE, .sequence.exp=assign, .sequence.count=1);
  while(tokens[*idx].type == TOK_COMMA){
    return seq;
  }
  return assign;
}*/

Node * parse_ternary(Token * tokens, char * text, size_t *idx){ //ternary        -> assignment ("?" expression ":" assignment)?
  
      Node *left = parse_comma(tokens, text, idx);
    //"COMMA GOOD\n");
      if(left->type == ERROR)
        return left; //new_node(.type=ERROR, .error.type=EXPECgTED_EXP, .error.token=tokens[*idx], .error.inner=left);
        
    if (tokens[*idx].type == TOK_QUESTION_MARK) {
        ++*idx;  // consume '?'
        Node *true_expr = parse_exp(tokens, text, idx);
        if(true_expr->type == ERROR)
          return true_expr; //new_node(.type=ERROR, .error.type=EXPEgCTED_EXP, .error.token=tokens[*idx], .error.inner=true_expr);

        if (tokens[*idx].type != TOK_COLON) {
            return new_node(.type=ERROR, .error.type=MISSING_COLON, .error.token=tokens[*idx]);
        }

        ++*idx;  // consume ':'
        Node *false_expr = parse_assignment(tokens, text, idx);
        if(false_expr->type == ERROR)
          return false_expr; //new_node(.type=ERROR, .error.type=EXPECgTED_EXP, .error.token=tokens[*idx], .error.inner=false_expr);

        return new_node(
            .type = TERNARY,
            .ternary.condition = left,
            .ternary.true_branch = true_expr,
            .ternary.false_branch = false_expr
        );
    }

  return left;
}

Node * parse_exp(Token * tokens, char * text, size_t *idx){  //expression     -> ternary
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


  */
  return parse_ternary(tokens, text, idx);
}





/*
statement      → exprStmt
               | forStmt
               | ifStmt
               | printStmt
               | returnStmt
               | whileStmt
               | block ;

exprStmt       → expression ";" ;
forStmt        → "for" "(" ( varDecl | exprStmt | ";" )
                           expression? ";"
                           expression? ")" statement ;
ifStmt         → "if" "(" expression ")" statement
                 ( "else" statement )? ;
printStmt      → "print" expression ";" ;
returnStmt     → "return" expression? ";" ;
whileStmt      → "while" "(" expression ")" statement ;
block          → "{" declaration* "}" ;
*/

Node * parse_block(Token * tokens, char * text, size_t *idx){  // block          → "{" declaration* "}" ;
  Token tok = tokens[*idx];
  Token bef = tok;
  if(tokens[*idx].type != TOK_LEFT_CURLY)
    return new_node(.type=ERROR, .error.type=TOKEN_SYNC_ERROR, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col);
  ++*idx;
  
    size_t count = 0;
    Node **statements = malloc(sizeof(Node *));
    Node * decl;
    while(tokens[*idx].type != TOK_RIGHT_CURLY && tokens[*idx].type !=TOK_EOF){
      tok = tokens[*idx];
      decl = parse_declaration(tokens, text, idx);

      if(decl->type == ERROR){
        //"+++ %s\n", token_to_str(tok));
        //if(tok.type == TOK_RETURN || tok.type == TOK_CONTINUE || tok.type == TOK_BREAK || tok.type == TOK_PRINT || tok.type == TOK_VAR)
         // return new_node(.type=ERROR, .error.type=NO_SEMICOLON, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=decl);
        return decl; //new_node(.type=ERROR, .error.type=SYNT8AX_ERROR, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=decl);
      }
      statements[count++] = decl;
      Node **temp_list = realloc(statements,  sizeof(Node *) * (count + 1));
      statements = temp_list;
    }
    

    if(tokens[*idx].type != TOK_RIGHT_CURLY)
     return new_node(.type=ERROR, .error.type=UNTERMINATED_BLOCK,
       .error.token.line=bef.line,
       .error.token.col=bef.col,  
       .error.token.length=1,
       .error.inner=new_node(.type=ERROR, .error.type=NO_RIGHT_BRACE,
                             .error.token.line=tokens[*idx].line,
                             .error.token.col=tokens[*idx].col,  
                             .error.token.length=1
                            )
      );
    ++*idx;

    return new_node(.type=STATEMENT, .statement.type=BLOCK, .statement.block.statements=statements, .statement.block.count = count);
}


Node * parse_if(Token * tokens, char * text, size_t *idx){
  /*
ifStmt         → "if" "(" expression ")" statement
                 ( "else" statement )? ;

*/
//"IFF__ \n");
  if(tokens[*idx].type != TOK_IF)
    return new_node(.type=ERROR, .error.type=TOKEN_SYNC_ERROR, .error.token=tokens[*idx]);
  ++*idx;
  if(tokens[*idx].type != TOK_LEFT_PAREN)
    return new_node(.type=ERROR, .error.type=NO_LEFT_PAREN, .error.token=tokens[*idx]);
  
  Token tok2 = tokens[*idx];
  ++*idx;
  Token tok = tokens[*idx];
  if(tok.type == TOK_RIGHT_PAREN){
    Node * dummy = new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token.col=tok2.col, .error.token.line=tok2.line, .error.token.length=tokens[*idx].col-tok2.col);
    return new_node(.type=ERROR, .error.type=SYNTAX_ERROR,
       .error.token.col=tok2.col+1, .error.token.line=tok.line,
        .error.token.length=tokens[*idx].col-tok2.col,
         .error.inner=dummy
        );
  }
  //"NNN__ %s\n", token_to_str(tok));
//"NNN__\n");
  Node * condition = parse_exp(tokens, text, idx);
  //"NNN__\n");
  if(condition->type == ERROR)
    return new_node(.type=ERROR, .error.type=SYNTAX_ERROR, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].start-tok.start+tokens[*idx].length, .error.inner=condition);

  if(tokens[*idx].type != TOK_RIGHT_PAREN)
    return new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col);
  ++*idx;

  tok = tokens[*idx];
  //"NNN__ %s\n", token_to_str(tok));
  Node * then_branch = parse_statement(tokens, text, idx);
  if(then_branch->type == ERROR)
    return new_node(.type=ERROR, .error.type=EXPECTED_STATEMENT, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=then_branch);
  
  tok = tokens[*idx];
  Node * else_branch = NULL;
  if(tokens[*idx].type == TOK_ELSE){
    ++*idx;
    tok = tokens[*idx];
    else_branch = parse_statement(tokens, text, idx);
    if(else_branch->type == ERROR)
      return new_node(.type=ERROR, .error.type=EXPECTED_STATEMENT, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=else_branch);
    
  }
  
  return new_node(.type=STATEMENT,
    .statement.type=IF_STMT,
    .statement.if_statement.condition=condition,
    .statement.if_statement.then_branch=then_branch,
    .statement.if_statement.else_branch=else_branch
    );
}


Node * parse_for(Token * tokens, char * text, size_t *idx){
/*
forStmt        → "for" "(" ( varDecl | exprStmt | ";" )
                           expression? ";"
                           expression? ")" statement ;

*/
  if(tokens[*idx].type != TOK_FOR)
    return new_node(.type=ERROR, .error.type=TOKEN_SYNC_ERROR, .error.token=tokens[*idx]);
  ++*idx;
  if(tokens[*idx].type != TOK_LEFT_PAREN)
    return new_node(.type=ERROR, .error.type=TOKEN_SYNC_ERROR, .error.token=tokens[*idx]);
  ++*idx;
  Token tok = tokens[*idx];
  
  Node * initializer;
  if(tokens[*idx].type == TOK_SEMICOLON){  //expecting expStmt, ";" will be consumed in this block!!!
    initializer = NULL;  // for(;;)
  }else{
    if(tokens[*idx].type == TOK_VAR){
      tok = tokens[*idx];
      initializer = parse_var_declaration(tokens, text, idx);  //for(exp; ___)
      if(initializer->type == ERROR)
          return new_node(.type=ERROR, .error.type=EXPECTED_EXP,
             .error.token.col=tok.col, .error.token.line=tok.line,
              .error.token.length=tokens[*idx].col-tok.col,
               .error.inner=initializer
              );
      //"tok:decl: %s\n", token_to_str(tokens[*idx]));
      if(tokens[*idx].type != TOK_SEMICOLON)
        return new_node(.type=ERROR, .error.type=NO_SEMICOLON, .error.token=tokens[*idx]);
      ++*idx;
    }else{
        tok = tokens[*idx];
        initializer = parse_exp(tokens, text, idx);
        if(initializer->type == ERROR){
          //"ERR: %s\n", error_to_str(initializer->error.type));
          return new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=initializer);
        }
        //"tok:exp: %s\n", token_to_str(tokens[*idx]));
        if(tokens[*idx].type != TOK_SEMICOLON)
          return new_node(.type=ERROR, .error.type=NO_SEMICOLON, .error.token=tokens[*idx]);
        ++*idx;
    }
  }

  Node * condition = new_node(.type=LITERAL, .literal.type=INT, .literal.num=1);

  tok = tokens[*idx];
  condition = parse_exp(tokens, text, idx);  //for(stmtexp; exp;)
  //"tok:cond: %s\n", token_to_str(tokens[*idx]));
  if(condition->type == ERROR){
    //"ERR2: %s\n", error_to_str(condition->error.type));
    return new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=condition);
  }
     
  if(tokens[*idx].type != TOK_SEMICOLON)
    return new_node(.type=ERROR, .error.type=NO_SEMICOLON, .error.token=tokens[*idx]);
  ++*idx; //consume semi -> init was empty, or parse_exp returned. either way MUST consume semi, at this stage it must be one

  Node* increment;
  if(tokens[*idx].type == TOK_SEMICOLON){  //expecting exp?";" SEMI_COLON will NOT be consumed in this block!!!
    increment = NULL;  // for(;;)
  }else{
    tok = tokens[*idx];
    increment = parse_exp(tokens, text, idx);  //for(stmtexp; exp; exp;)
    if(increment->type == ERROR)
      return new_node(.type=ERROR, .error.type=EXPECTED_EXP,
         .error.token.col=tok.col, .error.token.line=tok.line,
          .error.token.length=tokens[*idx].col-tok.col,
           .error.inner=increment
          );
  } 
  

  if(tokens[*idx].type != TOK_RIGHT_PAREN)
    return new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx]);
  ++*idx;

  Node* body = parse_statement(tokens, text, idx);
  if(body->type == ERROR)
    return body;

  return new_node(.type=STATEMENT, 
                  .statement.type=FOR_STMT,
                  .statement.for_statement.initializer=initializer,
                  .statement.for_statement.condition=condition,
                  .statement.for_statement.increment=increment,
                  .statement.for_statement.body=body
                  );
}

Node * parse_print(Token * tokens, char * text, size_t *idx){  // printStmt      → "print" expression ";" ;
  Token bef = tokens[*idx];
  if(tokens[*idx].type == TOK_PRINT){
    ++*idx;
  }else{
    fprintf(stdout, "%sTIME TO PANIC!!!!  someone called print without verifying print token exists!%s\n", COLOR_RED, COLOR_RESET);
    return new_node(.type=ERROR, .error.type=TOKEN_SYNC_ERROR, .error.token=tokens[*idx]);
  }
  Token tok = tokens[*idx];
  Node * expr = parse_comma(tokens, text, idx);
    if(expr->type == ERROR){
     return new_node(.type=ERROR, .error.type=SYNTAX_ERROR,
        .error.token.col= tok.col,
         .error.token.line=tokens[*idx].line,
          .error.token.length=tok.length,
           .error.inner=expr);
    }

  ++*idx;
  Token tt = (Token){
    .type = tok.type,
    .start = tok.start,
    .line=bef.line,
    .length=tok.length,
    .col=tok.col
  };
  return new_node(.type=STATEMENT, .statement.type=PRINT, .statement.print.exp = expr, .statement.print.token_exp = tt);
}





Node * parse_while(Token * tokens, char * text, size_t *idx){  // whileStmt      → "while" "(" expression ")" statement ;
  if(tokens[*idx].type == TOK_WHILE){
    ++*idx;
  }else{
    fprintf(stdout, "%sTIME TO PANIC!!!!  someone called while without verifying while token exists!%s\n", COLOR_RED, COLOR_RESET);
    return new_node(.type=ERROR, .error.type=TOKEN_SYNC_ERROR, .error.token=tokens[*idx]);
  }
  if(tokens[*idx].type == TOK_LEFT_PAREN){
    ++*idx;
  }else{
    fprintf(stdout, "%sPANIC TIME, expected left parenthesis%s\n", COLOR_RED, COLOR_RESET);
    return new_node(.type=ERROR, .error.type=TOKEN_SYNC_ERROR, .error.token=tokens[*idx]);
  }
  Node * condition = parse_exp(tokens, text, idx);
  if(tokens[*idx].type == TOK_RIGHT_PAREN){
    ++*idx;
  }else{
    fprintf(stdout, "%sPANIC TIME, expected right parenthesis%s\n", COLOR_RED, COLOR_RESET);
    return condition; //new_node(.type=ERROR, .error.type=NO_RIGHT_PAREN, .error.token=tokens[*idx]);
  }
 
  Node * body = parse_statement(tokens, text, idx);
  if(body->type == ERROR)
    return body;
  return new_node(.type=STATEMENT, .statement.type=WHILE_STMT, .statement.while_statement.condition=condition, .statement.while_statement.body=body);
 
}


Node * parse_return(Token * tokens, char * text, size_t *idx){ // returnStmt     → "return" expression? ";" ;
  Token ret_tok = tokens[*idx];
  ++*idx;  // consume "return"
  if(tokens[*idx].type == TOK_SEMICOLON){
    ++*idx;
    //returning control flow done is returning void essentialy
    return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_RETURN, .control_flow.carry_back=new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE));
  }
  Token tok = tokens[*idx];
  Node * exp = parse_exp(tokens, text, idx);
  if(exp->type == ERROR){
    return new_node(.type=ERROR, .error.type=EXPECTED_EXP, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=exp);
  }
  if(tokens[*idx].type != TOK_SEMICOLON){
    return new_node(.type=ERROR, .error.type=NO_SEMICOLON, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col);
  }
  ++*idx;
  Token back = (Token){
    .type=tok.type,
    .line=ret_tok.line,
    .start=ret_tok.start,
    .col=tok.col,
    .length=5
  };
  return new_node(.type=STATEMENT, .statement.type=RETURN_STMT, .statement.return_statement.value= exp, .statement.return_statement.token_return_value=back);
}


Node * parse_statement(Token * tokens, char * text, size_t *idx){
  Token tok = tokens[*idx];
  Node* expr;
  switch(tokens[*idx].type){
    case TOK_LEFT_CURLY:         // statement      → block ;
      return parse_block(tokens, text, idx);
    case TOK_IF:                 // statement      → ifStmt
      return parse_if(tokens, text, idx);
    case TOK_FOR:                // statement      → forStmt
      return parse_for(tokens, text, idx);
    case TOK_PRINT:              // statement      → printStmt
      return parse_print(tokens, text, idx);
    case TOK_WHILE:              // statement      → whileStmt
      return parse_while(tokens, text, idx);
    case TOK_RETURN:             // statement      → returnStmt
      return parse_return(tokens, text, idx);
    
    case TOK_CONTINUE:           // statement      → [CONTROL_FLOW]
      Token tok2 = tokens[*idx];
      ++*idx;
      if(tokens[*idx].type != TOK_SEMICOLON)
        return new_node(.type=ERROR, .error.type=NO_SEMICOLON, .error.token=tokens[*idx-1]);
      ++*idx;
      tok2.length++;
      return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_CONTINUE, .control_flow.carry_back=new_node(.token = tok2) );
    
    case TOK_BREAK:              // statement      → [CONTROL_FLOW]
     Token tok3 = tokens[*idx];
      ++*idx;
      if(tokens[*idx].type != TOK_SEMICOLON)
        return new_node(.type=ERROR, .error.type=NO_SEMICOLON, .error.token=tokens[*idx-1]);
      ++*idx;
      tok3.length++;
      return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_BREAK, .control_flow.carry_back=new_node(.token = tok3));

    case TOK_RIGHT_CURLY:        // ERROR unterminated block
      ++*idx;
      return new_node(.type=ERROR, .error.type=UNMATCHED_RIGHT_CURLY_BRACE, .error.token=tokens[*idx-1]);

    case TOK_EOF:                // ERROR reached end of file
      return new_node(.type=ERROR, .error.type=TOKEN_SYNC_ERROR, .error.token=tokens[*idx]);

    default:                     // exprStmt  → expression ";" ;
      tok = tokens[*idx];
      expr = parse_exp(tokens, text, idx); 
      if(expr->type == ERROR)
        return expr;
      if (tokens[*idx].type != TOK_SEMICOLON)
        return new_node(.type=ERROR, .error.type=NO_SEMICOLON, .error.token=tok);
      ++*idx;
      return expr;
  }
}


Node * parse_class_declaration(Token * tokens, char * text, size_t *idx){
  // prolly not going to happen, but who knows
  // UPDATE: ^ HAHAHA YOU FOOL
  //classDecl      → "class" IDENTIFIER ( "<" IDENTIFIER )?
  //               "{" function* "}" ;
  ++*idx; //safe to advance
  if(tokens[*idx].type != IDENTIFIER)
    return new_node(.type=ERROR, .error.type=EXPECTED_IDENTIFIER, .error.token=tokens[*idx]);

  char * class_name = malloc(sizeof(char) * (tokens[*idx].length + 1) );
  strncpy(class_name, text + tokens[*idx].start, tokens[*idx].length);
  class_name[tokens[*idx].length] = '\0';



  Token tok = tokens[*idx];
  ++*idx;  //consume identifier

  //"class" consumed. IDENTIFIER consumed. expecting block
    //if(tokens[*idx].type != TOK_LEFT_CURLY)
   // return new_node(.type=ERROR, .error.type=EXPECTED_BLOCK, .error.token=tokens[*idx]);
    

   // Node * class_body = parse_block(tokens, text, idx);  // specific class body. cannot rely on parse_block to know whether it should see methods or statements
//________________________________________________________________________________________________________________________________________________
 //tok = tokens[*idx];
  Token bef = tok;
  if(tokens[*idx].type != TOK_LEFT_CURLY)
    return new_node(.type=ERROR, .error.type=EXPECTED_BLOCK, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col);
  ++*idx;
  
  size_t count = 0;
  Node * constructor = NULL;
  Node **methods = malloc(sizeof(Node *));
  Node * decl;
  while(tokens[*idx].type != TOK_RIGHT_CURLY && tokens[*idx].type !=TOK_EOF){
    tok = tokens[*idx];
    decl = parse_function(tokens, text, idx);

    if(decl->type == ERROR){
      return decl; //new_node(.type=ERROR, .error.type=SYNT8AX_ERROR, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=decl);
    }
    if(strcmp(decl->function_definiton.name, class_name) == 0){
      constructor = decl;
      continue;
      //return new_node(.type=CLASS, .class.name=class_name, .class.methods=methods, .class.methods.count = count);
    }
    methods[count++] = decl;
    Node **temp_list = realloc(methods,  sizeof(Node *) * (count + 1));
    methods = temp_list;
  }
    

  if(tokens[*idx].type != TOK_RIGHT_CURLY)
    return new_node(.type=ERROR, .error.type=UNTERMINATED_BLOCK,
      .error.token.line=bef.line,
      .error.token.col=bef.col,  
      .error.token.length=1,
      .error.inner=new_node(.type=ERROR, .error.type=NO_RIGHT_BRACE,
                            .error.token.line=tokens[*idx].line,
                            .error.token.col=tokens[*idx].col,  
                            .error.token.length=1
                          )
    );
  ++*idx;  // consume class declaration end "}"

  //for(size_t i = 0; i < count; i ++){
  if(constructor == NULL)
    return new_node(.type=ERROR, .error.type=MISSING_CONSTRUCTOR, .error.token=tok);

  return new_node(.type=STATEMENT,
     .statement.type=DECLARATION,
     .statement.declaration.type=DT_CLASS,
     .statement.declaration.name=class_name,
     .statement.declaration.initializer=new_node(.type=CLASS,
                        .class_definiton.name=class_name,
                          .class_definiton.constructor=constructor, 
                          .class_definiton.methods=methods, 
                          .class_definiton.method_count = count
                        )
    );
  
  
}

Node * parse_function_declaration(Token * tokens, char * text, size_t *idx){  
// will always receive TOK_FUNCTION
++*idx;  //safe to advance
  Node * func = parse_function(tokens, text, idx);
  if(func->type == ERROR)  //should handle func decl errors better (chaining errors)
    return func;
  //assign_func();
  return new_node(.type=STATEMENT,
     .statement.type=DECLARATION,
     .statement.declaration.type=DT_FUNCTION,
     .statement.declaration.name=func->function_definiton.name,
     .statement.declaration.initializer=func);
}


Node * parse_var_declaration(Token * tokens, char * text, size_t *idx) {  // varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
  char * name;
  Node * initializer = new_node(.type=LITERAL, .literal.type=NILL);
  Token tok = tokens[*idx];

  if(tokens[*idx].type != TOK_VAR)
    return new_node(.type=ERROR, .error.type=TOKEN_SYNC_ERROR, .error.token=tokens[*idx]);
  ++*idx;
  Token beff = tokens[*idx];

  if(tokens[*idx].type == IDENTIFIER){

    if(tokens[*idx + 1].type == TOK_ASSIGN) {
      name = malloc( sizeof(char) * (tokens[*idx].length + 1) ); //copy to here the identifier name
      strncpy(name, text + tokens[*idx].start, tokens[*idx].length);
      name[tokens[*idx].length] = '\0';
      ++*idx; //consume IDENTIFIER
      ++*idx;  //consume "="
      tok = tokens[*idx];


        initializer = parse_assignment(tokens, text, idx);
        //"TOK: %s\n", token_to_str(tokens[*idx]));
        if(initializer->type == ERROR){
          //if(initializer->error.type == NO_SEMICOLON)
          return new_node(.type=ERROR, .error.type=SYNTAX_ERROR, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=initializer);
        }
        //if(tokens[*idx].type != TOK_SEMICOLON)
        //  return new_node(.type=ERROR, .error.type=NO_SEMICOLON, .error.token.col=tok.col, .error.token.line=tok.line, .error.token.length=tokens[*idx].col-tok.col, .error.inner=initializer);
        //++*idx;
        return new_node(.type=STATEMENT,
            .statement.type=DECLARATION,
            .statement.declaration.type=DT_VAR,
            .statement.declaration.name=name,
            .statement.declaration.initializer=initializer);
    //  }
    }else if(tokens[*idx + 1].type == TOK_SEMICOLON){
      name = malloc( sizeof(char) * (tokens[*idx].length + 1) ); //copy to here the identifier name
      strncpy(name, text + tokens[*idx].start, tokens[*idx].length);
      name[tokens[*idx].length] = '\0';
      ++*idx;
      //++*idx;   //NEEDED????
      //Node * target = parse_exp(tokens, text, idx);  known target it is identifier
      return new_node(.type=STATEMENT,
          .statement.type=DECLARATION,
          .statement.declaration.type=DT_VAR,
          .statement.declaration.name=name,
          .statement.declaration.initializer=initializer);
    }else{
      Node * target = parse_comma(tokens, text, idx);
      if(!(target->type == SEQUENCE || target->type == VARIABLE))
        return new_node(.type=ERROR, .error.type=EXPECTED_VAR_NAME, .error.token=tokens[*idx], .error.inner=target);
      if(target->type == ERROR)
        return target;

      if(tokens[*idx].type == TOK_SEMICOLON){
       // ++*idx;
       // "RETURNING SEQ %s\n", token_to_str(tokens[*idx]));
        return new_node(.type=STATEMENT,
          .statement.type=DECLARATION,
          .statement.declaration.type=DT_VAR,
          .statement.declaration.name="",
          .statement.declaration.initializer=target);
      }else //if(tokens[*idx].type == TOK_ASSIGN){
         return new_node(.type=ERROR, .error.type=NOT_IMPLEMENTED,
                  .error.token=tokens[*idx]
                  );
     // }else{ //prolly error

     // }
       
    }
   
  }else{  //no identifier after var
    return new_node(.type=ERROR, .error.type=EXPECTED_VAR_NAME, .error.token=tokens[*idx]);
  }
  
  return new_node(.type=STATEMENT, .statement.type=DECLARATION,
                  .statement.declaration.type=DT_VAR,
                  .statement.declaration.name=name,
                  .statement.declaration.initializer=initializer
                  );
}

Node * parse_declaration(Token * tokens, char * text, size_t *idx){
  Node * res;
  switch(tokens[*idx].type){ 
    case TOK_VAR: //declaration → varDecl
      res = parse_var_declaration(tokens, text, idx);
      if(tokens[*idx].type != TOK_SEMICOLON)
        return new_node(.type=ERROR, .error.type=NO_SEMICOLON);
      ++*idx;
      break;
    case TOK_FUNCTION:  //declaration → funDecl
      res = parse_function_declaration(tokens, text, idx);
      break;
    case TOK_CLASS:   //declaration → classDecl
      res =  parse_class_declaration(tokens, text, idx);
      break;
    default:  //declaration → statement ;
      res =  parse_statement(tokens, text, idx);
  }
  return res;
}

void append_statment(Node * program, Node * stmt){
  if(program->statement.block.count == 0){
    program->statement.block.statements = malloc(sizeof(Node *));
    program->statement.block.statements[program->statement.block.count++] = stmt;
  }else{
    Node ** temp_list = realloc(program->statement.block.statements, sizeof(Node *) * (program->statement.block.count + 1) );
    program->statement.block.statements = temp_list;
    program->statement.block.statements[program->statement.block.count++] = stmt;
  }
}

Node * parse_program(Token * tokens, char * text){  //program        → declaration* EOF ;
  Node * program = new_node(.type=STATEMENT, .statement.block.statements=NULL, .statement.block.count=0);
  for(size_t idx = 0; tokens[idx].type != TOK_EOF; ){
    Node* stmt = parse_declaration(tokens, text, &idx);
    if (stmt->type == ERROR) {
      append_statment(program, stmt);
      return program;
    }else
      append_statment(program, stmt);
  }
  return program;
}
#endif
