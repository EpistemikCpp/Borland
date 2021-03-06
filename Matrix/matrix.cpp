// FILE:  matrix.cpp
// NAME:  Mark Sattolo
#include <iostream.h>
#include "matrix.h"
int min(const int *a, int l)
  {
	if ( l == 1 )  return *a ; // one element: return the value
	else if ( l > 1 )		// more than one element
			{
			int temp = *a ;		 // store the value of the first element
			int temp_min = min( ++a,  l - 1 ) ; // recursive call to find the min of the tail of the array
			return temp_min < temp ? temp_min : temp ; // compare head & tail to get overall min
			}
		else if ( l == 0 ) throw m1_exception() ;
	return -1 ; 	// should never reach here
  }
	/* min()
		calculates the minimum of array a which is of length l.  if l is zero, an ass1_exception
		is thrown.  You may use any kind of loop or recursion! */
int max(const int *a, int l)
  {
	if ( l == 1 )  return *a ; // one element: return the value
	else if ( l > 1 )	// more than one element
			{
			int temp = *a ;		 // store the value of the first element
			int temp_max = max( ++a,  l - 1 ) ; // recursive call to find the max of the tail of the array
			return temp_max > temp ? temp_max : temp ; // compare head & tail to get overall max
			}
		else if ( l == 0 ) throw m1_exception() ;
	return -1 ;	 // should never reach here
  }
	/* max()
		calculates the maximum of array a which is of length l.  if l is zero, an ass1_exception
		is thrown.  max must be recursive! */
int* vector_alloc(int n)
  {
	int* temp = new int[n] ; // create a new dynamic array
	return temp ;			// return the pointer to the array
  }
	/* vector_alloc()
		returns the address of a dynamically allocated -- allocated by new[] -- vector which
		contains n integers	(do not initialize them!).  this function is really just a wrapper
		for the new[] operator. */
void vector_dealloc(int*& v)
  {
	delete [] v ; 	// delete all the vector elements
	v = 0 ;			// pass v by reference so v itself gets 0 here and not a copy of v.
  }
	/* vector_dealloc()
		deallocates vector v using delete [] and sets v to 0.
		Also answer in a comment line of your implementation why v is passed by reference! */
int** matrix_alloc(int n, int m)
  {
	int** temp = new int* [n] ;		 // create an array of rows
	for ( int i = 0 ; i < n ; i++ )
		temp[i] = vector_alloc( m ) ;	 // allocate an array of columns for each row
	return temp ; 		// return the pointer to the new matrix
  }
	/* matrix_alloc()
		allocates an n x m matrix dynamically and returns its address.
		matrix_alloc must use vector_alloc to allocate the n rows! */
void matrix_dealloc(int**& a, int n)
  {
	for ( int i = 0 ; i < n ; i++ ) 		// use vector_dealloc on each row
		vector_dealloc( a[i] ) ;
		// no need to specify the columns as delete [] will automatically delete all columns
	a = 0 ;
  }
	/* matrix_dealloc()
		deallocates matrix a which has n rows and sets a to 0.  Also answer in a comment
		line of your implementation why there is no need for the number of columns! */
int* vector_copy(const int* v, int l)
  {
	int* temp = vector_alloc( l ) ; // create a new array with the same # of elements as the target
	for ( int i = 0 ; i < l ; i++ )
		temp[i] = v[i] ; 		// copy the target values into the new array
	return temp ;			 // return the pointer to the copy array
  }
	/* vector_copy()
		creates a carbon copy of vector v.  First it allocates a new vector of length l using
		vector_alloc and then it copies the elements.  The address of this new vector is returned. */
int** matrix_copy(const int* const * M, int n, int m)
  {
	int** temp = matrix_alloc( n, m ) ; 	// create a new matrix the same size as the target matrix
	for ( int i = 0 ; i < n ; i++ ) 			// for each row
		for ( int j = 0 ; j < m ; j++ )			 // for each column in each row
			temp[i][j] = M[i][j] ; 			// copy the target values into the new matrix
	return temp ; 			// return the pointer to the new matrix
  }
	/* matrix_copy
		creates a carbon copy of matrix M which has n rows and m columns.
		The new matrix is dynamically allocated and the elements of M are copied.
		The address of this new matrix is returned.
		note:
						  const int * const *
							 |     			  |
							 |           		  |
				this const is         this const is
				for the elements   for each row
				of the rows								*/
void vector_print(ostream& os, const int* v, int l)
  {
	os << "{ " ; 	// initial brace
	for ( int i = 1 ; i <= l ; i++ )		// loop for each element
	  {
		os << *v ; 			// print the current value of v
		if ( i < l ) os << ", " ;	 // print a comma if not at the last value
		v++ ; 				// increment the pointer
	  }
	os << " }" ; 	// closing brace
  }
	/* vector_print()
		prints vector v on the output stream os. v is of length l. The vector is enclosed
		in a pair of curly braces ({ and }) and the elements are comma separated.
		for example:	{1,2,3,4,5}		*/
void matrix_print(ostream& os, const int * const * M, int n, int m)
  {
	os << " {" << endl ; 	// opening brace
	for ( int i = 1 ; i <= n ; i++ ) 		// for each vector in the matrix
	  {
		os << "\t" ; 		// tab over
		vector_print( os, *M, m ) ; 	// print the vector
		if ( i < n ) os << ", " ; 		// print a comma if not at the last vector
		os << endl ;
		*M++ ; 		// increment the pointer
	  }
	os << " }" << endl ; 	// closing brace
  }
	/* matrix_print
		prints matrix M which has n rows and m columns on the output stream os.
		The matrix is enclosed in a pair of curly braces and the rows are comma separated.
		The rows themselves are printed by vector_print.
		for example:	{
										  {1,2,3,4},
										  {3,2,0,1},
										  {5,6,0,2}
										}
Part C [ 20 marks ]
	  for this part you will implement four functions which are a bit more interesting.	*/
//[ 8 marks ]
void quick_sort(int* v, int l, int h)
  {
	if ( l < h ) 		// base case: do nothing if the lower index is >= the upper index
	  {
		int i = l ; int j = h ; 	// i and j start at l and h respectively and move through the array
		int Pivot = *( v + ( (i + j) / 2 ) ) ;	// set Pivot to middle value
		do {
			 while ( v[i] < Pivot )  i++ ;		// move i forward until >= Pivot
			 while ( v[j] > Pivot )  j-- ;		// move j backward until <= Pivot
			 if ( i <= j ) 		// if they haven't crossed
				{
				 int temp = v[i]; v[i] = v[j]; v[j] = temp ; 	// swap v[i] & v[j]
				 i++ ;  j-- ;		// update indices by one just in case they are both at the middle element
				}
			} while (i <= j) ;		// continue until i and j cross
		// sort the two halves recursively
		quick_sort( v, l, j ) ; 	//lower half of the original array
		quick_sort( v, i, h ) ; 	// upper half of the original array
	  }
  }
	/* quick_sort()
		the well known quick sort algorithm partitions the vector v into	two parts:
		one where the elements are smaller and one where the elements are bigger or equal
		to a pivot value chosen from the vector.  It is often convenient to select the first or the last
		element as pivot.  Then these two parts are sorted recursively.  See the pseudo code and
		a trace!  Lab 7 also has a generic version of quick_sort already typed in (notes page 169) */
//[ 6 marks ]
int* apply(int** M, int n, int m, int (*f) (const int*, int))
  {
	int* result = vector_alloc( n ) ; 	// create a new vector
	for ( int i = 0 ; i < n ; i++ ) 		// for each row of target matrix M
		result[i] = f( M[i], m ) ; // each element of the new vector gets the result of parameter function f
	return result ; 			// return the pointer to the new vector
  }
	/* apply()
		M is an n x m matrix of integers and f is a function which returns an int and takes a
		vector and its length as arguments.  apply	simply executes f on each end every row of M
		and records the results in a dynamically allocated vector of length n and returns its address. */
//[ 4 marks ]
int matrix_max(const int* const* M, int n, int m)
  {
	int** M_temp = const_cast<int**>( M ) ; 		// have to cast M to an int** to pass to 'apply'
	int* result = vector_alloc( n ) ; 			// create a vector to hold the result of 'apply'
	result = apply( M_temp, n, m, max ) ; 		// 'apply' calls 'max' on each row of M_temp
	int temp = max( result, n ) ; 		// temp holds the value of calling 'max' on 'result'
	vector_dealloc(result) ; 		// deallocate vector 'result'
	return temp ; 			// return int 'temp'
  }
	/* matrix_max()
		M is an n x m matrix of integers.  matrix_max returns the maximum element of M,
		this function must make use of functions apply and max! */
//[ 2 marks ]
void matrix_sort(int** M, int n, int m)
  {
	for ( int i = 0 ; i < n ; i++ ) 		// for each row of M apply quick_sort
		quick_sort( M[i], 0, m-1 ) ;
  }
	/* matrix_sort()
		M is an n x m matrix of integers.
		this function simply calls quick_sort on every row of M. */
