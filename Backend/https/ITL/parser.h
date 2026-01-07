#ifndef PARSER_H
#define PARSER_H



Node * parse_literal(Token * tokens, char * text, size_t *idx);
Node * parse_primary(Token * tokens, char * text, size_t *idx);
Node * parse_index(Node * callee, Token * tokens, char * text, size_t *idx);
Node * parse_call(Node * callee, Token * tokens, char * text, size_t *idx);
Node * parse_postfix(Token * tokens, char * text, size_t *idx);


Node * parse_unary(Token * tokens, char * text, size_t *idx);    // unary      -!  prefix_unary -> ("!" | "-") unary | primary
//Node * parse_power(Token * tokens, char * text, size_t *idx);  // power
Node * parse_factor(Token *tokens, char * text, size_t *idx);    // factor     %*/ prefix_unary (("*" | "/") prefix_unary)*
Node * parse_term(Token *tokens, char * text, size_t *idx);      // term       +-  factor (("+" | "-") factor)*
//--------------------------------------------------------------------------------------------------------------
Node * parse_comparison(Token *tokens, char * text, size_t *idx);// comparison ><  term ((">" | ">=" | "<" | "<=") term)*
Node * parse_equality(Token *tokens, char * text, size_t *idx);  // equality   ==  comparison (("!=" | "==") comparison)*
Node * parse_logic_and(Token *tokens, char * text, size_t *idx); // logic_and  AND equality ("&&" equality)*
Node * parse_logic_or(Token *tokens, char * text, size_t *idx);  // logic_or   OR  logic_and ("||" logic_and)*
//--------------------------------------------------------------------------------------------------------------
Node * parse_assignment(Token *tokens, char * text, size_t *idx);
Node * parse_ternary(Token * tokens, char * text, size_t *idx);
Node * parse_comma(Token *tokens, char *text, size_t *idx);
Node * parse_declaration(Token * tokens, char * text, size_t *idx);

Node * parse_group(Token * tokens, char * text, size_t *idx);

Node * parse_statement(Token * tokens, char * text, size_t *idx);
Node * parse(Token * tokens, char * text);

Node * parse_var_declaration(Token * tokens, char * text, size_t *idx);


Node * parse_exp(Token * tokens, char * text, size_t *idx);
Node * parse_parameters(Token * tokens, char * text, size_t *idx);
Node * parse_arguments(Node * callee, Token * tokens, char * text, size_t *idx);
Node * parse_block(Token * tokens, char * text, size_t *idx);
Node * parse_if(Token * tokens, char * text, size_t *idx);
Node * parse_for(Token * tokens, char * text, size_t *idx);
Node * parse_print(Token * tokens, char * text, size_t *idx);
Node * parse_while(Token * tokens, char * text, size_t *idx);
Node * parse_return(Token * tokens, char * text, size_t *idx);
Node * parse_class_declaration(Token * tokens, char * text, size_t *idx);
Node * parse_function_declaration(Token * tokens, char * text, size_t *idx);
void append_statment(Node * program, Node * stmt);
Node * parse_program(Token * tokens, char * text);


Node * parse_function(Token * tokens, char * text, size_t *idx); 
Node * parse_bitwise_and(Token *tokens, char * text, size_t *idx);  //logic_and      -> equality ("&&" equality)*
Node * parse_bitwise_xor(Token *tokens, char * text, size_t *idx);  //logic_and      -> equality ("&&" equality)*
Node * parse_bitwise_or(Token *tokens, char * text, size_t *idx);  //logic_and      -> equality ("&&" equality)*
/*

literal        -> NUMBER_LITERAL | STRING_LITERAL
primary        -> "(" expression ")" | identifier | literal
index          -> "[" expression "]"

________________________________________________________________________________
call           → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
primary        → "true" | "false" | "nil" | "this"
               | NUMBER | STRING | IDENTIFIER | "(" expression ")"
               | "super" "." IDENTIFIER ;
________________________________________________________________________________
call           -> "(" [expression ("," expression)*] ")"                       
call           → primary ( "(" arguments? ")" )* ;





arguments      → expression ( "," expression )* ;
postfix        -> primary (call | index)*



prefix_unary   -> ("!" | "-") prefix_unary | postfix                        unary          → ( "!" | "-" ) unary | call ;

factor         -> prefix_unary (("*" | "/") prefix_unary)*  
term           -> factor (("+" | "-") factor)* 



comparison     -> term ((">" | ">=" | "<" | "<=") term)*
equality       -> comparison (("!=" | "==") comparison)*
logic_and      -> equality ("&&" equality)*
logic_or       -> logic_and ("||" logic_and)*
assignment     -> identifier "=" assignment | logic_or     
| assignment    → ( call "." )? IDENTIFIER "=" assignment | logic_or ;

parse_comma → parse_assignment → parse_equality
ternary        -> assignment ("?" expression ":" assignment)?
expression     -> ternary

logic_or -> logic_and ("||" logic_and)*
logic_and -> equality ("&&" equality)*
equality -> comparison (("!=" | "==") comparison)*
comparison -> term ((">" | ">=" | "<" | "<=") term)*
term -> factor (("+" | "-") factor)*
factor -> prefix_unary (("*" | "/") prefix_unary)*
prefix_unary -> ("!" | "-") unary | primary



-------------------------------------------------------------------+
https://craftinginterpreters.com/appendix-i.html#syntax-grammar    |

{ for (i = 0; i < 10; i++) { if (i == 5) print "halfway"; else return; } }


expression     → assignment ;

assignment     → ( call "." )? IDENTIFIER "=" assignment
               | logic_or ;



logic_or       → logic_and ( "or" logic_and )* ;
logic_and      → equality ( "and" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;


term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;



unary          → ( "!" | "-" ) unary | call ;
call           → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
primary        → "true" | "false" | "nil" | "this"
               | NUMBER | STRING | IDENTIFIER | "(" expression ")"
               | "super" "." IDENTIFIER ;

*/

/*
NUMBER         → DIGIT+ ( "." DIGIT+ )? ;
STRING         → "\"" <any char except "\"">* "\"" ;
IDENTIFIER     → ALPHA ( ALPHA | DIGIT )* ;
ALPHA          → "a" ... "z" | "A" ... "Z" | "_" ;
DIGIT          → "0" ... "9" ;
*/
/*
  TODO:

  ** - SCOPE
RECURSION
function fibonacci(n){
   if(n >= 0)
      print n;
   if(n <= 0)
      return 0;
    else if(n == 1)
      return 1;
   else
      return fibonacci(n - 1) + fibonacci(n - 2);
}


print fibonacci(10);


 - unary ops, getters/setters??/method acess fix

 - field declarations|properties




___________________________________________________________________________________
class obj1{
   obj1(){
      this.cdfgh = 9;
   }
}
class obj2{
   obj2(){
      this.test = new obj1();
   }
}

 //var g = new obj2();
var e = (new obj2()).cdfgh;
print e;
_______________________________________________________________________________



  *** - METHOD PARSING (functions/ variables, STRING.PROTOTYPE, INT.PROTOTYPE)
  **** - CLASS

   - SWITCH


___ handle assignments (var, func, class) getters and setters internal mechanism
  
   -- Arrays, Lists

   - Type checking, type comparison, INT,STRING,NULL,  ARRAY,LIST,OBJECT,BOOLEAN,FLOAT
   - handle floats
_____- division handling, by 0, --> infinity, boundaries
 

   - hashmap
   - linked list
   - dictionaries

   - Pointers? unary pre/post(*), prefix(&)
   - type casting

  ISSUES:
   func params/args
   1 1 1 1;
   print 1++; //works
   print ++1; not
   print ++var++ works

   for ( var i = 0; i < 5; i + + ) if ( i == 3 ) return i * 2;  //infinite loop

   FIBONACHI:
   in = 10; a = 1; b = 0; i = 1; while ( i <= in ) { temp = a + b; b = a; a = temp; print b; i + + }
   for(i=1; i <= in; i++){temp = a + b; b = a; a = temp; print b;}

   //in = 40; function a(){ for(a=1, b=0, i=1; i <= in; i++){ temp = a + b; b = a; a = temp; print b;} }; a();


   /*var iy =    10 ;
for(  var  i = 0;   i <= iy; i++){
   temp  = a + b;
   b = a;
   a = temp;
  
}

 
  var  b =   0;
var a   =  1;
var temp = 0;
*/

/*
for ( var i = 0; i < 5; i++ )
    if ( i == 3 )
       return i * 2;
*/
/*
var in = 50;
var a = 1;
var b = 0;
var i = 1;
var temp = a + b;
while ( i <= in ) {
temp = a + b;
b = a;
a = temp;
print b;
i++;
}
   
*/



#endif
