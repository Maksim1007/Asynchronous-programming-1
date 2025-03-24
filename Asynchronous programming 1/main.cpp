#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <future>
#include <numeric>

template<typename Iterator, typename Function>
void parallel_for_each(Iterator first, Iterator last, Function func)
{
    const auto length = std::distance(first, last);

    if (length <= 1000) {
        std::for_each(first, last, func);
        return;
    }

    Iterator mid = first + length / 2;

    std::promise<void> p1;
    std::future<void> f1 = p1.get_future();

    std::thread([=, &p1]() mutable {
        parallel_for_each(first, mid, func);
        p1.set_value();
        }).detach();

    parallel_for_each(mid, last, func);

    f1.get();
}

int main()
{
    setlocale(LC_ALL, "Russian");
    std::vector<int> data(10000);
    std::iota(data.begin(), data.end(), 1);

    parallel_for_each(data.begin(), data.end(), [](int& value)
        {
            value *= value;
        });

    std::cout << "Первые 10 элементов массива после обработки: ";
    for (int i = 0; i < 10; ++i)
    {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}


