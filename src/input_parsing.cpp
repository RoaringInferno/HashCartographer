#include "input_parsing.hpp"

#include <vector>
#include <iostream>

void parse_args(Args &args, int argc, char **argv)
{
    std::vector<std::string> args_vector;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-') // Option
        {
            if (argv[i][1] == '-') // Long option
            {
                // Long option
                std::string long_option = std::string(argv[i]).substr(2);

                if (long_option == "depth") // --depth
                {
                    args.depth = std::stoi(argv[++i]);
                    continue;
                }
                std::cout << "Unknown option: " << long_option << ". Ignoring...\n"; // Unknown option
            }
            else // Short option
            {
                for (int j = 1; argv[i][j] != '\0'; j++) // Flags
                {
                    args.flags[argv[i][j]] = 1;
                }
            }
        }
        else if (argv[i][0] == '\\') // Escape character
        {
            args_vector.push_back(std::string(argv[i]).substr(1));
        }
        else // Regular argument
        {
            args_vector.push_back(std::string(argv[i]));
        }
    }

    args.count = args_vector.size();
    args.args = new std::string[args.count];
    for (word_index_t i = 0; i < args.count; i++)
    {
        args.args[i] = args_vector[i];
    }

    return;
}