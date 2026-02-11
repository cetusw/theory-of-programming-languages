#ifndef CYK_GRAMMARPARSER_H
#define CYK_GRAMMARPARSER_H

#include "Grammar.h"
#include <sstream>

class GrammarParser
{
public:
	static Grammar Parse(const std::vector<std::string>& lines)
	{
		Grammar g;
		for (const auto& line : lines)
		{
			ParseLine(line, g);
		}

		const std::vector<Production> rules = g.GetRules();
		if (!rules.empty())
		{
			g.SetStartSymbol(rules[0].lhs);
		}
		return g;
	}

private:
	static void ParseLine(const std::string& line, Grammar& g)
	{
		const std::string trimmedLine = Trim(line);
		if (trimmedLine.empty())
		{
			return;
		}

		const size_t pos = trimmedLine.find("->");
		if (pos == std::string::npos)
		{
			return;
		}

		const std::string lhsValue = Trim(trimmedLine.substr(0, pos));
		const Symbol lhs = { lhsValue, false };

		const std::string rhs = trimmedLine.substr(pos + 2);
		std::stringstream rhsStream(rhs);
		std::string option;

		while (std::getline(rhsStream, option, '|'))
		{
			g.AddRule(lhs, Tokenize(option));
		}
	}

	static std::vector<Symbol> Tokenize(std::string s)
	{
		s = Trim(s);
		if (s == "e" || s == "ε" || s.empty())
		{
			return {};
		}

		std::vector<Symbol> symbols;
		for (const char c : s)
		{
			if (std::isspace(static_cast<unsigned char>(c)))
			{
				continue;
			}
			const bool isTerminal = !std::isupper(static_cast<unsigned char>(c));
			symbols.push_back({ std::string(1, c), isTerminal });
		}
		return symbols;
	}

	static std::string Trim(const std::string& s)
	{
		const size_t first = s.find_first_not_of(' ');
		if (first == std::string::npos)
		{
			return "";
		}
		const size_t last = s.find_last_not_of(' ');
		return s.substr(first, last - first + 1);
	}
};

#endif // CYK_GRAMMARPARSER_H