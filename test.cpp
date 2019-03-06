// include 
# include <stdio.h> // I/O

// type define
typedef char Str30[30] ;
enum TokenType {
  TYPE_LEFT_PAREN,
  TYPE_RIGHT_PAREN,
  TYPE_INT,
  TYPE_STRING,
  TYPE_DOT,
  TYPE_FLOAT, 
  TYPE_NIL, 
  TYPE_T,
  TYPE_QUOTE,
  TYPE_SYMBOL
} ; // TokenType
struct Token {
  TokenType type ;
  int id ;
} ; // token

// global variables
char next_char ; // for scanner

// flags
bool flag_EOF ;
bool flag_exit ;

// functions
void init() ;
bool ReadSExp(  ) ;
bool PrintSExp( EXPR expr ) ;

Token scanner() ;

// initiate global variables and flags
void init() {
  flag_EOF = 0 ;
  flag_exit = 0 ;
} // init()

int main() {
  printf( "Welcome to OurScheme!\n" ) ;

  while ( true ) {
  	printf( "> " ) ;
    
    if ( ReadSExp( expr ) ) {
      if ( flag_EOF == true ) {
      	printf( "ERROR (no more input) : END-OF-FILE encountered\n" ) ;
        flag_exit = true ;
      } // if
      else
        PrintSExp( expr ) ;
    } // if
    else {
      printf( "ReadSExp() error\n" ) ;
      flag_exit = true ;
    } // else 

    if ( flag_exit == true )
      break ;
  } // while

  printf( "Thanks for using OurScheme!\n" ) ;
} // main()
