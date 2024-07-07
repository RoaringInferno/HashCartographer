#include "hash_function_tree.hpp"
#include "operations.hpp"

struct ConstantSymbolIterator
{
    uint8_t value = 0;

    void generate(Symbol &symbol)
    {
        symbol.is_constant = true;
        symbol.value = value;
    }
    void increment()
    {
        value++;
    }
    void reset()
    {
        value = 0;
    }
};

struct VariableSymbolIterator
{
    unsigned int character = 0;

    void generate(Symbol &symbol)
    {
        symbol.is_constant = false;
        symbol.character = character;
    }
    void increment()
    {
        character++;
    }
    void reset()
    {
        character = 0;
    }
};

void single_layer_hash_function_search(MetaData &meta_data)
{
    auto test_hash_function = [&](SingleOpHashFunction &hash_function) {
        bool valid =  hash_function.evaluate(meta_data.symbols);
        if (valid) meta_data.solutions.push_back(hash_function);
    };

    for (const Operation op : operations)
    {
        VariableSymbolIterator lhs_iterator;
        SingleOpHashFunction hash_function(meta_data.word_count);
        hash_function.operation.operation = op; // Set the operation

        for (unsigned int i = 0; i < meta_data.symbols.get_min_word_length(); i++) // Iterate across all inputted characters
        {
            ConstantSymbolIterator const_rhs_iterator;
            lhs_iterator.generate(hash_function.operation.lhs);
            // First, try every constant value
            for (unsigned int j = 0; j < 256; j++)
            {
                const_rhs_iterator.generate(hash_function.operation.rhs); // Try for each constant
                test_hash_function(hash_function);
                const_rhs_iterator.increment();
            }

            // Then, try every variable value
            VariableSymbolIterator variable_rhs_iterator;
            for (unsigned int j = 0; j < meta_data.symbols.get_min_word_length(); j++)
            {
                variable_rhs_iterator.generate(hash_function.operation.rhs); // Try for each variable
                if (hash_function.operation.lhs.character == hash_function.operation.rhs.character) continue; // Skip if the same variable
                test_hash_function(hash_function);
                variable_rhs_iterator.increment();
            }

            lhs_iterator.increment();
        }
    }
}