#include "../Parser/GrammarParser.h"
#include <gtest/gtest.h>

TEST(GrammarParserTest, BasicRuleParsing)
{
	const std::vector<std::string> input = { "S -> aB" };
	Grammar g = GrammarParser::Parse(input);

	const std::vector<Production> rules = g.GetRules();
	ASSERT_EQ(rules.size(), 1);
	EXPECT_EQ(rules[0].GetLhs().GetValue(), "S");
	EXPECT_FALSE(rules[0].GetLhs().IsTerminal());

	ASSERT_EQ(rules[0].GetRhs().size(), 2);
	EXPECT_EQ(rules[0].GetRhs()[0].GetValue(), "a");
	EXPECT_TRUE(rules[0].GetRhs()[0].IsTerminal());
	EXPECT_EQ(rules[0].GetRhs()[1].GetValue(), "B");
	EXPECT_FALSE(rules[0].GetRhs()[1].IsTerminal());
}

TEST(GrammarParserTest, AlternativesParsing)
{
	const std::vector<std::string> input = { "S -> a | B | c" };
	Grammar g = GrammarParser::Parse(input);

	const std::vector<Production> rules = g.GetRules();
	ASSERT_EQ(rules.size(), 3);
	EXPECT_EQ(rules[0].GetRhs()[0].GetValue(), "a");
	EXPECT_EQ(rules[1].GetRhs()[0].GetValue(), "B");
	EXPECT_EQ(rules[2].GetRhs()[0].GetValue(), "c");
}

TEST(GrammarParserTest, EpsilonParsing)
{
	const std::vector<std::string> input = { "A -> e" };
	Grammar g = GrammarParser::Parse(input);

	const std::vector<Production> rules = g.GetRules();
	ASSERT_EQ(rules.size(), 1);
	EXPECT_TRUE(rules[0].GetRhs().empty());
}

TEST(GrammarParserTest, StartSymbolDetection)
{
	const std::vector<std::string> input = {
		"A -> a",
		"S -> b"
	};
	Grammar g = GrammarParser::Parse(input);

	EXPECT_EQ(g.GetStartSymbol().GetValue(), "A");
}

TEST(GrammarParserTest, SymbolSetsValidation)
{
	const std::vector<std::string> input = { "S -> aB | b" };
	Grammar g = GrammarParser::Parse(input);

	const std::set<Symbol> nonTerminals = g.GetNonTerminals();
	EXPECT_EQ(nonTerminals.size(), 2);
	EXPECT_TRUE(nonTerminals.count({ "S", false }));
	EXPECT_TRUE(nonTerminals.count({ "B", false }));

	const std::set<Symbol> terminals = g.GetTerminals();
	EXPECT_EQ(terminals.size(), 2);
	EXPECT_TRUE(terminals.count({ "a", true }));
	EXPECT_TRUE(terminals.count({ "b", true }));
}

TEST(GrammarParserTest, WhitespaceHandling)
{
	const std::vector<std::string> input = { "  S   ->   A b  |  e  " };
	Grammar g = GrammarParser::Parse(input);

	const std::vector<Production> rules = g.GetRules();
	ASSERT_EQ(rules.size(), 2);
	EXPECT_EQ(rules[0].GetLhs().GetValue(), "S");
	ASSERT_EQ(rules[0].GetRhs().size(), 2);
	EXPECT_EQ(rules[0].GetRhs()[1].GetValue(), "b");
	EXPECT_TRUE(rules[1].GetRhs().empty());
}