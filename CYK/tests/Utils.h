#pragma once

#include "../Algorithms/ChainRuleEliminator.h"
#include "../Algorithms/EpsilonEliminator.h"
#include "../Algorithms/UselessSymbolEliminator.h"
#include "../CNF/ChomskyNormalForm.h"
#include "../Parser/GrammarParser.h"
#include "../Utils.h"
#include "Logger/Printer/GrammarPrinter.h"
#include <algorithm>
#include <gtest/gtest.h>

inline std::string GetGrammarString(const Grammar& g)
{
	std::stringstream ss;
	GrammarPrinter::Print(ss, g);
	return ss.str();
}

inline void AssertGrammarTransformation(
	const std::string& input, const std::string& expected, Utils::TransformationMode mode)
{
	std::stringstream inputStream(input);
	Grammar inputGrammar = GrammarParser::Parse(inputStream);
	Grammar result;

	switch (mode)
	{
	case Utils::TransformationMode::Epsilon:
		result = EpsilonEliminator(inputGrammar).Execute();
		break;
	case Utils::TransformationMode::Chain:
		result = ChainRuleEliminator(EpsilonEliminator(inputGrammar).Execute()).Execute();
		break;
	case Utils::TransformationMode::CNF:
		result = ChomskyNormalForm(inputGrammar).Convert();
		break;
	case Utils::TransformationMode::Useless:
		result = UselessSymbolEliminator(inputGrammar).Execute();
		break;
	}

	std::string actualOutput = GetGrammarString(result);

	std::stringstream expectedStream(expected);
	Grammar expectedGrammar = GrammarParser::Parse(expectedStream);
	std::string expectedOutput = GetGrammarString(expectedGrammar);

	ASSERT_EQ(actualOutput, expectedOutput);
}
