#pragma once

#include "Production.h"
#include "Symbol.h"
#include <set>
#include <vector>

class Grammar
{
public:
	void AddRule(const Symbol& lhs, const std::vector<Symbol>& rhs);

	[[nodiscard]] std::set<Symbol> GetTerminals() const;
	[[nodiscard]] std::set<Symbol> GetNonTerminals() const;
	[[nodiscard]] std::vector<Production> GetRules() const;
	[[nodiscard]] Symbol GetStartSymbol() const;
	void SetStartSymbol(const Symbol& symbol);

private:
	std::set<Symbol> m_terminals;
	std::set<Symbol> m_nonTerminals;
	std::vector<Production> m_rules;
	Symbol m_startSymbol;
};
