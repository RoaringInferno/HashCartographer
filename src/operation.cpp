#include "operation.hpp"

hash_value_t execute(const Operation operation, const hash_value_t lhs, const hash_value_t rhs)
{
    switch (operation)
    {
    case Operation::XOR:
        return lhs ^ rhs;
    case Operation::OR: 
        return lhs | rhs;
    case Operation::AND:
        return lhs & rhs;
    default:
        return 0;
    };
}

char get_operation_char(const Operation operation)
{
    switch (operation)
    {
    case Operation::XOR:
        return '^';
    case Operation::OR:
        return '|';
    case Operation::AND:
        return '&';
    default:
        return ' ';
    };
}
