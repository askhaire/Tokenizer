#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Aishwarya Khaire, Syracuse University                //
//              askhaire@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This is a new version, based on the State Design Pattern.  Older versions
 * exist, based on an informal state machine design.
 *
 Public Interface:
 =================
 Toker t;                        // create tokenizer instance
 t.returnComments();             // request comments return as tokens
 if(t.attach(someFileName))      // select file for tokenizing
 string tok = t.getTok();        // extract first token
 t.setSpecialSingleChar("*")     //Set "*" as special single character token
 t.setSpecialDoubleChar("*")     //Set "**" as special double character token
 t.dontreturnComments();         // request commentsnot be returned as tokens
 t.reset();                      //Facility to create more than one object of Toker
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv Tokenizer.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 :09 Feb 2016
 * First Release
 */
#include <iosfwd>
#include<vector>
#include <string>

namespace Scanner
{
  class ConsumeState;

  class Toker
  {
  public:
    Toker();
    Toker(const Toker&)=delete;
    ~Toker();
    Toker& operator=(const Toker&) = delete;
    bool attach(std::istream* pIn);
    std::string getTok();
    bool canRead();
	void setSpecialSingleChar(std::string singleCharTokens);
	void setSpecialDoubleChar(std::string doubleCharTokens);
	void returnComments();
	void dontreturnComments();
	void reset();
  private:
    ConsumeState* pConsumer;

  };
}
#endif