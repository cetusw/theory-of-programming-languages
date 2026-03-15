#pragma once
#include "grammar/Grammar.h"
#include <map>

using GroupedRules = std::map<Symbol, std::vector<std::vector<Symbol>>>;

class LeftRecursionEliminator
{
public:
	explicit LeftRecursionEliminator(Grammar g);
	[[nodiscard]] Grammar Execute() const;

private:
	static void EliminateImmediate(const Symbol& A, GroupedRules& rules);
	static void ReplaceIndirect(const Symbol& Ai, const Symbol& Aj, GroupedRules& rules);

	static GroupedRules GroupRules(const std::vector<Production>& rules);
	static Grammar Finalize(const Grammar& src, const GroupedRules& rules);
	static Symbol CreateNewSymbol(const Symbol& oldSymbol);
	std::vector<Symbol> GetOrderedNonTerminals() const;

	Grammar m_grammar;
};