#include "LeftRecursionEliminator.h"
#include "printer/GrammarPrinter.h"
#include <algorithm>
#include <iostream>

LeftRecursionEliminator::LeftRecursionEliminator(Grammar g)
	: m_grammar(std::move(g))
{
}

Grammar LeftRecursionEliminator::Execute() const
{
	auto rules = GroupRules(m_grammar.GetRules());
	auto nonTerminals = m_grammar.GetNonTerminals();
	const std::vector orderedNt(nonTerminals.begin(), nonTerminals.end());

	for (size_t i = 0; i < orderedNt.size(); ++i)
	{
		for (size_t j = 0; j < i; ++j)
		{
			ReplaceIndirect(orderedNt[i], orderedNt[j], rules);
		}
		EliminateImmediate(orderedNt[i], rules);
	}
	Grammar result = Finalize(m_grammar, rules);

	GrammarPrinter::Print(std::cout, result);
	return result;
}

void LeftRecursionEliminator::ReplaceIndirect(const Symbol& Ai, const Symbol& Aj, GroupedRules& rules)
{
	std::vector<std::vector<Symbol>> newRhsList;
	for (const auto& rhs : rules[Ai])
	{
		if (!rhs.empty() && rhs[0] == Aj)
		{
			std::vector gamma(rhs.begin() + 1, rhs.end());
			for (const auto& ajRhs : rules[Aj])
			{
				std::vector<Symbol> combined = ajRhs;
				combined.insert(combined.end(), gamma.begin(), gamma.end());
				newRhsList.push_back(combined);
			}
		}
		else
		{
			newRhsList.push_back(rhs);
		}
	}
	rules[Ai] = newRhsList;
}

void LeftRecursionEliminator::EliminateImmediate(const Symbol& A, GroupedRules& rules)
{
	std::vector<std::vector<Symbol>> alphas, betas;
	for (const auto& rhs : rules[A])
	{
		if (!rhs.empty() && rhs[0] == A)
		{
			alphas.emplace_back(rhs.begin() + 1, rhs.end());
		}
		else
		{
			betas.push_back(rhs);
		}
	}

	if (alphas.empty())
	{
		return;
	}

	const Symbol newA = CreateNewSymbol(A);
	rules[A].clear();
	rules[newA].clear();

	for (auto& beta : betas)
	{
		beta.push_back(newA);
		rules[A].push_back(beta);
	}
	for (auto& alpha : alphas)
	{
		alpha.push_back(newA);
		rules[newA].push_back(alpha);
	}
	rules[newA].emplace_back();
}

GroupedRules LeftRecursionEliminator::GroupRules(const std::vector<Production>& rules)
{
	GroupedRules grouped;
	for (const auto& r : rules)
	{
		grouped[r.GetLhs()].push_back(r.GetRhs());
	}
	return grouped;
}

Grammar LeftRecursionEliminator::Finalize(const Grammar& src, const GroupedRules& rules)
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

Symbol LeftRecursionEliminator::CreateNewSymbol(const Symbol& oldSymbol)
{
	return { oldSymbol.GetValue() + "'", false };
}