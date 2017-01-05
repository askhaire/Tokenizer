#include"../SemiExp/SemiExp.h"
#include"../Tokenizer/Tokenizer.h"
#include<fstream>
#include<iostream>
namespace Scanner {
	class Test {
	public:
		Toker tok;
		int test1();
		int test2();
		void testSemi();
		void testfind();
	};
	int Test::test1()
	{
		std::cout << "\n\n-----------Requirement--------------\n\n";
		std::cout << "Tokenizer package that declares and defines a Toker class that implements the State Pattern with an abstract ConsumeState class and derived classes for collecting the following token types:\nalphanumeric tokens \npunctuator tokens \nspecial one and two character tokens with defaults that may be changed by calling setSpecialSingleChars(string ssc) and / or setSpecialCharPairs(string scp).\nC style comments returned as a single token.\nC++ style comments returned as a single token \nquoted strings\n\n";
		tok.returnComments();
		std::string fileSpec = "TestTokenizer.txt";
		std::cout << "\n--------------Input File----------------\n\n";
		std::cout << "\nTesting Tokenizer\n/*This is a C Comment*/\n//This is a Cpp comment\n\"this is a \"string\"\"\n@<<::[] ? ?//\"?\" set as special single character\n\n";
		std::ifstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  can't open " << fileSpec << "\n\n";
			return 1;
		}
		std::cout << "\n-------------Output Generated-------------\n\n";
		tok.attach(&in);
		tok.setSpecialSingleChar("?");
		while (in.good())
		{
			std::string toker = tok.getTok();
			if (toker == "\n")
				toker = "newline";
			std::cout << "\n -- " << toker;
		}
	}
	int Test::test2()
	{
		std::cout << "\n\n-----------Requirement--------------\n\n";
		std::cout << "\nTokenizer with option to return comments turned off and '??' set as special double character\n\n";
		std::ifstream in2("TestTokenizer.txt");
		std::cout << "\n-------------Output Generated-------------\n\n";
		tok.attach(&in2);
		if (!in2.good())
		{
			std::cout << "\n  can't open \n\n";
			return 1;
		}
		tok.setSpecialDoubleChar("?");
		tok.dontreturnComments();
		while (in2.good())
		{
			std::string toker = tok.getTok();
			if (toker == "\n")
				toker = "newline";
			std::cout << "\n -- " << toker;
		}
	}
	void Test::testSemi()
	{
		SemiExp sem1(&tok);
		std::cout << "\n\n-----------Requirement--------------\n\n";
		std::cout << "\n\n Testing SemiExpression\nterminate a token collection after extracting any of the single character tokens:\n semicolon, \nopen brace, \nclosed brace.\n Also on extracting newline if a \"#\" is the first character on that line. \nAlso, the extraction of a single character token consisting of \":\" if immediately preceeded by one of the tokens: \"public\", \"protected\", or \"private\".\n";
		std::cout << "\n-------------- Input File----------------\n\n";
		std::cout << "#include<iostream>\n#include<string>\nclass Testing\n{\npublic:\nint a, b, c;\nprotected:\n\n\nfor (int i = 0; i <= 9; i++)\n{\nstd::cout << \"Folding of for loop\";\n}\nHELLO_THERE;\n}\n\n";
		std::ifstream in3("TestSemiExpression.txt");
		tok.attach(&in3);
		std::cout << "\n-------------Output Generated-------------\n\n";

		while (sem1.get()) {
			std::cout << "\n  -- semiExpression --";
			sem1.show();
		}
		sem1.get();
		if (sem1.length() > 0)
		{
			std::cout << "\n  -- semiExpression --";
			sem1.show();
			std::cout << "\n\n";
		}
	}
	void Test::testfind()
	{
		tok.reset();
		SemiExp semi(&tok);
		std::cout << "\n\n------TESTING SEMIEXPRESSION FUNCTIONS------\n\n";
		std::cout << "\n-------------- Input File----------------\n\n";
		std::ifstream in3("semiFunctions.txt");
		tok.attach(&in3);
		semi.get();
		semi.show();
		std::cout << "\n\nTESTING FIND FUNCTION\n";
		std::cout << "\nContents of vector\n";
		semi.show();
		size_t loc = semi.find("2");
		std::cout << "Location of \"2\" is " << loc;
		std::cout << "\n\nTESTING REMOVE FUNCTION GIVEN TOKEN 4\n";
		std::cout << "\nContents of vector\n";
		semi.show();
		semi.remove("4");
		std::cout << "\nAfter Applying remove()\n";
		semi.show();
		std::cout << "\n\nTESTING REMOVE FUNCTION GIVEN LOCATION 2\n";
		std::cout << "\nContents of vector\n";
		semi.show();
		semi.remove(2);
		std::cout << "\nAfter Applying remove()\n";
		semi.show();
		std::cout << "\n\nTESTING TO LOWER FUNCTION\n";
		semi.clear();
		semi.push_back("\n\nHELLO");
		std::cout << "\nContents of vector\n";
		semi.show();
		semi.toLower();
		std::cout << "\nAfter Applying tolower()\n";
		semi.show();

	}
	int main()
	{
		Test t, t1;
		t.test1();
		t.test2();
		t.testfind();
		t.testSemi();
		return 0;
	}
};