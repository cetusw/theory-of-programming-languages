#pragma once
#include "../grammar/Grammar.h"

class GrammarParser
{
public:
	static Grammar Parse(std::istream& input);

	static constexpr std::string_view RULE_OPERATOR = "->";
	static constexpr char OR_OPERATOR = '|';

private:
	static void ParseLine(const std::string& line, Grammar& grammar);
	[[nodiscard]] static Symbol ParseLhs(const std::string& line, size_t endOfLhs);
	static void AddRule(const std::string& line, size_t endOfLhs, const Symbol& lhs, Grammar& grammar);
	static std::vector<Symbol> TokenizeRhs(const std::string& segment);

	static std::string Trim(const std::string& string);
};
