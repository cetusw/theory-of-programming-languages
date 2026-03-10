#include "GrammarPrinter.h"

#include <algorithm>
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

std::string GrammarPrinter::RhsToString(const std::vector<Symbol>& rhs)
{
	if (rhs.empty())
	{
		return "e";
	}
	std::string res;
	for (const auto& sym : rhs)
	{
		res += sym.GetValue() + " ";
	}
	if (!res.empty())
	{
		res.pop_back();
	}
	return res;
}

StringRules GrammarPrinter::GetStringRules(const std::vector<Production>& rules)
{
	std::map<std::string, std::vector<Production>> grouped;
	for (const auto& rule : rules)
	{
		grouped[rule.GetLhs().GetValue()].push_back(rule);
	}

	StringRules stringRules;

	auto hasNonTerminals = [](const std::vector<Symbol>& rhs) {
		return std::ranges::any_of(rhs, [](const Symbol& s) {
			return !s.IsTerminal();
		});
	};

	for (auto& [lhs, productions] : grouped)
	{
		std::ranges::sort(productions, [&](const Production& a, const Production& b) {
			const auto& rhsA = a.GetRhs();
			const auto& rhsB = b.GetRhs();

			if (rhsA.empty() != rhsB.empty())
			{
				return rhsB.empty();
			}

			const bool ntA = hasNonTerminals(rhsA);
			const bool ntB = hasNonTerminals(rhsB);
			if (ntA != ntB)
			{
				return ntA;
			}

			return RhsToString(rhsA) < RhsToString(rhsB);
		});

		for (const auto& p : productions)
		{
			stringRules[lhs].push_back(RhsToString(p.GetRhs()));
		}
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