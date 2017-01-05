#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Aishwarya Khaire, Syracuse University                //
//              askhaire@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a 
* sequence of tokens that ends in {, }, ;, and '\n' if the line begins with #

Public Interface:
=================
Toker t;                                    // create tokenizer instance
SemiExp semi(&t);                           // create instance and attach
if(semi.get())                              // collect a semiExpression
int n = semi.length();                      // number of tokens in se
std::string tok = semi[2] = "a change";     // indexed read and write
if(semi.find("aTok") < semi.length())       // search for a token
std::cout << "found aTok";
semi.clear();                               // remove all tokens
*
* Build Process:
* --------------
* Required Files: 
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
* 
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0
* First Release
*/

#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "iTokCollection.h"

namespace Scanner
{
  using Token = std::string;

  class SemiExp:public ITokCollection
  {
  public:
    SemiExp(Toker* pToker=nullptr);
    SemiExp(const SemiExp&)=delete;
    SemiExp& operator=(const SemiExp&) = delete;
	bool get(bool clear=true);
    Token operator[](size_t n);
	size_t find(const std::string& token);
	bool remove(const std::string& token);
	//bool getFor(bool clear=true);
	bool remove(size_t i);
	bool merge(const std::string& firstTok, const std::string& secondTok);
	void trimFront();
	void toLower();
	void clear();
	void push_back(const std::string& tok);
    size_t length();
	std::string show();
  private:
    std::vector<Token> _tokens;
    Toker* _pToker;
  };
}
#endif
