#pragma once
#include "IGrammarTransformer.h"
#include "Logger/ILogger.h"
#include "Types/Grammar.h"
#include <memory>

class GrammarNormalizer final : public IGrammarTransformer
{
public:
	explicit GrammarNormalizer(std::shared_ptr<ILogger> logger);
	Grammar Execute(const Grammar& grammar) override;

private:
	std::shared_ptr<ILogger> m_logger;
};
