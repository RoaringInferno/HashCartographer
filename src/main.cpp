#include "input_parsing.hpp"
#include "daemon.hpp"

#include <iostream>

void eval_operation(HashFunctionTree& tree, const uint8_t operation_id)
{
    
}

int main(int argc, char** argv)
{
    Args args;
    parse_args(args, argc, argv);
    std::cout << "For closed set of strings:\n";
    for (word_index_t i = 0; i < args.count; i++)
    {
        std::cout << "- " << args.args[i] << std::endl;
    }

    HashFunctionTree tree(args.args, args.count, args.depth);

    for (uint8_t operation_id = 0; operation_id < OPERATION_COUNT; operation_id++) {
        //eval_operation(tree, operation_id);
        for (operation_index_t i = 1; i <= tree.get_total_depth(); i++)
        {
            Operation operation = OPERATIONS[operation_id]; // Get the operation
            operation_index_t depth_to_search = i-1; // The depth to search is the previous layer
            
            tree.evaluate(operation, depth_to_search); // Evaluate the operation
        }
        std::cout << "Tree created\n"; // Debugging
    }

    Symbol fastest = tree.get_fastest();
    std::cout << "Fastest: " << fastest.to_string() << "\n";
    Symbol tightest = tree.get_tightest();
    std::cout << "Tightest: " << tightest.to_string() << "\n";
}