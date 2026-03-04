#pragma once

#include "Types/Symbol.h"
#include <set>
#include <vector>

using Table = std::vector<std::vector<std::set<Symbol>>>;

class CYKTablePrinter
{
public:
	static void Print(std::ostream& os, const Table& table, int n);

private:
	static size_t CalculateMaxCellWidth(const Table& table, int n);
	static void PrintHeader(std::ostream& os);
	static void PrintAllRows(std::ostream& os, const Table& table, int n, size_t cellWidth);
	static void PrintFooter(std::ostream& os, int n, size_t cellWidth);
	static void PrintSeparatorLine(std::ostream& os, int n, size_t cellWidth);
	static void PrintDataRow(std::ostream& os, const Table& table, int n, int len, size_t cellWidth);
	static void PrintFormattedCell(std::ostream& os, const std::string& content, size_t width);

	static std::string SetToString(const std::set<Symbol>& symbols);
};
