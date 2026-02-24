#include "Utils.h"
#include <gtest/gtest.h>

TEST(UselessSymbolElimination, NonGenerating)
{
	const std::string input = R"(
        S -> A | b
        A -> aA
    )";

	const std::string expected = "S -> b";

	AssertGrammarTransformation(input, expected, Utils::TransformationMode::Useless);
}