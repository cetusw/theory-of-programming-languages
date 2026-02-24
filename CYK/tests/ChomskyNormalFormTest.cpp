#include "Utils.h"
#include <gtest/gtest.h>

TEST(FullCNF, TerminalIsolation)
{
	const std::string inputSimple = "S -> abc";
	const Grammar g = GrammarParser::Parse(Split(inputSimple));
	const Grammar res = ChomskyNormalForm(g).Convert();

	for (const auto& rule : res.GetRules())
	{
		EXPECT_TRUE(rule.IsInChomskyForm());
	}
}
