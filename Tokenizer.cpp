/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Aishwarya Khaire                                   //
//              askhaire@syr.edu                                   //
/////////////////////////////////////////////////////////////////////
#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include<conio.h>
#include<vector>
namespace Scanner
{
	class ConsumeState
	{
	public:

		ConsumeState();
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		static std::vector<std::string> doubleCharTokens;
		static std::vector<std::string> singleCharTokens;
		static bool choice;
		static bool firstcon;
		static bool firstdes;
        ConsumeState* nextState();
		protected:
		static bool isSpecialSingleChar(int current, std::vector<std::string> singleCharTokens);
		static bool isSpecialDoubleChar(int current, std::vector<std::string> doubleCharTokens);
		static std::string token;
		static std::istream* _pIn;
		static int prevChar;
		static int currChar;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatQuote;
		static ConsumeState* _pEatSpecialChar;

	};
	std::vector<std::string> ConsumeState::doubleCharTokens = { ":","<",">","+","-","*","/",";","=","&","|" };
	std::vector<std::string> ConsumeState::singleCharTokens = { "[","]","{","}","(",")","<",">",":",";","!","+","-","=","/","*","\n","~","&",".","," };
	std::string ConsumeState::token;
	std::istream* ConsumeState::_pIn=nullptr;
	int ConsumeState::prevChar;
	int ConsumeState::currChar;
	bool ConsumeState::choice = false;
	bool ConsumeState::firstcon = true;
	bool ConsumeState::firstdes = true;
	ConsumeState* ConsumeState::_pState = nullptr;
	ConsumeState* ConsumeState::_pEatCppComment = nullptr;
	ConsumeState* ConsumeState::_pEatCComment = nullptr;
	ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
	ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
	ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
	ConsumeState* ConsumeState::_pEatQuote = nullptr;
	ConsumeState* ConsumeState::_pEatSpecialChar = nullptr;
}
using namespace Scanner;
	void testLog(const std::string& msg);

	ConsumeState* ConsumeState::nextState()
	{
		if (!(_pIn->good()))
		{
			return nullptr;
		}
		int chNext = _pIn->peek();
		if (std::isspace(currChar) && currChar != '\n')
		{
			testLog("state: eatWhitespace");
			return _pEatWhitespace;
		}
		if (currChar == '/' && chNext == '/')
		{
			testLog("state: eatCppComment");
			return _pEatCppComment;
		}
		if (currChar == '/' && chNext == '*')
		{
			testLog("state: eatCComment");
			return _pEatCComment;
		}

		if (std::isalnum(currChar) || currChar == '_')
		{
			testLog("state: eatAlphanum");
			return _pEatAlphanum;
		}
		if (isSpecialDoubleChar(currChar, doubleCharTokens) || isSpecialSingleChar(currChar, singleCharTokens))
		{
			testLog("state: eatSpecialChar");
			return _pEatSpecialChar;
		}
		if (!(isSpecialDoubleChar(currChar, doubleCharTokens)) && !(isSpecialSingleChar(currChar, singleCharTokens)) && currChar != '"')
		{
			testLog("state: eatPunctuator");
			return _pEatPunctuator;
		}
		if (currChar == '"')
		{
			testLog("state: eatQuote");
			return _pEatQuote;
		}
		if (!_pIn->good())
			return _pEatWhitespace;
		throw(std::logic_error("invalid type"));
	}

	class EatWhitespace : public ConsumeState
	{
	public:
		virtual void eatChars()
		{
			token.clear();
			do {
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();
			} while (std::isspace(currChar) && currChar != '\n');
		}
	};

	class EatCppComment : public ConsumeState
	{
	public:
		virtual void eatChars()
		{
			
			token.clear();
			do {
				if (choice)
					token += currChar;
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();
			} while (currChar != '\n');
		}
	};


	class EatCComment : public ConsumeState
	{
	public:
		virtual void eatChars()
		{
			token.clear();
			if (choice)
			{
				do {

					token += currChar;
					if (!_pIn->good())  // end of stream
						return;
					currChar = _pIn->get();
				} while (currChar != '*' || _pIn->peek() != '/');
				token += currChar;
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
			}
			else
			{
				do {
					currChar = _pIn->get();
				} while (currChar != '*' || _pIn->peek() != '/');
				currChar = _pIn->get();
				currChar = _pIn->get();
			}
		}
	};



	class EatPunctuator : public ConsumeState
	{
	public:

		virtual void eatChars()
		{
			bool found = false;
			token.clear();
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			if (currChar == '\''&&_pIn->peek() == '"')
			{
				currChar = _pIn->get();
				token += currChar;
			}
			currChar = _pIn->get();

		}
	};


	class EatAlphanum : public ConsumeState
	{
	public:
		virtual void eatChars()
		{
			token.clear();
			do {
				token += currChar;
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();
			} while (isalnum(currChar));
		}
	};

	class EatQuote :public ConsumeState
	{
	public:
		virtual void eatChars()
		{
			token.clear();
			do {
				token += currChar;
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();
				if (currChar == '\\' && (_pIn->peek() == '"' || _pIn->peek() == '\''))
				{
					currChar = _pIn->get();
					token += currChar;
					currChar = _pIn->get();
				}


			} while (currChar != '"');
			token += currChar;
			currChar = _pIn->get();
		}
	};

	class EatSpecialChar : public ConsumeState
	{
	public:

		virtual void eatChars()
		{
			bool found = false;
			token.clear();
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;

			found = isSpecialDoubleChar(currChar, doubleCharTokens);
			if (found)
			{
				token += currChar;
				currChar = _pIn->get();
				currChar = _pIn->get();
			}

			else
				currChar = _pIn->get();
		}
	};

	ConsumeState::ConsumeState()
	{
		if (firstcon)
		{
			firstcon = false;
			_pEatAlphanum = new EatAlphanum();
			_pEatCComment = new EatCComment();
			_pEatCppComment = new EatCppComment();
			_pEatPunctuator = new EatPunctuator();
			_pEatWhitespace = new EatWhitespace();
			_pEatQuote = new EatQuote();
			_pEatSpecialChar = new EatSpecialChar();
			_pState = _pEatWhitespace;
		}
	}

	ConsumeState::~ConsumeState()
	{
		if (firstdes)
		{
			firstdes = false;
			delete _pEatAlphanum;
			delete _pEatCComment;
			delete _pEatCppComment;
			delete _pEatPunctuator;
			delete _pEatWhitespace;
			delete _pEatQuote;
			delete _pEatSpecialChar;
		}
	}
	bool ConsumeState::isSpecialDoubleChar(int currChar, std::vector<std::string> doubleCharTokens)
	{
		std::string tok = std::string(1, currChar);
		for (size_t i = 0; i < doubleCharTokens.size(); i++)
		{
			if (tok == doubleCharTokens[i])
			{
				if (_pIn->peek() == currChar || _pIn->peek() == '=')//check if token qualifies for special double character
				{
					return true;
				}
			}
		}return false;
	}
	void Toker::reset()
	{
		pConsumer->firstcon = true;
		pConsumer->firstdes = true;
	}
	
	bool ConsumeState::isSpecialSingleChar(int currChar, std::vector<std::string> singleCharTokens)
	{
		std::string tok = std::string(1, currChar);
		for (size_t i = 0; i < singleCharTokens.size(); i++)
		{
			if (tok == singleCharTokens[i])
			{

				return true;
			}
		}
		return false;
	}

	void Toker::setSpecialDoubleChar(std::string sdc)
	{
		if (!(std::find(pConsumer->doubleCharTokens.begin(), pConsumer->doubleCharTokens.end(), sdc) != pConsumer->doubleCharTokens.end()))
		pConsumer->doubleCharTokens.push_back(sdc);//add new token to existing list
	}
	void Toker::setSpecialSingleChar(std::string ssc)
	{
		if (!(std::find(pConsumer->singleCharTokens.begin(), pConsumer->singleCharTokens.end(), ssc) != pConsumer->singleCharTokens.end()))
		pConsumer->singleCharTokens.push_back(ssc);
	}
	void Toker::returnComments()
	{
		pConsumer->choice = true;
	}
	void Toker::dontreturnComments()
	{
		pConsumer->choice = false;
	}
	Toker::Toker() : pConsumer(new EatWhitespace()) {}

	Toker::~Toker() { delete pConsumer; }

	bool Toker::attach(std::istream* pIn)
	{
		if (pIn != nullptr && pIn->good())
		{
			pConsumer->attach(pIn);
			return true;
		}
		return false;
	}

	std::string Toker::getTok()
	{
		while (true)
		{
			if (!pConsumer->canRead())
				return "";
			pConsumer->consumeChars();
			if (pConsumer->hasTok())
				break;
			
		}
		return pConsumer->getTok();
	}

	bool Toker::canRead() { return pConsumer->canRead(); }

	void testLog(const std::string& msg)
	{
#ifdef TEST_LOG
		std::cout << "\n  " << msg;
#endif
	}

	/*----< test stub >--------------------------------------------------*/

#ifdef TEST_TOKENIZER

#include <fstream>

	int main()
	{
		std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
		/*std::string fileSpec = "../Tokenizer/Tokenizer.h";*/
		//std::string fileSpec = "../Tokenizer/Test.txt";

		std::ifstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  can't open " << fileSpec << "\n\n";
			return 1;
		}
		Toker toker;
		toker.attach(&in);
		while (in.good())
		{
			std::string tok = toker.getTok();
			if (tok == "\n")
				tok = "newline";
			std::cout << "\n -- " << tok;
		}
		std::cout << "\n\n\"asd";
		_getch();
		return 0;
	}
#endif
