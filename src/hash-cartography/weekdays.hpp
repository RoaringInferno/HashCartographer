#pragma once

#include <string>

class Weekday
{
public:
    const enum Value
    {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday,
        Err_Weekday,
    } value;

    constexpr Weekday(Value value = Err_Weekday) : value(value) {}
    constexpr Weekday(Weekday& other) : value(other.value) {}

    std::string to_string()
    {
        switch (value)
        {
        case Weekday::Monday:
            return "Monday";
        case Weekday::Tuesday:
            return "Tuesday";
        case Weekday::Wednesday:
            return "Wednesday";
        case Weekday::Thursday:
            return "Thursday";
        case Weekday::Friday:
            return "Friday";
        case Weekday::Saturday:
            return "Saturday";
        case Weekday::Sunday:
            return "Sunday";
        default:
            return "Err_Weekday";
        };
    };

    constexpr bool operator==(Weekday& other) { return (value == other.value); }
};

namespace hacr
{
    /**
     * A hash class for Weekday strings.
     * 
     * For for "Weekday" strings and "Wkdy" strings:
     * - "Monday" or "Mon"
     * - "Tuesday" or "Tue"
     * - "Wednesday" or "Wed"
     * - "Thursday" or "Thu"
     * - "Friday" or "Fri"
     * - "Saturday" or "Sat"
     * - "Sunday" or "Sun"
     * 
     * Hash function XORs the ASCII values of the first and third characters in the string:
     * - "Mon" = 77 XOR 110 = 01001101 XOR 01101110 = 00100011 = 35
     * - "Tue" = 84 XOR 101 = 01010100 XOR 01100101 = 00110001 = 49
     * - "Wed" = 87 XOR 100 = 01010111 XOR 01100100 = 00110011 = 51
     * - "Thu" = 84 XOR 117 = 01010100 XOR 01110101 = 00100001 = 33
     * - "Fri" = 70 XOR 105 = 01000110 XOR 01101001 = 00101111 = 47
     * - "Sat" = 83 XOR 116 = 01010011 XOR 01110100 = 00100111 = 39
     * - "Sun" = 83 XOR 110 = 01010011 XOR 01101110 = 00111101 = 61
     */
    class Wkdy_hash
    {
    private:
        const uint8_t hash;

        static const uint8_t cycle_cost = 1; // The cycle cost of the hash function's execution.
        static const uint8_t mem_overhead = 61 - 7; // The memory overhead of the hash table (largest hash value - 7).
    public:
        static constexpr uint8_t get_cycle_cost() { return cycle_cost; }
        static constexpr uint8_t get_mem_overhead() { return mem_overhead; }

        static uint8_t encode(const std::string& text)
        {
            uint8_t result = text[0] ^ text[2];
            return result;
        }
        static uint8_t encode (const Weekday weekday)
        {
            switch (weekday.value)
            {
            case Weekday::Monday:
                return 35;
            case Weekday::Tuesday:
                return 49;
            case Weekday::Wednesday:
                return 51;
            case Weekday::Thursday:
                return 33;
            case Weekday::Friday:
                return 47;
            case Weekday::Saturday:
                return 39;
            case Weekday::Sunday:
                return 61;
            default:
                return 0;
            };
        }
        static Weekday decode(uint8_t hash)
        {
            switch (hash)
            {
            case 35:
                return Weekday::Monday;
            case 49:
                return Weekday::Tuesday;
            case 51:
                return Weekday::Wednesday;
            case 33:
                return Weekday::Thursday;
            case 47:
                return Weekday::Friday;
            case 39:
                return Weekday::Saturday;
            case 61:
                return Weekday::Sunday;
            default:
                return Weekday::Err_Weekday;
            };
        }

        Wkdy_hash(Weekday weekday = Weekday::Sunday) : hash(encode(weekday)) {}
        Wkdy_hash(std::string& text) : hash(encode(text)) {}
        Wkdy_hash(Wkdy_hash& other) : hash(other.hash) {}

        bool operator==(Wkdy_hash& other) { return (hash == other.hash); }

        std::string to_string() const { return decode(hash).to_string(); }

        uint8_t get_hash() const { return hash; }
        Weekday get_weekday() const { return decode(hash); }
    };
} // namespace hacr

#undef HASH_TEMPLATE