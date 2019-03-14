// headers //////////////////////////////////////////////////////////////////////////////////////////////////

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
bool g_flag_EOF ; // end of file flag, if true then print eof message and quit the system

// functions
void Init() ; // initiate global variables and flags

// class Token //////////////////////////////////////////////////////////////////////////////////////////////
class Token {
  public :
  Token() {
    m_str = NULL ;
    m_type = TYPE_UNKNOWN ;
    m_line = 1 ;
    m_column = 0 ;
  } // Token()

  void PrintToken() {
    if ( m_type == TYPE_DOT ) printf( "." ) ;
    else if ( m_type == TYPE_NIL ) printf( "nil" ) ;
    else if ( m_type == TYPE_T ) printf( "#t" ) ;
    else if ( m_type == TYPE_QUOTE ) printf( "quote" ) ;
    else if ( m_type == TYPE_INT ) printf( "%d", atoi( m_str ) ) ;
    else if ( m_type == TYPE_FLOAT ) printf( "%.3f", atof( m_str ) ) ;
    else PrintStr() ;
  } // PrintToken()

  void PrintStr() {
    printf( "%s", m_str ) ;
  } // PrintStr()

  void PrintType() {
    if ( m_type == TYPE_DOT ) printf( "TYPE_DOT" ) ;
    else if ( m_type == TYPE_SYMBOL ) printf( "TYPE_SYMBOL" ) ;
    else if ( m_type == TYPE_INT ) printf( "TYPE_INT" ) ;
    else if ( m_type == TYPE_FLOAT ) printf( "TYPE_FLOAT" ) ;
    else if ( m_type == TYPE_QUOTE ) printf( "TYPE_QUOTE" ) ;
    else if ( m_type == TYPE_T ) printf( "TYPE_T" ) ;
    else if ( m_type == TYPE_NIL ) printf( "TYPE_NIL" ) ;
    else if ( m_type == TYPE_STRING ) printf( "TYPE_STRING" ) ;
    else if ( m_type == TYPE_COMMENT ) printf( "TYPE_COMMENT" ) ;
    else if ( m_type == TYPE_UNKNOWN ) printf( "TYPE_UNKNOWN" ) ;
    else if ( m_type == TYPE_RIGHT_PAREN ) printf( "TYPE_RIGHT_PAREN" ) ;
    else if ( m_type == TYPE_LEFT_PAREN ) printf( "TYPE_LEFT_PAREN" ) ;
    else printf( "DEFAULT" ) ;
  } // PrintType()

  void PrintLine() {
    printf( "%d", m_line ) ;
  } // PrintLine()

  void PrintColumn() {
    printf( "%d", m_column ) ;
  } // PrintColumn()

  TOKENTYPE GetType() {
    return m_type ;
  } // GetType()

  CharPtr GetStr() {
    return m_str ;
  } // GetStr()

  int GetLine() {
    return m_line ;
  } // GetLine()

  int GetColumn() {
    return m_column ;
  } // GetColumn()

  void SetStr( CharPtr input_str ) {
    m_str = input_str ;
  } // SetStr()

  void SetType( TOKENTYPE input_type ) {
    m_type = input_type ;
  } // SetType()

  void SetLine( int input_line ) {
    m_line = input_line ;
  } // SetLine()

  void SetColumn( int input_column ) {
    m_column = input_column ;
  } // SetColumn()

  private :
  CharPtr m_str ;
  TOKENTYPE m_type ;
  int m_line ;
  int m_column ;
} ; // class Token
typedef Token *TokenPtr ;

// class PparsingTree ///////////////////////////////////////////////////////////////////////////////////////
class ParsingTree {
  public :
  ParsingTree() {
    m_currentToken = NULL ;
    m_leftSubTree = NULL ;
    m_rightSubTree = NULL ;
    m_isFromRightTree = false ;
    m_printedLevel = 0 ;
    m_currentLevel = 0 ;
  } // ParsingTree()

  bool SetLeftSubTree( ParsingTree *tree ) {
    if ( tree == NULL ) return false ;
    m_leftSubTree = tree ;
    return true ;
  } // SetLeftSubTree()

  bool SetRightSubTree( ParsingTree *tree ) {
    if ( tree == NULL ) return false ;
    m_rightSubTree = tree ;
    return true ;
  } // SetRightSubTree()

  bool SetCurrentToken( TokenPtr token ) {
    if ( token == NULL ) return false ;
    m_currentToken = token ;
    return true ;
  } // SetCurrentToken()

  ParsingTree* GetLeftSubTree() {
    return m_leftSubTree ;
  } // GetLeftSubTree()

  ParsingTree* GetRightSubTree() {
    return m_rightSubTree ;
  } // GetRightSubTree()

  TokenPtr GetCurrentToken() {
    return m_currentToken ;
  } // GetCurrentToken()

  void PrintParsingTree( ParsingTree *tree ) {
    ParsingTree *leftPtr = tree->GetLeftSubTree() ;
    ParsingTree *rightPtr = tree->GetRightSubTree() ;
    TokenPtr tokenPtr = tree->GetCurrentToken() ;

    if ( tokenPtr != NULL ) { // in leaves
      if ( m_isFromRightTree == true ) { // is the end of current level
        if ( tokenPtr->GetType() != TYPE_NIL ) { // if not nil
          for ( int i = 0 ; i < m_currentLevel + 1 ; i++ )
            printf( "  " ) ;
          printf( ".\n" ) ;
          for ( int i = 0 ; i < m_currentLevel + 1 ; i++ )
            printf( "  " ) ;
          tokenPtr->PrintToken() ;
          printf( "\n" ) ;
        } // if
      } // if
      else {
        if ( m_currentLevel > m_printedLevel ) { // level change, print (
          for ( int i = 0 ; i < m_printedLevel ; i++ )
            printf( "  " ) ;
          for ( int i = 0 ; i < m_currentLevel - m_printedLevel ; i++ )
            printf( "( " ) ;
          tokenPtr->PrintToken() ;
          printf( "\n" ) ;
          m_printedLevel = m_currentLevel ;
        } // if
        else { // same level
          for ( int i = 0 ; i < m_currentLevel ; i++ )
            printf( "  " ) ;
          tokenPtr->PrintToken() ;
          printf( "\n" ) ;
        } // else
      } // else
    } // if

    if ( leftPtr != NULL ) { // go to left
      m_isFromRightTree = false ;
      m_currentLevel++ ;
      PrintParsingTree( leftPtr ) ;
      m_currentLevel-- ;
    } // if

    if ( rightPtr != NULL ) { // go to right
      m_isFromRightTree = true ;
      PrintParsingTree( rightPtr ) ;
    } // if

    if ( rightPtr == NULL && m_printedLevel > m_currentLevel ) { // print )
      m_printedLevel-- ;
      for ( int i = 0 ; i < m_printedLevel ; i++ )
        printf( "  " ) ;
      printf( ")\n" ) ;
    } // if
  } // PrintParsingTree()

  private :
  int m_printedLevel ;
  int m_currentLevel ;
  bool m_isFromRightTree ;
  TokenPtr m_currentToken ;
  ParsingTree *m_leftSubTree ;
  ParsingTree *m_rightSubTree ;
} ; // class ParsingTree
typedef ParsingTree *ParsingTreePtr ;

// class Scanner ////////////////////////////////////////////////////////////////////////////////////////////
class Scanner {
  public :
  Scanner() {
    m_flag_multi_cmd_in_same_line = false ;
    m_flag_get_first_token = false ;
    m_pre_line = 1 ;
    m_pre_column = 0 ;
    m_current_line = 1 ;
    m_current_column = 0 ;
    m_latest_token_line = 1 ;
    m_latest_token_column = 0 ;
    m_current_token = NULL ;
    m_next_token = NULL ;
    m_nextChar = '\0' ;
  } // Scanner()

  bool m_flag_multi_cmd_in_same_line ;

  void Init() {
    m_flag_get_first_token = false ;
    m_pre_line = 1 ;
    m_pre_column = 0 ;
    m_current_line = 1 ;
    m_current_column = 0 ;
    m_latest_token_line = 1 ;
    m_latest_token_column = 0 ;
    m_current_token = NULL ;
  } // Init()

  bool AdvanceToNextToken() {
    if ( m_next_token != NULL ) {
      m_current_token = m_next_token ;
      m_next_token = NULL ;
    } // if
    else {
      CharPtr newToken ;
      newToken = ReadNewToken() ;
      if ( newToken == NULL ) return false ;
      m_current_token = new Token ;
      m_current_token->SetStr( newToken ) ;
      m_current_token->SetType( CheckType( m_current_token->GetStr() ) ) ;
      m_current_token->SetLine( m_latest_token_line ) ;
      m_current_token->SetColumn( m_latest_token_column ) ;
    } // else

    // m_current_token->PrintStr() ;
    // printf( " " ) ;
    // m_current_token->PrintLine() ;
    // printf( " " ) ;
    // m_current_token->PrintColumn() ;
    // printf( "\n" ) ;

    m_flag_multi_cmd_in_same_line = false ;
    return true ;
  } // AdvanceToNextToken() 

  TOKENTYPE PeakNextTokenType() {
    if ( m_next_token == NULL ) {
      m_next_token = new Token ;
      m_next_token->SetType( TYPE_UNKNOWN ) ;
      CharPtr newToken ;
      newToken = ReadNewToken() ;
      if ( newToken != NULL ) {
        m_next_token->SetStr( newToken ) ;
        m_next_token->SetType( CheckType( m_next_token->GetStr() ) ) ;
        m_next_token->SetLine( m_latest_token_line ) ;
        m_next_token->SetColumn( m_latest_token_column ) ;
      } // if
    } // if

    return m_next_token->GetType() ;
  } // PeakNextTokenType()

  TOKENTYPE GetType() {
    return m_current_token->GetType() ;
  } // GetType()

  int GetLine() {
    return m_current_token->GetLine() ;
  } // GetLine()

  int GetColumn() {
    return m_current_token->GetColumn() ;
  } // GetColumn()

  CharPtr GetStr() {
    return m_current_token->GetStr() ;
  } // GetStr()

  TokenPtr GetCurrentToken() {
    return m_current_token ;  
  } // GetCurrentToken()

  void ClearThisLine() {
    m_nextChar = '\0' ;
    while ( getchar() != '\n' ) ;
  } // ClearThisLine()

  private :
  bool m_flag_got_token ;
  bool m_flag_get_first_token ;
  int m_pre_line ;
  int m_pre_column ;
  int m_current_line ;
  int m_current_column ;
  int m_latest_token_column ;
  int m_latest_token_line ;
  TokenPtr m_current_token ;
  TokenPtr m_next_token ;
  char m_nextChar ; // store next char while there is a char doesn't be done yet, and it was gotten

  CharPtr ReadNewToken() {
    bool flag_get_token = false ;
    bool flag_break = false ;
    CharPtr str = ( CharPtr ) malloc( sizeof( char ) * MAX_STRING_LENGTH ) ;
    int top = 0 ;
    str[top] = GetNewChar() ;

    // getting token
    while ( flag_break == false ) {
      if ( str[top] == EOF ) { // check EOF 
        if ( flag_get_token == true )
          StoreChar( str[top] ) ;
        else g_flag_EOF = true ;
        flag_break = true ;
      } // if
      else if ( isspace( str[top] ) ) { // check white space
        if ( flag_get_token == true ) {
          StoreChar( str[top] ) ;
          flag_break = true ;
        } // if
        else
          str[top] = GetNewChar() ;
      } // else if
      else if ( Isseparator( str[top] ) ) { // check separator ( ) ' " ;
        flag_break = true ;
        if ( flag_get_token == true ) StoreChar( str[top] ) ;
        else {
          if ( str[top] == ';' ) { // comment
            flag_break = false ;
            while ( str[top] != '\n' ) 
              str[top] = GetNewChar() ;
          } // if
          else {
            m_latest_token_line = m_current_line ;
            m_latest_token_column = m_current_column ;
            flag_get_token = true ;

            if ( str[top] == '"' ) { // string
              for ( str[++top] = GetNewChar() ; str[top] != '"' ; str[++top] = GetNewChar() ) {
                if ( str[top] == '\\' ) {
                  char ch = GetNewChar() ;
                  if ( ch == 'n' ) str[top] = '\n' ;
                  else if ( ch == '"' ) str[top] = '"' ;
                  else if ( ch == 't' ) str[top] = '\t' ;
                  else if ( ch == '\\' ) str[top] = '\\' ;
                  else str[++top] = ch ;
                } // if
                else if ( str[top] == '\n' ) {
                  str[top] = '\0' ;
                  printf( "ERROR (no closing quote) : END-OF-LINE encountered at Line" ) ;
                  printf( " %d Column %d\n", m_pre_line, m_pre_column ) ;

                  return NULL ;
                } // else if
              } // for
            } // if

            top++ ;
          } // else
        } // else 
      } // else if
      else { // others
        if ( flag_get_token == false ) {
          m_latest_token_line = m_current_line ;
          m_latest_token_column = m_current_column ;
          flag_get_token = true ;
        } // if

        str[++top] = GetNewChar() ;
      } // else
    } // while

    if ( flag_get_token == true ) 
      m_flag_get_first_token = true ;

    // return success
    str[top] = '\0' ;
    return str ; 
  } // ReadNewToken() 

  TOKENTYPE CheckType( CharPtr str ) {
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
      bool has_symbol = false ;
      bool has_digit = false ;
      bool has_other_char = false ;
      int dot_num = 0 ;
      int top = 0 ;

      if ( str[top] == '+' || str[top] == '-' ) {
        has_symbol = true ;
        top++ ;
      } // if

      for ( ; str[top] != '\0' ; top++ ) {
        if ( str[top] == '.' ) dot_num++ ;
        else if ( isdigit( str[top] ) ) has_digit = true ;
        else has_other_char = true ; 
      } // for

      if ( has_other_char == false && has_digit == true && dot_num == 1 ) type = TYPE_FLOAT ;
      else if ( has_other_char == false && has_digit == true && dot_num == 0 ) type = TYPE_INT ;
      else if ( has_symbol == false && has_other_char == false && has_digit == false && dot_num == 1 ) 
        type = TYPE_DOT ;
      else type = TYPE_SYMBOL ;
    } // else 

    return type ;
  } // CheckType()

  bool Isseparator( char ch ) {
    char separator_list[5] = { '(', ')', '\'', '"', ';' } ;
    for ( int i = 0 ; i < 5 ; i++ )
      if ( ch == separator_list[i] )
        return true ;

    return false ;
  } // Isseparator()

  char GetNewChar() {
    char ch ;
    if ( m_nextChar != '\0' ) {
      ch = m_nextChar ; // get new char from store if there is a char doesn't done yet but it is gotten
      m_nextChar = '\0' ;
    } // if
    else ch = getchar() ; // get first char

    if ( ch == '\n' ) {
      m_pre_line = m_current_line ;
      m_pre_column = m_current_column ;
      m_current_line++ ;
      m_current_column = 0 ;
    } // if
    else { 
      m_pre_column = m_current_column ;
      m_current_column++ ;
    } // else

    if ( m_flag_get_first_token == false && m_flag_multi_cmd_in_same_line == true && ch == '\n' ) {
      m_flag_multi_cmd_in_same_line = false ;
      m_current_line = 1 ;
      m_current_column = 0 ;
      m_pre_line = 1 ;
      m_pre_column = 0 ;
    } // if

    return ch ;
  } // GetNewChar()

  void StoreChar( char ch ) {
    m_nextChar = ch ;
    m_current_line = m_pre_line ;
    m_current_column = m_pre_column ;
  } // StoreChar()
} ; // class Scanner

// class Parser /////////////////////////////////////////////////////////////////////////////////////////////
class Parser {
  public :
  Parser() {
    m_parsingTree = NULL ;
  } // Parser()

  bool ReadSExp() {
    m_scanner.Init() ;

    m_parsingTree = SExp() ;
    if ( m_parsingTree == NULL ) return false ;
    else {
      m_scanner.m_flag_multi_cmd_in_same_line = true ;
      return true ;
    } // else 
  } // ReadSExp()

  bool IsExit() {
    ParsingTreePtr leftPtr ;
    TokenPtr token ;
    if ( m_parsingTree != NULL && m_parsingTree->GetRightSubTree() == NULL ) {
      leftPtr = m_parsingTree->GetLeftSubTree() ;
      if ( leftPtr != NULL )
        if ( leftPtr->GetRightSubTree() == NULL && leftPtr->GetLeftSubTree() == NULL ) {
          token = leftPtr->GetCurrentToken() ;
          if ( token != NULL )
            if ( strcmp( token->GetStr(), "exit" ) == 0 )
              return true ;
        } // if
    } // if

    return false ;
  } // IsExit()

  void PrintSExp() {
    m_parsingTree->PrintParsingTree( m_parsingTree ) ;
  } // PrintSExp()

  private :
  Scanner m_scanner ;
  ParsingTreePtr m_parsingTree ;

  ParsingTreePtr SExp() {
    // <S-exp> ::= <ATOM> | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN | QUOTE <S-exp>
    ParsingTreePtr headPtr, currentPtr, accessPtr ; // creat sexp tree
    headPtr = new ParsingTree ;
    currentPtr = headPtr ;
    accessPtr = NULL ;

    // do syntax
    if ( m_scanner.PeakNextTokenType() == TYPE_LEFT_PAREN ) { // LEFT-PAREN 
      if ( m_scanner.AdvanceToNextToken() == false ) return NULL ; // get LEFT-PAREN
      if ( m_scanner.PeakNextTokenType() == TYPE_RIGHT_PAREN ) { // () nil
        TokenPtr token = m_scanner.GetCurrentToken() ;
        token->SetType( TYPE_NIL ) ;
        if ( m_scanner.AdvanceToNextToken() == false ) return NULL ; // get RIGHT-PAREN
        if ( headPtr->SetCurrentToken( token ) == true ) return headPtr ;
      } // if
      else {
        if ( headPtr->SetLeftSubTree( SExp() ) == false ) return NULL ; // get <S-exp> and put it to left tree

        while ( ! ( m_scanner.PeakNextTokenType() == TYPE_DOT || 
                    m_scanner.PeakNextTokenType() == TYPE_RIGHT_PAREN ) ) { // stop with DOT and QUOTE
          accessPtr = new ParsingTree ; // creat new sub tree 
          currentPtr->SetRightSubTree( accessPtr ) ; // add this tree to right tree
          currentPtr = currentPtr->GetRightSubTree() ; // jump to that tree
          if ( currentPtr->SetLeftSubTree( SExp() ) == false ) return NULL ; // put it to left tree
        } // while

        if ( m_scanner.PeakNextTokenType() == TYPE_DOT ) { // [ DOT <S-exp> ], 
          if ( m_scanner.AdvanceToNextToken() == false ) return NULL ; // get DOT
          if ( currentPtr->SetRightSubTree( SExp() ) == false ) return NULL ; // put it to right tree
        } // if

        if ( m_scanner.AdvanceToNextToken() == false ) return NULL ; // get RIGHT-PAREN 
        if ( m_scanner.GetType() == TYPE_RIGHT_PAREN ) { // RIGHT-PAREN 
          return headPtr ; // success, return full tree
        } // if
        else { // no RIGHT-PAREN 
          CharPtr str = m_scanner.GetStr() ;
          printf( "ERROR (unexpected token) : ')' expected when token at Line %d ", m_scanner.GetLine() ) ;
          printf( "Column %d is >>%c<<\n", m_scanner.GetColumn(), str[0] ) ;
          m_scanner.ClearThisLine() ;
          return NULL ;
        } // else
      } // else
    } // if
    else if ( m_scanner.PeakNextTokenType() == TYPE_QUOTE ) { // QUOTE <S-exp>
      if ( m_scanner.AdvanceToNextToken() == false ) return NULL ; // advance to next token QUOTE
      accessPtr = new ParsingTree ; // creat new sub tree 
      currentPtr->SetLeftSubTree( accessPtr ) ; // add this tree to left tree
      currentPtr = currentPtr->GetLeftSubTree() ; // jump to that tree
      if ( currentPtr->SetCurrentToken( m_scanner.GetCurrentToken() ) == false ) return NULL ; // put quote 
      if ( currentPtr->SetRightSubTree( SExp() ) == false ) return NULL ; // put it to right tree
      return headPtr ; // success, return full tree
    } // else if
    else if ( m_scanner.PeakNextTokenType() != TYPE_UNKNOWN ) { // get atom
      if ( m_scanner.AdvanceToNextToken() == false ) return NULL ; // advance to next token atom
      if ( headPtr->SetCurrentToken( Atom() ) == true ) return headPtr ;
      else {
        CharPtr str = m_scanner.GetStr() ;
        printf( "ERROR (unexpected token) : atom or '(' expected when token at Line" ) ;
        printf( " %d Column", m_scanner.GetLine() ) ;
        printf( " %d is >>%c<<\n", m_scanner.GetColumn(), str[0] ) ;
        m_scanner.ClearThisLine() ;
        return NULL ;
      } // else
    } // else if
    else {
      if ( m_scanner.AdvanceToNextToken() == false ) return NULL ; // advance to next token
      return NULL ;
    } // else

    return NULL ;
  } // SExp()

  TokenPtr Atom() {
    // <ATOM>  ::= SYMBOL | INT | FLOAT | STRING | NIL | T | LEFT-PAREN RIGHT-PAREN
    if ( m_scanner.GetType() == TYPE_SYMBOL ) return m_scanner.GetCurrentToken() ;
    else if ( m_scanner.GetType() == TYPE_INT ) return m_scanner.GetCurrentToken() ;
    else if ( m_scanner.GetType() == TYPE_FLOAT ) return m_scanner.GetCurrentToken() ;
    else if ( m_scanner.GetType() == TYPE_STRING ) return m_scanner.GetCurrentToken() ;
    else if ( m_scanner.GetType() == TYPE_NIL ) return m_scanner.GetCurrentToken() ;
    else if ( m_scanner.GetType() == TYPE_T ) return m_scanner.GetCurrentToken() ;
    else return NULL ;
  } // Atom()
} ; // class Parser

// main /////////////////////////////////////////////////////////////////////////////////////////////////////
void Init() {
  g_flag_EOF = false ;
} // Init()

int main() {
  while ( getchar() != '\n' ) ; 

  bool flag_exit = false ;
  Parser parser ;
  Init() ;

  printf( "Welcome to OurScheme!\n" ) ;

  while ( flag_exit == false ) {
    printf( "\n> " ) ;

    if ( parser.ReadSExp() == true ) {
      if ( g_flag_EOF == true ) {
        printf( "ERROR (no more input) : END-OF-FILE encountered\n" ) ;
        flag_exit = true ;
      } // if
      else if ( parser.IsExit() ) {
        printf( "\n" ) ;
        flag_exit = true ;
      } // else if
      else parser.PrintSExp() ;
    } // if
  } // while

  printf( "Thanks for using OurScheme!" ) ;
} // main()
