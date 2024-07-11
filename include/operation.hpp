#pragma once

#include "parameters.hpp"

static const Operation OPERATIONS[] = { Operation::XOR, Operation::OR, Operation::AND };
static const uint8_t OPERATION_COUNT = 3;

enum class Operation
{
    XOR,
    OR,
    AND
};

hash_value_t execute(const Operation operation, const hash_value_t lhs, const hash_value_t rhs);

char get_operation_char(const Operation operation);