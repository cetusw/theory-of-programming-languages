#include "Utils.h"
#include <gtest/gtest.h>

TEST(EpsilonElimination, SimpleRemoval)
{
	const std::string input = "S -> a | e";
	const std::string expected = "S1 -> S | e\nS -> a";
	AssertGrammarTransformation(input, expected, Utils::TransformationMode::Epsilon);
}

TEST(EpsilonElimination, ComplexDependencies)
{
	const std::string input = R"(
        S -> A B
        A -> a | e
        B -> b | e
    )";
	const std::string expected = R"(
        S1 -> S | e
        S -> A B | A | B
        A -> a
        B -> b
    )";
	AssertGrammarTransformation(input, expected, Utils::TransformationMode::Epsilon);
}
