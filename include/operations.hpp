#pragma once

#include <cstdint>

enum class Operation
{
    XOR,
    AND,
    OR,
};

const Operation operations[] = {Operation::XOR, Operation::AND, Operation::OR};

uint8_t execute(Operation operation, uint8_t rhs, uint8_t lhs);