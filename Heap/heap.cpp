// heap.cpp

#if defined(__GNUG__)
#pragma implementation
#endif

#include "heap.h"

/*************************
				HANDLE METHODS
				*******************************/

// used to generate the unique ids - initialize the static variable
template<class data_type>
long heap<data_type>::handle::last_id = 0 ;


// CONSTRUCTOR
// set the parameter values via the initializer and increment the id variables
template<class data_type>
heap<data_type>::handle::handle(heap<data_type>::compare_fxn& f, heap<data_type>::order& o, const data_type& e)
								:handle_comparison(f), handle_ordering(o), elem(e)
  { id = ++last_id ; }


// HIGHER_PRIORITY() - returns true if 'this' is higher priority than h
template<class data_type>
bool heap<data_type>::handle::higher_priority(const heap<data_type>::handle& h)
  {
	if (handle_ordering == heap<data_type>::SMALLER_FIRST)
	  {
		if (handle_comparison(h.elem, elem)) return false;
		if (handle_comparison(elem, h.elem)) return true;
		return id < h.id;
	  }
	else if (handle_ordering == heap<data_type>::LARGER_FIRST)
	  {
		if (handle_comparison(elem, h.elem)) return false;
		if (handle_comparison(h.elem, elem)) return true;
		return id > h.id;
	  }
	  else throw exception();
  } // handle::higher_priority()


// SWAP() - swaps 'this' and handle
// IMPORTANT: id is a private instance variable, hence subclasses of handle
// that overload this method should explicitly invoke this method like
//    handle::swap(h)
// and do not forget that elem is swapped here!
template<class data_type>
void heap<data_type>::handle::swap(heap<data_type>::handle& h)
  {
	long temp_i = id;
	id = h.id;
	h.id = temp_i;

	data_type temp_e = elem;
	elem = h.elem;
	h.elem = temp_e;
  } // handle::swap()


// OPERATOR*() - return a reference (alias) to the element (class data_type) held by handle
template<class data_type>
const data_type&  heap<data_type>::handle::operator*() const
  { return elem ; }


/************************
				 HEAP METHODS
				 *****************************/

// CONSTRUCTOR
template<class data_type>
heap<data_type>::heap(heap<data_type>::compare_fxn f, heap<data_type>::order o)
  {
	compare = f ;
	number_of_elements = 0 ;
	ordering = o ;
  }


// virtual destructor because of polymorphism
template<class data_type>
heap<data_type>::~heap()
  {  }


// PUSH()
// most probably does not have to be overloaded in the subclass
// IMPORTANT: increments the number of elements so create_new() shouldn't !!!
template<class data_type>
heap<data_type>::handle& heap<data_type>::push(const data_type& ex)
  {
	handle& h = create_new(ex);
	++number_of_elements;
	sift_up(h);
	return h;
  } // push()


// PRIORITY_CHANGE()
// notify the heap that the element with handle h may have had its priority changed!
template<class data_type>
void heap<data_type>::priority_change(handle& h)
  {
	sift_up(index(h));   // does nothing if priority is lower
	sift_down(index(h)); // does nothing if priority is higher
  } // priority_change


// POP()
// most probably does not have to be overloaded in the subclass
// IMPORTANT: decrements the number of elements so delete_last() shouldn't !!!
template<class data_type>
void heap<data_type>::pop()
  {
	if( empty() )
	  throw exception();
	move_last_to_first();
	delete_last();
	--number_of_elements;
	if( !empty() )
	  sift_down(first());
  } // pop()


// EMPTY()
template<class data_type>
bool heap<data_type>::empty() const
  { return number_of_elements == 0 ; }


// SIZE() - return the number of elements
template<class data_type>
int heap<data_type>::size() const
  { return number_of_elements ; }

