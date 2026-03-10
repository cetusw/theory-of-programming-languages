#include "EpsilonEliminator.h"
#include "printer/GrammarPrinter.h"
#include <algorithm>
#include <iostream>

EpsilonEliminator::EpsilonEliminator(Grammar grammar)
	: m_grammar(std::move(grammar))
{
}

Grammar EpsilonEliminator::Execute()
{
	FindNullable();
	Grammar result;
	result.SetNonTerminals(m_grammar.GetNonTerminals());
	result.SetTerminals(m_grammar.GetTerminals());

	TransformRules(result);
	HandleStart(result);
	Cleanup(result);

	GrammarPrinter::Print(std::cout, result);
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
			if (IsRhsNullable(rule.GetRhs()) && !m_empty.contains(rule.GetLhs()))
			{
				m_empty.insert(rule.GetLhs());
				changed = true;
			}
		}
	}
}

bool EpsilonEliminator::IsRhsNullable(const std::vector<Symbol>& rhs) const
{
	return std::ranges::all_of(rhs, [this](const Symbol& s) {
		return !s.IsTerminal() && m_empty.contains(s);
	});
}

void EpsilonEliminator::TransformRules(Grammar& result)
{
	const std::vector<Production> rules = m_grammar.GetRules();
	for (const auto& rule : rules)
	{
		if (rule.GetRhs().empty())
		{
			continue;
		}
		std::vector<std::vector<Symbol>> combinations;
		std::vector<Symbol> buffer;
		GenerateCombinations(rule.GetRhs(), 0, buffer, combinations);
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
	std::vector<Symbol>& current,
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

	if (m_empty.contains(rhs[index]))
	{
		GenerateCombinations(rhs, index + 1, current, result);
	}
}

void EpsilonEliminator::HandleStart(Grammar& result) const
{
	Symbol oldStart = m_grammar.GetStartSymbol();
	const Symbol newStart = { oldStart.GetValue() + "1", false };
	if (m_empty.contains(oldStart))
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
	std::ranges::sort(rules, [](const Production& a, const Production& b) {
		if (a.GetLhs().GetValue() != b.GetLhs().GetValue())
		{
			return a.GetLhs().GetValue() < b.GetLhs().GetValue();
		}
		return a.GetRhs() < b.GetRhs();
	});
	rules.erase(std::ranges::unique(rules, [](const Production& a, const Production& b) {
		return a.GetLhs() == b.GetLhs() && a.GetRhs() == b.GetRhs();
	}).begin(),
		rules.end());

	result.SetRules(rules);
}