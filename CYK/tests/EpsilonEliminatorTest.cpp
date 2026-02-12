#include "../EpsilonEliminator.h"
#include "../Grammar.h"
#include "../GrammarParser.h"
#include <gtest/gtest.h>

bool HasRule(Grammar& g, const std::string& lhs, const std::string& rhsString)
{
	for (const auto& rule : g.GetRules())
	{
		if (rule.lhs.value != lhs)
		{
			continue;
		}
		std::string actualRhs;
		if (rule.rhs.empty())
		{
			actualRhs = "e";
		}
		else
		{
			for (const auto& symbol : rule.rhs)
			{
				actualRhs += symbol.value;
			}
		}

		if (actualRhs == rhsString)
		{
			return true;
		}
	}
	return false;
}

TEST(EpsilonEliminatorTest, SimpleEpsilonRemoval)
{
	std::vector<std::string> input = {
		"A -> a | e"
	};
	Grammar g = GrammarParser::Parse(input);
	EpsilonEliminator eliminator(g);
	Grammar result = eliminator.Execute();

	EXPECT_FALSE(HasRule(result, "A", "e"));
	EXPECT_TRUE(HasRule(result, "A", "a"));
}

TEST(EpsilonEliminatorTest, CombinationsGeneration)
{
	std::vector<std::string> input = {
		"S -> Ab",
		"A -> e"
	};
	Grammar g = GrammarParser::Parse(input);
	EpsilonEliminator eliminator(g);
	Grammar result = eliminator.Execute();

	EXPECT_TRUE(HasRule(result, "S", "Ab"));
	EXPECT_TRUE(HasRule(result, "S", "b"));
	EXPECT_FALSE(HasRule(result, "A", "e"));
}

TEST(EpsilonEliminatorTest, NullablePropagation)
{
	std::vector<std::string> input = {
		"S -> A",
		"A -> B",
		"B -> e"
	};
	Grammar g = GrammarParser::Parse(input);
	EpsilonEliminator eliminator(g);
	Grammar result = eliminator.Execute();

	EXPECT_EQ(result.GetStartSymbol().value, "S1");
	EXPECT_TRUE(HasRule(result, "S1", "e"));
	EXPECT_TRUE(HasRule(result, "S1", "S"));
}

TEST(EpsilonEliminatorTest, DuplicateRemoval)
{
	std::vector<std::string> input = {
		"S -> AA",
		"A -> e"
	};
	Grammar g = GrammarParser::Parse(input);
	EpsilonEliminator eliminator(g);
	Grammar result = eliminator.Execute();

	int countStoA = 0;
	for (const auto& rule : result.GetRules())
	{
		if (rule.lhs.value == "S" && rule.rhs.size() == 1 && rule.rhs[0].value == "A")
		{
			countStoA++;
		}
	}
	EXPECT_EQ(countStoA, 1);
}

TEST(EpsilonEliminatorTest, ComplexExampleFromPrompt)
{
	std::vector<std::string> input = {
		"S -> BC | Ab | AB",
		"A -> Aa | e",
		"B -> e",
		"C -> c"
	};
	Grammar g = GrammarParser::Parse(input);
	EpsilonEliminator eliminator(g);
	Grammar result = eliminator.Execute();

	EXPECT_TRUE(HasRule(result, "S", "BC"));
	EXPECT_TRUE(HasRule(result, "S", "C"));
	EXPECT_TRUE(HasRule(result, "S", "Ab"));
	EXPECT_TRUE(HasRule(result, "S", "b"));
	EXPECT_TRUE(HasRule(result, "S", "AB"));
	EXPECT_TRUE(HasRule(result, "S", "A"));
	EXPECT_TRUE(HasRule(result, "S", "B"));

	EXPECT_TRUE(HasRule(result, "A", "Aa"));
	EXPECT_TRUE(HasRule(result, "A", "a"));
	EXPECT_FALSE(HasRule(result, "A", "e"));

	EXPECT_EQ(result.GetStartSymbol().value, "S1");
	EXPECT_TRUE(HasRule(result, "S1", "e"));
}