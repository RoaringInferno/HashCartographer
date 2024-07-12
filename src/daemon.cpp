#include "daemon.hpp"

#include "operation.hpp"

#include <bitset>
#include <iostream>

/**
 * The sizes of the layers of the hash tree are as follows:
 * Depth 0: word_count
 * Depth 1: word_count choose 2 * OPERATION_COUNT
 * Depth 2: word_count choose 2 * OPERATION_COUNT choose 2 * OPERATION_COUNT
 * etc.
 * Every layer has a size of <previous> choose 2 * OPERATION_COUNT
 */

HashFunctionTreeLayer::HashFunctionTreeLayer() :
    size(0),
    symbol(),
    opchunk_size(0)
{
}

HashFunctionTreeLayer::HashFunctionTreeLayer(const hash_index_t size) : size(size),
                                                                        symbol(size),
                                                                        opchunk_size(size / OPERATION_COUNT)
{}

void HashFunctionTreeLayer::set_symbol(const Operation operation, const hash_index_t index, const Symbol &symbol)
{
    this->symbol[opchunk_size * static_cast<hash_index_t>(operation) + index] = symbol;
}

  // HashFunctionTree

inline hash_index_t next_depth_size(const hash_index_t previous_depth_size)
{
    hash_index_t permutation = (previous_depth_size * (previous_depth_size-1)) / 2;
    return (permutation + previous_depth_size * 256) * OPERATION_COUNT;
}

HashFunctionTree::HashFunctionTree(const std::string *words, const word_index_t word_count, const operation_index_t depth) :
    total_depth(depth),
    hash_tree_depth(depth + 1),
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
    hash_index_t current_depth_size = character_count;
    for (operation_index_t current_depth = 0; current_depth <= depth; current_depth++) // Set up the layers of the hash tree (allocate memory)
    {
        hash_tree_depth[current_depth] = *new HashFunctionTreeLayer(current_depth_size);
        if (current_depth < depth) current_depth_size = next_depth_size(current_depth_size);
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
    for (character_index_t characher_index = 0; characher_index < character_count; characher_index++) // For each character
    {
        std::vector<hash_value_t> symbol_value(word_count); // Allocate memory for the character hash
        hash_value_t min, max; // The minimum and maximum values, for tightness calculation
        std::bitset<256> unique; // The unique characters
        for (word_index_t j = 0; j < word_count; j++) // For each word
        {
            symbol_value[j] = words[j][character_hash_table[characher_index]]; // Set the character hash
            unique.set(symbol_value[j]); // Set the unique character
            if (j == 0) // If this is the first word
            {
                min = symbol_value[j]; // Set the minimum value
                max = symbol_value[j]; // Set the maximum value
            }
            else // If this is not the first word
            {
                if (symbol_value[j] < min) // If the value is less than the minimum
                {
                    min = symbol_value[j]; // Set the minimum value
                }
                if (symbol_value[j] > max) // If the value is greater than the maximum
                {
                    max = symbol_value[j]; // Set the maximum value
                }
            }
        }
        hash_tree_depth[0].symbol[characher_index] = Symbol(
            "[" + std::to_string(character_hash_table[characher_index]) + "]", // symbol
            symbol_value, // value
            word_count, // word_count
            unique.count() == word_count, // valid_solution
            setting::character_read_opcount_cost, // opcount
            max - min // tightness
        );
    }
}



void HashFunctionTree::evaluate(const Operation operation, const operation_index_t depth_to_search) // const
{
    hash_index_t write_to_index = 0;

    // For every combination of 2 symbols in the depth to search
    for (hash_index_t lhs = 0; lhs < hash_tree_depth[depth_to_search].size; lhs++) // For each symbol
    {
        for (hash_index_t rhs = lhs+1; rhs < hash_tree_depth[depth_to_search].size; rhs++) // For each symbol after the current one
        {
            // Generate a Symbol
            std::vector<hash_value_t> symbol_value = std::vector<hash_value_t>(this->word_count); // Allocate memory for the symbol value array
            std::bitset<256> unique; // The unique characters
            hash_value_t min, max; // The minimum and maximum values, for tightness calculation
            for (word_index_t i = 0; i < this->word_count; i++) // For each word
            {
                symbol_value[i] = execute(
                    operation,
                    this->hash_tree_depth[depth_to_search].symbol[lhs].get_value(i), // lhs
                    this->hash_tree_depth[depth_to_search].symbol[rhs].get_value(i) // rhs
                ); // Set the symbol value
                unique.set(symbol_value[i]); // Set the unique character
                if (i == 0) // If this is the first word
                {
                    min = symbol_value[i]; // Set the minimum value
                    max = symbol_value[i]; // Set the maximum value
                }
                else // If this is not the first word
                {
                    if (symbol_value[i] < min) // If the value is less than the minimum
                    {
                        min = symbol_value[i]; // Set the minimum value
                    }
                    if (symbol_value[i] > max) // If the value is greater than the maximum
                    {
                        max = symbol_value[i]; // Set the maximum value
                    }
                }
            }
            Symbol symbol_to_write = Symbol(
                "(" + this->hash_tree_depth[depth_to_search].symbol[lhs].get_symbol() + " " + get_operation_char(operation) + " " + this->hash_tree_depth[depth_to_search].symbol[rhs].get_symbol() + ")", // symbol
                symbol_value, // value
                this->word_count, // word_count
                unique.count() == this->word_count, // valid_solution
                this->hash_tree_depth[depth_to_search].symbol[lhs].get_opcount() + this->hash_tree_depth[depth_to_search].symbol[rhs].get_opcount() + setting::operation_opcount_cost, // opcount
                max-min // tightness
            );
            this->hash_tree_depth[depth_to_search+1].set_symbol(operation, write_to_index, symbol_to_write); // Set the symbol
            write_to_index++; // Increment the write_to index
        }
        for (uint16_t rhs = 0; rhs < 256; rhs++) // For each character
        {
            // Generate a Symbol
            std::vector<hash_value_t> symbol_value = std::vector<hash_value_t>(this->word_count); // Allocate memory for the symbol value array
            std::bitset<256> unique; // The unique characters
            hash_value_t min, max; // The minimum and maximum values, for tightness calculation
            for (word_index_t i = 0; i < this->word_count; i++) // For each word
            {
                symbol_value[i] = execute(
                    operation,
                    this->hash_tree_depth[depth_to_search].symbol[lhs].get_value(i), // lhs
                    rhs
                ); // Set the symbol value
                unique.set(symbol_value[i]); // Set the unique character

                if (i == 0) // If this is the first word
                {
                    min = symbol_value[i]; // Set the minimum value
                    max = symbol_value[i]; // Set the maximum value
                }
                else // If this is not the first word
                {
                    if (symbol_value[i] < min) // If the value is less than the minimum
                    {
                        min = symbol_value[i]; // Set the minimum value
                    }
                    if (symbol_value[i] > max) // If the value is greater than the maximum
                    {
                        max = symbol_value[i]; // Set the maximum value
                    }
                }
            }
            Symbol symbol_to_write = Symbol(
                "(" + this->hash_tree_depth[depth_to_search].symbol[lhs].get_symbol() + " " + get_operation_char(operation) + " " + std::to_string(rhs) + ")", // symbol
                symbol_value, // value
                this->word_count, // word_count
                unique.count() == this->word_count, // valid_solution
                this->hash_tree_depth[depth_to_search].symbol[lhs].get_opcount() + setting::operation_opcount_cost + setting::constant_read_opcount_cost, // opcount
                max-min // tightness
            );
            this->hash_tree_depth[depth_to_search+1].set_symbol(operation, write_to_index, symbol_to_write); // Set the symbol
            write_to_index++; // Increment the write_to index
        }
    }
}

hash_index_t HashFunctionTree::get_opchunk_size(const operation_index_t layer_index) const
{
    return this->hash_tree_depth[layer_index].opchunk_size;
}

Symbol HashFunctionTree::get_fastest() const
{
    hash_index_t min_opcount_index = 0;
    operation_index_t min_opcount_depth = 0;
    bool found = false;
    operation_index_t min_opcount = 0;
    for (operation_index_t i = 0; i <= this->total_depth; i++)
    {
        for (hash_index_t j = 0; j < this->hash_tree_depth[i].size; j++)
        {
            if (this->hash_tree_depth[i].symbol[j].is_valid_solution())
            {
                operation_index_t opcount = this->hash_tree_depth[i].symbol[j].get_opcount(); // Cache the opcount
                if (opcount <= i) // Perfect solution
                {
                    return this->hash_tree_depth[i].symbol[j];
                }
                if (opcount < min_opcount || !found) // If the opcount is less than the minimum opcount
                {
                    min_opcount_depth = i;
                    min_opcount_index = j;
                    min_opcount = opcount;
                    found = true;
                }
            }
        }
    }
    return this->hash_tree_depth[min_opcount_depth].symbol[min_opcount_index];
}

Symbol HashFunctionTree::get_tightest() const
{
    hash_index_t min_tightness_index = 0;
    operation_index_t min_tightness_depth = 0;
    bool found = false;
    hash_value_t min_tightness = 0;
    for (operation_index_t i = 0; i <= this->total_depth; i++)
    {
        for (hash_index_t j = 0; j < this->hash_tree_depth[i].size; j++)
        {
            if (this->hash_tree_depth[i].symbol[j].is_valid_solution())
            {
                hash_value_t tightness = this->hash_tree_depth[i].symbol[j].get_tightness(); // Cache the tightness
                if (tightness == word_count-1) // Perfect solution
                {
                    return this->hash_tree_depth[i].symbol[j];
                }
                if (tightness < min_tightness || !found) // If the tightness is less than the minimum tightness
                {
                    min_tightness_depth = i;
                    min_tightness_index = j;
                    min_tightness = tightness;
                    found = true;
                }
            }
        }
    }
    return this->hash_tree_depth[min_tightness_depth].symbol[min_tightness_index];
}