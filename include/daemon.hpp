#pragma once

#include "parameters.hpp"
#include "symbol.hpp"
#include "operation.hpp"

#include <vector>

typedef uint32_t hash_index_t;

struct HashFunctionTreeLayer
{
    std::vector<Symbol> symbol;
    hash_index_t size;
    hash_index_t opchunk_size;

    HashFunctionTreeLayer();
    HashFunctionTreeLayer(const hash_index_t size);

    void set_symbol(const Operation operation, const hash_index_t index, const Symbol& symbol);
};

class HashFunctionTree
{
private:
    std::vector<HashFunctionTreeLayer> hash_tree_depth; // The layers of the tree
    operation_index_t total_depth; // The total depth of the tree
    word_index_t word_count; // The number of words in the input
    character_index_t character_count; // The number of interesting characters in each word
    character_index_t *character_hash_table; // The hash table for the characters. Translates character index into their position in the original word
public:
    HashFunctionTree() = delete;
    HashFunctionTree(const std::string* words, const word_index_t word_count, const operation_index_t depth);

    void evaluate(const Operation operation, const operation_index_t depth_to_search); // const

    hash_index_t get_opchunk_size(const operation_index_t layer_index) const;

    Symbol get_fastest() const;
};