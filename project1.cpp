// headers //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// include 
# include <iostream> // I/O
using namespace std ;

// define
# define MAX_STRING_LENGTH 30

// typedef
typedef char *CharPtr ; 

// flags
bool flag_EOF ; // end of file flag, if true then print eof message and quit the system
bool flag_quit ; // quit flag, if true then quit the system

// functions
void Init() ; // initiate global variables and flags

// classes
class Scanner {
  public :
  enum TOKENTYPE {
    TYPE_NIL,
    TYPE_DOT,
    TYPE_QUOTE,
    TYPE_SYMBOL,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_T,
    TYPE_RIGHT_PAREN,
    TYPE_LEFT_PAREN,
    TYPE_UNKNOWN
  } ; // enum TOKENTYPE
    Scanner() ;
    bool AdvanceToNextToken() ;
    TOKENTYPE GetType() ;
  private :
    CharPtr token ;
    TOKENTYPE type ;
    char nextCharStore ; // store next char while there is a char doesn't be done yet, and it was gotten
} // class Scanner

class Parser {
  public :
    Parser() ;
    bool ReadSExp() ; // read s-exp, if quit or end of file reture false
    bool PrintSExp() ; // print s-exp and it's structure, if no s-exp then return false
  private :
    Scanner scaner ;
    bool SExp() ; // <S-exp> ::= <ATOM> | LEFT-PAREN <ParenExp> | QUOTE <S-exp>
    bool Atom() ; // <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | LEFT-PAREN | RIGHT-PAREN
} // class Parser

// class Scanner ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Scanner::Scanner() {
  token = NULL ;
  type = TYPE_UNKNOWN ;
  nextChar = '\0' ;
} // Scanner::Scanner()

bool Scanner::AdvanceToNextToken() {
  token = malloc( sizeof( char ) * MAX_STRING_LENGTH ) ;
  char newChar = '\0' ; // getting new char
  bool flag_got_token = false ; // token got or not
  CharPtr ap = NULL ; // access pointer, point to the place where next char need to access
  
  ap = token ; // point to the first place of token
  if ( nextCharStore != '\0' ) newChar = nextCharStore ; // get new char from store if there is a char doesn't done yet but it is gotten
  else newChar = getchar() ; // get first char

  // getting token
  while ( true ) {
    // check EOF 
    if ( newChar == EOF ) {
      flag_EOF = true ;
      if ( flag_got_token == false ) { // if the scanner doesn't get the token
        nextCharStore = newChar ;
        break ;
      } // if
      else return false ;
    } // if

    // check 

  } // while
} // AdvanceToNextToken() 

TOKENTYPE Scanner::GetType() {
  return type ;
} // GetType()

// class Parser /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Parser::Parser() {

} // Parser::Parser()

bool Parser::ReadSExp() {
  if ( scanner.AdvanceToNextToken() == false ) return false ; // advance to next token
  return SExp() ; // start recursive decent
} // Parser::ReadSExp()

bool Parser::SExp() {
  // <S-exp> ::= <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN | QUOTE <S-exp>
  if ( scanner.GetType() == TYPE_LEFT_PAREN ) {
    if ( scanner.AdvanceToNextToken() == false ) return false ; // advance to next token

    do {  // do <S-exp> { <S-exp> }, stop with DOT and QUOTE
      if ( SExp() == false ) return false ; // try to get SExp
    } while ( !( scanner.GetType() == TYPE_DOT || scanner.GetType() == TYPE_RIGHT_PAREN ) ) ; // stop with DOT and QUOTE

    if ( scanner.GetType() == TYPE_DOT ) { // do [ DOT <S-exp> ], 
      if ( scanner.AdvanceToNextToken() == false ) return false ; // advance to next token
      if ( SExp() == false ) return false ; // try to get SExp
    } // if

    if ( scanner.GetType() != TYPE_RIGHT_PAREN ) return false ; // check RIGHT-PAREN
  } // if
  else if ( scanner.GetType() == TYPE_QUOTE ) {
    if ( scanner.AdvanceToNextToken() == false ) return false ; // advance to next token
    if ( SExp() == false ) return false ; // try to get SExp
  } // else if
  else return Atom() ;

  return true ; // end pattern
} // Parser::SExp()

bool Parser::Atom() {
  // <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | LEFT-PAREN RIGHT-PAREN
  switch ( scanner.GetType() ) {
    case TYPE_SYMBOL : return true ;
    case TYPE_INT : return true ;
    case TYPE_FLOAT : return true ;
    case TYPE_STRING : return true ;
    case TYPE_NIL : return true ;
    case TYPE_T : return true ;
    case TYPE_LEFT_PAREN : 
      if ( scanner.AdvanceToNextToken() == false ) return false ; // advance to next token
      if ( scanner.GetType() != TYPE_RIGHT_PAREN ) return false ; // check RIGHT-PAREN
      return true ;
    default : return false ;
  } // switch
} // Parser::Atom()

bool Parser::PrintSExp() {

} // Parser::PrintSExp()

// main /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Init() {
  flag_EOF = false ;
  flag_quit = false ;
} // Init()

int main() {
  SExpReader sExpReader ;
  Init() ;

  cout << "Welcome to OurScheme!" << endl ;

  while ( true ) {
  	cout << "> " ;
    
    if ( sExpReader.ReadSExp() == true ) {
      if ( flag_EOF == true ) {
      	cout << "ERROR (no more input) : END-OF-FILE encountered" << endl ; ;
        flag_quit = true ;
      } // if
      else
        sExpReader.PrintSExp( expr ) ;
    } // if
    else {
      cout << "ReadSExp() error" << endl ;
      flag_quit = true ;
    } // else 

    if ( flag_quit == true )
      break ;
  } // while

  cout << "Thanks for using OurScheme!" << endl ;
} // main()
