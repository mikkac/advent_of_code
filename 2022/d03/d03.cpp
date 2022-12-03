#include <spdlog/spdlog.h>

#include <fstream>
#include <numeric>
#include <sstream>

using Rucksacks = std::vector<std::string>;

std::string readRucksacksFromFile(const std::string& file_path) {
    std::ifstream file(file_path);
    std::stringstream lines;
    if (not file.is_open()) {
        throw std::invalid_argument("File does not exist");
    }
    lines << file.rdbuf();
    file.close();
    return lines.str();
}

Rucksacks parseRucksacks(const std::string& rucksacks_list) {
    Rucksacks rucksacks;

    std::string rucksack{};
    std::stringstream rucksacks_ss(rucksacks_list);
    while (std::getline(rucksacks_ss, rucksack, '\n')) {
        rucksacks.push_back(rucksack);
        spdlog::info(rucksack);
    }
    return rucksacks;
}

int getSharedPriorityValue(const std::string& rucksack) {
    if (rucksack.size() % 2) {
        spdlog::error("Number of elements in rucksack '{}' is not even",
                      rucksack);
        throw std::invalid_argument("Not even number of elements");
    }
    const auto get_prio{[](char c) {
        if (::islower(c)) return c - 'a' + 1;
        return c - 'A' + 27;
    }};

    int max_prio{0};
    const auto len{rucksack.size()};
    const auto first_half{rucksack.substr(0, len / 2)};
    const auto second_half{rucksack.substr(len / 2, len / 2)};
    std::string checked{};
    for (std::size_t i{0}; i < first_half.size(); ++i) {
        if (checked.find(first_half[i]) == std::string::npos &&
            second_half.find(first_half[i]) != std::string::npos) {
            spdlog::info("New shared: {}, value: {}", first_half[i],
                         get_prio(first_half[i]));
            max_prio = std::max(max_prio, get_prio(first_half[i]));
        }
        checked += first_half[i];
    }
    return max_prio;
}

int getSharedPriorityValueBetweenThree(const Rucksacks& rucksacks) {
    const auto get_prio{[](char c) {
        if (::islower(c)) return c - 'a' + 1;
        return c - 'A' + 27;
    }};

    int max_prio{0};
    std::string checked{};
    for (std::size_t i{0}; i < rucksacks[0].size(); ++i) {
        if (checked.find(rucksacks[0][i]) == std::string::npos) {
            const auto repeated{
                std::count_if(std::cbegin(rucksacks) + 1, std::cend(rucksacks),
                              [c = rucksacks[0][i]](const auto& rucksack) {
                                  return rucksack.find(c) != std::string::npos;
                              })};
            if (repeated == rucksacks.size() - 1) {
                return get_prio(rucksacks[0][i]);
            }
        }
        checked += rucksacks[0][i];
    }
    return max_prio;
}

int getSumOfSharedPriorities(const std::string& rucksacks_file) {
    auto rucksacks{parseRucksacks(rucksacks_file)};
    // Get sum of badges shared between elves in groups of 3
    int sum{};
    for (std::size_t i{0}; i < rucksacks.size() - 2; i += 3) {
        sum += getSharedPriorityValueBetweenThree(
            Rucksacks({rucksacks[i], rucksacks[i + 1], rucksacks[i + 2]}));
    }
    return sum;
    // Get sum of priority shared in two sides of rucksack
    // return std::accumulate(std::cbegin(rucksacks), std::cend(rucksacks), 0,
    //                        [](auto sum, auto next) {
    //                            return sum +
    //                                   getSharedPriorityValueBetweenThree(next);
    //                        });
}

int main(int argc, char** argv) {
    if (argc != 2) {
        spdlog::error("Please provide path to input file");
        return -1;
    }
    const std::string file_path = argv[1];
    try {
        const auto rucksacks{readRucksacksFromFile(file_path)};
        spdlog::info("Sum of shared priorities: {}",
                     getSumOfSharedPriorities(rucksacks));
    } catch (const std::invalid_argument& ex) {
        spdlog::error("Exception: {}", ex.what());
    }

    return 0;
}
