// headers //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dfdsfdaf
// include 
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

// define
# define MAX_STRING_LENGTH 100

// typedef
typedef char *CharPtr ;
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
  TYPE_UNKNOWN,
  TYPE_COMMENT
} ; // enum TOKENTYPE 

// flags
bool flag_EOF ; // end of file flag, if true then print eof message and quit the system
bool flag_quit ; // quit flag, if true then quit the system

// functions
void Init() ; // initiate global variables and flags

// classes
class Scanner {
  public :
    Scanner() ;
    bool AdvanceToNextToken() ;
    TOKENTYPE GetType() ;
    CharPtr GetToken() ;
    void PrintToken() ;
    void PrintType() ;
    TOKENTYPE PeakNextTokenType() ;
    int GetLine() ;
    int GetColumn() ;
  private :
    int line ;
    int column ;
    CharPtr token ;
    TOKENTYPE type ;
    CharPtr nextToken ;
    TOKENTYPE nextTokenType ;
    char nextChar ; // store next char while there is a char doesn't be done yet, and it was gotten
    bool Isseparator( char ch ) ;
    bool ReadNextToken() ;
    bool CheckNextTokenType() ;
    char GetNewChar() ;
} ; // class Scanner

class Parser {
  public :
    Parser() ;
    bool ReadSExp() ; // read s-exp, if quit or end of file reture false
    bool PrintSExp() ; // print s-exp and it's structure, if no s-exp then return false
  private :
    Scanner scanner ;
    bool SExp() ; // <S-exp> ::= <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN | QUOTE <S-exp>
    bool Atom() ; // <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | LEFT-PAREN RIGHT-PAREN
} ; // class Parser

// class Scanner ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Scanner::Scanner() {
  line = 1 ;
  column = -1 ;
  token = NULL ;
  type = TYPE_UNKNOWN ;
  nextChar = '\0' ;
  nextToken = NULL ;
  nextTokenType = TYPE_UNKNOWN ;
} // Scanner::Scanner()

bool Scanner::AdvanceToNextToken() {
  if ( nextToken == NULL ) {
    ReadNextToken() ;
    CheckNextTokenType() ;
  } // if

  token = nextToken ;
  type = nextTokenType ;
  nextToken = NULL ;
  nextTokenType = TYPE_UNKNOWN ;
  printf( "\ntoken = " ) ;
  PrintToken() ;
  printf( " type = " ) ;
  PrintType() ;
  return true ;
} // Scanner::AdvanceToNextToken() 

bool Scanner::ReadNextToken() {
  nextToken = (char*) malloc( sizeof( char ) * MAX_STRING_LENGTH ) ;
  char newChar = '\0' ; // getting new char
  bool flag_got_token = false ; // token got or not
  CharPtr ap = NULL ; // access pointer, point to the place where next char need to access
  
  ap = nextToken ; // point to the first place of token
  if ( nextChar != '\0' ) {
    newChar = nextChar ; // get new char from store if there is a char doesn't done yet but it is gotten
    nextChar = '\0' ;
  } // if
  else newChar = GetNewChar() ; // get first char

  // getting token
  while ( true ) {
    // check EOF 
    if ( newChar == EOF ) {
      flag_EOF = true ;
      nextChar = newChar ;
      break ;
    } // if

    // check white space
    if ( isspace( newChar ) ) { 
      if ( flag_got_token == true ) break ;
      else {
        newChar = GetNewChar() ;
        continue ;
      } // else
    } // if

    // check separator ( ) ' " ;
    if ( Isseparator( newChar ) ) {
      if ( flag_got_token == true ) nextChar = newChar ;
      else {
        flag_got_token = true ;
        *ap++ = newChar ;
        if ( newChar == '(' && ( nextChar = GetNewChar() ) == ')' ) { // nil 
          *ap++ = nextChar ;
          nextChar = '\0' ;
        } // if
        else if ( newChar == '"' ) // string
          for ( ; *ap = GetNewChar() ; ap++ ) {
            if ( *ap == '\\' ) {
              char ch = GetNewChar() ;
              if ( ch == 'n' ) *ap = '\n' ;
              else if ( ch == '"' ) *ap = '"' ;
              else if ( ch == 't' ) *ap = '\t' ;
              else if ( ch == '\\' ) *ap = '\\' ;
              else *++ap = ch ;
            } // if
            else if ( *ap == '"' ) {
              ap++ ;
              break ;
            } // else if
          } // for
        else if ( newChar == ';' ) // comment
          while ( ( *ap++ = GetNewChar() ) != '\n' ) ;
      } // else 

      break ;
    } // if

    // others
    *ap++ = newChar ;
    newChar = GetNewChar() ;
    flag_got_token = true ;
  } // while

  // return success
  *ap = '\0' ;
  return flag_got_token ; 
} // Scanner::ReadNextToken() 

TOKENTYPE Scanner::GetType() {
  return type ;
} // Scanner::GetType()

CharPtr Scanner::GetToken() {
  return token ;
} // Scanner::GetToken()

int Scanner::GetLine() {
  return line ;
} // Scanner::GetLine()

int Scanner::GetColumn() {
  return column ;
} // Scanner::GetColumn()

bool Scanner::Isseparator( char ch ) {
  char separator_list[5] = { '(', ')', '\'', '"', ';' } ;
  for ( int i = 0 ; i < 5 ; i++ )
    if ( ch == separator_list[i] )
      return true ;

  return false ;
} // Scanner::Isseparator()

bool Scanner::CheckNextTokenType() {
  if ( strcmp( nextToken, "()" ) == 0 || strcmp( nextToken, "nil" ) == 0 || strcmp( nextToken, "#f" ) == 0 ) 
    nextTokenType = TYPE_NIL ;
  else if ( strcmp( nextToken, "t" ) == 0 || strcmp( nextToken, "#t" ) == 0 ) nextTokenType = TYPE_T ;
  else if ( *nextToken == '(' ) nextTokenType = TYPE_LEFT_PAREN ;
  else if ( *nextToken == ')' ) nextTokenType = TYPE_RIGHT_PAREN ;
  else if ( *nextToken == '\'' ) nextTokenType = TYPE_QUOTE ;
  else if ( *nextToken == '"' ) nextTokenType = TYPE_STRING ;
  else if ( *nextToken == ';' ) nextTokenType = TYPE_COMMENT ;
  else { // int float dot symbol
    bool has_digit = false ;
    bool has_other_char = false ;
    int dot_num = 0 ;
    CharPtr tp = nextToken ;

    if ( *tp == '+' || *tp == '-' )
      tp++ ;

    for ( ; *tp != '\0' ; tp++ ) {
      if ( *tp == '.' ) dot_num++ ;
      else if ( isdigit( *tp ) ) has_digit = true ;
      else has_other_char = true ; 
    } // for

    if ( has_other_char == false && has_digit == true && dot_num == 1 ) nextTokenType = TYPE_FLOAT ;
    else if ( has_other_char == false && has_digit == true && dot_num == 0 ) nextTokenType = TYPE_INT ;
    else if ( has_other_char == false && has_digit == false && dot_num == 1 ) nextTokenType = TYPE_DOT ;
    else nextTokenType = TYPE_SYMBOL ;
  } // else 

  return true ;
} // Scanner::CheckTokenType()

void Scanner::PrintToken() {
  printf( "%s", token ) ;
} // Scanner::PrintToken()

void Scanner::PrintType() {
  switch ( type ) {
    case TYPE_DOT : printf( "TYPE_DOT" ) ; break ;
    case TYPE_SYMBOL : printf( "TYPE_SYMBOL" ) ; break ;
    case TYPE_INT : printf( "TYPE_INT" ) ; break ;
    case TYPE_FLOAT : printf( "TYPE_FLOAT" ) ; break ;
    case TYPE_QUOTE : printf( "TYPE_QUOTE" ) ; break ;
    case TYPE_T : printf( "TYPE_T" ) ; break ;
    case TYPE_NIL : printf( "TYPE_NIL" ) ; break ;
    case TYPE_STRING : printf( "TYPE_STRING" ) ; break ;
    case TYPE_COMMENT : printf( "TYPE_COMMENT" ) ; break ;
    case TYPE_UNKNOWN : printf( "TYPE_UNKNOWN" ) ; break ;
    case TYPE_RIGHT_PAREN : printf( "TYPE_RIGHT_PAREN" ) ; break ; 
    case TYPE_LEFT_PAREN : printf( "TYPE_LEFT_PAREN" ) ; break ;
    default : printf( "DEFAULT" ) ; break ;
  } // switch ()
} // Scanner:: PrintType()

TOKENTYPE Scanner::PeakNextTokenType() {
  if ( nextToken == NULL ) {
    ReadNextToken() ;
    CheckNextTokenType() ;
  } // if
  return nextTokenType ;
} // Scanner::PeakNextTokenType()

char Scanner::GetNewChar() {
  char ch = getchar() ;
  if ( ch == '\n' ) {
    line++ ;
    column = -1 ;
  } // if
  else column++ ;
  return ch ;
} // Scanner::GetNewChar()

// class Parser /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Parser::Parser() {
} // Parser::Parser()

bool Parser::ReadSExp() {
  return SExp() ; // start recursive decent
} // Parser::ReadSExp()

bool Parser::SExp() {
  // <S-exp> ::= <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN | QUOTE <S-exp>
  if ( scanner.AdvanceToNextToken() == false ) return false ; // advance to next token
  
  if ( scanner.GetType() == TYPE_LEFT_PAREN ) {
    do { // do <S-exp> { <S-exp> }, stop with DOT and QUOTE
      if ( SExp() == false ) return false ; // try to get SExp
    } while ( !( scanner.PeakNextTokenType() == TYPE_DOT || scanner.PeakNextTokenType() == TYPE_RIGHT_PAREN ) ) ; // stop with DOT and QUOTE

    if ( scanner.AdvanceToNextToken() == false ) return false ; // advance to next token
    
    if ( scanner.GetType() == TYPE_DOT ) { // do [ DOT <S-exp> ], 
      if ( SExp() == false ) return false ; // try to get SExp
      if ( scanner.AdvanceToNextToken() == false ) return false ; // advance to next token
    } // if

    if ( scanner.GetType() == TYPE_RIGHT_PAREN ) return true ;
    else {
      printf( "\nERROR (unexpected token) : ')' expected when token at Line %d Column %d is >>%s<<", scanner.GetLine(), scanner.GetColumn(), scanner.GetToken() ) ;
      return false ; // check RIGHT-PAREN
    } // else
  } // if
  else if ( scanner.GetType() == TYPE_QUOTE ) return SExp() ; // try to get SExp
  else return Atom() ; // get atom

  printf( "\nunknown sexp" ) ;
  return false ; // end pattern
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
    default : return false ;
  } // switch
} // Parser::Atom()

bool Parser::PrintSExp() {
  printf( "\nParser::PrintSExp()\n" ) ;
} // Parser::PrintSExp()

// main /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Init() {
  flag_EOF = false ;
  flag_quit = false ;
} // Init()

int main() {
  Init() ;

  printf( "Welcome to OurScheme!" ) ;

  while ( true ) {
    printf( "\n> " ) ;
    Parser parser ;

    if ( parser.ReadSExp() == true ) {
      if ( flag_EOF == true ) {
        printf( "\nERROR (no more input) : END-OF-FILE encountered" ) ;
        flag_quit = true ;
      } // if
      else
        parser.PrintSExp() ;
    } // if
    else flag_quit = true ;

    if ( flag_quit == true )
      break ;
  } // while

  printf( "\nThanks for using OurScheme!" ) ;
} // main()
