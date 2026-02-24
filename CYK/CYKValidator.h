#ifndef CYK_CYKVALIDATOR_H
#define CYK_CYKVALIDATOR_H

#include "Logger/Logger.h"
#include "Types/Grammar.h"

class CYKValidator : public Logger
{
public:
	explicit CYKValidator(Grammar g);

	[[nodiscard]] bool Validate(const std::string& input);

private:
	Grammar m_grammar;

	void FillBaseCase(std::vector<std::vector<std::set<Symbol>>>& table, const std::string& input) const;
	void FillRecursiveStep(std::vector<std::vector<std::set<Symbol>>>& table, int n) const;
	void ApplyRules(std::vector<std::vector<std::set<Symbol>>>& table, int len, int i, int k) const;
	[[nodiscard]] bool CheckEmptyString() const;
};

#endif // CYK_CYKVALIDATOR_H
