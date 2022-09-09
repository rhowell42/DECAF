
#include <iostream>
#include <vector>
#include <algorithm>

void fill_vectors();

template<typename T>
std::vector<int> findItems(std::vector<T> const &v, int target) {
    std::vector<int> indices;
    auto it = v.begin();
    while ((it = std::find_if(it, v.end(), [&] (T const &e) { return int(e) == target; }))
        != v.end())
    {
        indices.push_back(std::distance(v.begin(), it));
        it++;
    }
    return indices;
} 
