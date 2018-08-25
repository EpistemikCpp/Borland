/*!***************************************************
    Mark Sattolo (mhsatto@po.cse)
   -----------------------------------------------
     File: @(#)knapsack.cc
     Version: 1.8
     Last Update: 03/03/13 15:01:31
   
***************************************************!*/
/*
  Best-First Search with Branch-and-Bound Pruning Algorithm for the 0-1 Knapsack Problem
  
  see p.235 of "Foundations of Algorithms: with C++ pseudocode", 2nd Ed. 1998, 
               by Richard Neapolitan & Kumarss Naimipour, Jones & Bartlett, ISBN 0-7637-0620-5
  
  Problem: Let n items be given, where each item has a weight and a profit.  The weights and profits
	   are positive integers.  Furthermore, let a positive integer W be given.  Determine a set
	   of items with maximum total profit, under the constraint that the sum of their weights 
	   cannot exceed W.
  
  Inputs: positive integers n and W,
	  arrays of positive integers w[] and p[],
	    ( each indexed from 1 to n, and each of which is sorted in non-increasing order
	      according to the values of p[i]/w[i] ).
  
  Outputs: an integer maxprofit that is the sum of the profits of an optimal set.
	   ?[an array giving the indices of the items comprising the optimal set]
*/

#include <algorithm> // for sort()
#include <fstream>
#include "node.hh"

// FUNCTION PROTOTYPES
float bound( Knapnode, knapVec&, int, int );
int bestFirstSearch( knapVec&, int, int, int&, string& );


/*!**** MAIN ********************************************************************************!*/

int main( int argc, char *argv[] )
{
  int n=0 ,		// total items
      W ,		// maximum allowed weight of items (user-supplied)
      totweight=0 ;	// of items selected
      
  string bestitems ;

  int p, w ;	// temps for file data
  string temp ; // item name
  
  // the p/w vector
  knapVec vpw ;

  /* check command line parameters */
  if( argc < 2 )
  {
    cerr << endl << "[1m Usage: " << argv[0] << " 'file name' [max weight]" << endl ;
    exit( 1 );
  }
  
  if( argc < 3 )
  {
    cout << endl << "Please enter the maximum weight: " ;
    cin >> W ;
  }  
  else
      W = atoi( argv[2] );

  cout << endl << "File name is '" << argv[1] << "' " << endl ;
  cout << "Max Weight == " << W << endl ;

  // open the file
  ifstream inputfile( argv[1] );
  if( !inputfile )
  {
    cerr << "[31m Error occurred opening file " <<  argv[1] << "[0m" << endl ;
    exit( 2 );
  }
  
  // get the data from the file
  while( !inputfile.eof() )
  {
    // insert to the temps
    inputfile >> temp ;
    inputfile >> p ;
    inputfile >> w ;
    
    if( inputfile.get() != EOF ) // use get() to eat the EOL
    {
      // create a new node and load it into the vector
      vpw.push_back( Knapnode(temp, p, w) );
 
#if KNAPSACK_MAIN_DEBUG > 1
      vpw.back().display( cout ) ;
#endif

      n++ ; // total number of items in the file
    }
    
  }// while( !inputfile.eof() )
  
  cout << endl << "There were " << n << " items in file '"<< argv[1] << "'" << endl ;

  // sort the vector - descending order
  sort( vpw.begin(), vpw.end(), order_pwr() );
  
  // display the sorted vector
  cout << endl << "[4m The p/w vector: [0m" ;
  displayVector( vpw, cout );

  // run the algorithm and display the results
  int maxProfit = bestFirstSearch( vpw, n, W, totweight, bestitems );
  cout << endl << "[1m For Weight limit " << W << ": Max Profit == " << maxProfit ;
  cout << "[0m (weight of items == " << totweight << ")" << endl ;
  cout << "[34mBest items are: " << bestitems << "[0m" << endl ;
  
  inputfile.close();
  
  cout << endl << " PROGRAM ENDED." << endl ;
  
  return 0 ;
  
}//! main()


///// FUNCTIONS /////////////////////////////////////////////////////////////////////////////////

//! bound()
float bound( Knapnode x, knapVec &vpw, int n, int W )
{
  int j ,		// to advance the iterator
      totweight ;	// of items in the bound
      
  float result ;
  
  // iterator for the pw vector
  knapVec::iterator vit ;
  
#if KNAPSACK_BOUND_DEBUG > 1
  cout << endl << "[4m INSIDE bound():[0m" << endl ;
  cout << " x.level() == " << x.level() << endl ;
  cout << " n == " << n ;
  displayVector( vpw, cout );
#endif

#if KNAPSACK_BOUND_DEBUG > 0
  cout << endl << " bound(1): " ;
  x.display( cout );
#endif
    
  if( x.weight() >= W )
    return 0.0 ;
  else
   {
     result = static_cast<float>( x.profit() );
     
     totweight = x.weight() ;

     // get to the proper level of vpw
     vit = vpw.begin();
     for( j=0; j < x.level()+1 ; j++ )
       vit++ ;
      
#if KNAPSACK_BOUND_DEBUG > 1
     cout << " bound(2): j == " << j << endl ;
#endif
     // grab as many items as possible     
     while( vit != vpw.end()  &&  (totweight + (*vit).weight() <= W) )
     {
       totweight += (*vit).weight() ;
       result += static_cast<float>( (*vit).profit() );
       
       vit++ ;
       j++ ; // just for debugging
#if KNAPSACK_BOUND_DEBUG > 0
       cout << " bound(3): result == " << result << endl ;
#endif
     }// while
     
#if KNAPSACK_BOUND_DEBUG > 1
       cout << " bound(4): now, j == " << j << endl ;
#endif
     if( vit != vpw.end() )
       // grab fraction of jth item
       result += ( (static_cast<float>(W - totweight)) * (*vit).pwr() );
     
#if KNAPSACK_BOUND_DEBUG > 0
     cout << " bound(5): node " << x.name() << " has bound == " << result << endl ;
#endif

     return result ;
     
   }// else
   
}//! bound()


//! bestFirstSearch()
int bestFirstSearch( knapVec &vpw, int n, int W, int &tw, string &best )
{
  int         i =    0  , // keep track of # of times through the while loop
          limit = 1024  , // for debugging
      maxprofit = 0 ;

  
  // working nodes
  Knapnode u( "_" );
  Knapnode v( "root", -1 ); // start at -1 so the root node will get the proper bound

  // the Priority Queue
  knapPqPred pr ;
  knapPqCont sst( pr );
  
  // iterator for the pw vector
  knapVec::iterator vit ;
  
  // get the initial bound
  v.bound( bound(v, vpw, n, W) );

#if KNAPSACK_BFS_DEBUG > 0
  cout << endl ;
  v.display( cout );
#endif
  
  // put the root node on the priority queue
  sst.push( Knapnode(v) );
  
#if KNAPSACK_BFS_DEBUG > 0
  displayPq( sst, cout );
  cout << "[33mSTART WHILE LOOP... [0m" << endl << endl ;
#endif
  
  //$ WHILE LOOP
  while( !sst.empty()  &&  i < limit ) //! limit prevents a runaway loop
  {
#if KNAPSACK_BFS_DEBUG > 1
    displayPq( sst, cout );
#endif
  
    v = sst.top() ; // remove node with best bound
    sst.pop();
    
#if KNAPSACK_BFS_DEBUG > 0
    cout << endl << "BFS( v ): " ;
    v.display( cout );
#endif
    
    if( v.bound() > maxprofit ) // check if node is still promising
    {
#if KNAPSACK_BFS_DEBUG > 0
      cout << "v.bound == " << v.bound() << endl ;
#endif

      //! SET u TO THE CHILD THAT INCLUDES THE NEXT ITEM
      u.level( v.level() + 1 );
      
      // iterate to the proper member of vpw
      vit = vpw.begin();
      for( int j=0; j < u.level() ; j++ )
        vit++ ;
      
      // keep track of all items in this node
      u.name( v.name() );
      u.addname( "&" );
      u.addname( (*vit).name() );
      
      u.weight( v.weight() + (*vit).weight() ); 
      u.profit( v.profit() + (*vit).profit() );
     
#if KNAPSACK_BFS_DEBUG > 0
      cout << endl << "BFS( u ): " ;
      u.display( cout );
#endif
      // check the total profit
      if( u.weight() <=  W  &&  u.profit() > maxprofit )
      {
        maxprofit = u.profit() ;
	tw = u.weight() ;
	cout << endl << "[31mBFS(" << i << "): maxprofit now == " << maxprofit << "[0m" << endl ;
	cout << "[31m\t current best items are " << u.name() << "[0m" << endl ;
	cout << "[31m\t current weight of items is " << tw << "[0m" << endl ;

        // keep track of overall list of best items
        best.assign( u.name() );
      }
      
      // see if this node is promising
      u.bound( bound(u, vpw, n, W) );
      if( u.bound() > maxprofit )
        sst.push( Knapnode(u) );


      //! SET u TO THE CHILD THAT DOES NOT INCLUDE THE NEXT ITEM

      // keep track of all items in this node
      u.name( v.name() );
      u.addname( "_" );
      
      // we already incremented the level in the previous section
      u.weight( v.weight() ); 
      u.profit( v.profit() );
      
      u.bound( bound(u, vpw, n, W) );
      if( u.bound() > maxprofit ) // if this node is still promising
        sst.push( Knapnode(u) );
    }
    
    i++ ;
#if KNAPSACK_BFS_DEBUG > 0
    cout << endl << "[1;34m i == " << i << "[0m" << endl << endl ;
#endif
    
  }// while( !sst.empty()  &&  i < limit )
  
#if KNAPSACK_BFS_DEBUG == 0
  cout << endl << "[1;34m Final i == " << i << "[0m" << endl ;
#endif

  return maxprofit ;

}//! bestFirstSearch()
