#include "UselessSymbolEliminator.h"

#include <algorithm>

UselessSymbolEliminator::UselessSymbolEliminator(Grammar g)
	: m_grammar(std::move(g))
{
}

Grammar UselessSymbolEliminator::Execute()
{
	LogStep("Eliminating useless symbols");

	FindGenerating();
	Grammar resultGrammar;
	resultGrammar.SetStartSymbol(m_grammar.GetStartSymbol());
	FilterGenerating(m_grammar, resultGrammar);

	const std::set<Symbol> reachable = FindReachable(resultGrammar);
	Grammar result;
	FilterReachable(resultGrammar, result, reachable);

	result.SetStartSymbol(resultGrammar.GetStartSymbol());

	LogGrammar(result, "Grammar with no useless symbols");
	return result;
}

void UselessSymbolEliminator::FindGenerating()
{
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (const auto& rule : m_grammar.GetRules())
		{
			if (!m_generating.count(rule.GetLhs()) && AllRhsGenerating(rule.GetRhs()))
			{
				m_generating.insert(rule.GetLhs());
				changed = true;
			}
		}
	}
}

bool UselessSymbolEliminator::AllRhsGenerating(const std::vector<Symbol>& rhs) const
{
	return std::all_of(rhs.begin(), rhs.end(), [&](const Symbol& s) {
		return s.IsTerminal() || m_generating.count(s);
	});
}

std::set<Symbol> UselessSymbolEliminator::FindReachable(Grammar& g)
{
	std::set reach = { g.GetStartSymbol() };
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (const auto& r : g.GetRules())
		{
			if (!reach.count(r.GetLhs()))
			{
				continue;
			}
			for (const auto& symbol : r.GetRhs())
			{
				if (reach.find(symbol) == reach.end())
				{
					reach.insert(symbol);
					changed = true;
				}
			}
		}
	}
	return reach;
}

void UselessSymbolEliminator::FilterGenerating(const Grammar& src, Grammar& dst) const
{
	for (const auto& r : src.GetRules())
	{
		bool ok = m_generating.count(r.GetLhs());
		for (const auto& s : r.GetRhs())
		{
			if (!s.IsTerminal() && !m_generating.count(s))
			{
				ok = false;
			}
		}
		if (ok)
		{
			dst.AddRule(r.GetLhs(), r.GetRhs());
		}
	}
}

void UselessSymbolEliminator::FilterReachable(const Grammar& src, Grammar& dst, const std::set<Symbol>& reach)
{
	for (const auto& r : src.GetRules())
	{
		if (reach.count(r.GetLhs()))
		{
			dst.AddRule(r.GetLhs(), r.GetRhs());
		}
	}
}