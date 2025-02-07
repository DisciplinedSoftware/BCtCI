// For example, given array S = [-1, 0, 1, 2, -1, -4],
// A solution set is:
// [
// [-1, 0, 1],
// [-1, -1, 2]
// ]

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>
#include <optional>

std::vector<std::array<int, 3>> get_unique_triplets_baseline(std::vector<int> data) {
    std::vector<std::array<int, 3>> triplets;
    std::ranges::sort(data);
    
    for (size_t i = 0; i < data.size(); ++i) {
        // Skip duplicates for i
        if (i > 0 && data[i] == data[i-1]) continue;
        
        size_t left = i + 1;
        size_t right = data.size() - 1;
        
        while (left < right) {
            int sum = data[i] + data[left] + data[right];
            
            if (sum == 0) {
                triplets.push_back({data[i], data[left], data[right]});
                
                // Skip duplicates for left
                while (left < right && data[left] == data[left + 1]) left++;
                // Skip duplicates for right
                while (left < right && data[right] == data[right - 1]) right--;
                
                left++;
                right--;
            }
            else if (sum < 0) {
                left++;
            }
            else {
                right--;
            }
        }
    }
    
    return triplets;
}

std::vector<int> generate_random_vector(size_t size, std::optional<int> seed = std::nullopt, int min_value = std::numeric_limits<int>::lowest(), int max_value = std::numeric_limits<int>::max()) {
    // Create a random number generator
    std::mt19937 gen(seed.has_value() ? seed.value() : 42);

    // Create a distribution for the desired range
    std::uniform_int_distribution<int> dist(min_value, max_value);

    // Create and fill the vector
    std::vector<int> result(size);
    std::ranges::generate(result, [&]() { return dist(gen); });

    return result;
}

inline void get_unique_triplets(std::vector<int> const & data, size_t i, std::vector<std::array<int, 3>>& triplets) {
    auto left = i+1;
    auto right = data.size() - 1;

    auto const di = data[i];

    while (left < right) {
        auto const dl = data[left];
        auto const dr = data[right];
        auto const sum = di + dl + dr;

        if (sum == 0) {
            triplets.emplace_back(std::array{di, dl, dr});
            ++left;
            --right;

            while (left < right && data[left] == dl) {
                ++left;
            }

            while (left < right && data[right] == dr) {
                --right;
            }
        }
        else if (sum < 0) {
            ++left;
        }
        else {
            --right;
        }
    }
}

std::vector<std::array<int, 3>> get_unique_triplets(std::vector<int> data) {
    std::vector<std::array<int, 3>> triplets;
    std::ranges::sort(data);

    auto const size = data.size();
    size_t i = 0;
    get_unique_triplets(data, i, triplets);

    auto old_di = data[i];
    for (size_t i = 1; i < size - 2; ++i) {
        auto const di = data[i];

        if (di == old_di) {
            continue;
        }

        get_unique_triplets(data, i, triplets);

        old_di = di;
    }

    return triplets;
}

int main() {
    auto const result = get_unique_triplets({-1, 0, 1, 2, -1, -4});
    std::vector<std::array<int, 3>> const reference = {{-1, -1, 2}, {-1, 0, 1}};

    // for (auto const & triplet : result) {
    //     std::cout << "[ " << triplet[0] << ", " << triplet[1] << ", " << triplet[2] << " ]" << std::endl;
    // }

    std::cout << std::boolalpha << (reference == result) << std::endl;

    auto data_baseline = generate_random_vector(1'000);
    auto const result_benchmark = get_unique_triplets(data_baseline);
    auto const reference_benchmark = get_unique_triplets_baseline(data_baseline);
    std::cout << std::boolalpha << (reference_benchmark == result_benchmark) << std::endl;

    std::vector<std::chrono::microseconds> times;
    for (int i = 0; i < 10; ++i) {
        auto&& data = generate_random_vector(1'000, i);

        auto const start = std::chrono::high_resolution_clock::now();
        get_unique_triplets(std::move(data));
        auto const finish = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
        times.emplace_back(time);
        std::cout << "time: " << time << std::endl;
    }

    using namespace std::chrono_literals;
    auto const mean = static_cast<double>(std::ranges::fold_left(times, 0ms, [](auto sum, auto value) {return sum + value;}).count()) / times.size();
    auto const std = std::sqrt(std::ranges::fold_left(times, 0.0, [mean](auto sum, auto value) {auto const diff = value.count() - mean; return sum + diff * diff;}) / (times.size() - 1));
    std::cout << "mean time: " << mean << std::endl;
    std::cout << "std time: " << std << std::endl;
}