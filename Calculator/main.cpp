// File: main.cpp// Mark Sattolo#include "expression.h"#include "parser.h"#include <iostream.h>#include "me.h"       // New#include "unary.h"  // New#include "binary.h"  // New#include "function.h"  // New int main()  {	me(cout) ;	parser P ; // A parser	P.add_unary_instantiator("+",instantiate_unary_plus);    // NEW	P.add_unary_instantiator("-",instantiate_unary_minus);    // NEW	P.add_binary_instantiator("+",5,instantiate_addition);    // NEW	P.add_binary_instantiator("-",5,instantiate_subtraction);   // NEW	P.add_binary_instantiator("*",10,instantiate_multiplication);    // NEW	P.add_binary_instantiator("/",10,instantiate_division);   // NEW	P.add_binary_instantiator("^",20,instantiate_power);    // NEW	P.add_function_instantiator("sin",1,instantiate_sine);  // NEW	P.add_function_instantiator("cos",1,instantiate_cosine);  // NEW	P.add_function_instantiator("tan",1,instantiate_tangent);  // NEW	P.add_function_instantiator("_e",0,instantiate_num_e);  // NEW	P.add_function_instantiator("log",2,instantiate_natlog);  // NEW	context C ; // A context	cout << "\n Enter expressions to be calculated, ending with ';' \n" ;	cout << " You can use:  +  -  *  /  ^  _e() sin(x) cos(x) tan(x) log(x,base)\n" ;	cout << " Enter 'exit' to quit. \n\n" ;	bool done = false ;	while(!done)	 {	  try		  {			expression *e = 0;			const char* t = P.peek_token(cin);			if (t == 0)			  {				cerr << "unexpected end of file" << endl;				return 1;			  }			if (strcmp(t,"exit") == 0)			  { done = true; }			else			  {				e = P.read(cin); // READ EXPRESSION				t = P.peek_token(cin);				if (t == 0)				  {					cerr << "unexpected end of file" << endl;					return 1;				  }				if (strcmp(t,";") != 0)				  {					cerr << ";" << " expected instead of \" " << t << "\"" << endl;					continue;				  }			  } // else			P.consume_token(cin);			if (e != 0)			  {				// PRINT AND EVALUATE EXPRESSION				cout << *e << " == " << e->evaluate(C) << endl;			  }			} // try	  catch (parser::exception& ex)		 {		  // probably recoverable: start reading next expression to recover		  cerr << "parser::exception: " << ex << endl;		  continue;		 }	  catch (lexer::exception& ex)		 {		  // probably irrecoverable		  cerr << "lexer::exception: " << ex << endl;		  return 1;		 }	  catch (context::exception& ex)		 {		  // probably recoverable: start reading next expression to recover		  cerr << "context::exception: " << ex << endl;		 }	  catch (...)		 {		  cout << "irrecoverable error" << endl;		  return 1;		 }	} // while  return 0; }