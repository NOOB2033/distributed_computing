#ifndef DISTRIBUTED_COMPUTING_ANALYZER_HPP
#define DISTRIBUTED_COMPUTING_ANALYZER_HPP


#include <map>
#include <vector>
#include <algorithm>


class Analyzer {
public:

    Analyzer(size_t thread_count) : maps(thread_count)
    {}

    void add_char_from_file(size_t index, const char c) {
        if (c < '!') {
            return;
        }
        auto it = maps[index].find(c);
        if (it == maps[index].end()) {
            maps[index].insert({c, 1});
        } else {
            it->second++;
        }
    }

    std::vector<char> most_common_letters(size_t quantity = 5) {
        if (quantity > full_map.size()) {
            throw std::runtime_error("Bad quantity\n");
        }
        std::vector<char> result;

        std::vector<std::pair<char, size_t>> temp;
        for (auto&& pair : full_map) {
            temp.push_back(pair);
        }

        auto comparator = [] (const std::pair<char, size_t>& f, const std::pair<char, size_t>& s) {
            return f.second > s.second;
        };

        std::sort(temp.begin(), temp.end(), comparator);

        auto it = temp.begin();
        for (size_t i = 0; i < quantity; ++i, ++it) {
            result.push_back(it->first);
        }

        return result;
    }

    std::vector<char> rarest_letters(size_t quantity = 3) {
        if (quantity > full_map.size()) {
            throw std::runtime_error("Bad quantity\n");
        }
        std::vector<char> result;

        std::vector<std::pair<char, size_t>> temp;
        for (auto&& pair : full_map) {
            temp.push_back(pair);
        }

        auto comparator = [] (const std::pair<char, size_t>& f, const std::pair<char, size_t>& s) {
            return f.second < s.second;
        };

        std::sort(temp.begin(), temp.end(), comparator);

        auto it = temp.begin();
        for (size_t i = 0; i < quantity; ++i, ++it) {
            result.push_back(it->first);
        }

        return result;
    }

    double letter_probability(char c) {
        auto it = full_map.find(c);
        if (it != full_map.end()) {
            size_t total = 0;
            for (auto&& val : full_map) {
                total += val.second;
            }
            return (double)it->second / (double)total;
        }
        return 0;
    }

    void endRead() {
        for (size_t i = 0; i < maps.size(); ++i) {
            for (auto&& elem : maps[i]) {
                auto it = full_map.find(elem.first);
                if (it != full_map.end()) {
                    it->second += elem.second;
                } else {
                    full_map.insert(elem);
                }
            }
            maps[i].clear();
        }
    }

private:

    std::map<char, size_t> full_map;
    std::vector<std::map<char, size_t>> maps;
};


#endif //DISTRIBUTED_COMPUTING_ANALYZER_HPP
