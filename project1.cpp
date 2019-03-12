// headers //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
class Token {
  public :
    Token() ;
    void PrintToken() ;
    void PrintStr() ;
    void PrintType() ;
    void PrintLine() ;
    void PrintColumn() ;
    CharPtr GetStr() ;
    TOKENTYPE GetType() ;
    int GetLine() ;
    int GetColumn() ;
    void SetStr( CharPtr input_str ) ;
    void SetType( TOKENTYPE input_type ) ;
    void SetLine( int input_line ) ;
    void SetColumn( int input_column ) ;
  private :
    CharPtr str ;
    TOKENTYPE type ;
    int line ;
    int column ;
} ; // class Token
typedef Token *TokenPtr ;

class Scanner {
  public :
    Scanner() ;
    bool AdvanceToNextToken() ;
    TOKENTYPE PeakNextTokenType() ;
    TOKENTYPE GetType() ;
    int GetLine() ;
    int GetColumn() ;
    CharPtr GetStr() ;
  private :
    int current_line ;
    int current_column ;
    int latest_token_line ;
    int latest_token_column ;
    TokenPtr current_token ;
    TokenPtr next_token ;
    char nextChar ; // store next char while there is a char doesn't be done yet, and it was gotten
    CharPtr ReadNewToken() ;
    TOKENTYPE CheckType( CharPtr str ) ;
    bool Isseparator( char ch ) ;
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

// class Token //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Token::Token() {
  str = NULL ;
  type = TYPE_UNKNOWN ;
  line = 1 ;
  column = 0 ;
} // Token::Token()

void Token::PrintStr() {
  printf( "%s", str ) ;
} // Token::PrintStr()

void Token::PrintType() {
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
} // Token:: PrintType()

void Token::PrintLine() {
  printf( "%d", line ) ;
} // Token::PrintLine()

void Token::PrintColumn() {
  printf( "%d", column ) ;
} // Token::PrintColumn()

void Token::PrintToken() {
  printf( "\n(%d,%d) >>> %s >>> ", line, column, str ) ;
  PrintType() ;
} // Token::PrintToken()

TOKENTYPE Token::GetType() {
  return type ;
} // Token::GetType()

CharPtr Token::GetStr() {
  return str ;
} // Token::GetStr()

int Token::GetLine() {
  return line ;
} // Token::GetLine()

int Token::GetColumn() {
  return column ;
} // Token::GetColumn()

void Token::SetStr( CharPtr input_str ) {
  str = input_str ;
} // Token::SetStr

void Token::SetType( TOKENTYPE input_type ) {
  type = input_type ;
} // Token::SetType

void Token::SetLine( int input_line ) {
  line = input_line ;
} // Token::SetLine

void Token::SetColumn( int input_column ) {
  column = input_column ;
} // Token::SetColumn

// class Scanner ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Scanner::Scanner() {
  current_line = 1 ;
  current_column = 0 ;
  latest_token_line = -1 ;
  latest_token_column = -1 ;
  current_token = NULL ;
  next_token = NULL ;
  nextChar = '\0' ;
} // Scanner::Scanner()

bool Scanner::AdvanceToNextToken() {
  if ( next_token != NULL ) {
    current_token = next_token ;
    next_token = NULL ;
  } // if
  else {
    current_token = new Token ;
    current_token->SetStr( ReadNewToken() ) ;
    current_token->SetType( CheckType( current_token->GetStr() ) ) ;
    current_token->SetLine( latest_token_line ) ;
    current_token->SetColumn( latest_token_column ) ;
  } // else

  current_token->PrintToken() ;
  return true ;
} // Scanner::AdvanceToNextToken() 

TOKENTYPE Scanner::PeakNextTokenType() {
  if ( next_token == NULL ) {
    next_token = new Token ;
    next_token->SetStr( ReadNewToken() ) ;
    next_token->SetType( CheckType( next_token->GetStr() ) ) ;
    next_token->SetLine( latest_token_line ) ;
    next_token->SetColumn( latest_token_column ) ;
  } // if
  return next_token->GetType() ;
} // Scanner::PeakNextTokenType()

TOKENTYPE Scanner::GetType() {
  return current_token->GetType() ;
} // Scanner::GetType()

int Scanner::GetLine() {
  return current_token->GetLine() ;
} // Scanner::GetLine()

int Scanner::GetColumn() {
  return current_token->GetColumn() ;
} // Scanner::GetColumn()

CharPtr Scanner::GetStr() {
  return current_token->GetStr() ;
} // Scanner::GetStr()

CharPtr Scanner::ReadNewToken() {
  bool flag_got_token = false ;
  CharPtr str = (CharPtr) malloc( sizeof( char ) * MAX_STRING_LENGTH ) ;
  CharPtr ap = NULL ; // access pointer, point to the place where next char need to access
  ap = str ; // point to the first place of token

  if ( nextChar != '\0' ) {
    *ap = nextChar ; // get new char from store if there is a char doesn't done yet but it is gotten
    nextChar = '\0' ;
  } // if
  else *ap = GetNewChar() ; // get first char

  // getting token
  while ( true ) {
    // check EOF 
    if ( *ap == EOF ) {
      flag_EOF = true ;
      nextChar = *ap ;
      break ;
    } // if

    // check white space
    if ( isspace( *ap ) ) { 
      if ( flag_got_token == true ) break ;
      else {
        *ap = GetNewChar() ;
        continue ;
      } // else
    } // if

    // check separator ( ) ' " ;
    if ( Isseparator( *ap ) ) {
      if ( flag_got_token == true ) nextChar = *ap ;
      else {
        if ( *ap == ';' ) { // comment
          while ( ( *ap = GetNewChar() ) != '\n' ) ;
          continue ;
        } // if

        latest_token_line = current_line ;
        latest_token_column = current_column ;
        flag_got_token = true ;
        if ( *ap == '(' && ( nextChar = GetNewChar() ) == ')' ) { // nil 
          ap++ ;
          *ap++ = nextChar ;
          nextChar = '\0' ;
        } // if
        else if ( *ap == '"' ) { // string
          for ( ap++ ; *ap = GetNewChar() ; ap++ ) {
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
        } // else if
        else ap++ ;
      } // else 

      break ;
    } // if

    // others
    if ( flag_got_token == false ) {
      latest_token_line = current_line ;
      latest_token_column = current_column ;
      flag_got_token = true ;
    } // if
    *++ap = GetNewChar() ;
  } // while

  // return success
  *ap = '\0' ;
  return str ; 
} // Scanner::ReadNewToken() 

TOKENTYPE Scanner::CheckType( CharPtr str ) {
  TOKENTYPE type ;
  if ( strcmp( str, "()" ) == 0 || strcmp( str, "nil" ) == 0 || strcmp( str, "#f" ) == 0 ) 
    type = TYPE_NIL ;
  else if ( strcmp( str, "t" ) == 0 || strcmp( str, "#t" ) == 0 ) type = TYPE_T ;
  else if ( *str == '(' ) type = TYPE_LEFT_PAREN ;
  else if ( *str == ')' ) type = TYPE_RIGHT_PAREN ;
  else if ( *str == '\'' ) type = TYPE_QUOTE ;
  else if ( *str == '"' ) type = TYPE_STRING ;
  else if ( *str == ';' ) type = TYPE_COMMENT ;
  else { // int float dot symbol
    bool has_digit = false ;
    bool has_other_char = false ;
    int dot_num = 0 ;
    CharPtr tp = str ;

    if ( *tp == '+' || *tp == '-' )
      tp++ ;

    for ( ; *tp != '\0' ; tp++ ) {
      if ( *tp == '.' ) dot_num++ ;
      else if ( isdigit( *tp ) ) has_digit = true ;
      else has_other_char = true ; 
    } // for

    if ( has_other_char == false && has_digit == true && dot_num == 1 ) type = TYPE_FLOAT ;
    else if ( has_other_char == false && has_digit == true && dot_num == 0 ) type = TYPE_INT ;
    else if ( has_other_char == false && has_digit == false && dot_num == 1 ) type = TYPE_DOT ;
    else type = TYPE_SYMBOL ;
  } // else 

  return type ;
} // Scanner::CheckType()

bool Scanner::Isseparator( char ch ) {
  char separator_list[5] = { '(', ')', '\'', '"', ';' } ;
  for ( int i = 0 ; i < 5 ; i++ )
    if ( ch == separator_list[i] )
      return true ;

  return false ;
} // Scanner::Isseparator()

char Scanner::GetNewChar() {
  char ch = getchar() ;
  if ( ch == '\n' ) {
    current_line++ ;
    current_column = 0 ;
  } // if
  else current_column++ ;
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
      printf( "\nERROR (unexpected token) : ')' expected when token at Line %d Column %d is >>%s<<", scanner.GetLine(), scanner.GetColumn(), scanner.GetStr() ) ;
      return false ; // check RIGHT-PAREN
    } // else
  } // if
  else if ( scanner.GetType() == TYPE_QUOTE ) return SExp() ; // try to get SExp
  else { // get atom
    if ( Atom() == true ) return true ;
    else {
      printf("\nERROR (unexpected token) : atom or '(' expected when token at Line %d Column %d is >>%s<<", scanner.GetLine(), scanner.GetColumn(), scanner.GetStr() ) ;
      return false ;
    } // else
  } // else

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
