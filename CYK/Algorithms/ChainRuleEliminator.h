#ifndef CYK_CHAINRULEELIMINATOR_H
#define CYK_CHAINRULEELIMINATOR_H

#include "../Logger/Logger.h"
#include "../Types/Grammar.h"

class ChainRuleEliminator : public Logger
{
public:
	explicit ChainRuleEliminator(Grammar g);

	Grammar Execute() const;

private:
	Grammar m_grammar{};

	std::set<std::pair<Symbol, Symbol>> BuildChainPairs() const;
	bool AddPath(std::set<std::pair<Symbol, Symbol>>& pairs, const Symbol& lhs, const Symbol& rhs) const;
};

#endif // CYK_CHAINRULEELIMINATOR_H
