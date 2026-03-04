#include "../CYKValidator.h"
#include "../CNF/ChomskyNormalForm.h"
#include "../Parser/GrammarParser.h"
#include <gtest/gtest.h>
#include <sstream>

bool IsAccepted(const std::string& grammarString, const std::string& word)
{
	std::stringstream ss(grammarString);
	Grammar g = GrammarParser::Parse(ss);
	CYKValidator validator(g);
	return validator.Validate(word);
}

TEST(CYKTest, SimpleCNF)
{
	const std::string grammar = R"(
		S -> A B
		A -> a
		B -> b
	)";
	EXPECT_TRUE(IsAccepted(grammar, "ab"));
	EXPECT_FALSE(IsAccepted(grammar, "a"));
	EXPECT_FALSE(IsAccepted(grammar, "b"));
	EXPECT_FALSE(IsAccepted(grammar, "ba"));
}

TEST(CYKTest, TextbookExample)
{
	const std::string grammar = R"(
		S -> A B | B C
		A -> B A | a
		B -> C C | b
		C -> A B | a
	)";
	EXPECT_TRUE(IsAccepted(grammar, "baaba"));
	EXPECT_FALSE(IsAccepted(grammar, "baab"));
	EXPECT_TRUE(IsAccepted(grammar, "aaaaa"));
}

TEST(CYKTest, NeedsBinarization)
{
	const std::string grammar = "S -> a b c";
	EXPECT_TRUE(IsAccepted(grammar, "abc"));
	EXPECT_FALSE(IsAccepted(grammar, "ab"));
}

TEST(CYKTest, HandlesEpsilon)
{
	const std::string grammar = "S -> a S | e";
	EXPECT_TRUE(IsAccepted(grammar, ""));
	EXPECT_TRUE(IsAccepted(grammar, "a"));
	EXPECT_TRUE(IsAccepted(grammar, "aaaa"));
	EXPECT_FALSE(IsAccepted(grammar, "b"));
}

TEST(CYKTest, BalancedBrackets)
{
	std::string grammar = "S -> a S b | S S | a b";
	EXPECT_TRUE(IsAccepted(grammar, "ab"));
	EXPECT_TRUE(IsAccepted(grammar, "aabb"));
	EXPECT_TRUE(IsAccepted(grammar, "abab"));
	EXPECT_TRUE(IsAccepted(grammar, "aaabbb"));
	EXPECT_FALSE(IsAccepted(grammar, "aab"));
	EXPECT_FALSE(IsAccepted(grammar, "aba"));
}

TEST(CYKTest, WithUselessSymbols)
{
	const std::string grammar = R"(
		S -> A B | a
		A -> a
		B -> B
		C -> c
	)";
	EXPECT_TRUE(IsAccepted(grammar, "a"));
	EXPECT_FALSE(IsAccepted(grammar, "aa"));
}