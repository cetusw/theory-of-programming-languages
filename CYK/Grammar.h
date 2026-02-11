#ifndef CYK_GRAMMAR_H
#define CYK_GRAMMAR_H

#include <set>
#include <string>
#include <vector>

struct Symbol
{
	std::string value{};
	bool isTerminal{};
	bool operator<(const Symbol& other) const
	{
		if (value != other.value)
		{
			return value < other.value;
		}
		return isTerminal < other.isTerminal;
	}
	bool operator==(const Symbol& other) const
	{
		return value == other.value && isTerminal == other.isTerminal;
	}
};

struct Production
{
	Symbol lhs;
	std::vector<Symbol> rhs;
};

class Grammar
{
public:
	void AddRule(const Symbol& lhs, const std::vector<Symbol>& rhs)
	{
		m_rules.push_back({ lhs, rhs });
		m_nonTerminals.insert(lhs);
		for (const auto& s : rhs)
		{
			if (s.isTerminal)
				m_terminals.insert(s);
			else
				m_nonTerminals.insert(s);
		}
	}

	std::set<Symbol> GetTerminals()
	{
		return m_terminals;
	}

	std::set<Symbol> GetNonTerminals()
	{
		return m_nonTerminals;
	}

	std::vector<Production> GetRules()
	{
		return m_rules;
	}

	Symbol GetStartSymbol()
	{
		return m_startSymbol;
	}

	void SetStartSymbol(const Symbol& symbol)
	{
		m_startSymbol = symbol;
	}

private:
	std::set<Symbol> m_terminals;
	std::set<Symbol> m_nonTerminals;
	std::vector<Production> m_rules;
	Symbol m_startSymbol;
};

#endif // CYK_GRAMMAR_H