#include "CsvExporter.h"

void CsvExporter::Export(const Table& table, const std::string& filename)
{
	std::ofstream file(filename);
	file << "No;Symbol;Guiding;Shift;Error;Pointer;Stack;End\n";
	for (const auto& row : table)
	{
		file << row.id << ";"
			 << row.currentSymbol << ";"
			 << FormatGuiding(row.guidingSymbols) << ";"
			 << (row.shift ? "Да" : "Нет") << ";"
			 << (row.error ? "Да" : "Нет") << ";"
			 << (row.pointer > 0 ? std::to_string(row.pointer) : "") << ";"
			 << (row.pushStackAddress > 0 ? std::to_string(row.pushStackAddress) : "") << ";"
			 << (row.endOfParsing ? "Да" : "Нет") << "\n";
	}
}

std::string CsvExporter::FormatGuiding(const std::set<std::string>& symbols)
{
	std::string res;
	for (const auto& symbol : symbols)
	{
		res += symbol + " ";
	}
	return res;
}