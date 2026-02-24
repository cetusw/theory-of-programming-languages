#include "Production.h"

#include <utility>

Production::Production(Symbol lhs, std::vector<Symbol> rhs)
	: m_lhs(std::move(lhs))
	, m_rhs(std::move(rhs))
{
}

bool Production::IsEpsilon() const
{
	return m_rhs.empty();
}

bool Production::IsChainRule() const
{
	return m_rhs.size() == 1 && !m_rhs[0].IsTerminal();
}

bool Production::IsTerminalRule() const
{
	return m_rhs.size() == 1 && m_rhs[0].IsTerminal();
}

bool Production::IsInChomskyForm() const
{
	if (IsTerminalRule())
	{
		return true;
	}
	if (m_rhs.size() == 2 && !m_rhs[0].IsTerminal() && !m_rhs[1].IsTerminal())
	{
		return true;
	}
	return false;
}

Symbol Production::GetLhs() const
{
	return m_lhs;
}

std::vector<Symbol> Production::GetRhs() const
{
	return m_rhs;
}