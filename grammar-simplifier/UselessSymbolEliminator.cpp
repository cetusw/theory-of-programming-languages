#include "UselessSymbolEliminator.h"
#include "printer/GrammarPrinter.h"
#include <algorithm>
#include <iostream>

UselessSymbolEliminator::UselessSymbolEliminator(Grammar g)
	: m_grammar(std::move(g))
{
}

Grammar UselessSymbolEliminator::Execute()
{
	FindGenerating();
	Grammar resultGrammar;
	resultGrammar.SetStartSymbol(m_grammar.GetStartSymbol());
	FilterGenerating(m_grammar, resultGrammar);

	const std::set<Symbol> reachable = FindReachable(resultGrammar);
	Grammar result;
	FilterReachable(resultGrammar, result, reachable);

	result.SetStartSymbol(resultGrammar.GetStartSymbol());

	GrammarPrinter::Print(std::cout, result);
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
			if (!m_generating.contains(rule.GetLhs()) && AllRhsGenerating(rule.GetRhs()))
			{
				m_generating.insert(rule.GetLhs());
				changed = true;
			}
		}
	}
}

bool UselessSymbolEliminator::AllRhsGenerating(const std::vector<Symbol>& rhs) const
{
	return std::ranges::all_of(rhs, [&](const Symbol& s) {
		return s.IsTerminal() || m_generating.contains(s);
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
			if (!reach.contains(r.GetLhs()))
			{
				continue;
			}
			for (const auto& symbol : r.GetRhs())
			{
				if (!reach.contains(symbol))
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
		bool ok = m_generating.contains(r.GetLhs());
		for (const auto& s : r.GetRhs())
		{
			if (!s.IsTerminal() && !m_generating.contains(s))
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
		if (reach.contains(r.GetLhs()))
		{
			dst.AddRule(r.GetLhs(), r.GetRhs());
		}
	}
}