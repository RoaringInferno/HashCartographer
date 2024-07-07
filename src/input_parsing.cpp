#include "input_parsing.hpp"

#include <vector>

void parse_args(Args &args, int argc, char **argv)
{
    std::vector<std::string> args_vector;

    for (int i = 1; i < argc; i++)
    {
        args_vector.push_back(argv[i]);
    }

    args.count = args_vector.size();
    args.args = args_vector.data();

    return;
}