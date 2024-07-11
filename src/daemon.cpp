#include "daemon.hpp"

#include "operation.hpp"

#include <bitset>

/**
 * The sizes of the layers of the hash tree are as follows:
 * Depth 0: word_count
 * Depth 1: word_count choose 2 * OPERATION_COUNT
 * Depth 2: word_count choose 2 * OPERATION_COUNT choose 2 * OPERATION_COUNT
 * etc.
 * Every layer has a size of <previous> choose 2 * OPERATION_COUNT
 */

HashFunctionTreeLayer::HashFunctionTreeLayer(const hash_index_t size) : size(size), symbol((Symbol*)(malloc(sizeof(Symbol) * size))) {}

HashFunctionTreeLayer::HashFunctionTreeLayer(const HashFunctionTreeLayer &other) : size(other.size) {}

HashFunctionTreeLayer::~HashFunctionTreeLayer() { delete[] symbol; }


  // HashFunctionTree

hash_index_t next_depth(const hash_index_t current_depth)
{
    hash_index_t permutation = (current_depth * (current_depth-1)) / 2;
    return permutation * OPERATION_COUNT;
}

HashFunctionTree::HashFunctionTree(const std::string *words, const word_index_t word_count, const operation_index_t depth) :
    hash_tree_depth((HashFunctionTreeLayer*)(malloc(sizeof(HashFunctionTreeLayer) * depth))),
    total_depth(depth),
    word_count(word_count),
    character_count(0),
    character_hash_table(nullptr)
{
    // Get smallest word length
    character_index_t unfiltered_character_count = 0;
    for (hash_index_t i = 0; i < word_count; i++) // For each word
    {
        if (words[i].length() < unfiltered_character_count || unfiltered_character_count == 0)
        {
            unfiltered_character_count = words[i].length(); // Set the character count to the smallest possible
        }
    }

    // Check unique characters
    std::bitset<256> *unique_characters = new std::bitset<256>[unfiltered_character_count]; // For each character index, count the number of unique characters among all the words at that index
    for (hash_index_t i = 0; i < word_count; i++) // For each word
    {
        for (character_index_t j = 0; j < words[i].length(); j++) // For each character in the word
        {
            unique_characters[j].set(words[i][j]); // Set the character in the bitset
        }
    }

    // Count the number of unique characters
    character_index_t character_count = 0; // The number of unique character indexes (i.e. interesting ones)
    for (character_index_t i = 0; i < unfiltered_character_count; i++) // For each character
    {
        if (unique_characters[i].count() == 1) // If all the characters are the same
        {
            continue; // Skip this character
        }
        character_count++; // Increment the unique character index count
    }

    // Allocate memory
    hash_index_t current_depth_size = character_count + 256;
    for (operation_index_t current_depth = 0; current_depth < depth; current_depth++) // Set up the layers of the hash tree (allocate memory)
    {
        hash_tree_depth[current_depth] = HashFunctionTreeLayer(current_depth_size);
        if (current_depth < depth) current_depth_size = next_depth(current_depth_size);
    }

    // Set up the character hash table to translate character indexes into their position in the original word
    character_hash_table = new character_index_t[character_count]; // Allocate memory for the character hash table
    character_index_t temp_character_index = 0; // The current character index
    for (character_index_t i = 0; i < unfiltered_character_count; i++) // For each character
    {
        if (unique_characters[i].count() == 1) // If all the characters are the same
        {
            continue; // Skip this character
        }
        character_hash_table[temp_character_index] = i; // Set the character hash table
        temp_character_index++; // Increment the character index
    }

    // Set up the first layer of the hash tree
    // Set character symbols
    for (character_index_t first_layer_initialization_index = 0; first_layer_initialization_index < character_count; first_layer_initialization_index++) // For each character
    {
        hash_value_t* symbol_value = new hash_value_t[word_count]; // Allocate memory for the character hash
        for (word_index_t j = 0; j < word_count; j++) // For each word
        {
            symbol_value[j] = words[j][character_hash_table[first_layer_initialization_index]]; // Set the character hash
        }
        hash_tree_depth[0].symbol[first_layer_initialization_index] = Symbol(
            "word[" + std::to_string(character_hash_table[first_layer_initialization_index]) + "]",
            symbol_value,
            word_count
        );
    }

    // Set constant symbols
    for (character_index_t constant_index; constant_index < 256; constant_index++)
    {
        hash_value_t* symbol_value = new hash_value_t[word_count]; // Allocate memory for the character hash
        for (word_index_t j = 0; j < word_count; j++) // For each word
        {
            symbol_value[j] = constant_index; // Set the character hash
        }
        hash_tree_depth[0].symbol[character_count + constant_index] = Symbol(
            std::to_string(constant_index),
            symbol_value,
            word_count
        );
    }
}

HashFunctionTree::~HashFunctionTree() { delete[] hash_tree_depth; }

void HashFunctionTree::evaluate(const Operation operation, const operation_index_t depth_to_search, Symbol *write_to) const
{
    hash_index_t write_to_index = 0;
    // For every combination of 2 symbols in the depth to search
    for (character_index_t rhs = 0; rhs < hash_tree_depth[depth_to_search].size; rhs++) // For each symbol
    {
        for (character_index_t lhs = rhs+1; lhs < hash_tree_depth[depth_to_search].size; lhs++) // For each symbol after the current one
        {
            // Generate a Symbol
            hash_value_t* symbol_value = new hash_value_t[word_count]; // Allocate memory for the symbol value array
            for (word_index_t i = 0; i < word_count; i++) // For each word
            {
                symbol_value[i] = execute(
                    operation,
                    hash_tree_depth[depth_to_search].symbol[lhs].get_value(i),
                    hash_tree_depth[depth_to_search].symbol[rhs].get_value(i)
                ); // Set the symbol value
            }
            write_to[write_to_index] = Symbol(
                "(" + hash_tree_depth[depth_to_search].symbol[lhs].get_symbol() + " " + get_operation_char(operation) + " " + hash_tree_depth[depth_to_search].symbol[rhs].get_symbol() + ")",
                symbol_value,
                word_count
            ); // Write the symbol to the write_to array
            write_to_index++; // Increment the write_to index
        }
    }
}

HashFunctionTreeLayer *HashFunctionTree::get_layers()
{
    return this->hash_tree_depth;
}

void HashFunctionTree::export_solutions(std::vector<std::vector<Symbol>> &solutions) const
{
    for (operation_index_t i = 0; i < total_depth; i++)
    {
        if (solutions.size() == i)
        {
            solutions.push_back(std::vector<Symbol>());
        }
        for (hash_index_t j = 0; j < hash_tree_depth[i].size; j++)
        {
            if (hash_tree_depth[i].symbol[j].is_valid_solution())
            {
                solutions[i].push_back(hash_tree_depth[i].symbol[j]);
            }
        }
    }
}
