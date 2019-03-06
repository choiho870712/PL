// headers //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// include 
# include <iostream> // I/O
using namespace std ;

// flags
bool flag_EOF ; // end of file flag, if true then print eof message and quit the system
bool flag_quit ; // quit flag, if true then quit the system

// functions
void Init() ; // initiate global variables and flags

// classes
class Parser {
  public :
    Parser() ;
    bool ReadSExp() ; // read s-exp, if quit or end of file reture false
    bool PrintSExp() ; // print s-exp and it's structure, if no s-exp then return false
  private :
} // class Parser

// class Parser /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Parser::Parser() {

} // Parser::Parser()

bool Parser::ReadSExp() {

} // Parser::ReadSExp()

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
