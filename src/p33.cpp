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

std::vector<std::array<int, 3>> get_unique_triplets(std::vector<int> data) {
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

int main() {
    auto const result = get_unique_triplets({-1, 0, 1, 2, -1, -4});
    std::vector<std::array<int, 3>> const reference = {{-1, -1, 2}, {-1, 0, 1}};

    for (auto const & triplet : result) {
        std::cout << "[ " << triplet[0] << ", " << triplet[1] << ", " << triplet[2] << " ]" << std::endl;
    }

    std::cout << std::boolalpha << (reference == result) << std::endl;
}