#ifndef CYK_GRAMMARPARSER_H
#define CYK_GRAMMARPARSER_H

#include "../Types/Grammar.h"

class GrammarParser
{
public:
	static Grammar Parse(const std::vector<std::string>& lines);

	static constexpr std::string_view RULE_OPERATOR = "->";
	static constexpr char OR_OPERATOR = '|';

private:
	static void ParseLine(const std::string& line, Grammar& grammar);
	static std::vector<Symbol> Tokenize(std::string string);
	static std::string Trim(const std::string& string);
};

#endif // CYK_GRAMMARPARSER_H
