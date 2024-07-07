#include "operations.hpp"

uint8_t execute(Operation operation, uint8_t rhs, uint8_t lhs)
{
    switch (operation)
    {
    case Operation::XOR:
        return rhs ^ lhs;
    case Operation::AND:
        return rhs & lhs;
    case Operation::OR:
        return rhs | lhs;
    default:
        return 0;
    }
}