// test.hpp

#ifndef MHS_HEAPS_TEST_HPP
#define MHS_HEAPS_TEST_HPP

using namespace std ;

#include <iostream> 

class test_class 
{
 private:
	  static int last_id ;
	  int id ;
	  double value ;

 public:
	  //constructor sets 'value'
	  test_class( const double& ) ;
	  //overload operator* to return 'value'
	  double& operator*() ;

 friend ostream& operator<<( ostream&, const test_class& ) ;
};

// free function - implemented in test.cpp
ostream& operator<<( ostream&, const test_class& ) ;


#endif // MHS_HEAPS_TEST_HPP
