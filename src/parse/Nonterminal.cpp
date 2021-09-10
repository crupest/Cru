#include "cru/parse/Nonterminal.hpp"

namespace cru::parse {
Nonterminal::Nonterminal(Grammar* grammar, String name)
    : Symbol(grammar, std::move(name)) {}

Nonterminal::~Nonterminal() {}
}  // namespace cru::parse
