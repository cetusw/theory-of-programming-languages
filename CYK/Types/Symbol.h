#ifndef CYK_SYMBOL_H
#define CYK_SYMBOL_H
#include <string>

class Symbol
{
public:
	Symbol(std::string value, bool isTerminal);
	Symbol();

	bool operator<(const Symbol& other) const;
	bool operator==(const Symbol& other) const;

	[[nodiscard]] std::string GetValue() const;
	[[nodiscard]] bool IsTerminal() const;
	void SetValue(const std::string& value);
	void SetIsTerminal(bool isTerminal);

private:
	std::string m_value{};
	bool m_isTerminal{};
};

#endif // CYK_SYMBOL_H
