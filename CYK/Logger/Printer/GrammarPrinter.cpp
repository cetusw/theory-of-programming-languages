#include "GrammarPrinter.h"
#include <map>
#include <ostream>

void GrammarPrinter::Print(std::ostream& os, const Grammar& grammar)
{
	const auto rules = grammar.GetRules();
	if (rules.empty())
	{
		os << "[Grammar is empty]" << std::endl;
		return;
	}
	const auto stringRules = GetStringRules(rules);
	const std::string startSymbol = grammar.GetStartSymbol().GetValue();

	PrintStartRule(os, stringRules, startSymbol);
	PrintRules(os, stringRules, startSymbol);
}

StringRules GrammarPrinter::GetStringRules(const std::vector<Production>& rules)
{
	StringRules stringRules;
	for (const auto& rule : rules)
	{
		std::string rhs;
		if (rule.IsEpsilon())
		{
			rhs = "e";
		}
		else
		{
			for (const auto& symbol : rule.GetRhs())
			{
				rhs += symbol.GetValue() + " ";
			}
			if (!rhs.empty())
			{
				rhs.pop_back();
			}
		}
		stringRules[rule.GetLhs().GetValue()].push_back(rhs);
	}

	return stringRules;
}

void GrammarPrinter::PrintRhsAlternatives(std::ostream& os, const std::vector<std::string>& alternatives)
{
	for (size_t i = 0; i < alternatives.size(); ++i)
	{
		os << alternatives[i] << (i == alternatives.size() - 1 ? "" : " " + OR_OPERATOR + " ");
	}
}

void GrammarPrinter::PrintStartRule(std::ostream& os, const StringRules& stringRules, const std::string& start)
{
	if (const auto it = stringRules.find(start); it != stringRules.end())
	{
		os << start << " " << RULE_OPERATOR << " ";
		PrintRhsAlternatives(os, it->second);
		os << std::endl;
	}
}

void GrammarPrinter::PrintRules(std::ostream& os, const StringRules& stringRules, const std::string& start)
{
	for (const auto& [lhs, rhsList] : stringRules)
	{
		if (lhs == start)
		{
			continue;
		}

		os << lhs << " " << RULE_OPERATOR << " ";
		PrintRhsAlternatives(os, rhsList);
		os << std::endl;
	}
}