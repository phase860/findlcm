#include "test_helper.hpp"

#include <iostream>


void mapToVector(std::map<uint64_t, uint64_t> &map, 
        std::vector<uint64_t> &vector)
{
    for(auto it = map.begin(); it != map.end(); ++it) {
        for(uint64_t i=0; i < it->second; ++i) 
            vector.push_back(it->first); 
    };
}

void showVector(std::vector<uint64_t> &vec) {
    for(auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void showMap(std::map<uint64_t, uint64_t> &map) {
    for(auto it = map.begin(); it != map.end(); ++it) {
        for(uint64_t i=0; i < it->second; ++i) 
            std::cout << it->first << " ";
    };
    std::cout << std::endl;
}
