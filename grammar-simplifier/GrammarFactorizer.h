#pragma once
#include "grammar/Grammar.h"
#include <map>
#include <vector>

using GroupedRules = std::map<Symbol, std::vector<std::vector<Symbol>>>;

class GrammarFactorizer
{
public:
	explicit GrammarFactorizer(Grammar g);
	Grammar Execute();

private:
	Grammar m_grammar;
	int m_newSymbolCounter = 0;
	std::map<std::vector<std::vector<Symbol>>, Symbol> m_factorCache;

	bool FactorizeStep(GroupedRules& rules);
	static std::vector<Symbol> FindLongestCommonPrefix(const std::vector<std::vector<Symbol>>& rhsList);
	void ApplyFactorization(const Symbol& A, const std::vector<Symbol>& prefix, GroupedRules& rules);

	static GroupedRules GroupRules(const std::vector<Production>& rules);
	static Grammar Finalize(const Grammar& src, const GroupedRules& rules);
	static Symbol CreateNewSymbol(const Symbol& oldSymbol, int index);
};