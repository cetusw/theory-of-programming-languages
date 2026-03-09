#include "Grammar.h"

#include <utility>

void Grammar::AddRule(const Symbol& lhs, const std::vector<Symbol>& rhs)
{
	m_rules.emplace_back(lhs, rhs);
	m_nonTerminals.insert(lhs);
	for (const auto& symbol : rhs)
	{
		if (symbol.IsTerminal())
		{
			m_terminals.insert(symbol);
		}
		else
		{
			m_nonTerminals.insert(symbol);
		}
	}
}

std::set<Symbol> Grammar::GetTerminals() const
{
	return m_terminals;
}

std::set<Symbol> Grammar::GetNonTerminals() const
{
	return m_nonTerminals;
}

std::vector<Production> Grammar::GetRules() const
{
	return m_rules;
}

Symbol Grammar::GetStartSymbol() const
{
	return m_startSymbol;
}

void Grammar::SetStartSymbol(const Symbol& symbol)
{
	m_startSymbol = symbol;
}