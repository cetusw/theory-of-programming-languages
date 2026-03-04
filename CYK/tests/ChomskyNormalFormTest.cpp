#include "Utils.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(FullCNF, TerminalIsolation)
{
	const std::string inputSimple = "S -> a b c";
	std::stringstream inputStream(inputSimple);
	const Grammar g = GrammarParser::Parse(inputStream);
	const Grammar res = ChomskyNormalForm(g).Convert();

	for (const auto& rule : res.GetRules())
	{
		EXPECT_TRUE(rule.IsInChomskyForm());
	}
}