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
    size_t nb = (generator() % 1000000) + 100000;

    numbers.reserve(nb);
    for (size_t i = 0; i < nb; ++i)
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

void writeHashList(std::vector<uint64_t> const& hash_list)
{
    std::fstream output_file("output.txt", std::ios_base::out | std::ios_base::trunc);
    for (auto hash : hash_list)
    {
        output_file << hash << "\n";
    }
}

int main(int, char**)
{
    std::vector<uint64_t> hash_list;

    for (size_t i = 0; i < 1000; ++i)
    {
        auto numbers = generateNumbers(i);
        sortNumbers(numbers);
        hash_list.push_back(computeHash(numbers));

        if ((i % 100) == 0)
        {
            std::cout << '\r' << (i / 10) << "%";
        }
    }

    writeHashList(hash_list);

    return 0;
}