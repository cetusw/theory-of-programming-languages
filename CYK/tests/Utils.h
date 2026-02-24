#ifndef CYK_UTILS_H
#define CYK_UTILS_H

#include "../Algorithms/ChainRuleEliminator.h"
#include "../Algorithms/EpsilonEliminator.h"
#include "../Algorithms/UselessSymbolEliminator.h"
#include "../CNF/ChomskyNormalForm.h"
#include "../Parser/GrammarParser.h"
#include "../Utils.h"
#include <algorithm>
#include <gtest/gtest.h>

inline std::vector<std::string> Split(const std::string& s)
{
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string line;
	while (std::getline(ss, line))
	{
		if (!line.empty())
		{
			result.push_back(line);
		}
	}
	return result;
}

inline std::vector<std::string> GetCanonicalRules(const Grammar& g)
{
	std::vector<std::string> ruleStrings;
	for (const auto& rule : g.GetRules())
	{
		std::string r = rule.GetLhs().GetValue() + " -> ";
		if (rule.GetRhs().empty())
		{
			r += "e";
		}
		else
		{
			for (const auto& symbol : rule.GetRhs())
			{
				r += symbol.GetValue() + " ";
			}
		}
		ruleStrings.push_back(r);
	}
	std::sort(ruleStrings.begin(), ruleStrings.end());
	return ruleStrings;
}

inline void AssertGrammarTransformation(
	const std::string& input, const std::string& expected, Utils::TransformationMode mode)
{
	std::vector<std::string> inputSplitted = Split(input);
	std::vector<std::string> expectedSplitted = Split(expected);
	Grammar inputGrammar = GrammarParser::Parse(inputSplitted);
	Grammar expectedGrammar = GrammarParser::Parse(expectedSplitted);
	Grammar result;

	switch (mode)
	{
	case Utils::TransformationMode::Epsilon:
	{
		result = EpsilonEliminator(inputGrammar).Execute();
		break;
	}
	case Utils::TransformationMode::Chain:
	{
		Grammar withoutEpsilon = EpsilonEliminator(inputGrammar).Execute();
		result = ChainRuleEliminator(withoutEpsilon).Execute();
		break;
	}
	case Utils::TransformationMode::CNF:
	{
		result = ChomskyNormalForm(inputGrammar).Convert();
		break;
	}
	case Utils::TransformationMode::Useless:
	{
		result = UselessSymbolEliminator(inputGrammar).Execute();
		break;
	}
	}

	auto actualRules = GetCanonicalRules(result);
	auto expectedRules = GetCanonicalRules(expectedGrammar);

	ASSERT_EQ(actualRules.size(), expectedRules.size()) << "Количество правил не совпадает!";

	for (size_t i = 0; i < actualRules.size(); ++i)
	{
		EXPECT_EQ(actualRules[i], expectedRules[i]);
	}
}

#endif // CYK_UTILS_H
