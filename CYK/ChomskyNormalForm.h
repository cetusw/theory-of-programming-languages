#ifndef CYK_CHOMSKYNORMALFORM_H
#define CYK_CHOMSKYNORMALFORM_H

#include "EpsilonEliminator.h"
#include "Grammar.h"
#include "UnitRuleEliminator.h"
#include "UselessSymbolEliminator.h"

#include <map>

class ChomskyNormalForm
{
public:
	explicit ChomskyNormalForm(Grammar g)
		: m_grammar(std::move(g))
	{
	}

	Grammar Convert()
	{
		Grammar g1 = EpsilonEliminator(m_grammar).Execute();
		Grammar g2 = UnitRuleEliminator(g1).Execute();
		Grammar g3 = UselessSymbolEliminator(g2).Execute();

		return FinalTransform(g3);
	}

private:
	Grammar m_grammar;
	int m_newNtCounter = 0;
	std::map<std::string, Symbol> m_terminalMap;

	Grammar FinalTransform(const Grammar& src)
	{
		Grammar result;
		result.SetStartSymbol(src.GetStartSymbol());
		for (const auto& rule : src.GetRules())
		{
			ProcessRule(result, rule);
		}
		return result;
	}

	void ProcessRule(Grammar& result, const Production& rule)
	{
		if (rule.rhs.size() <= 1)
		{
			result.AddRule(rule.lhs, rule.rhs);
		}
		else
		{
			const std::vector<Symbol> binarizedRhs = ReplaceTerminals(result, rule.rhs);
			SplitLongRule(result, rule.lhs, binarizedRhs);
		}
	}

	std::vector<Symbol> ReplaceTerminals(Grammar& result, const std::vector<Symbol>& rhs)
	{
		std::vector<Symbol> newRhs;
		for (const auto& s : rhs)
		{
			if (s.isTerminal)
			{
				if (!m_terminalMap.count(s.value))
				{
					Symbol newNt = { "U_" + s.value, false };
					m_terminalMap[s.value] = newNt;
					result.AddRule(newNt, { s });
				}
				newRhs.push_back(m_terminalMap[s.value]);
			}
			else
				newRhs.push_back(s);
		}
		return newRhs;
	}

	void SplitLongRule(Grammar& res, const Symbol& lhs, std::vector<Symbol> rhs)
	{
		if (rhs.size() <= 2)
		{
			res.AddRule(lhs, rhs);
			return;
		}
		Symbol nextNt = { "N" + std::to_string(m_newNtCounter++), false };
		res.AddRule(lhs, { rhs[0], nextNt });
		rhs.erase(rhs.begin());
		SplitLongRule(res, nextNt, rhs);
	}
};

#endif // CYK_CHOMSKYNORMALFORM_H
