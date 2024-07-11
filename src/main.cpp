#include "input_parsing.hpp"
#include "daemon.hpp"
#include <iostream>

int main(int argc, char** argv) {
    Args args;
    parse_args(args, argc, argv);

    HashFunctionTree tree(args.args, args.count, args.depth);

    HashFunctionTreeLayer* layers = tree.get_layers(); // Get the layers of the tree
    for (operation_index_t i = 1; i < args.depth; i++) {
        HashFunctionTreeLayer* current_layer = layers + i; // Get the current layer
        hash_index_t operation_chunk_size = current_layer->size / OPERATION_COUNT; // The size of each operation chunk
        for (uint8_t operation_id = 0; operation_id < OPERATION_COUNT; operation_id++)
        {
            Operation operation = OPERATIONS[operation_id]; // Get the operation
            operation_index_t depth_to_search = i-1; // The depth to search is the previous layer
            Symbol* write_to = layers[i].symbol + operation_chunk_size * operation_id; // Get the write-to location
            
            tree.evaluate(operation, depth_to_search, write_to); // Evaluate the operation
        }
    }

    std::vector<std::vector<Symbol>> solutions;
    tree.export_solutions(solutions);

    for (operation_index_t depth = 0; depth < solutions.size(); depth++)
    {
        std::cout << "Solutions for Depth " << depth << ":" << std::endl;
        for (hash_index_t i = 0; i < solutions[depth].size(); i++)
        {
            std::cout << solutions[depth][i].get_symbol() << std::endl;
        }
    }

    return 0;
}