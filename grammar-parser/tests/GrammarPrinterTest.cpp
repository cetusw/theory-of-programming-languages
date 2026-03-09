#include "../parser/GrammarParser.h"
#include "../printer/GrammarPrinter.h"
#include <gtest/gtest.h>
#include <sstream>

std::string GetPrintedGrammar(const Grammar& g)
{
	std::stringstream ss;
	GrammarPrinter::Print(ss, g);
	return ss.str();
}

TEST(GrammarPrinterTest, EmptyGrammar)
{
	const Grammar g;
	EXPECT_EQ(GetPrintedGrammar(g), "[Grammar is empty]\n");
}

TEST(GrammarPrinterTest, SimpleRule)
{
	std::stringstream input("S -> a B");
	const Grammar g = GrammarParser::Parse(input);

	EXPECT_EQ(GetPrintedGrammar(g), "S -> a B\n");
}

TEST(GrammarPrinterTest, Alternatives)
{
	std::stringstream input("S -> a | B | c");
	const Grammar g = GrammarParser::Parse(input);

	EXPECT_EQ(GetPrintedGrammar(g), "S -> a | B | c\n");
}

TEST(GrammarPrinterTest, EpsilonRule)
{
	std::stringstream input("A -> e");
	const Grammar g = GrammarParser::Parse(input);

	EXPECT_EQ(GetPrintedGrammar(g), "A -> e\n");
}

TEST(GrammarPrinterTest, StartSymbolFirst)
{
	std::string input = "S -> A\n"
						"A -> a";
	std::stringstream ss(input);
	Grammar g = GrammarParser::Parse(ss);

	std::string output = GetPrintedGrammar(g);

	size_t posS = output.find("S -> A");
	size_t posA = output.find("A -> a");

	EXPECT_NE(posS, std::string::npos);
	EXPECT_NE(posA, std::string::npos);
	EXPECT_LT(posS, posA);
}

TEST(GrammarPrinterTest, ComplexRhs)
{
	std::stringstream input("E -> T + E | T");
	Grammar g = GrammarParser::Parse(input);

	EXPECT_EQ(GetPrintedGrammar(g), "E -> T + E | T\n");
}

TEST(GrammarPrinterTest, GroupingRules)
{
	Grammar g;
	const Symbol S("S", false);
	g.AddRule(S, { Symbol("a", true) });
	g.AddRule(S, { Symbol("b", true) });
	g.SetStartSymbol(S);

	EXPECT_EQ(GetPrintedGrammar(g), "S -> a | b\n");
}