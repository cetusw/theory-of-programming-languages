#pragma once
#include "TableRow.h"
#include <fstream>

class CsvExporter
{
public:
	static void Export(const Table& table, const std::string& filename);

private:
	static std::string FormatGuiding(const std::set<std::string>& symbols);
};