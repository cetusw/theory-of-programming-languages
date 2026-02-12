#ifndef CYK_USELESSSYMBOLELIMINATOR_H
#define CYK_USELESSSYMBOLELIMINATOR_H

#include "Grammar.h"

#include <algorithm>

class UselessSymbolEliminator
{
public:
	explicit UselessSymbolEliminator(Grammar g)
		: m_grammar(std::move(g))
	{
	}

	Grammar Execute()
	{
		const std::set<Symbol> generating = FindGenerating();
		Grammar resultGrammar;
		resultGrammar.SetStartSymbol(m_grammar.GetStartSymbol());
		FilterGenerating(m_grammar, resultGrammar, generating);

		const std::set<Symbol> reachable = FindReachable(resultGrammar);
		Grammar result;
		FilterReachable(resultGrammar, result, reachable);

		result.SetStartSymbol(resultGrammar.GetStartSymbol());
		return result;
	}

private:
	Grammar m_grammar;

	std::set<Symbol> FindGenerating()
	{
		std::set<Symbol> gen;
		bool changed = true;
		while (changed)
		{
			changed = false;
			for (const auto& rule : m_grammar.GetRules())
			{
				if (!gen.count(rule.lhs) && AllRhsGenerating(rule.rhs, gen))
				{
					gen.insert(rule.lhs);
					changed = true;
				}
			}
		}
		return gen;
	}

	static bool AllRhsGenerating(const std::vector<Symbol>& rhs, const std::set<Symbol>& gen)
	{
		return std::all_of(rhs.begin(), rhs.end(), [&](const Symbol& s) {
			return s.isTerminal || gen.count(s);
		});
	}

	static std::set<Symbol> FindReachable(Grammar& g)
	{
		std::set reach = { g.GetStartSymbol() };
		bool changed = true;
		while (changed)
		{
			changed = false;
			for (const auto& r : g.GetRules())
			{
				if (!reach.count(r.lhs))
				{
					continue;
				}
				for (const auto& symbol : r.rhs)
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

	static void FilterGenerating(Grammar& src, Grammar& dst, const std::set<Symbol>& gen)
	{
		for (const auto& r : src.GetRules())
		{
			bool ok = gen.count(r.lhs);
			for (const auto& s : r.rhs)
			{
				if (!s.isTerminal && !gen.count(s))
				{
					ok = false;
				}
			}
			if (ok)
			{
				dst.AddRule(r.lhs, r.rhs);
			}
		}
	}

	static void FilterReachable(Grammar& src, Grammar& dst, const std::set<Symbol>& reach)
	{
		for (const auto& r : src.GetRules())
		{
			if (reach.count(r.lhs))
			{
				dst.AddRule(r.lhs, r.rhs);
			}
		}
	}
};

#endif // CYK_USELESSSYMBOLELIMINATOR_H
