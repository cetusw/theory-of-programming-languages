#include "GrammarFactorizer.h"
#include "printer/GrammarPrinter.h"
#include <algorithm>
#include <iostream>

GrammarFactorizer::GrammarFactorizer(Grammar g)
	: m_grammar(std::move(g))
{
}

Grammar GrammarFactorizer::Execute()
{
	auto rules = GroupRules(m_grammar.GetRules());
	while (FactorizeStep(rules))
	{
	}

	Grammar result = Finalize(m_grammar, rules);
	GrammarPrinter::Print(std::cout, result);
	return result;
}

bool GrammarFactorizer::FactorizeStep(GroupedRules& rules)
{
	for (auto& [lhs, rhsList] : rules)
	{
		std::vector<Symbol> prefix = FindLongestCommonPrefix(rhsList);
		if (!prefix.empty())
		{
			ApplyFactorization(lhs, prefix, rules);
			return true;
		}
	}
	return false;
}

std::vector<Symbol> GrammarFactorizer::FindLongestCommonPrefix(const std::vector<std::vector<Symbol>>& rhsList)
{
	for (size_t i = 0; i < rhsList.size(); ++i)
	{
		for (size_t j = i + 1; j < rhsList.size(); ++j)
		{
			if (rhsList[i].empty() || rhsList[j].empty() || rhsList[i][0] != rhsList[j][0])
			{
				continue;
			}
			return { rhsList[i][0] };
		}
	}
	return {};
}

void GrammarFactorizer::ApplyFactorization(const Symbol& A, const std::vector<Symbol>& prefix, GroupedRules& rules)
{
	const Symbol newA = CreateNewSymbol(A, ++m_newSymbolCounter);
	std::vector<std::vector<Symbol>> remainingRules, newRulesForA;

	for (const auto& rhs : rules[A])
	{
		if (rhs.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), rhs.begin()))
		{
			const auto offset = static_cast<std::ptrdiff_t>(prefix.size());
			std::vector suffix(rhs.begin() + offset, rhs.end());
			newRulesForA.push_back(suffix);
		}
		else
		{
			remainingRules.push_back(rhs);
		}
	}

	std::vector<Symbol> factorizedRule = prefix;
	factorizedRule.push_back(newA);
	remainingRules.push_back(factorizedRule);

	rules[A] = remainingRules;
	rules[newA] = newRulesForA;
}

GroupedRules GrammarFactorizer::GroupRules(const std::vector<Production>& rules)
{
	GroupedRules grouped;
	for (const auto& r : rules)
	{
		grouped[r.GetLhs()].push_back(r.GetRhs());
	}
	return grouped;
}

Grammar GrammarFactorizer::Finalize(const Grammar& src, const GroupedRules& rules)
{
	Grammar result;
	result.SetStartSymbol(src.GetStartSymbol());
	for (auto const& [lhs, rhsList] : rules)
	{
		for (const auto& rhs : rhsList)
		{
			result.AddRule(lhs, rhs);
		}
	}
	return result;
}

Symbol GrammarFactorizer::CreateNewSymbol(const Symbol& oldSymbol, int index)
{
	std::string name = oldSymbol.GetValue();
	const size_t primePos = name.find('\'');
	if (primePos != std::string::npos)
	{
		name = name.substr(0, primePos);
	}
	return { name + "'" + std::to_string(index), false };
}
