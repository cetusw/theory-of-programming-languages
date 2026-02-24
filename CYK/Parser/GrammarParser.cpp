#include "GrammarParser.h"
#include <sstream>

Grammar GrammarParser::Parse(const std::vector<std::string>& lines)
{
	Grammar g;
	for (const auto& line : lines)
	{
		ParseLine(line, g);
	}

	const std::vector<Production> rules = g.GetRules();
	if (!rules.empty())
	{
		g.SetStartSymbol(rules[0].GetLhs());
	}
	return g;
}

void GrammarParser::ParseLine(const std::string& line, Grammar& grammar)
{
	const std::string trimmedLine = Trim(line);
	if (trimmedLine.empty())
	{
		return;
	}

	const size_t endOfLhs = trimmedLine.find(RULE_OPERATOR);
	if (endOfLhs == std::string::npos)
	{
		return;
	}

	const std::string lhsValue = Trim(trimmedLine.substr(0, endOfLhs));
	const Symbol lhs(lhsValue, false);

	const std::string rhs = trimmedLine.substr(endOfLhs + RULE_OPERATOR.length());
	std::stringstream rhsStream(rhs);
	std::string option;

	while (std::getline(rhsStream, option, OR_OPERATOR))
	{
		grammar.AddRule(lhs, Tokenize(option));
	}
}

std::vector<Symbol> GrammarParser::Tokenize(std::string string)
{
	string = Trim(string);
	if (string == "e" || string == "ε" || string.empty())
	{
		return {};
	}

	std::vector<Symbol> symbols;
	for (const char c : string)
	{
		if (std::isspace(static_cast<unsigned char>(c)))
		{
			continue;
		}
		const bool isTerminal = !std::isupper(static_cast<unsigned char>(c));
		symbols.emplace_back(std::string(1, c), isTerminal);
	}
	return symbols;
}

std::string GrammarParser::Trim(const std::string& string)
{
	const size_t first = string.find_first_not_of(' ');
	if (first == std::string::npos)
	{
		return "";
	}
	const size_t last = string.find_last_not_of(' ');
	return string.substr(first, last - first + 1);
}
