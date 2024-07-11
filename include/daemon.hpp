#pragma once

#include "parameters.hpp"
#include "symbol.hpp"
#include "operation.hpp"

#include <vector>

typedef uint32_t hash_index_t;

struct HashFunctionTreeLayer
{
    Symbol* symbol;
    hash_index_t size;

    HashFunctionTreeLayer() = delete;
    HashFunctionTreeLayer(const hash_index_t size);
    HashFunctionTreeLayer(const HashFunctionTreeLayer& other);
    //HashFunctionTreeLayer(const operation_index_t depth, const word_index_t word_count);
    ~HashFunctionTreeLayer();
};

class HashFunctionTree
{
private:
    HashFunctionTreeLayer* hash_tree_depth;
    operation_index_t total_depth;
    word_index_t word_count;
    character_index_t character_count; // The number of interesting characters in each word
    character_index_t *character_hash_table; // The hash table for the characters. Translates character index into their position in the original word
public:
    HashFunctionTree() = delete;
    HashFunctionTree(const std::string* words, const word_index_t word_count, const operation_index_t depth);
    ~HashFunctionTree();

    void evaluate(const Operation operation, const operation_index_t depth_to_search, Symbol* write_to) const;

    HashFunctionTreeLayer* get_layers();

    void export_solutions(std::vector<std::vector<Symbol>> &solutions) const;
};