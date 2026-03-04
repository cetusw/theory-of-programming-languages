#include "ConsoleLogger.h"

#include "Printer/GrammarPrinter.h"

#include <iostream>

void ConsoleLogger::LogStep(const std::string& message)
{
	std::cout << "[STEP] " << message << std::endl;
}

void ConsoleLogger::LogGrammar(const Grammar& grammar, const std::string& title)
{
	std::cout << "[GRAMMAR] " << title << std::endl;
	GrammarPrinter::Print(std::cout, grammar);
}

void ConsoleLogger::LogTable(const Table& table, const int n, const std::string& title)
{
	std::cout << "[TABLE] " << title << std::endl;
	CYKTablePrinter::Print(std::cout, table, n);
}