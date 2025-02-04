// For example, given array S = [-1, 0, 1, 2, -1, -4],
// A solution set is:
// [
// [-1, 0, 1],
// [-1, -1, 2]
// ]

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

std::vector<std::array<int, 3>> get_triplets(std::vector<int> data) {
    std::vector<std::array<int, 3>> triplets;

    auto const size = data.size();
    for (size_t i = 0; i < size; ++i) {
        auto const di = data[i];
        for (size_t j = i+1; j < size; ++j) {
            auto const dj = data[j];
            auto const partial_sum = di + dj;
            for (size_t k = j+1; k < size; ++k) {
                auto const dk = data[k];
                auto const sum = partial_sum + dk;
                if (sum == 0) {
                    std::array<int, 3> triplet({di, dj, dk});
                    std::ranges::sort(triplet);
                    triplets.emplace_back(triplet);
                }
            }
        }
    }

    return triplets;
}

std::vector<std::array<int, 3>> get_unique_triplets(std::vector<int> data) {
    auto triplets = get_triplets(data);
    std::ranges::sort(triplets);
    auto r = std::ranges::unique(triplets);
    triplets.erase(r.begin(), r.end());
    return triplets;
}

int main() {
    auto const result = get_unique_triplets({-1, 0, 1, 2, -1, -4});
    std::vector<std::array<int, 3>> const reference = {{-1, -1, 2}, {-1, 0, 1}};

    for (auto const & triplet : result) {
        std::cout << "[ " << triplet[0] << ", " << triplet[1] << ", " << triplet[2] << " ]" << std::endl;
    }

    std::cout << std::boolalpha << (reference == result) << std::endl;
}