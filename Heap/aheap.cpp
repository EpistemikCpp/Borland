// FILE: aheap.cpp
// NAME: Mark Sattolo
/*
#ifdef __GNUG__
#pragma implementation
#endif
*/
#include "aheap.hpp"

/****************************
					A_NODE METHODS
		 *******************************/

// constructor: a_node's ind variable will record each node's position in the array
template<class data_type>
array_heap<data_type>::a_node::a_node( const data_type& e, heap<data_type>::compare_fxn& f,
													             heap<data_type>::order& o, int index  )
												       : heap<data_type>::handle(f, o, e)
{ ind = index ; }

// assignment overload: must copy the ind variable
template<class data_type>
array_heap<data_type>::a_node&  array_heap<data_type>::a_node::operator=( const a_node& a )
{
	ind = a.ind ;
	return *this ;
}


/********************************
				  ARRAY_HEAP METHODS
		 **********************************/

// constructor: create the array and store its size
template<class data_type>
array_heap<data_type>::array_heap( compare_fxn f, order o, int c ) : heap<data_type>(f, o)
{
	array = new a_node*[c] ;
	max_size = c ;
}

// copy constructor: create a new copy of the each array element
template<class data_type>
array_heap<data_type>::array_heap( const array_heap<data_type>& H ) : heap<data_type>(H)
{
	array = new a_node*[H.max_size] ;
	for( int i = 0 ; i < H.size() ; i++ )
	  push(**H.array[i]) ;
	max_size = H.max_size ;
}

// assignment overload: destroy the old array and copy the newly-assigned one
template<class data_type>
array_heap<data_type>& array_heap<data_type>::operator=( const array_heap<data_type>& H )
{
	heap<data_type>::operator=( H ) ;

	for( int i = 0 ; i < size() ; i++ )
	  delete array[i] ;
	delete [] array ;

	array = new a_node*[H.max_size] ;
	for( int j = 0 ; j < H.size() ; j++ )
	  push( **H.array[j] ) ;

	max_size = H.max_size ;
	return *this;
}

// destructor: have to delete the array elements as they were dynamically allocated
template<class data_type>
array_heap<data_type>::~array_heap()
{
	for( int i = 0 ; i < size() ; i++ )
	  delete array[i] ;
	delete [] array ;
}

// swap(): swap the positions of two a_nodes -- used in sift_up() and sift_down()
template<class data_type>
void array_heap<data_type>::swap( heap<data_type>::handle& h1, heap<data_type>::handle& h2 )
{
	h1.swap( h2 ) ;
	a_node& a1 = static_cast<a_node&>( h1 ) ;
	a_node& a2 = static_cast<a_node&>( h2 ) ;

	a_node temp = *array[a1.ind] ;
	*array[a1.ind] = *array[a2.ind] ;
	*array[a2.ind] = temp ;

	for( int i=0; i < size(); i++ )
	  (*array[i]).ind = i ;
}

// left(): find the array index of the left child
template<class data_type>
int array_heap<data_type>::left( int a ) const
{
	if( empty() )
    throw exception() ;
	return (a+1)*2 - 1 ;
}

// right(): find the arry index of the right child
template<class data_type>
int array_heap<data_type>::right( int a ) const
{
	if( empty() )
    throw exception() ;
	return (a+1)*2 ;
}

// parent(): find the array index of the parent node
template<class data_type>
int array_heap<data_type>::parent( int a ) const
{
	if( empty() )
    throw exception() ;
	return (a+1)/2 - 1 ;
}

// sift_up: move child node up the array if it is higher priority than its parent
template<class data_type>
void array_heap<data_type>::sift_up( handle& h )
{
	a_node& a = static_cast<a_node&>( h ) ;
	int son = a.ind ;
	while( son != 0 )
	{
		int dad = parent( son ) ;
		if( (*array[son]).higher_priority(*array[dad]) )
		{
			swap( *array[son], *array[dad] ) ;
			son = dad ;
		}
		else break ;
	}
}

/* sift_down(): continually move a node down if it is lower priority
					 than its highest priority child */
template<class data_type>
void array_heap<data_type>::sift_down( handle& h )
{
	a_node& a = dynamic_cast<a_node&>( h );
	int upper = a.ind ;
	while( true )
  {
		if( left(upper) > last() )
		  // no children
		  break ;
		int hpchild ;
		if( right(upper) > last() )
		  // if no right then highest priority is left child
		  hpchild = left( upper ) ;
		else
			// find highest priority child
			hpchild = ( *array[left(upper)] ).higher_priority( *array[right(upper)] )
						    ? left(upper) : right(upper) ;

		// swap node and child if child is higher priority
		if( ( *array[hpchild] ).higher_priority( *array[upper] ) )
		{
			swap( *array[hpchild], *array[upper] ) ;
			upper = hpchild ;
		}
		else break;
	} // while
}

// create_new(): used in heap::push()
template<class data_type>
heap<data_type>::handle&  array_heap<data_type>::create_new( const data_type& e )
{
	if( size() >= max_size )
	  throw exception() ;
  
	array[size()] = new a_node( e, compare, ordering, size() ) ;
	return *array[size()] ;
}

// first(): reference the first element
template<class data_type>
heap<data_type>::handle&  array_heap<data_type>::first()
{
	if( empty() )
	  throw exception() ;
	return *array[0] ;
}

// last(): index of the current last element
template<class data_type>
int array_heap<data_type>::last() const
{
	if( empty() || size() > max_size )
	  throw exception() ;
	return size()-1 ;
}

// move_last_to_first(): used in heap::pop()
template<class data_type>
void array_heap<data_type>::move_last_to_first()
{ swap( *array[0], *array[last()] ) ; }

// delete_last(): ONLY used in heap::pop()
template<class data_type>
void array_heap<data_type>::delete_last()
{
	if( empty() )
    throw exception() ;
	delete array[last()] ;
}

// index(): used in heap::priority_change()
template<class data_type>
heap<data_type>::handle&  array_heap<data_type>::index( const heap<data_type>::handle& h ) const
{
	const a_node& a = dynamic_cast<const a_node&>( h );
	return *array[a.ind] ;
}

// top(): get the value at the top of the array
template<class data_type>
const data_type& array_heap<data_type>::top() const
{
	if( empty() )
    throw exception() ;
	return **array[0] ;
}

// print(): recursively print a node and all its sub-nodes
template<class data_type>
void array_heap<data_type>::print( ostream& os, const array_heap<data_type>::a_node* n, int k = 0 ) const
{
	for( int i=0; i < k; ++i )
	  os << "   " ;

	os << **n << "   ( this=" << (void*)n << ", l=" << array[left( (*n).ind )]
		 << ", r=" << array[right( (*n).ind )] << ", p=" << array[parent( (*n).ind )] << ")" <<  endl ;

	if( left( (*n).ind ) <= last() )
	  print( os, array[left( (*n).ind )], k+1 ) ;
	else
	{
		for( int i=0; i < k+1; ++i )
		  os << "   " ;
		os << "."  << endl ;
	}

	if( right( (*n).ind ) <= last() )
	  print( os, array[right( (*n).ind )], k+1 ) ;
	else
	{
		for( int i=0; i < k+1; ++i )
		  os << "   " ;
		os << "." << endl ;
	}
}

// print(): print the first and last node addresses then print the array
template<class data_type>
void array_heap<data_type>::print( ostream& os ) const
{
	os << "First = " << array[0] << "   Last = " << array[last()] << endl ;
	print( os, array[0] ) ;
}
