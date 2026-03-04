#include "CYKTablePrinter.h"

#include <ostream>

void CYKTablePrinter::Print(std::ostream& os, const Table& table, const int n)
{
	if (n == 0)
	{
		return;
	}

	const size_t cellWidth = CalculateMaxCellWidth(table, n);

	PrintHeader(os);
	PrintAllRows(os, table, n, cellWidth);
	PrintFooter(os, n, cellWidth);
}

size_t CYKTablePrinter::CalculateMaxCellWidth(const Table& table, const int n)
{
	size_t maxWidth = 5;
	for (int len = 1; len <= n; ++len)
	{
		for (int i = 0; i <= n - len; ++i)
		{
			maxWidth = std::max(maxWidth, SetToString(table[len][i]).length() + 2);
		}
	}
	return maxWidth;
}

void CYKTablePrinter::PrintHeader(std::ostream& os)
{
	os << "CYK Table:" << std::endl;
}

void CYKTablePrinter::PrintAllRows(
	std::ostream& os, const Table& table, const int n, const size_t cellWidth)
{
	for (int len = n; len >= 1; --len)
	{
		PrintSeparatorLine(os, n, cellWidth);
		PrintDataRow(os, table, n, len, cellWidth);
	}
	PrintSeparatorLine(os, n, cellWidth);
}

void CYKTablePrinter::PrintFooter(std::ostream& os, const int n, const size_t cellWidth)
{
	os << " ";
	for (int i = 0; i < n; ++i)
	{
		const size_t pad = cellWidth / 2;
		os << std::string(pad, ' ') << i << std::string(cellWidth - pad, ' ');
	}
	os << " (pos)\n\n";
}

void CYKTablePrinter::PrintSeparatorLine(std::ostream& os, const int n, const size_t cellWidth)
{
	os << "+";
	for (int i = 0; i < n; ++i)
	{
		os << std::string(cellWidth, '-') << "+";
	}
	os << "\n";
}

void CYKTablePrinter::PrintDataRow(
	std::ostream& os, const Table& table, const int n, const int len, const size_t cellWidth)
{
	os << "|";
	for (int i = 0; i < n; ++i)
	{
		std::string content = (i <= n - len) ? SetToString(table[len][i]) : "";
		PrintFormattedCell(os, content, cellWidth);
	}
	os << " len=" << len << "\n";
}

void CYKTablePrinter::PrintFormattedCell(
	std::ostream& os, const std::string& content, const size_t width)
{
	const size_t padding = width - content.length();
	const size_t padLeft = padding / 2;
	const size_t padRight = padding - padLeft;

	os << std::string(padLeft, ' ')
	   << content
	   << std::string(padRight, ' ')
	   << "|";
}

std::string CYKTablePrinter::SetToString(const std::set<Symbol>& symbols)
{
	if (symbols.empty())
	{
		return "{}";
	}

	std::string res;
	for (auto it = symbols.begin(); it != symbols.end(); ++it)
	{
		res += it->GetValue() + (std::next(it) == symbols.end() ? "" : ",");
	}
	return res;
}