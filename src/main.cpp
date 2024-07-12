#include "input_parsing.hpp"
#include "daemon.hpp"

#include <iostream>
#include <thread>
#include <chrono>

void eval_operation(HashFunctionTree& tree, const operation_index_t depth_to_search, const Operation operation)
{
    tree.evaluate(operation, depth_to_search);
}

void pull_fastest(const HashFunctionTree& tree, Symbol& fastest)
{
    fastest = tree.get_fastest();
}

void pull_tightest(const HashFunctionTree& tree, Symbol& tightest)
{
    tightest = tree.get_tightest();
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

    auto start = std::chrono::high_resolution_clock::now();
    for (operation_index_t depth = 1; depth <= tree.get_total_depth(); depth++) {
        std::vector<std::thread> threads(OPERATION_COUNT);
        for (uint8_t operation_id = 0; operation_id < OPERATION_COUNT; operation_id++)
        {
            Operation operation = OPERATIONS[operation_id]; // Get the operation
            operation_index_t depth_to_search = depth-1; // The depth to search is the previous layer
            
            threads[operation_id] = std::thread(eval_operation, std::ref(tree), depth_to_search, operation);
        }
        for (std::thread& thread : threads)
        {
            thread.join();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    Symbol fastest, tightest;
    pull_fastest(tree, fastest);
    pull_tightest(tree, tightest);
    std::cout << "Fastest: " << fastest.to_string() << "\n";
    std::cout << "Tightest: " << tightest.to_string() << "\n";

    //std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
}