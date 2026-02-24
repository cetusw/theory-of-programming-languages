#include "Utils.h"
#include <gtest/gtest.h>

TEST(ChainRuleElimination, SimpleChain)
{
	const std::string input = R"(
        S -> A
        A -> a
    )";
	const std::string expected = R"(
        S -> a
        A -> a
    )";
	AssertGrammarTransformation(input, expected, Utils::TransformationMode::Chain);
}

TEST(ChainRuleElimination, CyclicChain)
{
	const std::string input = R"(
        S -> A
        A -> S | b
    )";
	const std::string expected = R"(
        S -> b
        A -> b
    )";
	AssertGrammarTransformation(input, expected, Utils::TransformationMode::Chain);
}