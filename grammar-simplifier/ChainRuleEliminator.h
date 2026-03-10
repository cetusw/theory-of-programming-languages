#pragma once
#include "grammar/Grammar.h"

class ChainRuleEliminator
{
public:
	explicit ChainRuleEliminator(Grammar g);

	Grammar Execute() const;

private:
	Grammar m_grammar{};

	std::set<std::pair<Symbol, Symbol>> BuildChainPairs() const;
	bool AddPath(std::set<std::pair<Symbol, Symbol>>& pairs, const Symbol& lhs, const Symbol& rhs) const;
};
