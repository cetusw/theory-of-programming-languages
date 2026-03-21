#include "GuidingSetProvider.h"
#include <algorithm>

GuidingSetProvider::GuidingSetProvider(Grammar g)
	: m_grammar(std::move(g))
{
}

GuidingSetsMap GuidingSetProvider::Execute()
{
	CalculateNullable();
	CalculateFirstSets();
	CalculateFollowSets();

	GuidingSetsMap result;
	const auto& rules = m_grammar.GetRules();
	for (int i = 0; i < rules.size(); ++i)
	{
		result.push_back(CreateGuidingSet(rules[i], i + 1));
	}
	return result;
}

void GuidingSetProvider::CalculateNullable()
{
	while (UpdateNullable())
	{
	}
}

void GuidingSetProvider::CalculateFirstSets()
{
	for (const auto& t : m_grammar.GetTerminals())
	{
		m_first[t] = { t };
	}
	m_first[m_epsilon] = { m_epsilon };
	while (UpdateFirstSets())
	{
	}
}

void GuidingSetProvider::CalculateFollowSets()
{
	m_follow[m_grammar.GetStartSymbol()].insert(m_eof);
	while (UpdateFollowSets())
	{
	}
}

bool GuidingSetProvider::UpdateNullable()
{
	bool changed = false;
	for (const auto& rule : m_grammar.GetRules())
	{
		if (m_nullable.contains(rule.GetLhs()))
		{
			continue;
		}

		const bool allNullable = std::all_of(rule.GetRhs().begin(), rule.GetRhs().end(),
			[this](const Symbol& s) { return m_nullable.contains(s); });

		if (allNullable)
		{
			m_nullable.insert(rule.GetLhs());
			changed = true;
		}
	}
	return changed;
}

bool GuidingSetProvider::UpdateFirstSets()
{
	bool changed = false;
	for (const auto& rule : m_grammar.GetRules())
	{
		auto& lhsFirst = m_first[rule.GetLhs()];
		const size_t sizeBefore = lhsFirst.size();

		std::set<Symbol> rhsFirst = GetFirstOfSequence(rule.GetRhs());
		lhsFirst.insert(rhsFirst.begin(), rhsFirst.end());

		if (lhsFirst.size() > sizeBefore)
		{
			changed = true;
		}
	}
	return changed;
}

bool GuidingSetProvider::UpdateFollowSets()
{
	bool changed = false;
	for (const auto& rule : m_grammar.GetRules())
	{
		ProcessRuleFollow(rule, changed);
	}
	return changed;
}

void GuidingSetProvider::ProcessRuleFollow(const Production& rule, bool& changed)
{
	const auto& rhs = rule.GetRhs();
	for (size_t i = 0; i < rhs.size(); ++i)
	{
		if (rhs[i].IsTerminal())
		{
			continue;
		}

		auto& currentFollow = m_follow[rhs[i]];
		const size_t sizeBefore = currentFollow.size();

		std::vector rest(rhs.begin() + i + 1, rhs.end());
		std::set<Symbol> nextFirst = GetFirstOfSequence(rest);

		for (const auto& s : nextFirst)
		{
			if (s.GetValue() != m_epsilon.GetValue())
			{
				currentFollow.insert(s);
			}
		}

		if (nextFirst.contains(m_epsilon))
		{
			const auto& lhsFollow = m_follow[rule.GetLhs()];
			currentFollow.insert(lhsFollow.begin(), lhsFollow.end());
		}
		if (currentFollow.size() > sizeBefore)
		{
			changed = true;
		}
	}
}

std::set<Symbol> GuidingSetProvider::GetFirstOfSequence(const std::vector<Symbol>& sequence)
{
	if (sequence.empty())
	{
		return { m_epsilon };
	}

	std::set<Symbol> result;
	bool allNullable = true;
	for (const auto& sym : sequence)
	{
		for (const auto& s : m_first[sym])
		{
			if (s.GetValue() != m_epsilon.GetValue())
			{
				result.insert(s);
			}
		}
		if (!m_nullable.contains(sym))
		{
			allNullable = false;
			break;
		}
	}
	if (allNullable)
	{
		result.insert(m_epsilon);
	}
	return result;
}

GuidingSet GuidingSetProvider::CreateGuidingSet(const Production& rule, const int index)
{
	std::set<Symbol> firstAlpha = GetFirstOfSequence(rule.GetRhs());
	GuidingSet gs{ rule.GetLhs(), rule.GetRhs(), {}, index };

	if (firstAlpha.contains(m_epsilon))
	{
		for (const auto& s : firstAlpha)
		{
			if (s.GetValue() != m_epsilon.GetValue())
			{
				gs.selectSet.insert(s);
			}
		}
		const auto& followLhs = m_follow[rule.GetLhs()];
		gs.selectSet.insert(followLhs.begin(), followLhs.end());
	}
	else
	{
		gs.selectSet = std::move(firstAlpha);
	}
	return gs;
}