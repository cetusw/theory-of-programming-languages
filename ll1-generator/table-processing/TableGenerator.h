#pragma once
#include "AddressMap.h"
#include "TableRow.h"
#include "../grammar-parser/grammar/Grammar.h"
#include "../guiding-set-provider/GuidingSetProvider.h"

class TableGenerator
{
public:
	static Table Generate(const Grammar& grammar);

private:
	static void GenerateNTEntries(const Grammar& g, const GuidingSetsMap& sets, const AddressMap& addr, Table& table);
	static void GenerateRhsRows(const Grammar& g, const GuidingSetsMap& sets, const AddressMap& addr, Table& table, const GuidingSetProvider& provider, int acceptId);

	static TableRow CreateSymbolRow(int id, const Symbol& s, bool isLast, bool isStartRule, const AddressMap& addr, const GuidingSetProvider& provider, int forcedReturn);
	static TableRow CreateEpsilonRow(int id, const GuidingSet& gs);

	static bool IsLastAlternative(const std::vector<Production>& rules, size_t index);
};
