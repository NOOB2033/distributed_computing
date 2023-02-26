#include <iostream>
#include <fstream>
#include <list>


#include "tinyThreadPool/thread_pool.hpp"


#include "Analyzer.hpp"


void command(Analyzer& analyzer) {
    std::cin.get();
    std::cout << "Most common letters = 1" << std::endl;;
    std::cout << "Rarest letters = 2" << std::endl;;
    std::cout << "Letter probability = 3" << std::endl;;
    std::cout << "Finish = 0" << std::endl;

    std::string command;
    while (true) {
        std::getline(std::cin, command);

        if (command[0] == '1') {
            for (auto&& val : analyzer.most_common_letters(5)) {
                std::cout << val << ' ';
            }
            std::cout << std::endl;
            continue;
        }
        if (command[0] == '2') {
            for (auto&& val : analyzer.rarest_letters(4)) {
                std::cout << val << ' ';
            }
            std::cout << std::endl;
            continue;
        }
        if (command[0] == '3') {
            std::cout << analyzer.letter_probability('1');
            std::cout << std::endl;
            continue;
        }
        if (command[0] == '0') {
            break;
        }
        std::cout << "Unknown command" << std::endl;
    }
}


void readFiles(Analyzer& analyzer, std::list<std::thread>& pool,
               size_t k, const std::vector<std::string>& filenames) {
    auto total_threads = k;
    size_t i = 0;
    bool end = true;
    for (; i < filenames.size() && end; ++i) {
        pool.emplace_back([&, fileNumber = i]() {
            std::ifstream file(filenames[fileNumber]);
            if (!file.is_open()) {
                std::cout << "Error: file " + filenames[fileNumber] + " did not open\n";
                return;
            }

            char c;
            while (file.get(c)) {
                analyzer.add_char_from_file(fileNumber, c);
            }
        });
        if (--total_threads == 0) {
            end = false;
        }
    }

    if (!end) {
        //  Можно лучше
        for (size_t index = 0; i < filenames.size(); ++i) {
            pool.front().join();
            pool.pop_front();
            pool.emplace_back([&, fileNumber = i, index = index]() {
                std::ifstream file(filenames[fileNumber]);
                if (!file.is_open()) {
                    std::cout << "Error: file " + filenames[fileNumber] + " did not open\n";
                    return;
                }

                char c;
                while (file.get(c)) {
                    analyzer.add_char_from_file(index, c);
                }
            });
            if (++index == pool.size()) {
                index = 0;
            }
        }
    }

    for (auto&& thread : pool) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    analyzer.endRead();
}


int main() {
    size_t k;
    std::cout << "Number of reading thread:";
    std::cin >> k;
    if (k == 0) {
        std::cout << "Error: 0 threads\n";
        return -1;
    }

    Analyzer analyzer(k);
    std::list<std::thread> pool;
    std::vector<std::string> filenames {
            {"text1.txt"}, {"text2.txt"}, {"text3.txt"}
    };


    readFiles(analyzer, pool, k, filenames);
    command(analyzer);
}