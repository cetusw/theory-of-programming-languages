#pragma once
#include <set>
#include <vector>

#include "grammar/Grammar.h"

class EpsilonEliminator
{
public:
	explicit EpsilonEliminator(Grammar grammar);
	Grammar Execute();

private:
	Grammar m_grammar;
	std::set<Symbol> m_empty;

	void FindNullable();
	[[nodiscard]] bool IsRhsNullable(const std::vector<Symbol>& rhs) const;
	void TransformRules(Grammar& result);
	void GenerateCombinations(
		const std::vector<Symbol>& rhs,
		size_t index,
		std::vector<Symbol>& current,
		std::vector<std::vector<Symbol>>& result);
	void HandleStart(Grammar& result) const;
	static void Cleanup(Grammar& result);
};
