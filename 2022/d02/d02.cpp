#include <spdlog/spdlog.h>

#include <fstream>
#include <numeric>
#include <sstream>

const auto rock{'A'};
const auto paper{'B'};
const auto scissors{'C'};
const auto lose{'X'};
const auto draw{'Y'};
const auto win{'Z'};

using GamesList = std::vector<std::string>;

std::string readGamesFromFile(const std::string& file_path) {
    std::ifstream games_file(file_path);
    std::stringstream games;
    if (not games_file.is_open()) {
        throw std::invalid_argument("File does not exist");
    }
    games << games_file.rdbuf();
    games_file.close();
    return games.str();
}

GamesList parseGamesList(const std::string& games_list) {
    GamesList games;

    std::string game{};
    std::stringstream games_list_ss(games_list);
    while (std::getline(games_list_ss, game, '\n')) {
        if (game.size() != 3) {
            throw std::invalid_argument("Malformed game: " + game);
        }
        games.push_back(game);
    }
    return games;
}

std::size_t getPlayerPoints(const std::string& game) {
    const auto player1{game[0]};
    const auto player2{game[2]};

    std::size_t result{};
    if (player2 == lose) {
        if (player1 == rock)
            result += 3;  // scissors
        else if (player1 == paper)
            result += 1;  // rock
        else
            result += 2;  // paper
    } else if (player2 == draw) {
        result += 3 + (player1 - 'A' + 1);
    } else if (player2 == win) {
        result += 6;
        if (player1 == rock)
            result += 2;  // paper
        else if (player1 == paper)
            result += 3;  // scissors
        else
            result += 1;  // rock
    }
    return result;
}

unsigned long getGamesSummary(const std::string& games_list) {
    auto games{parseGamesList(games_list)};
    return std::accumulate(
        std::cbegin(games), std::cend(games), 0,
        [](auto sum, auto next) { return sum + getPlayerPoints(next); });
}

int main(int argc, char** argv) {
    if (argc != 2) {
        spdlog::error("Please provide path to input file");
        return -1;
    }
    const std::string file_path = argv[1];
    try {
        const auto games_list{readGamesFromFile(file_path)};
        spdlog::info("Player's number of points: {}",
                     getGamesSummary(games_list));
    } catch (const std::invalid_argument& ex) {
        spdlog::error("Exception: {}", ex.what());
    }

    return 0;
}
