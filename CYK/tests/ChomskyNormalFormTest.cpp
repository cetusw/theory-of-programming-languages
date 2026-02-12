#include "../ChomskyNormalForm.h"
#include "../GrammarParser.h"
#include <gtest/gtest.h>

bool IsInChomskyForm(Grammar& g)
{
	const auto& rules = g.GetRules();
	const Symbol& start = g.GetStartSymbol();

	return std::all_of(rules.begin(), rules.end(), [&start](const Production& rule) {
		if (rule.rhs.empty())
		{
			return rule.lhs == start;
		}

		if (rule.rhs.size() == 1)
		{
			return rule.rhs[0].isTerminal;
		}

		if (rule.rhs.size() == 2)
		{
			return !rule.rhs[0].isTerminal && !rule.rhs[1].isTerminal;
		}

		return false;
	});
}

TEST(UnitRuleEliminatorTest, RemovesChainRules)
{
	std::vector<std::string> input = {
		"S -> A",
		"A -> B",
		"B -> a"
	};
	Grammar g = GrammarParser::Parse(input);
	UnitRuleEliminator eliminator(g);
	Grammar result = eliminator.Execute();

	bool found = false;
	for (auto& r : result.GetRules())
	{
		if (r.lhs.value == "S" && r.rhs.size() == 1 && r.rhs[0].value == "a")
		{
			found = true;
		}
	}
	EXPECT_TRUE(found);

	for (auto& r : result.GetRules())
	{
		if (r.rhs.size() == 1 && !r.rhs[0].isTerminal)
		{
			ADD_FAILURE() << "Found unit rule: " << r.lhs.value << " -> " << r.rhs[0].value;
		}
	}
}

TEST(UselessSymbolEliminatorTest, RemovesNonGeneratingAndUnreachable)
{
	std::vector<std::string> input = {
		"S -> a | A",
		"A -> AB",
		"B -> b",
		"C -> d"
	};
	Grammar g = GrammarParser::Parse(input);
	UselessSymbolEliminator eliminator(g);
	Grammar result = eliminator.Execute();

	for (auto& r : result.GetRules())
	{
		EXPECT_NE(r.lhs.value, "A");
		EXPECT_NE(r.lhs.value, "C");
	}
	EXPECT_EQ(result.GetRules().size(), 1);
}

TEST(ChomskyNormalFormTest, BinarizationTest)
{
	std::vector<std::string> input = {
		"S -> ABC",
		"A -> a",
		"B -> b",
		"C -> c"
	};
	Grammar g = GrammarParser::Parse(input);
	ChomskyNormalForm cnf(g);
	Grammar result = cnf.Convert();

	EXPECT_TRUE(IsInChomskyForm(result));
}

TEST(ChomskyNormalFormTest, MixedRulesTest)
{
	std::vector<std::string> input = {
		"S -> aB",
		"B -> b"
	};
	Grammar g = GrammarParser::Parse(input);
	ChomskyNormalForm cnf(g);
	Grammar result = cnf.Convert();

	EXPECT_TRUE(IsInChomskyForm(result));

	bool foundTerminalRule = false;
	for (auto& r : result.GetRules())
	{
		if (r.rhs.size() == 1 && r.rhs[0].value == "a")
		{
			foundTerminalRule = true;
		}
	}
	EXPECT_TRUE(foundTerminalRule);
}

TEST(ChomskyNormalFormTest, ComplexGrammarTest)
{
	std::vector<std::string> input = {
		"S -> BC | Ab | AB",
		"A -> Aa | a | e",
		"B -> b | e",
		"C -> c"
	};
	Grammar g = GrammarParser::Parse(input);
	ChomskyNormalForm cnf(g);
	Grammar result = cnf.Convert();

	EXPECT_TRUE(IsInChomskyForm(result));

	std::string startName = result.GetStartSymbol().value;
	bool foundBC = false;
	for (const auto& r : result.GetRules())
	{
		if (r.lhs.value == startName && r.rhs.size() == 2 &&
			r.rhs[0].value == "B" && r.rhs[1].value == "C")
		{
			foundBC = true;
		}
	}
	EXPECT_TRUE(foundBC);
}
