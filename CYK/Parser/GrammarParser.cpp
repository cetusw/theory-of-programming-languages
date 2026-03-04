#include "GrammarParser.h"
#include <algorithm>
#include <sstream>

Grammar GrammarParser::Parse(std::istream& input)
{
	Grammar g;
	std::string line;
	while (std::getline(input, line))
	{
		ParseLine(line, g);
	}

	const auto rules = g.GetRules();
	if (!rules.empty())
	{
		g.SetStartSymbol(rules[0].GetLhs());
	}
	return g;
}

void GrammarParser::ParseLine(const std::string& line, Grammar& grammar)
{
	const std::string trimmed = Trim(line);
	if (trimmed.empty() || trimmed[0] == '#')
	{
		return;
	}

	const size_t endOfLhs = trimmed.find(RULE_OPERATOR);
	if (endOfLhs == std::string::npos)
	{
		return;
	}

	const Symbol lhs = ParseLhs(trimmed, endOfLhs);
	AddRule(trimmed, endOfLhs, lhs, grammar);
}

Symbol GrammarParser::ParseLhs(const std::string& line, const size_t endOfLhs)
{
	const std::string lhsValue = Trim(line.substr(0, endOfLhs));
	const Symbol lhs(lhsValue, false);
	return lhs;
}

void GrammarParser::AddRule(
	const std::string& line, const size_t endOfLhs, const Symbol& lhs, Grammar& grammar)
{
	const std::string rhs = line.substr(endOfLhs + RULE_OPERATOR.length());
	std::stringstream rhsStream(rhs);
	std::string segment;

	while (std::getline(rhsStream, segment, OR_OPERATOR))
	{
		grammar.AddRule(lhs, TokenizeRhs(segment));
	}
}

std::vector<Symbol> GrammarParser::TokenizeRhs(const std::string& segment)
{
	std::vector<Symbol> symbols;
	std::stringstream ss(segment);
	std::string token;

	while (ss >> token)
	{
		if (token == "e" || token == "ε")
		{
			continue;
		}

		const bool hasUppercase = std::any_of(
			token.begin(), token.end(), [](const unsigned char c) {
				return std::isupper(c);
			});

		symbols.emplace_back(token, !hasUppercase);
	}
	return symbols;
}

std::string GrammarParser::Trim(const std::string& string)
{
	const char* whitespace = " \t\r\n";
	const size_t first = string.find_first_not_of(whitespace);
	if (first == std::string::npos)
	{
		return "";
	}
	const size_t last = string.find_last_not_of(whitespace);
	return string.substr(first, last - first + 1);
}
