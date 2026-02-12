#ifndef CYK_EPSILONELIMINATOR_H
#define CYK_EPSILONELIMINATOR_H
#include <utility>

#include "Grammar.h"

#include <algorithm>

class EpsilonEliminator
{
public:
	explicit EpsilonEliminator(Grammar g)
		: m_grammar(std::move(g))
	{
	}

	Grammar Execute()
	{
		FindNullable();
		Grammar result;
		result.SetNonTerminals(m_grammar.GetNonTerminals());
		result.SetTerminals(m_grammar.GetTerminals());

		TransformRules(result);
		HandleStart(result);
		Cleanup(result);

		return result;
	}

private:
	Grammar m_grammar;
	std::set<Symbol> m_empty;

	void FindNullable()
	{
		bool changed = true;
		const std::vector<Production> rules = m_grammar.GetRules();
		while (changed)
		{
			changed = false;
			for (const auto& rule : rules)
			{
				if (IsRhsNullable(rule.rhs) && m_empty.find(rule.lhs) == m_empty.end())
				{
					m_empty.insert(rule.lhs);
					changed = true;
				}
			}
		}
	}

	bool IsRhsNullable(const std::vector<Symbol>& rhs)
	{
		return std::all_of(rhs.begin(), rhs.end(), [this](const Symbol& s) {
			return !s.isTerminal && m_empty.find(s) != m_empty.end();
		});
	}

	void TransformRules(Grammar& result)
	{
		const std::vector<Production> rules = m_grammar.GetRules();
		for (const auto& rule : rules)
		{
			if (rule.rhs.empty())
			{
				continue;
			}
			std::vector<std::vector<Symbol>> combinations;
			GenerateCombinations(rule.rhs, 0, {}, combinations);
			for (auto& combination : combinations)
			{
				if (!combination.empty())
				{
					result.AddRule(rule.lhs, combination);
				}
			}
		}
	}

	void GenerateCombinations(
		const std::vector<Symbol>& rhs,
		const size_t index,
		std::vector<Symbol> current,
		std::vector<std::vector<Symbol>>& result)
	{
		if (index == rhs.size())
		{
			result.push_back(current);
			return;
		}

		current.push_back(rhs[index]);
		GenerateCombinations(rhs, index + 1, current, result);
		current.pop_back();

		if (m_empty.count(rhs[index]))
		{
			GenerateCombinations(rhs, index + 1, current, result);
		}
	}

	void HandleStart(Grammar& result)
	{
		Symbol oldStart = m_grammar.GetStartSymbol();
		const Symbol newStart = { oldStart.value + "1", false };
		if (m_empty.count(oldStart))
		{
			std::set<Symbol> nonTerminals = result.GetNonTerminals();
			nonTerminals.insert(newStart);
			result.SetNonTerminals(nonTerminals);
			result.SetStartSymbol(newStart);
			result.AddRule(newStart, { oldStart });
			result.AddRule(newStart, {});
		}
		else
		{
			result.SetStartSymbol(oldStart);
		}
	}

	static void Cleanup(Grammar& result)
	{
		std::vector<Production> rules = result.GetRules();
		std::sort(rules.begin(), rules.end(), [](const Production& a, const Production& b) {
			if (a.lhs.value != b.lhs.value)
			{
				return a.lhs.value < b.lhs.value;
			}
			return a.rhs < b.rhs;
		});
		rules.erase(std::unique(rules.begin(), rules.end(), [](const Production& a, const Production& b) {
			return a.lhs == b.lhs && a.rhs == b.rhs;
		}),
			rules.end());

		result.SetRules(rules);
	}
};

#endif // CYK_EPSILONELIMINATOR_H
