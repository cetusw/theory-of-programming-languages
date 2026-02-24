#ifndef CYK_PRODUCTION_H
#define CYK_PRODUCTION_H
#include "Symbol.h"

#include <vector>

// TODO поменять на Rule
class Production
{
public:
	Production(Symbol lhs, std::vector<Symbol> rhs);

	[[nodiscard]] bool IsEpsilon() const;
	[[nodiscard]] bool IsChainRule() const;
	[[nodiscard]] bool IsTerminalRule() const;
	[[nodiscard]] bool IsInChomskyForm() const;

	[[nodiscard]] Symbol GetLhs() const;
	[[nodiscard]] std::vector<Symbol> GetRhs() const;

private:
	Symbol m_lhs;
	std::vector<Symbol> m_rhs;
};

#endif // CYK_PRODUCTION_H
