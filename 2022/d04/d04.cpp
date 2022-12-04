#include <spdlog/spdlog.h>

#include <algorithm>
#include <fstream>
#include <sstream>

struct AssignmentPair {
    using Pair = std::pair<int, int>;
    Pair first_elf{};
    Pair second_elf{};

    bool isOverlappingAtAll() const {
        return (first_elf.first >= second_elf.first and
                first_elf.first <= second_elf.second) or
               (second_elf.first >= first_elf.first and
                second_elf.first <= first_elf.second);
    }

    bool isOverlappingCompletely() const {
        return (first_elf.first <= second_elf.first and
                first_elf.second >= second_elf.second) or
               (second_elf.first <= first_elf.first and
                second_elf.second >= first_elf.second);
    }
};

using Assignments = std::vector<AssignmentPair>;

std::string readAssignmentPairsFromFile(const std::string& file_path) {
    std::ifstream file(file_path);
    std::stringstream lines;
    if (not file.is_open()) {
        throw std::invalid_argument("File does not exist");
    }
    lines << file.rdbuf();
    file.close();
    return lines.str();
}

Assignments parseAssignments(const std::string& assignments_list) {
    Assignments assignments;

    std::string pair{};
    std::stringstream pairs_ss(assignments_list);
    while (std::getline(pairs_ss, pair, '\n')) {
        const auto comma_pos{pair.find(",")};
        const auto first_pair{pair.substr(0, comma_pos)};
        const auto second_pair{pair.substr(comma_pos + 1, pair.size() - 1)};

        const auto first_dash_pos{first_pair.find("-")};
        const auto second_dash_pos{second_pair.find("-")};

        AssignmentPair ass{};
        ass.first_elf.first = std::stoi(first_pair.substr(0, first_dash_pos));
        ass.first_elf.second = std::stoi(
            first_pair.substr(first_dash_pos + 1, first_pair.size() - 1));
        ass.second_elf.first =
            std::stoi(second_pair.substr(0, second_dash_pos));
        ass.second_elf.second = std::stoi(
            second_pair.substr(second_dash_pos + 1, second_pair.size() - 1));
        assignments.push_back(ass);
    }
    return assignments;
}

std::pair<int, int> getNumberOfOverlappingAssignments(
    const std::string& file_path) {
    auto assignments{parseAssignments(file_path)};
    std::pair<int, int> overlaps{};
    overlaps.first = std::count_if(
        std::cbegin(assignments), std::cend(assignments),
        [](const auto& ass) { return ass.isOverlappingCompletely(); });
    overlaps.second =
        std::count_if(std::cbegin(assignments), std::cend(assignments),
                      [](const auto& ass) { return ass.isOverlappingAtAll(); });
    return overlaps;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        spdlog::error("Please provide path to input file");
        return -1;
    }
    const std::string file_path = argv[1];
    try {
        const auto assignment_pairs{readAssignmentPairsFromFile(file_path)};
        const auto overlaps{
            getNumberOfOverlappingAssignments(assignment_pairs)};
        spdlog::info("Number of assignments that overlap completely: {}",
                     overlaps.first);
        spdlog::info("Number of assignments that overlap at all: {}",
                     overlaps.second);
    } catch (const std::invalid_argument& ex) {
        spdlog::error("Exception: {}", ex.what());
    }

    return 0;
}
