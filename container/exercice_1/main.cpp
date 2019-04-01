#include <cstdint>
#include <iostream>
#include <iomanip>
#include <string>
#include <random>
#include <chrono>
#include <array>
#include <vector>
#include <list>
#include <deque>
#include <forward_list>

struct ContainerTest
{
public:
    using chrono = std::chrono::high_resolution_clock;
    using time_point = std::chrono::time_point<chrono>;
    static const size_t nb_element = 10000;
    static const size_t nb_loop = 5;

    enum Method
    {
        PUSH_BACK,
        PUSH_FRONT,
        INSERT_BACK,
        INSERT_FRONT,
        INSERT_RANDOM,
        RESERVE_PUSH_BACK,
        RESERVE_PUSH_FRONT,
        RESERVE_INSERT_BACK,
        RESERVE_INSERT_FRONT,
        RESERVE_INSERT_RANDOM,
        POP_BACK,
        POP_FRONT,
        ERASE_FRONT,
        ERASE_BACK,
        ERASE_RANDOM,
        ACCESS_CONTINUOUS,
        ACCESS_RANDOM,
        CLEAR,
        SORT,
        MAX
    };

    std::string name;
    std::array<uint32_t, Method::MAX> durationArray;


    ContainerTest(std::string const& name)
        : name(name)
    {
        ::memset(durationArray.data(), 0, sizeof(durationArray));
    }

    inline uint32_t duration(time_point const& begin)
    {
        uint32_t d = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(chrono::now() - begin).count());
        return d > 0 ? d : 1;
    }

    inline void duration(time_point const& begin, Method enumValue, uint32_t addedDuration = 0)
    {
        auto d = duration(begin) + addedDuration;
        if (durationArray[enumValue] < d)
            durationArray[enumValue] = d;
    }

    template <class Container>
    void reserve(Container& container)
    {
        _reserve = true;
        auto time = chrono::now();
        container.reserve(nb_element);
        _reserveDuration = duration(time);
    }

    template <class Container>
    void sort(Container& container)
    {
        auto time = chrono::now();
        std::sort(container.begin(), container.end());
        duration(time, Method::SORT);
    }

    template <class Container>
    void list_sort(Container& container)
    {
        auto time = chrono::now();
        container.sort();
        duration(time, Method::SORT);
    }

    template <class Container>
    void push_back(Container& container)
    {
        auto time = chrono::now();
        for (size_t i = 0; i < nb_element; ++i)
        {
            container.push_back(_generator());
        }

        if (_reserve)
        {
            _reserve = false;
            duration(time, Method::RESERVE_PUSH_BACK, _reserveDuration);
        }
        else
            duration(time, Method::PUSH_BACK);
    }

    template <class Container>
    void push_front(Container& container)
    {
        auto time = chrono::now();
        for (size_t i = 0; i < nb_element; ++i)
        {
            container.push_front(_generator());
        }

        if (_reserve)
        {
            _reserve = false;
            duration(time, Method::RESERVE_PUSH_FRONT, _reserveDuration);
        }
        else
            duration(time, Method::PUSH_FRONT);
    }

    template <class Container>
    void pop_back(Container& container)
    {
        auto time = chrono::now();
        while (!container.empty())
        {
            _total += container.back();
            container.pop_back();
        }
        duration(time, Method::POP_BACK);
    }

    template <class Container>
    void pop_front(Container& container)
    {
        auto time = chrono::now();
        while (!container.empty())
        {
            _total += container.front();
            container.pop_front();
        }
        duration(time, Method::POP_FRONT);
    }

    template <class Container>
    void insert_back(Container& container)
    {
        auto time = chrono::now();
        for (size_t i = 0; i < nb_element; ++i)
        {
            container.insert(container.end(), _generator());
        }

        if (_reserve)
        {
            _reserve = false;
            duration(time, Method::RESERVE_INSERT_BACK, _reserveDuration);
        }
        else
            duration(time, Method::INSERT_BACK);
    }

    template <class Container>
    void insert_front(Container& container)
    {
        auto time = chrono::now();
        for (size_t i = 0; i < nb_element; ++i)
        {
            container.insert(container.begin(), _generator());
        }

        if (_reserve)
        {
            _reserve = false;
            duration(time, Method::RESERVE_INSERT_FRONT, _reserveDuration);
        }
        else
            duration(time, Method::INSERT_FRONT);
    }

    template <class Container>
    void insert_random(Container& container)
    {
        std::mt19937 random(42);

        auto time = chrono::now();
        container.insert(container.begin(), _generator());
        for (size_t i = 0; i < nb_element - 1; ++i)
        {
            auto it = it_increment(container.begin(), random() % container.size());
            container.insert(it, _generator());
        }

        if (_reserve)
        {
            _reserve = false;
            duration(time, Method::RESERVE_INSERT_RANDOM, _reserveDuration);
        }
        else
            duration(time, Method::INSERT_RANDOM);
    }

    void insert_random(std::forward_list<uint32_t>& container)
    {
        std::mt19937 random(42);

        auto time = chrono::now();
        container.push_front(_generator());
        for (size_t i = 0; i < nb_element - 1; ++i)
        {
            auto it = it_increment(container.before_begin(), random() % (i + 1));
            container.insert_after(it, _generator());
        }

        if (_reserve)
        {
            _reserve = false;
            duration(time, Method::RESERVE_INSERT_RANDOM, _reserveDuration);
        }
        else
            duration(time, Method::INSERT_RANDOM);
    }

    template <class Container>
    void erase_back(Container& container)
    {
        auto time = chrono::now();
        while (!container.empty())
        {
            container.erase(--container.end());
        }

        duration(time, Method::ERASE_BACK);
    }

    template <class Container>
    void erase_front(Container& container)
    {
        auto time = chrono::now();
        while (!container.empty())
        {
            container.erase(container.begin());
        }
        duration(time, Method::ERASE_FRONT);
    }

    void erase_front(std::forward_list<uint32_t>& container)
    {
        auto time = chrono::now();
        while (!container.empty())
        {
            container.erase_after(container.before_begin());
        }
        duration(time, Method::ERASE_FRONT);
    }

    template <class Container>
    void erase_random(Container& container)
    {
        std::mt19937 random(42);

        auto time = chrono::now();
        while (container.size() > 1)
        {
            auto it = it_increment(container.begin(), random() % container.size());
            container.erase(it);
        }
        container.erase(container.begin());
        duration(time, Method::ERASE_RANDOM);
    }

    void erase_random(std::forward_list<uint32_t>& container)
    {
        std::mt19937 random(42);

        auto time = chrono::now();
        uint32_t size = nb_element - 1;
        while (size > 0)
        {
            auto it = it_increment(container.before_begin(), random() % size);
            container.erase_after(it);
            --size;
        }
        while (!container.empty())
            container.erase_after(container.before_begin());
        duration(time, Method::ERASE_RANDOM);
    }

    template <class Container>
    void access_continuous(Container& container)
    {
        std::mt19937 random(42);

        auto time = chrono::now();
        for (auto nb : container)
        {
            _total += nb;
            _total ^= random();
        }
        duration(time, Method::ACCESS_CONTINUOUS);
    }

    template <class Container>
    void access_random(Container& container)
    {
        std::mt19937 random(42);

        auto time = chrono::now();
        for (size_t i = 0; i < nb_element; ++i)
        {
            auto it = it_increment(container.begin(), random() % nb_element);
            _total += *it;
        }
        duration(time, Method::ACCESS_RANDOM);
    }

    template <class Container>
    void clear(Container& container)
    {
        auto time = chrono::now();
        container.clear();
        duration(time, Method::CLEAR);
    }

    template <class Container>
    void clearMemory(Container& container)
    {
        container.clear();
        container.shrink_to_fit();
    }

    static void writeHeader(std::ostream& stream)
    {
        stream << std::setw(12) << "";
        stream << std::setw(12) << "push_bk";
        stream << std::setw(12) << "push_ft";
        stream << std::setw(12) << "ins_bk";
        stream << std::setw(12) << "ins_ft";
        stream << std::setw(12) << "ins_rand";
        stream << std::setw(12) << "r+push_bk";
        stream << std::setw(12) << "r+push_ft";
        stream << std::setw(12) << "r+ins_bk";
        stream << std::setw(12) << "r+ins_ft";
        stream << std::setw(12) << "r+ins_rand";
        stream << std::setw(12) << "pop_bk";
        stream << std::setw(12) << "pop_ft";
        stream << std::setw(12) << "erase_bk";
        stream << std::setw(12) << "erase_ft";
        stream << std::setw(12) << "erase_rand";
        stream << std::setw(12) << "access_con";
        stream << std::setw(12) << "access_rand";
        stream << std::setw(12) << "clear";
        stream << std::setw(12) << "sort";
        stream << std::endl;
    }

    void writeResult(std::ostream& stream) const
    {
        stream << std::setw(12) << this->name;
        for (size_t i = 0; i < Method::MAX; ++i)
        {
            if (durationArray[i] != 0)
                stream << std::setw(12) << durationArray[i];
            else
                stream << std::setw(12) << "";
        }
        stream << std::endl;
    }

    template <class Container>
    void avoidCompilerOptimization(Container& container)
    {
        for (auto nb : container)
        {
            _total += nb ^ _total;
        }
    }

private:
    template <typename T_Iterator>
    T_Iterator it_increment(T_Iterator it, size_t value)
    {
        return it + value;
    }

    template <typename T_Iterator>
    T_Iterator list_it_increment(T_Iterator it, size_t value)
    {
        while (value > 0)
        {
            --value;
            ++it;
        }
        return it;
    }

    std::list<uint32_t>::iterator it_increment(std::list<uint32_t>::iterator it, size_t value)
    {
        return list_it_increment(it, value);
    }

    std::forward_list<uint32_t>::iterator it_increment(std::forward_list<uint32_t>::iterator it, size_t value)
    {
        return list_it_increment(it, value);
    }

    uint64_t _total = 0;
    bool _reserve = false;
    uint32_t _reserveDuration = 0;
    std::mt19937 _generator;
};

void test_vector(std::ostream& stream)
{
    ContainerTest containerTest("vector");
    std::vector<uint32_t> container;

    for (size_t i = 0; i < ContainerTest::nb_loop; ++i)
    {
        // Test push_back
        containerTest.push_back(container);
        containerTest.avoidCompilerOptimization(container);
        // Test push_back + reserve
        containerTest.clearMemory(container);
        containerTest.reserve(container);
        containerTest.push_back(container);
        containerTest.avoidCompilerOptimization(container);

        // Test insert_back
        containerTest.clearMemory(container);
        containerTest.insert_back(container);
        containerTest.avoidCompilerOptimization(container);
        // Test insert_front
        containerTest.clearMemory(container);
        containerTest.insert_front(container);
        containerTest.avoidCompilerOptimization(container);
        // Test insert_random
        containerTest.clearMemory(container);
        containerTest.insert_random(container);
        containerTest.avoidCompilerOptimization(container);

        // Test insert_back + reserve
        containerTest.clearMemory(container);
        containerTest.reserve(container);
        containerTest.insert_back(container);
        containerTest.avoidCompilerOptimization(container);
        // Test insert_front + reserve
        containerTest.clearMemory(container);
        containerTest.reserve(container);
        containerTest.insert_front(container);
        containerTest.avoidCompilerOptimization(container);
        // Test insert_random + reserve
        containerTest.clearMemory(container);
        containerTest.reserve(container);
        containerTest.insert_random(container);
        containerTest.avoidCompilerOptimization(container);

        // Test pop_back
        containerTest.pop_back(container);
        // Test erase_back
        containerTest.push_back(container);
        containerTest.avoidCompilerOptimization(container);
        containerTest.erase_back(container);
        // Test erase_front
        containerTest.push_back(container);
        containerTest.avoidCompilerOptimization(container);
        containerTest.erase_front(container);
        // Test erase_random
        containerTest.push_back(container);
        containerTest.avoidCompilerOptimization(container);
        containerTest.erase_random(container);

        // Test access_continuous
        containerTest.push_back(container);
        containerTest.avoidCompilerOptimization(container);
        containerTest.access_continuous(container);
        // Test access_random
        containerTest.access_random(container);

        // Test clear
        containerTest.clear(container);

        // Test sort
        container.clear();
        containerTest.push_back(container);
        containerTest.avoidCompilerOptimization(container);
        containerTest.sort(container);

        container.clear();
    }

    containerTest.writeResult(stream);
}

void test_list(std::ostream& stream)
{
    ContainerTest containerTest("list");
    std::list<uint32_t> container;

    for (size_t i = 0; i < ContainerTest::nb_loop; ++i)
    {
        // Test push_back
        containerTest.push_back(container);
        containerTest.avoidCompilerOptimization(container);
        // Test push_front
        container.clear();
        containerTest.push_front(container);
        containerTest.avoidCompilerOptimization(container);

        // Test insert_back
        container.clear();
        containerTest.insert_back(container);
        containerTest.avoidCompilerOptimization(container);
        // Test insert_front
        container.clear();
        containerTest.insert_front(container);
        containerTest.avoidCompilerOptimization(container);
        // Test insert_random
        container.clear();
        containerTest.insert_random(container);
        containerTest.avoidCompilerOptimization(container);

        // Test pop_front
        containerTest.pop_front(container);
        // Test pop_back
        containerTest.push_back(container);
        containerTest.pop_back(container);

        // Test erase_back
        containerTest.push_back(container);
        containerTest.erase_back(container);
        // Test erase_front
        containerTest.push_back(container);
        containerTest.erase_front(container);
        // Test erase_random
        containerTest.push_back(container);
        containerTest.erase_random(container);

        // Test access_continuous
        containerTest.push_back(container);
        containerTest.access_continuous(container);
        // Test access_random
        containerTest.access_random(container);

        // Test clear
        containerTest.clear(container);

        // Test sort
        container.clear();
        containerTest.push_back(container);
        containerTest.list_sort(container);

        container.clear();
    }

    containerTest.writeResult(stream);
}

void test_deque(std::ostream& stream)
{
    ContainerTest containerTest("deque");
    std::deque<uint32_t> container;

    for (size_t i = 0; i < ContainerTest::nb_loop; ++i)
    {
        // Test push_back
        containerTest.push_back(container);
        containerTest.avoidCompilerOptimization(container);
        // Test push_front
        containerTest.clearMemory(container);
        containerTest.push_front(container);
        containerTest.avoidCompilerOptimization(container);

        // Test insert_back
        containerTest.clearMemory(container);
        containerTest.insert_back(container);
        // Test insert_front
        containerTest.avoidCompilerOptimization(container);
        containerTest.clearMemory(container);
        containerTest.insert_front(container);
        // Test insert_random
        containerTest.avoidCompilerOptimization(container);
        containerTest.clearMemory(container);
        containerTest.insert_random(container);
        containerTest.avoidCompilerOptimization(container);

        // Test pop_front
        containerTest.pop_front(container);
        // Test pop_back
        containerTest.push_back(container);
        containerTest.pop_back(container);

        // Test erase_back
        containerTest.push_back(container);
        containerTest.erase_back(container);
        // Test erase_front
        containerTest.push_back(container);
        containerTest.erase_front(container);
        // Test erase_random
        containerTest.push_back(container);
        containerTest.erase_random(container);

        // Test access_continuous
        containerTest.push_back(container);
        containerTest.access_continuous(container);
        // Test access_random
        containerTest.access_random(container);

        // Test clear
        containerTest.clear(container);

        // Test sort
        container.clear();
        containerTest.push_back(container);
        containerTest.sort(container);

        container.clear();
    }

    containerTest.writeResult(stream);
}

void test_forward_list(std::ostream& stream)
{
    ContainerTest containerTest("forward_list");
    std::forward_list<uint32_t> container;

    for (size_t i = 0; i < ContainerTest::nb_loop; ++i)
    {
        // Test push_front
        containerTest.push_front(container);

        // Test insert_random
        containerTest.avoidCompilerOptimization(container);
        container.clear();
        containerTest.insert_random(container);

        // Test pop_front
        containerTest.pop_front(container);

        // Test erase_front
        containerTest.push_front(container);
        containerTest.erase_front(container);
        // Test erase_random
        containerTest.push_front(container);
        containerTest.erase_random(container);

        // Test access_continuous
        containerTest.push_front(container);
        containerTest.access_continuous(container);
        // Test access_random
        containerTest.access_random(container);

        // Test clear
        containerTest.clear(container);

        // Test sort
        container.clear();
        containerTest.push_front(container);
        containerTest.list_sort(container);

        container.clear();
    }

    containerTest.writeResult(stream);
}

int main(int, char**)
{
    std::ostream& stream = std::cout;

    ContainerTest::writeHeader(stream);
    test_vector(stream);
    test_list(stream);
    test_deque(stream);
    test_forward_list(stream);
    return 0;
}