#pragma once

#include "Printer/CYKTablePrinter.h"
#include "Types/Grammar.h"
#include <string>

class ILogger
{
public:
	virtual ~ILogger() = default;
	virtual void LogStep(const std::string& message) = 0;
	virtual void LogGrammar(const Grammar& grammar, const std::string& title) = 0;
	virtual void LogTable(const Table& table, int n, const std::string& title) = 0;
};
