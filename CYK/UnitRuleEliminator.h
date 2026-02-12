#ifndef CYK_UNITRULEELIMINATOR_H
#define CYK_UNITRULEELIMINATOR_H

#include "Grammar.h"

class UnitRuleEliminator
{
public:
	explicit UnitRuleEliminator(Grammar g)
		: m_grammar(std::move(g))
	{
	}

	Grammar Execute()
	{
		auto unitPairs = BuildUnitPairs();
		Grammar result;
		result.SetStartSymbol(m_grammar.GetStartSymbol());
		result.SetTerminals(m_grammar.GetTerminals());
		result.SetNonTerminals(m_grammar.GetNonTerminals());

		for (const auto& [A, B] : unitPairs)
		{
			for (const auto& rule : m_grammar.GetRules())
			{
				if (rule.lhs == B && !IsUnitRule(rule))
				{
					result.AddRule(A, rule.rhs);
				}
			}
		}
		return result;
	}

private:
	Grammar m_grammar{};

	static bool IsUnitRule(const Production& p)
	{
		return p.rhs.size() == 1 && !p.rhs[0].isTerminal;
	}

	std::set<std::pair<Symbol, Symbol>> BuildUnitPairs()
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
				if (IsUnitRule(rule))
				{
					if (AddPath(pairs, rule.lhs, rule.rhs[0]))
					{
						changed = true;
					}
				}
			}
		}
		return pairs;
	}

	bool AddPath(std::set<std::pair<Symbol, Symbol>>& pairs, const Symbol& lhs, const Symbol& rhs)
	{
		bool added = false;
		for (auto& NT : m_grammar.GetNonTerminals())
		{
			if (pairs.count({ NT, lhs }) && !pairs.count({ NT, rhs }))
			{
				pairs.insert({ NT, rhs });
				added = true;
			}
		}
		return added;
	}
};

#endif // CYK_UNITRULEELIMINATOR_H
