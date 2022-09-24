#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void Sort(std::vector<int> &data)
{
    for (size_t i = 0; i < data.size(); i++) {
        int min_idx = i;
        for (size_t j = i + 1; j < data.size(); j++) {
            if (data[j] < data[min_idx])
                min_idx = j;
        }
        std::swap(data[i], data[min_idx]);
    }
    // std::sort(data.begin(), data.end());
}

int Max(const std::vector<int> &data)
{
    int max = data[0];
    for (auto elem : data) {
        if (max < elem) {
            max = elem;
        }
    }
    return max;
    // return *std::max_element(data.begin(), data.end());
}

int Min(const std::vector<int> &data)
{
    int min = data[0];
    for (auto elem : data) {
        if (min > elem) {
            min = elem;
        }
    }
    return min;
    // return *std::min_element(data.begin(), data.end());
}

int Sum(const std::vector<int> &data)
{
    int sum = 0;
    for (auto elem : data) {
        sum += elem;
    }
    return sum;
}

std::vector<char> Ununique(const std::vector<int> &data)
{
    std::vector<char> ununique;
    for (auto it = data.begin(); it != data.end(); it++) {
        if (std::find(ununique.begin(), ununique.end(), *it) != ununique.end())
            continue;
        if (std::find(it + 1, data.end(), *it) != data.end())
            ununique.push_back(*it);
    }
    return ununique;
}

int main(int argc, char const *argv[])
{
    std::vector<int> data;
    bool is_int;

    if (isalpha(*argv[1])) {
        is_int = false;
    } else if (isdigit(*argv[1])) {
        is_int = true;
    } else {
        std::cerr << "Wrong input" << std::endl;
        exit(1);
    }

    if (is_int) {
        for (int i = 1; i < argc; i++) {
            if (isdigit(*argv[i])) {
                data.push_back(atoi(argv[i]));
            } else {
                std::cerr << "Wrong input" << std::endl;
                exit(1);
            }
        }
    } else {
        for (int i = 1; i < argc; i++) {
            if (isalpha(*argv[i])) {
                data.push_back(*argv[i]);
            } else {
                std::cerr << "Wrong input" << std::endl;
                exit(1);
            }
        }
    }

    std::mutex out_mtx, data_mtx;

    std::thread t1([&data, &out_mtx, &data_mtx, is_int]() {
        {
            std::lock_guard<std::mutex> l(data_mtx);
            Sort(data);
        }

        {
            std::lock_guard<std::mutex> l(out_mtx);
            std::cout << "sorted: ";
            if (is_int) {
                for (auto i : data)
                    std::cout << i << " ";
            } else {
                for (auto i : data)
                    std::cout << static_cast<char>(i) << " ";
            }

            std::cout << std::endl;
        }
    });

    std::thread t2([&data, &out_mtx, &data_mtx, is_int]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        {
            std::lock_guard<std::mutex> l(data_mtx);
        }

        int min = Min(data);

        {
            std::lock_guard<std::mutex> l(out_mtx);
            if (is_int)
                std::cout << "min: " << min << std::endl;
            else
                std::cout << "min: " << static_cast<char>(min) << std::endl;
        }
    });

    std::thread t3([&data, &out_mtx, &data_mtx, is_int]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        {
            std::lock_guard<std::mutex> l(data_mtx);
        }

        int max = Max(data);

        {
            std::lock_guard<std::mutex> l(out_mtx);
            if (is_int)
                std::cout << "max: " << max << std::endl;
            else
                std::cout << "max: " << static_cast<char>(max) << std::endl;
        }
    });

    std::thread t4([&data, &out_mtx, &data_mtx, is_int]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        {
            std::lock_guard<std::mutex> l(data_mtx);
        }
        if (is_int) {
            int sum = Sum(data);

            {
                std::lock_guard<std::mutex> l(out_mtx);
                std::cout << "sum: " << sum << std::endl;
            }
        } else {
            std::vector<char> ununique = Ununique(data);

            {
                std::lock_guard<std::mutex> l(out_mtx);
                std::cout << "ununique: ";
                for (auto i : ununique)
                    std::cout << i << " ";
                std::cout << std::endl;
            }
        }
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
