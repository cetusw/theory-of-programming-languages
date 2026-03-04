#pragma once

#include "ILogger.h"
#include "Types/Grammar.h"
#include <string>

class NullLogger final : public ILogger
{
	void LogStep(const std::string&) override;
	void LogGrammar(const Grammar&, const std::string&) override;
};
