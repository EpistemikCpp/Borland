/*!***************************************************
    Mark Sattolo (mhsatto@po.cse)
   -----------------------------------------------
	  File: @(#)node.cpp
     Version: 1.9
     Last Update: 03/03/14 08:41:01
   
***************************************************!*/

#include "node.h"

Knapnode::Knapnode( string s, int l=0 )
{
  _name.assign( s ) ;
  _level  = l ;
  _profit = _weight = 0 ;
  _pwr    = _bound  = 0.0 ;
}

Knapnode::Knapnode( string s, int p, int w, int l=0, float b=0.0 )
{
  _name.assign( s );
  _level  = l ;
  _profit = p ;
  _weight = w ;
  _pwr    = (w > 0) ? (static_cast<float>(p) / static_cast<float>(w)) : 0.0 ;
  _bound  = b ;
}

Knapnode Knapnode::operator=( Knapnode k )
{
  _name   = k.name()  ;
  _level  = k.level() ;
  _profit = k.profit() ;
  _weight = k.weight() ;
  _pwr    = k.pwr() ;
  _bound  = k.bound() ;

  return *this ;
}

void Knapnode::display( ostream &strm ) const
{
  strm << setw(20) << _name << ":  level " << setw(3) << _level ;
  strm << " ; profit " << setw(6) << _profit << " ; weight " << setw(5) << _weight ;
  strm.setf( ios_base::fixed, ios_base::floatfield );
  strm << setprecision(3) << " ; bound " << setw(7) << _bound ;
  strm << " ; pwr " << setw(7) << _pwr << endl ;
};


///// PRIORITY QUEUE FUNCTIONS ////////////////////////////////////////////////////////////

//! function objects for ordering the priority_queues
bool operator< ( const Knapnode& k1, const Knapnode& k2 )
{
  if( k1.bound() < k2.bound() )
	 return true ;

  return false ;
}


bool operator== ( const Knapnode& k1, const Knapnode k2 )
{
  return( k1.bound() == k2.bound() );
}


//$ CAN'T GET THIS ONE TO WORK !
struct order_bound
{
  bool operator()( Knapnode a, Knapnode b ) const
  {
	 return( a.bound() < b.bound() );
  }
}
 bound_compare ;


//! displayPq()
void displayPq( knapPqCont pq, ostream &strm )
{
  strm << endl << "[4mtop of Pq:[0m" << endl ;
  pq.top().display( strm );

  // ??displayVector( reinterpret_cast< vector<Knapnode&> >( pq ) );
}


///// VECTOR FUNCTIONS ///////////////////////////////////////////////////////////////////


//! function object for ordering the vector
struct order_pwr
{
  bool operator()( Knapnode a, Knapnode b ) const
  {
	 return( a.pwr() > b.pwr() ); // descending order
  }
}
 pwr_compare ;


//! displayVector()
void displayVector( knapVec v, ostream &strm )
{
  int i=1 ;
  knapVec::iterator vit = v.begin();

  strm << endl << "[1m Vector Elements: [0m" << endl ;
  while( vit != v.end() )
  {
	 strm << setw(3) << i << ". " ;
	 (*vit).display( strm );

	 vit++ ;
	 i++ ;
  }
  strm << endl ;
}

// node.cpp

