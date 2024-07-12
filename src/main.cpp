#include "input_parsing.hpp"
#include "daemon.hpp"
#include <iostream>

int main(int argc, char** argv) {
    Args args;
    parse_args(args, argc, argv);
    std::cout << "For closed set of strings:\n";
    for (word_index_t i = 0; i < args.count; i++)
    {
        std::cout << "- " << args.args[i] << std::endl;
    }

    HashFunctionTree tree(args.args, args.count, args.depth);

    for (operation_index_t i = 1; i <= args.depth; i++) {
        for (uint8_t operation_id = 0; operation_id < OPERATION_COUNT; operation_id++)
        {
            Operation operation = OPERATIONS[operation_id]; // Get the operation
            operation_index_t depth_to_search = i-1; // The depth to search is the previous layer
            
            tree.evaluate(operation, depth_to_search); // Evaluate the operation
        }
    }
}