/*!***************************************************
    Mark Sattolo (mhsatto@po.cse)
   -----------------------------------------------
     File: @(#)node.hh
     Version: 1.9
     Last Update: 03/03/14 08:41:01
   
***************************************************!*/

#ifndef MHS_ALG_NODE_HH
#define MHS_ALG_NODE_HH

#include <cstdlib>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <iomanip>

using namespace std ;

class Knapnode
{
 protected:
  
     string	_name   ;
     int	_level  ; // in the state space tree
     int	_profit ;
     int	_weight ;
     float	_pwr    ; // profit to weight ratio
     float	_bound  ; // i.e. potential max profit of each node

 public:
  
     Knapnode( string s, int l=0 )
     {
       _name.assign( s ) ;
       _level  = l ;
       _profit = _weight = 0 ;
       _pwr    = _bound  = 0.0 ;
     };
	
     Knapnode( string s, int p, int w, int l=0, float b=0.0 )
     {
       _name.assign( s );
       _level  = l ;
       _profit = p ;
       _weight = w ;
       _pwr    = (w > 0) ? (static_cast<float>(p) / static_cast<float>(w)) : 0.0 ;
       _bound  = b ;
     };

     string  name() const { return _name ; };
     void    name( string s ) { _name.assign( s ); };
     void    addname( string s ) { _name.append( s ); };

     int     level() const { return _level ; };
     void    level( int l ) { _level = l ; };

     int     profit() const { return _profit ; };
     void    profit( int p ) { _profit = p ; };

     int     weight() const { return _weight ; };
     void    weight( int w ) { _weight = w ; };

     float   pwr() const { return _pwr ; };
     void    pwr( float r ) { _pwr = r ; };

     float   bound() const { return _bound ; };
     void    bound( float b ) { _bound = b ; };

     Knapnode operator=( Knapnode k )
     {
       _name   = k.name()  ;
       _level  = k.level() ;
       _profit = k.profit() ;
       _weight = k.weight() ;
       _pwr    = k.pwr() ;
       _bound  = k.bound() ;
       
       return *this ;
     };

     void display( ostream &strm ) const
     {
       strm << setw(20) << _name << ":  level " << setw(3) << _level ;
       strm << " ; profit " << setw(6) << _profit << " ; weight " << setw(5) << _weight ;
       strm.setf( ios_base::fixed, ios_base::floatfield );
       strm << setprecision(3) << " ; bound " << setw(7) << _bound ;
       strm << " ; pwr " << setw(7) << _pwr << endl ;
     };
     
     // FUNCTIONS TO ENABLE less<Knapnode>
     friend bool operator< ( const Knapnode&, const Knapnode& );
     friend bool operator== ( const Knapnode&, const Knapnode& );

};//! class Knapnode


///// PRIORITY QUEUE FUNCTIONS ////////////////////////////////////////////////////////////

//! typedefs for priority_queue
// - see "The C++ Standard Template Library", by Plauger et al., p.455
typedef allocator<Knapnode> knapPqAl ;
typedef less<Knapnode> knapPqPred ;
typedef vector<Knapnode, knapPqAl> knapPqImpl ;
typedef priority_queue<Knapnode, knapPqImpl, knapPqPred> knapPqCont ;
knapPqCont::container_type *p_cont = (knapPqImpl*)0 ;
knapPqCont::value_type *p_val = (Knapnode*)0 ;
knapPqCont::size_type *p_size = (size_t*)0 ;


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

//! typedef for vector
typedef vector<Knapnode> knapVec ;


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


#endif // MHS_ALG_NODE_HH
