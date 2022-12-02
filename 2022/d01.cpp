#include <spdlog/spdlog.h>

#include <fstream>
#include <numeric>
#include <sstream>

using CaloriesList = std::vector<unsigned long>;

std::string readCaloriesFromFile(const std::string& file_path) {
    std::ifstream calories_file(file_path);
    std::stringstream calories;
    if (not calories_file.is_open()) {
        throw std::invalid_argument("File does not exist");
    }
    calories << calories_file.rdbuf();
    calories_file.close();
    return calories.str();
}

CaloriesList parseCaloriesList(const std::string& calories_list) {
    CaloriesList result;
    result.push_back({});

    std::string calories{};
    std::stringstream calories_list_ss(calories_list);
    while (std::getline(calories_list_ss, calories, '\n')) {
        if (calories.empty() or calories == "\n") {
            result.push_back({});
            continue;
        }
        result.back() += std::stoul(calories);
    }
    return result;
}

unsigned long getSumOfNTopCalories(const std::string& calories_list,
                                   std::size_t number_of_max_calories) {
    auto calories{parseCaloriesList(calories_list)};
    std::nth_element(std::begin(calories),
                     std::begin(calories) + number_of_max_calories,
                     std::end(calories), std::greater{});
    return std::accumulate(std::cbegin(calories),
                           std::cbegin(calories) + number_of_max_calories, 0);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        spdlog::error("Please provide path to input file");
        return -1;   
    }
    const std::string file_path = argv[1];
    try {
        const auto calories_list{readCaloriesFromFile(file_path)};
        spdlog::info("Maximum number of calories for top 1: {}",
                     getSumOfNTopCalories(calories_list, 1));
        spdlog::info("Maximum number of calories for top 3: {}",
                     getSumOfNTopCalories(calories_list, 3));
    } catch (const std::invalid_argument& ex) {
        spdlog::error("Exception: {}", ex.what());
    } catch (const std::out_of_range& ex) {
        spdlog::error("Exception: {}", ex.what());
    }

    return 0;
}
