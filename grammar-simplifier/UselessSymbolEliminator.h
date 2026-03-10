#pragma once
#include "grammar/Grammar.h"

class UselessSymbolEliminator
{
public:
	explicit UselessSymbolEliminator(Grammar g);

	Grammar Execute();

private:
	Grammar m_grammar;
	std::set<Symbol> m_generating;

	void FindGenerating();
	[[nodiscard]] bool AllRhsGenerating(const std::vector<Symbol>& rhs) const;
	static std::set<Symbol> FindReachable(Grammar& g);
	void FilterGenerating(const Grammar& src, Grammar& dst) const;
	static void FilterReachable(const Grammar& src, Grammar& dst, const std::set<Symbol>& reach);
};
