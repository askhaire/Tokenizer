///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Aishwarya Khaire, Syracuse University                //
//              askhaire@syr.edu                                     //
///////////////////////////////////////////////////////////////////////


#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

bool SemiExp::get(bool clear)
{
 
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	_tokens.clear();
	while (clear) {
		std::string token = _pToker->getTok();
		if (token == "")
			break;
		_tokens.push_back(token);
		
		trimFront();
		if (token == "{" || token == "}" || token == ";")
		{
			if (token == ";" && (std::find(_tokens.begin(), _tokens.end(), "for") != _tokens.end()))//test for folding of for()
			{
				do {
					token = _pToker->getTok();
					_tokens.push_back(token);
				} while (token != "{" || token == "}");

			}
			return true;
		}
		if (token == "public" || token == "private" || token == "protected")
		{
			token = _pToker->getTok();
			if (token == ":")
				_tokens.push_back(token);
			return true;
		}
		if (token == "\n")      //test for #include
		{
			if (std::find(_tokens.begin(), _tokens.end(), "#") != _tokens.end())
				return true;
		}
		
	}
	return false;
}

Token SemiExp::operator[](size_t n)
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

size_t SemiExp::find(const std::string& token)
{
	for (size_t i = 0; i < length(); i++)
		if (token == _tokens[i])
			return i;
	return length();
}
bool SemiExp::remove(const std::string& token)
{
	std::vector<std::string>::iterator i;
	i = std::find(_tokens.begin(), _tokens.end(), token);
	if (i != _tokens.end()) {

		_tokens.erase(i);
		return true;
	}
	return false;
}
bool SemiExp::remove(size_t i)
{
	if (i < 0 || length() < i)
		return false;
	_tokens.erase(_tokens.begin() + i);
	return true;
}
size_t SemiExp::length()
{
	return _tokens.size();
}

std::string SemiExp::show()
{
	std::cout << "\n  ";
	for (auto token : _tokens)
		if (token != "\n")
			std::cout << token << " ";
	std::cout << "\n";
	return _tokens[0];
}
bool SemiExp::merge(const std::string& firstTok, const std::string& secondTok)
{
	return true;
}
void SemiExp::trimFront()
{
	while (_tokens.size() > 0 && (_tokens[0] == "\n" || _tokens[0] == ""))
		remove(0);
}
void SemiExp::toLower()
{
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		std::string temp = _tokens[i];
		int j = 0;
		while (temp[j])
		{
			temp[j] = tolower(temp[j]); j++;
		}
		_tokens[i] = temp;
	}

}
void SemiExp::clear()
{
	_tokens.clear();
}
void SemiExp::push_back(const std::string& tok)
{
	_tokens.push_back(tok);
}
#ifdef TEST_SEMI

int main()
{
	Toker toker;
	std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);
	SemiExp semi(&toker);
	while (semi.get())
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
		std::cout << "\n\n";
	}
	return 0;
}
#endif

