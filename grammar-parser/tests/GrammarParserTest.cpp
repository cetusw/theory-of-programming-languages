#include "../parser/GrammarParser.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(GrammarParserTest, BasicRuleParsing)
{
    std::string input = "S -> a B";
    std::stringstream ss(input);
    Grammar g = GrammarParser::Parse(ss);

    const auto rules = g.GetRules();
    ASSERT_EQ(rules.size(), 1);
    EXPECT_EQ(rules[0].GetLhs().GetValue(), "S");
    ASSERT_EQ(rules[0].GetRhs().size(), 2);
    EXPECT_EQ(rules[0].GetRhs()[0].GetValue(), "a");
    EXPECT_TRUE(rules[0].GetRhs()[0].IsTerminal());
    EXPECT_EQ(rules[0].GetRhs()[1].GetValue(), "B");
    EXPECT_FALSE(rules[0].GetRhs()[1].IsTerminal());
}

TEST(GrammarParserTest, AlternativesParsing)
{
    std::string input = "S -> a | B | c";
    std::stringstream ss(input);
    Grammar g = GrammarParser::Parse(ss);

    const auto rules = g.GetRules();
    ASSERT_EQ(rules.size(), 3);
    EXPECT_EQ(rules[0].GetRhs()[0].GetValue(), "a");
    EXPECT_EQ(rules[1].GetRhs()[0].GetValue(), "B");
    EXPECT_EQ(rules[2].GetRhs()[0].GetValue(), "c");
}

TEST(GrammarParserTest, EpsilonParsing)
{
    std::string input = "A -> e";
    std::stringstream ss(input);
    Grammar g = GrammarParser::Parse(ss);

    const auto rules = g.GetRules();
    ASSERT_EQ(rules.size(), 1);
    EXPECT_TRUE(rules[0].GetRhs().empty());
}

TEST(GrammarParserTest, StartSymbolDetection)
{
    const std::string input = R"(
        A -> a
        S -> b
    )";
    std::stringstream ss(input);
    const Grammar g = GrammarParser::Parse(ss);

    EXPECT_EQ(g.GetStartSymbol().GetValue(), "A");
}

TEST(GrammarParserTest, SymbolSetsValidation)
{
    std::string input = "S -> a B | b";
    std::stringstream ss(input);
    Grammar g = GrammarParser::Parse(ss);

    const auto nonTerminals = g.GetNonTerminals();
    EXPECT_EQ(nonTerminals.size(), 2);
    EXPECT_TRUE(nonTerminals.count({ "S", false }));
    EXPECT_TRUE(nonTerminals.count({ "B", false }));

    const auto terminals = g.GetTerminals();
    EXPECT_EQ(terminals.size(), 2);
    EXPECT_TRUE(terminals.count({ "a", true }));
    EXPECT_TRUE(terminals.count({ "b", true }));
}

TEST(GrammarParserTest, WhitespaceHandling)
{
    std::string input = "  S   ->   A b  |  e  ";
    std::stringstream ss(input);
    Grammar g = GrammarParser::Parse(ss);

    const auto rules = g.GetRules();
    ASSERT_EQ(rules.size(), 2);
    EXPECT_EQ(rules[0].GetLhs().GetValue(), "S");
    ASSERT_EQ(rules[0].GetRhs().size(), 2);
    EXPECT_EQ(rules[0].GetRhs()[1].GetValue(), "b");
    EXPECT_TRUE(rules[1].GetRhs().empty());
}
