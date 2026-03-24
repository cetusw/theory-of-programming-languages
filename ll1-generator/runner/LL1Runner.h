#pragma once
#include "../table-processing/TableRow.h"
#include "ParseTreeNode.h"
#include <iostream>
#include <stack>

class LL1Runner
{
public:
	static std::shared_ptr<ParseTreeNode> Run(const Table& table, const std::vector<std::string>& tokens);

private:
	static bool ProcessMatch(const TableRow& row, size_t& tIdx, std::stack<int>& addrSt, int& rIdx, std::stack<std::shared_ptr<ParseTreeNode>>& treeSt);
};
