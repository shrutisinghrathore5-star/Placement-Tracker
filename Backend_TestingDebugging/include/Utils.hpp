#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <limits>

// A small toolbox of free functions used across the project.
// Kept header-only (inline) since they are tiny and used everywhere.
namespace Utils {

    // Split a string on a delimiter character. Empty fields are preserved,
    // which matters for our "|" delimited flat-file records.
    inline std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> tokens;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            tokens.push_back(item);
        }
        // getline drops a trailing empty field (e.g. "a|b|" -> ["a","b"])
        if (!s.empty() && s.back() == delim) tokens.push_back("");
        return tokens;
    }

    inline std::string join(const std::vector<std::string>& parts, char delim) {
        std::string out;
        for (size_t i = 0; i < parts.size(); ++i) {
            out += parts[i];
            if (i + 1 < parts.size()) out += delim;
        }
        return out;
    }

    inline std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    inline std::string toUpper(const std::string& s) {
        std::string r = s;
        std::transform(r.begin(), r.end(), r.begin(), ::toupper);
        return r;
    }

    inline std::string toLower(const std::string& s) {
        std::string r = s;
        std::transform(r.begin(), r.end(), r.begin(), ::tolower);
        return r;
    }

    // Reads a line of raw text (no validation beyond "not empty" if required).
    inline std::string readLine(const std::string& prompt, bool allowEmpty = false) {
        std::string line;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, line);
            line = trim(line);
            if (!line.empty() || allowEmpty) return line;
            std::cout << "  Input cannot be empty. Please try again.\n";
        }
    }

    // Robust integer input with an inclusive range check.
    inline int readInt(const std::string& prompt, int minVal = std::numeric_limits<int>::min(),
                        int maxVal = std::numeric_limits<int>::max()) {
        while (true) {
            std::cout << prompt;
            std::string line;
            std::getline(std::cin, line);
            line = trim(line);
            try {
                size_t pos;
                int val = std::stoi(line, &pos);
                if (pos != line.size()) throw std::invalid_argument("trailing chars");
                if (val < minVal || val > maxVal) {
                    std::cout << "  Please enter a value between " << minVal << " and " << maxVal << ".\n";
                    continue;
                }
                return val;
            } catch (...) {
                std::cout << "  Invalid number. Please try again.\n";
            }
        }
    }

    inline double readDouble(const std::string& prompt, double minVal = -1e18, double maxVal = 1e18) {
        while (true) {
            std::cout << prompt;
            std::string line;
            std::getline(std::cin, line);
            line = trim(line);
            try {
                size_t pos;
                double val = std::stod(line, &pos);
                if (pos != line.size()) throw std::invalid_argument("trailing chars");
                if (val < minVal || val > maxVal) {
                    std::cout << "  Please enter a value between " << minVal << " and " << maxVal << ".\n";
                    continue;
                }
                return val;
            } catch (...) {
                std::cout << "  Invalid number. Please try again.\n";
            }
        }
    }

    // Yes/No prompt -> returns true for yes.
    inline bool readYesNo(const std::string& prompt) {
        while (true) {
            std::string line = toLower(readLine(prompt));
            if (line == "y" || line == "yes") return true;
            if (line == "n" || line == "no") return false;
            std::cout << "  Please answer y/n.\n";
        }
    }

    inline void pause() {
        std::cout << "\nPress Enter to continue...";
        std::string dummy;
        std::getline(std::cin, dummy);
    }

    inline std::string currentDate() {
        time_t now = time(nullptr);
        tm* ltm = localtime(&now);
        char buf[32];
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
        return std::string(buf);
    }

} // namespace Utils

#endif // UTILS_HPP
