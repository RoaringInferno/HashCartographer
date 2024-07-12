#include "input_parsing.hpp"
#include "daemon.hpp"

#include <iostream>
#include <thread>

void eval_operation(HashFunctionTree& tree, const operation_index_t depth, const uint8_t operation_id)
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

    for (operation_index_t depth = 1; depth <= tree.get_total_depth(); depth++) {
        std::vector<std::thread> threads(OPERATION_COUNT);
        for (uint8_t operation_id = 0; operation_id < OPERATION_COUNT; operation_id++)
        {
            Operation operation = OPERATIONS[operation_id]; // Get the operation
            operation_index_t depth_to_search = depth-1; // The depth to search is the previous layer
            
            //tree.evaluate(operation, depth_to_search); // Evaluate the operation
            threads[operation_id] = std::thread(eval_operation, std::ref(tree), depth_to_search, operation_id);
        }
        for (std::thread& thread : threads)
        {
            thread.join();
        }
    }

    Symbol fastest = tree.get_fastest();
    std::cout << "Fastest: " << fastest.to_string() << "\n";
    Symbol tightest = tree.get_tightest();
    std::cout << "Tightest: " << tightest.to_string() << "\n";
}