#ifndef CYK_EPSILONELIMINATOR_H
#define CYK_EPSILONELIMINATOR_H

#include "../Logger/Logger.h"
#include "../Types/Grammar.h"

class EpsilonEliminator : Logger
{
public:
	explicit EpsilonEliminator(Grammar g);

	Grammar Execute();

	static void Cleanup(Grammar& result);

private:
	Grammar m_grammar;
	std::set<Symbol> m_empty;

	void FindNullable();
	bool IsRhsNullable(const std::vector<Symbol>& rhs);
	void TransformRules(Grammar& result);
	void GenerateCombinations(
		const std::vector<Symbol>& rhs,
		size_t index,
		std::vector<Symbol> current,
		std::vector<std::vector<Symbol>>& result);
	void HandleStart(Grammar& result) const;
};

#endif // CYK_EPSILONELIMINATOR_H
