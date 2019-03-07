#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>

std::vector<uint32_t> generateNumbers(size_t seed)
{
    std::mt19937 generator(seed);
    std::vector<uint32_t> numbers;

    numbers.reserve(1000000);
    for (auto i = 0; i < 1000000; ++i)
    {
        numbers.emplace_back(generator());
    }
    return numbers;
}

void sortNumbers(std::vector<uint32_t>& numbers)
{
    std::sort(numbers.begin(), numbers.end());
}

uint64_t computeHash(std::vector<uint32_t> const& numbers)
{
    uint64_t hash = numbers.size();
    for (auto& i : numbers)
    {
        hash ^= i + 0x9e3779b9f83da20c + (hash << 6) + (hash >> 2);
    }
    return hash;
}

int main(int, char**)
{
    std::fstream    output_file("output.txt", std::ios_base::out | std::ios_base::trunc);

    for (size_t i = 0; i < 1000; ++i)
    {
        auto numbers = generateNumbers(i);
        sortNumbers(numbers);
        output_file << computeHash(numbers) << "\n";

        if ((i % 100) == 0)
        {
            std::cout << '\r' << (i / 10) << "%";
        }
    }

    return 0;
}