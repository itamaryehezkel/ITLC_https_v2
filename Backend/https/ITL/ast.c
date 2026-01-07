#ifndef AST_C
#define AST_C


int is_true(Node * condition){
  return (int)(condition->literal.type != NILL && (condition->literal.type == INT &&  ((long int)condition->literal.num)) || (condition->literal.type == STRING && (strlen(condition->literal.str) > 0)));
}


char * substring(char * src, size_t start, size_t end){
  size_t len = end - start;
  char * result = malloc((len + 2)*sizeof(char));
  strncpy(result, (src + start), len);
  return result;
}

Node * get_literal(char * name, Token tok, Scope * scope);
Node * get_func(char * name, Token tok, Scope * scope);


Node * print_literal(Node * result, char * end){
  switch(result->literal.type){
    case STRING:
      colors(-1, color((Token){STRING_LITERAL}), "%s%s", result->literal.str, end);
      break;
    case INT:
      colors(-1, color((Token){NUMBER_LITERAL}), "%ld%s", (long int)result->literal.num, end);
      break;
    case NILL:
      colors(-1, color((Token){TOK_NULL}), "%s%s", "NULL", end);
      break;
    default: //delete this! error comes from traverse(literal)
      return new_node(.type=ERROR, .error.type=PARSER_ERROR, .error.token=result->token);
  }
  return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE);
}

Node * traverse(Node * entry, size_t recursions, Scope * scope){
  TRACE_AST print_node(entry, recursions, "\n");
  Node * right;
  Node * left;
  Node * result;
  switch(entry->type){

    case CONTROL_FLOW:
      if(entry->control_flow.carry_back != NULL)
        return traverse(entry->control_flow.carry_back, recursions+1, scope);
      return entry;

    case ERROR:
      return entry;
    
    case GROUP:
      return traverse(entry->group.exp, recursions+1, scope);

    case SEQUENCE:
      Node * res;
      for(size_t i = 0; i < entry->sequence.count; i++){
        res = traverse(entry->sequence.exp[i], recursions+1, scope);
      }
      return res;

    case VARIABLE:
      return get_literal(entry->variable, entry->token, scope);

    case LITERAL:
      switch(entry->literal.type){
        case NILL:
        case STRING:
        case INT:     
          return entry;
      }
      return new_node(.type=ERROR, .error.type=PARSER_ERROR, .error.token=entry->token);

    case OBJECT:
      //entry->object
      /*
      typedef struct{
        Node * type;
        VarList fields;
      } Object;*/
      return entry;//new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE); //traverse(entry->object.caller, recursions+2, scope);
    
    case FUNCTION:
      traverse(entry->function_definiton.body, recursions + 1, scope);
      /*
      .type=FUNCTION,
      .function_definiton.name=name,
      .function_definiton.body=body,
      .function_definiton.arity=params->function_definiton.arity,
      .function_definiton.params=params->function_definiton.params,

      struct {
        char * name;
        Node** params;
        size_t arity;
        Node * body;
      } function_definiton;  //definition  */
      return new_node(.type=LITERAL, .literal.type=NILL);

    case CLASS:
      /*
      struct {
        char * name;  
        Node * constructor;  //construct method
        Node ** methods;  
        size_t method_count;
      } class_definiton;  //definition and constructor

      typedef struct{
        Node * type;
        VarList fields;
      } Object;  //*/
      return new_node(.type=LITERAL, .literal.type=NILL);

    case CALL:
      switch(entry->call.type){
        case METHOD:
          Scope * curr = scope;
        //  (stdout, "_CALL METHOD__\n");
          //prihntf("CALL NODE\n");
          //Node * obj;
          switch(entry->call.callee->type){
            case CALL:
                result = traverse(entry->call.callee, recursions, scope);
              /*  if(ress->type != OBJECT)
                  return new_node(.type=ERROR, .error.type=PARSER_ERROR, .error.token=ress->token);

                 obj = traverse(ress, recursions+1, scope);
                  //Node * res = get_node(obj->object.fields,  entry->call.method.caller->variable);
                  // prihntf("not this %s  %ld\n", entry->call.method.caller->variable,
                  // (long int)res->literal.num//->object.type
                  if(get_node_index(obj->object.fields, entry->call.method.caller->variable) >= 0){
                      result = get_node(obj->object.fields,  entry->call.method.caller->variable); //set_node(obj->object.fields, entry->call.method.caller->variable, entry->call.method.caller);
                  }else{ 
                    return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=entry->call.method.caller->token);  //change it. redeclaration error
                  }
                  */
              if(result->type != OBJECT || entry->call.method.caller->type != VARIABLE)
                return new_node(.type=ERROR, .error.type=PARSER_ERROR, .error.token=result->token);
/*
              while(scope->global.classes != NULL && scope->current.functions != NULL){
                scope = scope->current.parent;
              }*/

              if(get_node_index(result->object.fields, entry->call.method.caller->variable) > -1){
                result = get_node(result->object.fields, entry->call.method.caller->variable);
              }else{ 
                result = new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=entry->call.method.caller->token);  //change it. redeclaration error
              }
             // scope = curr;
              return result;

              //return traverse(entry->call.callee, recursions, scope);
              break;

            case VARIABLE:
             // (stdout, "__Callee is VARIABLE %s.%s\n", entry->call.callee->variable, entry->call.method.caller->variable);
              result = traverse(entry->call.callee, recursions, scope);

              if(result->type != OBJECT || entry->call.method.caller->type != VARIABLE)
                return new_node(.type=ERROR, .error.type=PARSER_ERROR, .error.token=result->token);
/*
              while(scope->global.classes != NULL && scope->current.functions != NULL){
                scope = scope->current.parent;
              }*/

              if(get_node_index(result->object.fields, entry->call.method.caller->variable) > -1){
                result = get_node(result->object.fields, entry->call.method.caller->variable);
              }else{ 
                result = new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=entry->call.method.caller->token);  //change it. redeclaration error
              }
             // scope = curr;
              return result;
             // if(entry->call.callee->object.type == NULL){  // "this" object
                //prihntf("CALL NODE this\n");
                /*if(entry->call.method.caller->type == VARIABLE){
                  while(scope->global.classes != NULL && scope->current.functions != NULL){
                    scope = scope->current.parent;
                  }
                  if(get_node_index(scope->current.vars, entry->call.method.caller->variable) == -1){
                      set_node(scope->current.vars, entry->call.method.caller->variable, entry->call.method.caller);
                  }else{ 
                    return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=entry->call.method.caller->token);  //change it. redeclaration error
                  }
                  scope = curr;
                }
                */
              break;
          }
          
          
          break;

        case CONSTRUCT:
          Node * class_def = get_node(scope->global.classes, entry->call.callee->variable);
          Scope * current = scope;

          Scope * obj_this_scope = malloc(sizeof(Scope));
          obj_this_scope->global.classes = NULL;
          obj_this_scope->current.parent = scope;
          obj_this_scope->current.functions = NULL;
          obj_this_scope->current.vars = create_new_node_map();

          Scope * obj_var_scope = malloc(sizeof(Scope));
          obj_var_scope->global.classes = current->global.classes;
          obj_var_scope->current.parent = obj_this_scope;
          obj_var_scope->current.functions = create_new_node_map();
          obj_var_scope->current.vars = create_new_node_map();

         // destroy_nodemap();
          scope = obj_var_scope;
          Node * object = traverse(class_def->class_definiton.constructor, recursions + 1, scope);
          destroy_nodemap(obj_var_scope->current.functions);
          destroy_nodemap(obj_var_scope->current.vars);
          free(obj_var_scope);
          scope = current;
          if(object->type == ERROR)
            return object;

          //prinhtf("%s\n", class_def->class_definiton.name);
          object = new_node(.type=OBJECT, .object.type=class_def, .object.fields=obj_this_scope->current.vars);
          return object;

        case CALL_FUNCTION:
        
          Node * func = get_func(entry->call.callee->variable, entry->call.callee->token, scope);
          // get_node(scope->current.functions, entry->call.callee->variable);
          if(func == NULL || func->type == ERROR){
            if(func == NULL){
              return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=entry->token);
            }else{
              
              return func;
            }
              
          }
          
          if(func->type != FUNCTION)
            return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=entry->token);

          if(func->function_definiton.arity != entry->call.function.argc){
            if(entry->call.function.argc < func->function_definiton.arity){
              return new_node(.type=ERROR, .error.type=MISSING_ARGS, .error.token=(Token){
                .type=entry->token.type,
                .line=entry->token.line,
                .col=entry->token.col,
                .length=entry->token.length,
                .start=entry->token.start
              });
                   
            }else{
              return new_node(.type=ERROR, .error.type=TOO_MANY_ARGS, .error.token=(Token){
                .type=entry->token.type, .line=entry->token.line,
                .col=entry->call.function.args[func->function_definiton.arity]->token.col,
                .length=entry->call.function.args[entry->call.function.argc-1]->token.col - entry->call.function.args[func->function_definiton.arity]->token.col + entry->call.function.args[entry->call.function.argc-1]->token.length,
                .start=entry->token.start}
              );
            } 
          }  


          Scope * func_scope = malloc(sizeof(Scope));
          func_scope->current.parent = scope;
          func_scope->global.classes = create_new_node_map();
          func_scope->current.functions = create_new_node_map();
          func_scope->current.vars = create_new_node_map();
          
         
          for(size_t i = 0; i < entry->call.function.argc; i++){
            Node * arg = entry->call.function.args[i];
            Node * resolve = arg; 
            while(resolve->type != ERROR && resolve->type != LITERAL && resolve->type != OBJECT){
              resolve = traverse(arg, recursions+1, scope);
            } 
            if(resolve->type == ERROR )
              return resolve; 
             set_node(func_scope->current.vars, func->function_definiton.params[i]->variable, resolve);
          }
          
          scope = func_scope;
          Node * control = traverse(func->function_definiton.body, recursions, scope);
          scope = scope->current.parent;
          if(control->type == ERROR)
              return control;
          if(control->type == CONTROL_FLOW)
            return traverse(control, recursions, scope);

          //if(control->type == CONTROL_FLOW && control->control_flow.carry_back && control->control_flow.carry_back->type == CONTROL_FLOW)
          //  return control->control_flow.carry_back;
          return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=control->token);
 
      }//end of call switch
      return new_node(.type=LITERAL, .literal.type=NILL);
      
    
      
    case INDEX:
      Node * i_target = traverse(entry->index.target, recursions+1, scope);
      Node * index = traverse(entry->index.index, recursions+1, scope);
      if(i_target->type != LITERAL || i_target->literal.type != STRING){
        return new_node(.type=ERROR, .error.type=INVALID_ENUMERABLE);
      }else if(index->type != LITERAL || index->literal.type != INT)
        return new_node(.type=ERROR, .error.type=EXPECTED_EXP);

      size_t str_len = strlen(i_target->literal.str);
      size_t num_index = index->literal.num;
      if(num_index >= 0 && num_index < str_len){
        char * cell = malloc(sizeof(char)*2);
        strncpy(cell, (i_target->literal.str) + num_index, 1);
        cell[1] = '\0';
        return new_node(.type=LITERAL, .literal.type=STRING, .literal.str=cell);
      }else{
        return new_node(.type=ERROR, .error.type=INDEX_OUT_OF_BOUNDS);
      }
      return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE);

    case ASSIGNMENT:  //fix comma assignment
      Node * target = entry->assignment.target;
      Node * value = entry->assignment.value;
      Scope * cur = scope;
      switch(target->type){
        case CALL:
          switch(target->call.type){
            case METHOD:
              Node * callee = target->call.callee;
              Node * caller = target->call.method.caller;
              /*
              if(entry->call.callee->object.type == NULL){  // "this" object
                if(entry->call.method.caller->type == VARIABLE){
                  while(scope->global.classes != NULL && scope->current.functions != NULL){
                    scope = scope->current.parent;
                  }
                  if(get_node_index(scope->current.vars, entry->call.method.caller->variable) == -1){
                      set_node(scope->current.vars, entry->call.method.caller->variable, entry->call.method.caller);
                    }else{ 
                      return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=entry->token);  //change it. redeclaration error
                    }
                  scope = curr;
                }
              }
              */
              if(callee->type == VARIABLE)
                callee = get_node(scope->current.vars, callee->variable);

              if(callee->type != OBJECT || caller->type != VARIABLE)
                return new_node(.type=ERROR, .error.type=NOT_IMPLEMENTED, .error.token=entry->token);


             
              if(value->type == LITERAL)
                result = value;
              else
                result = traverse(value, recursions+1, scope);

              

              if(result->type == ERROR){
                return result;
              }
             // prinftf("callee: %s, caller: %s, value: %ld\n", callee->variable, caller->variable, (long int)value->literal.num);
              /*if(entry->assignment.op == OP_ADD_AFTER){
                set_node(callee->object.fields, caller->variable, result);
                result->literal.num--;
              }else if(entry->assignment.op == OP_SUBTRACT_AFTER){
                set_node(callee->object.fields, caller->variable, result);
                result->literal.num++;
              }else {
                set_node(callee->object.fields, caller->variable, result);
                //return result; 
              }*/
             // prinftf("CALLEE %s\n", callee->variable);
              if(callee->type == OBJECT && callee->object.type == NULL){
               // prinftf("Assigning to %s %ld\n", caller->variable, (long int)result->literal.num);

                while(scope->global.classes != NULL && scope->current.functions != NULL){
                  scope = scope->current.parent;
                }
                
                if(get_node_index(scope->current.vars, caller->variable) == -1){
                  
                  set_node(scope->current.vars, caller->variable, result);
                 
                  switch(entry->assignment.op){
                    case OP_ADD_AFTER:      result->literal.num--; break;
                    case OP_SUBTRACT_AFTER: result->literal.num++; break;
                  }
                }else{ 
                  scope = cur;
                  return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=entry->token);  //change it. redeclaration error
                }
                scope = cur;
              }else if(callee->type == OBJECT){
                 

                //  switch(entry->assignment.op){
                //     case OP_ADD_AFTER:      result->literal.num--; break;
                //     case OP_SUBTRACT_AFTER: result->literal.num++; break;
                //   }
                set_node(callee->object.fields, caller->variable, result);
                //(stdout, "aft: %ld\n", (long int) result->literal.num);
                // if(callee->type != VARIABLE)
                //  switch(entry->assignment.op){
                //     case OP_ADD_AFTER:      result->literal.num--; break;
                //     case OP_SUBTRACT_AFTER: result->literal.num++; break;
                //   }
                
              }
              return result;
              //scope = cur;
              break;  // END OF METHOD
          }  //case CALL
          break; // switch CALL TYPE

        default: //not a call. direct reference ???
          // while(scope->global.classes != NULL && scope->current.functions != NULL){
          //   scope = scope->current.parent;
          // }
          if(value->type == SEQUENCE){
            result = traverse(value->sequence.exp[0], recursions+1, scope);
            set_node(scope->current.vars, target->variable, result);
            traverse(value, recursions+1, scope);
           // scope = cur;
            return result;
          }
          result = traverse(value, recursions+1, scope);
          if(result->type == ERROR){
           // scope = cur;
            return result;
          }

          if(entry->assignment.op == OP_ADD_AFTER){
            set_node(scope->current.vars, target->variable, result);
            result->literal.num--;
          }else if(entry->assignment.op == OP_SUBTRACT_AFTER){
            set_node(scope->current.vars, target->variable, result);
            result->literal.num++;
          }else {
            set_node(scope->current.vars, target->variable, result);
            //return result; 
          }
          //scope = cur;
      }
      return result; 


    case UNARY:  //fix unary dec/inc prefix, not
      right = traverse(entry->unary.right, recursions+1, scope);
      switch(entry->unary.op){
        case OP_UNARY_MINUS:
          if(right->literal.type == INT){
             return new_node(.type=LITERAL, .literal.num = (-1)* (right->literal.num), .literal.type=INT);
          }else
            return new_node(.type=ERROR, .error.type=TYPE_MISMATCH, .error.token=right->token);
          break;
        case OP_UNARY_NOT:
          if(right->literal.type == INT){
             return new_node(.type=LITERAL, .literal.num=!(right->literal.num), .literal.type=INT);
          }else
            return new_node(.type=ERROR, .error.type=TYPE_MISMATCH);
        case NOOP:
          return right;
      }
      return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE);

    case BINARY: //fix unary dec/inc prefix, nulls and ints
      left = traverse(entry->binary.left, recursions+1, scope);
      if(left->type == ERROR)
        return new_node(.type=ERROR, .error.type=left->error.type, .error.token=left->error.token);
      right = traverse(entry->binary.right, recursions+1, scope);
      if(right->type == ERROR)
        return new_node(.type=ERROR, .error.type=left->error.type, .error.token=left->error.token);
      if(right->type == VARIABLE){
        Node * c_right = get_literal(right->variable, right->token, scope); //funcall type
        if(c_right->type == ERROR)
          return c_right;
      }
      while(left->type == CONTROL_FLOW)  // ?
        left = traverse(left, recursions, scope);
      while(right->type == CONTROL_FLOW)  // ?
        right = traverse(right, recursions, scope);

      int solution;
      switch(left->literal.type){
        case NILL:
        case INT:
        ////"INT\n");
          long double left_num = left->literal.type == INT ? left->literal.num : 0;
          long double right_num = right->literal.num;
          Node * buff;
          Node * _var;
          switch(entry->binary.op){
            case OP_ADD_AFTER:
              return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num) + ((long int)right_num)), .literal.type=INT);
 /*              _var = traverse(left, recursions+1);
              
              if(_var->type != LITERAL)
                return new_node(.type=ERROR, .error.type=INVALID_ASSIGNMENT_TARGET);

              
              if(_var->literal.type != INT && _var->literal.type != NILL)
                return new_node(.type=ERROR, .error.type=INVALID_ENUMERABLE);

              buff = traverse(left, recursions+1);
              if(buff->type == ERROR || buff->type != LITERAL)
                return buff;

              return new_node(.type=LITERAL, .literal.num = (buff->literal.type == INT ? buff->literal.num+1 : 1), .literal.type=INT);
*/
            case OP_SUBTRACT_AFTER:
              _var = traverse(left, recursions+1, scope);
              if(_var->type != LITERAL)
               return new_node(.type=ERROR, .error.type=INVALID_ASSIGNMENT_TARGET);
              if(_var->literal.type != INT && _var->literal.type != NILL)
               return new_node(.type=ERROR, .error.type=INVALID_ENUMERABLE);
              
              buff = traverse(left, recursions+1, scope);
              if(buff->type == ERROR || buff->type != LITERAL)
               return buff;
    //  return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num) - ((long int)right_num)), .literal.type=INT);
         //     solution = left_num + right_num;
       //       return new_node(.type=LITERAL, .literal.num = solution, .literal.type=INT);
              return new_node(.type=LITERAL, .literal.num = (buff->literal.type == INT ? buff->literal.num-1 : -1), .literal.type=INT);

            case OP_ADD:
              return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num) + ((long int)right_num)), .literal.type=INT);
              //solution = left_num + right_num;
              //return new_node(.type=LITERAL, .literal.num = solution, .literal.type=INT);
            case OP_SUBTRACT:
              return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num) - ((long int)right_num)), .literal.type=INT);
            case OP_MULTIPLY:
              return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num) * ((long int)right_num)), .literal.type=INT);
            case OP_DIVISION:
            ////"DIV\n");
              if(right_num == 0)
                return new_node(.type=ERROR, .error.type=ZERO_DIVISION, .error.token=entry->binary.token_right); 
              return new_node(.type=LITERAL, .literal.num = (left_num / right_num), .literal.type=INT);
            case OP_EQUAL:
            ////"EQUALS? %ld\n",(long double) (((long int)left_num) == ((long int)right_num)));
              return new_node(.type=LITERAL, .literal.num =(long double) (((long int)left_num) == ((long int)right_num)), .literal.type=INT);
            case OP_NOT_EQUAL:
              return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num)!= ((long int)right_num)), .literal.type=INT);
            case OP_LESS:
              return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num) < ((long int)right_num)), .literal.type=INT);
            case OP_LESS_EQUAL:
              return new_node(.type=LITERAL, .literal.num =(long double) (((long int)left_num) <= ((long int)right_num)), .literal.type=INT);
            case OP_GREATER:
              return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num) > ((long int)right_num)), .literal.type=INT);
            case OP_GREATER_EQUAL:
              return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num) >= ((long int)right_num)), .literal.type=INT);
            case OP_LOGIC_OR:
              return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num) || ((long int)right_num)), .literal.type=INT);
            case OP_LOGIC_AND:
              return new_node(.type=LITERAL, .literal.num =(long double) (((long int)left_num) && ((long int)right_num)), .literal.type=INT);
            case OP_BITWISE_OR:
              return new_node(.type=LITERAL, .literal.num =(long double) (((long int)left_num) | ((long int)right_num)), .literal.type=INT);
            case OP_BITWISE_XOR:
              return new_node(.type=LITERAL, .literal.num = (long double)(((long int)left_num) ^ ((long int)right_num)), .literal.type=INT);
            case OP_BITWISE_AND:
              return new_node(.type=LITERAL, .literal.num =(long double) (((long int)left_num) & ((long int)right_num)), .literal.type=INT);
            case OP_MODULO:
              return new_node(.type=LITERAL, .literal.num =(long double) (((long int)left_num) % ((long int)right_num)), .literal.type=INT);
          }
        case STRING:
          switch(entry->binary.op){
            case OP_ADD:
              size_t cat_size = strlen(left->literal.str) + strlen(right->literal.str) + 1;
              char * res = malloc(cat_size * sizeof(char));
              strcpy(res, left->literal.str);
              strcat(res, right->literal.str);
              return new_node(.type=LITERAL, .literal.str = res, .literal.type=STRING);
            case OP_EQUAL:
              return new_node(.type=LITERAL, .literal.num = strcmp(left->literal.str, right->literal.str) == 0, .literal.type=INT);
            case OP_NOT_EQUAL:
              return new_node(.type=LITERAL, .literal.num = (strcmp(left->literal.str, right->literal.str) != 0), .literal.type=INT);
          }
          break;
        default:  //unrecognized data type
          break;
      } //end of type switch
      return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE); //BINARY_OP DONE (needs fix, this is an error)
      
    case TERNARY: 
      //"TERNARY\n");
      Node * con = traverse(entry->ternary.condition, recursions+1, scope);
      if (con->type == ERROR) return con;
      if (is_true(con) != 0) {
          Node *true_result = traverse(entry->ternary.true_branch, recursions+1, scope);
          return true_result;
      } else {
          Node *false_result = traverse(entry->ternary.false_branch, recursions+1, scope);
          return false_result;
      }
      return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE);



    
    case STATEMENT:
      Node * initializer;
      Node * condition;
      Node * body;
      switch(entry->statement.type){

        case RETURN_STMT:
          result = entry->statement.return_statement.value;
          if(result->type == ERROR)
            return result;
          result = traverse(result, recursions+1, scope);
          if(result->type == ERROR)
            return result;
          return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_RETURN, .control_flow.carry_back=result);

        case EXPRESSION:
          return traverse(entry->statement.exp, recursions+1, scope);

        case BLOCK:
          for(size_t i = 0; i < entry->statement.block.count; i++){
            result = traverse(entry->statement.block.statements[i], recursions+1, scope);
            if(result->type == ERROR || (result->type == CONTROL_FLOW && result->control_flow.type != CF_DONE)){
              return result;
            }
          }
          return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE);
          
        case IF_STMT:
          condition = traverse(entry->statement.if_statement.condition, recursions+2, scope);
          if(condition->type == ERROR)
            return condition;
          if(is_true(condition)){
            result = traverse(entry->statement.if_statement.then_branch, recursions+1, scope);
            if(result->type == ERROR || (result->type == CONTROL_FLOW && result->control_flow.type != CF_DONE))
              return result;
          }else if(entry->statement.if_statement.else_branch){
            TRACE_AST colors(recursions, color((Token){TOK_IF}), "%s", "ELSE\n");
            result = traverse(entry->statement.if_statement.else_branch, recursions+1, scope);
            if(result->type == ERROR || (result->type == CONTROL_FLOW && result->control_flow.type != CF_DONE))
              return result;
          }
          return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE);

        case WHILE_STMT:
          condition = traverse(entry->statement.while_statement.condition, recursions+1, scope);
          while(is_true(condition)){
            body = traverse(entry->statement.while_statement.body, recursions+1, scope);
            if(body->type == CONTROL_FLOW){  
              if(body->control_flow.type == CF_BREAK){
                break;
              }else if(body->control_flow.type == CF_RETURN){ // propagte upwards
                return body;
              }else if(body->control_flow.type == CF_DONE){
              }
            }else if(body->type == ERROR){
              return body;
            }
            condition = traverse(entry->statement.while_statement.condition, recursions+1, scope);
          }
          return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE);

        case PRINT:
          result = entry->statement.print.exp;
          
          switch(result->type){
            case SEQUENCE:
              for(size_t i = 0; i < result->sequence.count; i++){
                Node * curr_exp = result->sequence.exp[i];
                Node * test_err;
                switch(curr_exp->type){

                  case LITERAL:  //seq
                    test_err = print_literal(curr_exp, "");
                    if(test_err->type == ERROR)
                      return test_err;
                    break;

                  case VARIABLE: //seq
                    curr_exp = get_literal(curr_exp->variable, curr_exp->token, scope);
                    switch(curr_exp->type){  //seq
                      case ERROR:     //seq var
                        return curr_exp;

                      case LITERAL:    //seq var
                        test_err = print_literal(curr_exp, "");
                        if(test_err->type == ERROR)
                          return test_err;
                        break;

                      case OBJECT:  //seq var 
                        colors(-1, BRIGHT_GREEN, "[Instance of %s]", curr_exp->object.type->class_definiton.name);
                        break;
                    }
                    break;

                  case CALL:
                    switch(curr_exp->call.type){
                      case METHOD:  //seq call
                        if(curr_exp->call.callee->type == VARIABLE && curr_exp->call.method.caller->type == VARIABLE){
                          Node * obj = get_literal(curr_exp->call.callee->variable, curr_exp->token, scope);
                          //Node * obj = traverse(entry->call.callee, recursions+1);  //get_literal()
                          Node * res = get_node(obj->object.fields,  curr_exp->call.method.caller->variable);
                          if(res->type == ERROR)
                            return res;
                          res = print_literal(res, "");
                          
                          if(res->type == ERROR)
                            return res;
                          //return res;
                        }
                        break;
                      case CONSTRUCT:  //seq call
                        //(stdout, "construct\n");
                        break;
                      case CALL_FUNCTION:  //seq call
                        //(stdout, "func call\n");
                        break;
                    }
                    break;

                 // default:
                }
                if(i < result->sequence.count-1)
                  fprintf(stdout, ", ");
              }
              fprintf(stdout, "\n");
              break;

            case LITERAL:
              return print_literal(result, "\n");

            case VARIABLE:
              result = get_literal(result->variable, result->token, scope);
              
              switch(result->type){
                case ERROR:
                  return result;
                case LITERAL:
                  return print_literal(result, "\n");
                case OBJECT:
                  colors(-1, BRIGHT_GREEN, "[Instance of %s]\n", result->object.type->class_definiton.name);
                  break;
                // default:
              }
              break;

            case CALL:
              
              switch(result->call.type){
                case METHOD:
                  
                  if(result->call.callee->type == VARIABLE && result->call.method.caller->type == VARIABLE){
                    
                    Node * obj = get_literal(result->call.callee->variable, result->token, scope);
                    
                    //priyntf("print METHOD: %s\n", obj->object.type->class_definiton.name);
                    //Node * obj = traverse(entry->call.callee, recursions+1);  //get_literal()
                    if(obj->object.type == NULL){
                      fprintf(stdout, "UNHANDLED? THIS IN PRINT! %s\n", node_type(*res));
                      Scope * curr = scope;
                      while(scope->global.classes != NULL && scope->current.functions != NULL)
                        scope = scope->current.parent;

                     // Node * ver = get_node(scope->current.vars, caller->variable);
                       Node * res = get_node(scope->current.vars,  result->call.method.caller->variable);
                       scope = curr;
                      if(res->type == ERROR)
                        return res;
                      res = print_literal(res, "\n");
                      
                      if(res->type == ERROR)
                        return res;
                    }else{
                     
                      Node * res = get_node(obj->object.fields,  result->call.method.caller->variable);
                      if(res == NULL)
                        return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=result->call.method.caller->token);
                      //(stdout, "CALL METHOD %s %s %s\n", (obj->object.type->class_definiton.name) , result->call.method.caller->variable, res == NULL ? "null": "");
                     //prigntf("non this in print: %s.%s=%s\n", obj->object.type->class_definiton.name,result->call.method.caller->variable, res == NULL ? "null" : "_");
                      if(res->type == ERROR)
                        return res;
                      
                      res = print_literal(res, "\n");
                      
                      if(res->type == ERROR)
                        return res;
                      
                      //return res;
                    }
                  }
                  break;
                case CONSTRUCT:
                  fprintf(stdout, "construct\n");
                  break;
                case CALL_FUNCTION:
                  fprintf(stdout, "func call\n");
                  break;
              }
              break;

            default:
              fprintf(stdout, "print sequence: %s\n", node_type(*result));
          }
          return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE);

        case FOR_STMT:
          initializer = traverse(entry->statement.for_statement.initializer, recursions+1, scope);
          if(initializer->type == ERROR){
            return initializer;
          }
          
          condition = traverse(entry->statement.for_statement.condition, recursions+1, scope);
          if(condition->type == ERROR)
              return condition;

          while(is_true(condition)){
            body = traverse(entry->statement.for_statement.body, recursions+1, scope);
            if(body->type == CONTROL_FLOW){  
              if(body->control_flow.type == CF_BREAK){
                break;
              }else if(body->control_flow.type == CF_RETURN){ // propagte upwards
                return body;
              }else if(body->control_flow.type == CF_DONE){
              }else if(body->control_flow.type == CF_CONTINUE){
              }
            }else if(body->type == ERROR)
              return body;

            Node * increment = traverse(entry->statement.for_statement.increment, recursions+1, scope);
            if(increment->type == ERROR){
              return increment;
            }
            
            condition = traverse(entry->statement.for_statement.condition, recursions+1, scope);
            if(condition->type == ERROR)
              return condition;
          }
          return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE);


        case DECLARATION:
          
          if (entry->statement.declaration.initializer) {  //add NULL type to empty declarations
            switch(entry->statement.declaration.type){

              case DT_VAR:
                result = entry->statement.declaration.initializer;
                while(result->type != LITERAL && result->type != OBJECT && result->type != ERROR){
                  result = traverse(entry->statement.declaration.initializer, recursions+2, scope);
                }
                if(result->type == ERROR)
                  return result;
                if(get_node_index(scope->current.vars, entry->statement.declaration.name) == -1){
                  set_node(scope->current.vars, entry->statement.declaration.name, result);
                }else{
                  return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER);  //change it. redeclaration error
                }
                break;

              case DT_FUNCTION:
                result = entry->statement.declaration.initializer;
                while(result->type != FUNCTION && result->type != ERROR){
                  result = traverse(entry->statement.declaration.initializer, recursions+2, scope);
                }
                if(result->type == ERROR)
                  return result;

                if(get_node_index(scope->current.functions, entry->statement.declaration.name) == -1){
                  set_node(scope->current.functions, entry->statement.declaration.name, result);
                }else{
                  return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER);  //change it. redeclaration error
                }
                break;

              case DT_CLASS:
                result = entry->statement.declaration.initializer;
                if(get_node_index(scope->global.classes, entry->statement.declaration.name) == -1){
                  set_node(scope->global.classes, entry->statement.declaration.name, result);
                }else{
                  return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER);  //change it. redeclaration error
                }
                break;
            }
          }//end of delcaration type switch
          return new_node(.type=CONTROL_FLOW, .control_flow.type=CF_DONE); //end of declarations
        
        default:  // INVALID STATEMENT
          return new_node(.type=ERROR, .error.type=PARSER_ERROR, .error.token=entry->token);
      }
      break; // STATEMENTS END 

    default: // UNRECOGNIZED AST NODE !!! 
      return new_node(.type=ERROR, .error.type=NOT_IMPLEMENTED, .error.token=entry->token);
  }
}

Node * get_literal(char * name, Token tok, Scope * scope){
  Scope * entry = scope;
  while(entry != NULL){
    if(get_node_index(entry->current.vars, name) > -1)
      return get_node(entry->current.vars, name);
    entry = entry->current.parent;
  }
  return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=tok);
}

Node * get_func(char * name, Token tok, Scope * scope){
  Scope * entry = scope;
  while(entry != NULL){
    if(get_node_index(entry->current.functions, name) > -1)
      return get_node(entry->current.functions, name);
    entry = entry->current.parent;
  }
  return new_node(.type=ERROR, .error.type=UNDECLARED_IDENTIFIER, .error.token=tok);
}

#endif