#include "input_parsing.hpp"
#include "hash_function_tree.hpp"

#include <iostream>

int main(int argc, char** argv) {
    Args args;
    parse_args(args, argc, argv);

    MetaData meta_data = {Symbols(args), args.count};
    single_layer_hash_function_search(meta_data);

    // Print Solutions
    for (const SingleOpHashFunction& solution : meta_data.solutions)
    {
        Symbol lhs = solution.operation.lhs;
        Symbol rhs = solution.operation.rhs;
        Operation op = solution.operation.operation;

        std::cout << "Solution found: ";
        // Print lhs
        if (lhs.is_constant)
        {
            std::cout << std::to_string(lhs.value);
        }
        else
        {
            std::cout << "text[" << std::to_string(lhs.character) << "]";
        }
        // Print operation
        switch (op)
        {
            case Operation::AND:
                std::cout << " & ";
                break;
            case Operation::OR:
                std::cout << " | ";
                break;
            case Operation::XOR:
                std::cout << " ^ ";
                break;
            default:
                std::cout << " ??? ";
                break;
        }
        // Print rhs
        if (rhs.is_constant)
        {
            std::cout << std::to_string(rhs.value);
        }
        else
        {
            std::cout << "text[" << std::to_string(rhs.character) << "]";
        }
    }

    return 0;
}