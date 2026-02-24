#include "EpsilonEliminator.h"

#include <algorithm>

EpsilonEliminator::EpsilonEliminator(Grammar g)
	: m_grammar(std::move(g))
{
}

Grammar EpsilonEliminator::Execute()
{
	LogStep("Eliminating empty rules");

	FindNullable();
	Grammar result;
	result.SetNonTerminals(m_grammar.GetNonTerminals());
	result.SetTerminals(m_grammar.GetTerminals());

	TransformRules(result);
	HandleStart(result);
	Cleanup(result);

	LogGrammar(result, "Grammar with no epsilon");
	return result;
}

void EpsilonEliminator::FindNullable()
{
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (const auto& rule : m_grammar.GetRules())
		{
			if (IsRhsNullable(rule.GetRhs()) && m_empty.find(rule.GetLhs()) == m_empty.end())
			{
				m_empty.insert(rule.GetLhs());
				changed = true;
			}
		}
	}
}

bool EpsilonEliminator::IsRhsNullable(const std::vector<Symbol>& rhs)
{
	return std::all_of(rhs.begin(), rhs.end(), [this](const Symbol& s) {
		return !s.IsTerminal() && m_empty.find(s) != m_empty.end();
	});
}

void EpsilonEliminator::TransformRules(Grammar& result)
{
	const std::vector<Production> rules = m_grammar.GetRules();
	for (const auto& rule : rules)
	{
		if (rule.IsEpsilon())
		{
			continue;
		}
		std::vector<std::vector<Symbol>> combinations;
		GenerateCombinations(rule.GetRhs(), 0, {}, combinations);
		for (auto& combination : combinations)
		{
			if (!combination.empty())
			{
				result.AddRule(rule.GetLhs(), combination);
			}
		}
	}
}

void EpsilonEliminator::GenerateCombinations(
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

void EpsilonEliminator::HandleStart(Grammar& result) const
{
	Symbol oldStart = m_grammar.GetStartSymbol();
	const Symbol newStart = { oldStart.GetValue() + "1", false };
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

void EpsilonEliminator::Cleanup(Grammar& result)
{
	std::vector<Production> rules = result.GetRules();
	std::sort(rules.begin(), rules.end(), [](const Production& a, const Production& b) {
		if (a.GetLhs().GetValue() != b.GetLhs().GetValue())
		{
			return a.GetLhs().GetValue() < b.GetLhs().GetValue();
		}
		return a.GetRhs() < b.GetRhs();
	});
	rules.erase(std::unique(rules.begin(), rules.end(), [](const Production& a, const Production& b) {
		return a.GetLhs() == b.GetLhs() && a.GetRhs() == b.GetRhs();
	}),
		rules.end());

	result.SetRules(rules);
}