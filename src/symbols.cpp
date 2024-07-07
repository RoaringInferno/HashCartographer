#include "symbols.hpp"

Symbols::Symbols(Args &args) : symbols(nullptr), word_count(args.count), min_word_length(args.args[0].length())
{
    for (unsigned int i = 1; i < word_count; ++i)
    {
        if (args.args[i].length() < min_word_length)
        {
            min_word_length = args.args[i].length();
        }
    }
    symbols = new uint8_t*[word_count];
    for (unsigned int i = 0; i < word_count; ++i)
    {
        symbols[i] = new uint8_t[args.args[i].length()];
        for (unsigned int j = 0; j < args.args[i].length(); ++j)
        {
            symbols[i][j] = args.args[i][j];
        }
    }
}

Symbols::~Symbols()
{
    for (unsigned int i = 0; i < word_count; ++i)
    {
        delete[] symbols[i];
    }
    delete[] symbols;
}

uint8_t Symbols::get_character(const unsigned int word, const unsigned int index) const
{
    return symbols[word][index];
}

uint8_t Symbols::pull_symbol(const unsigned int word, Symbol &symbol) const
{
    if (symbol.is_constant) { return symbol.value; }
    return symbols[word][symbol.character];
}
