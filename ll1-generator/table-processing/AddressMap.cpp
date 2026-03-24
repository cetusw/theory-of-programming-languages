#include "AddressMap.h"

void AddressMap::Calculate(const std::vector<Production>& rules)
{
	int currentId = 1;
	for (const auto& p : rules)
	{
		if (!ntRows.contains(p.GetLhs().GetValue()))
		{
			ntRows[p.GetLhs().GetValue()] = currentId;
		}
		currentId++;
	}
	for (int i = 0; i < static_cast<int>(rules.size()); ++i)
	{
		rhsRows[i] = currentId;
		const size_t rhsSize = rules[i].GetRhs().size();
		currentId += (rhsSize == 0) ? 1 : static_cast<int>(rhsSize);
	}
	acceptRowId = currentId;
}