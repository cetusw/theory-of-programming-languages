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
	std::vector<Symbol> longest;
	for (size_t i = 0; i < rhsList.size(); ++i)
	{
		for (size_t j = i + 1; j < rhsList.size(); ++j)
		{
			std::vector<Symbol> currentPrefix;
			const size_t minLen = std::min(rhsList[i].size(), rhsList[j].size());
			for (size_t k = 0; k < minLen; ++k)
			{
				if (rhsList[i][k] == rhsList[j][k])
				{
					currentPrefix.push_back(rhsList[i][k]);
				}
				else
				{
					break;
				}
			}
			if (currentPrefix.size() > longest.size())
			{
				longest = currentPrefix;
			}
		}
	}
	return longest;
}

void GrammarFactorizer::ApplyFactorization(const Symbol& A, const std::vector<Symbol>& prefix, GroupedRules& rules)
{
	std::vector<std::vector<Symbol>> remainingRules, newRulesForFactor;

	for (const auto& rhs : rules[A])
	{
		if (rhs.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), rhs.begin()))
		{
			std::vector suffix(rhs.begin() + static_cast<int>(prefix.size()), rhs.end());
			newRulesForFactor.push_back(suffix);
		}
		else
		{
			remainingRules.push_back(rhs);
		}
	}

	std::ranges::sort(newRulesForFactor);

	Symbol factorSymbol;
	if (m_factorCache.contains(newRulesForFactor))
	{
		factorSymbol = m_factorCache[newRulesForFactor];
	}
	else
	{
		factorSymbol = CreateNewSymbol(A, ++m_newSymbolCounter);
		m_factorCache[newRulesForFactor] = factorSymbol;
		rules[factorSymbol] = newRulesForFactor;
	}

	std::vector<Symbol> factorizedRule = prefix;
	factorizedRule.push_back(factorSymbol);
	remainingRules.push_back(factorizedRule);

	rules[A] = remainingRules;
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
