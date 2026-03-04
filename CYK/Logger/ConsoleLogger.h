#pragma once

#include "ILogger.h"
#include "Printer/CYKTablePrinter.h"

class ConsoleLogger final : public ILogger
{
	void LogStep(const std::string& message) override;
	void LogGrammar(const Grammar& grammar, const std::string& title) override;
	void LogTable(const Table& table, int n, const std::string& title) override;
};
