#include "ChainRuleEliminator.h"
#include <iostream>

ChainRuleEliminator::ChainRuleEliminator(Grammar g)
	: m_grammar(std::move(g))
{
}

Grammar ChainRuleEliminator::Execute() const
{
	LogStep("Eliminating chain rules");

	const auto chainPairs = BuildChainPairs();
	Grammar result;
	result.SetStartSymbol(m_grammar.GetStartSymbol());
	result.SetTerminals(m_grammar.GetTerminals());
	result.SetNonTerminals(m_grammar.GetNonTerminals());

	for (const auto& pair : chainPairs)
	{
		for (const auto& rule : m_grammar.GetRules())
		{
			if (rule.GetLhs() == pair.second && !rule.IsChainRule())
			{
				result.AddRule(pair.first, rule.GetRhs());
			}
		}
	}

	LogGrammar(result, "Grammar with no chain rules");
	return result;
}

std::set<std::pair<Symbol, Symbol>> ChainRuleEliminator::BuildChainPairs() const
{
	std::set<std::pair<Symbol, Symbol>> pairs;
	for (auto& symbol : m_grammar.GetNonTerminals())
	{
		pairs.insert({ symbol, symbol });
	}

	bool changed = true;
	while (changed)
	{
		changed = false;
		for (const auto& rule : m_grammar.GetRules())
		{
			if (rule.IsChainRule())
			{
				if (AddPath(pairs, rule.GetLhs(), rule.GetRhs()[0]))
				{
					changed = true;
				}
			}
		}
	}
	return pairs;
}

bool ChainRuleEliminator::AddPath(
	std::set<std::pair<Symbol, Symbol>>& pairs, const Symbol& lhs, const Symbol& rhs) const
{
	bool added = false;
	for (auto& nonTerminal : m_grammar.GetNonTerminals())
	{
		if (pairs.count({ nonTerminal, lhs }) && !pairs.count({ nonTerminal, rhs }))
		{
			pairs.insert({ nonTerminal, rhs });
			added = true;
		}
	}
	return added;
}