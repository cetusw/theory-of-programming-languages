#include "../CYKValidator.h"
#include "../CNF/ChomskyNormalForm.h"
#include "../Parser/GrammarParser.h"
#include <gtest/gtest.h>

bool IsAccepted(const std::vector<std::string>& grammarLines, const std::string& word)
{
	Grammar g = GrammarParser::Parse(grammarLines);
	ChomskyNormalForm cnfConverter(g);
	Grammar cnfGrammar = cnfConverter.Convert();

	CYKValidator validator(cnfGrammar);
	return validator.Validate(word);
}

TEST(CYKTest, SimpleCNF)
{
	const std::vector<std::string> grammar = {
		"S -> AB",
		"A -> a",
		"B -> b"
	};
	EXPECT_TRUE(IsAccepted(grammar, "ab"));
	EXPECT_FALSE(IsAccepted(grammar, "a"));
	EXPECT_FALSE(IsAccepted(grammar, "b"));
	EXPECT_FALSE(IsAccepted(grammar, "ba"));
}

TEST(CYKTest, TextbookExample)
{
	const std::vector<std::string> grammar = {
		"S -> AB | BC",
		"A -> BA | a",
		"B -> CC | b",
		"C -> AB | a"
	};
	EXPECT_TRUE(IsAccepted(grammar, "baaba"));
	EXPECT_FALSE(IsAccepted(grammar, "baab"));
	EXPECT_TRUE(IsAccepted(grammar, "aaaaa"));
}

TEST(CYKTest, NeedsBinarization)
{
	const std::vector<std::string> grammar = {
		"S -> abc"
	};
	EXPECT_TRUE(IsAccepted(grammar, "abc"));
	EXPECT_FALSE(IsAccepted(grammar, "ab"));
}

TEST(CYKTest, HandlesEpsilon)
{
	const std::vector<std::string> grammar = {
		"S -> aS | e"
	};
	EXPECT_TRUE(IsAccepted(grammar, ""));
	EXPECT_TRUE(IsAccepted(grammar, "a"));
	EXPECT_TRUE(IsAccepted(grammar, "aaaa"));
	EXPECT_FALSE(IsAccepted(grammar, "b"));
}

TEST(CYKTest, BalancedBrackets)
{
	std::vector<std::string> grammar = {
		"S -> aSb | SS | ab"
	};
	EXPECT_TRUE(IsAccepted(grammar, "ab"));
	EXPECT_TRUE(IsAccepted(grammar, "aabb"));
	EXPECT_TRUE(IsAccepted(grammar, "abab"));
	EXPECT_TRUE(IsAccepted(grammar, "aaabbb"));
	EXPECT_FALSE(IsAccepted(grammar, "aab"));
	EXPECT_FALSE(IsAccepted(grammar, "aba"));
}

TEST(CYKTest, WithUselessSymbols)
{
	const std::vector<std::string> grammar = {
		"S -> AB | a",
		"A -> a",
		"B -> B",
		"C -> c"
	};
	EXPECT_TRUE(IsAccepted(grammar, "a"));
	EXPECT_FALSE(IsAccepted(grammar, "aa"));
}