#ifndef UFO_ARRAY_H
#define UFO_ARRAY_H

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <iomanip>
#include <iostream>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#endif

template <typename T>
class UFOArray {
public:
    UFOArray() = default;

    UFOArray<T>& setClassName(const std::string& className) {
        class_name = className;
        return *this;
    }

    UFOArray& add(const std::string& key, const T& value) {
        data[class_name][key] = value;
        return *this;
    }

    template <typename... Args>
    UFOArray& add(const std::string& key, const T& value, Args... args) {
        add(key, value);
        return add(args...);
    }

    std::string toString() const {
        std::ostringstream result;
        if (data.find(class_name) == data.end() || data.at(class_name).empty()) {
            return  "(" + class_name + ")\n{\n}";
        }

        result << "(" << class_name << ")\n" << "{\n";
        for (auto it = data.at(class_name).cbegin(); it != data.at(class_name).cend(); ++it) {
            result << "\t\"" << it->first << "\": " << it->second;
            if (std::next(it) != data.at(class_name).cend()) {
                result << ",\n";
            }
        }
        result << "\n}";
        return result.str();
    }

    void save(const std::string& filename, bool append = true) const {
        std::ofstream outFile(filename, append ? std::ios::app : std::ios::out);
        if (!outFile) {
            throw std::runtime_error("Cannot open file for writing");
        }
        outFile << toString() << "\n";
    }

    void load(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            throw std::runtime_error("Cannot open file for reading");
        }

        data.clear();
        std::string line;
        std::string currentClass;

        while (std::getline(inFile, line)) {
            if (line.find("(") != std::string::npos) {
                currentClass = line.substr(1, line.find(")") - 1);
                continue;
            }
            if (line.find(":") != std::string::npos && !currentClass.empty()) {
                size_t pos = line.find(":");
                std::string key = line.substr(1, pos - 3); 
                try {
                    T value = valueFromString(line.substr(pos + 1));
                    data[currentClass][key] = value;
                } catch (const std::runtime_error& e) {
                    std::cerr << "Error parsing value: " << e.what() << std::endl;
                }
            }
        }
    }


    std::map<std::string, T> getAllByClass(const std::string& className) const {
        if (data.find(className) != data.end()) {
            return data.at(className);
        }
        throw std::runtime_error("Class not found");
    }

    std::pair<std::string, T> getFirstByClass(const std::string& className) const {
         if (data.find(className) != data.end() && !data.at(className).empty()) {
            return *data.at(className).cbegin();  // Use cbegin for const correctness
        }
        throw std::runtime_error("No elements to retrieve");
    }

    std::pair<std::string, T> getLastByClass(const std::string& className) const {
        if (data.find(className) != data.end() && !data.at(className).empty()) {
            return *data.at(className).crbegin(); // Use crbegin for const correctness
        }
        throw std::runtime_error("No elements to retrieve");
    }

    std::pair<std::string, T> getByIndexAndClass(const std::string& className, size_t index) const {
         if (data.find(className) != data.end() && index < data.at(className).size()) {
            auto it = std::next(data.at(className).cbegin(), index); // Use cbegin
            return *it;
        }
        throw std::out_of_range("Index out of range or class not found");
    }


#ifdef _WIN32
    void openFile(const std::string& filename) const {
        ShellExecuteA(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOW);
    }
#endif


private:
    std::map<std::string, std::map<std::string, T>> data;
    std::string class_name;


    T valueFromString(const std::string& str) {
        T value{};
        std::istringstream iss(str);

        if constexpr (std::is_same_v<T, std::string>) {
            value = str;
            size_t first = value.find_first_not_of(' ');
            size_t last = value.find_last_not_of(' ');
            if (std::string::npos != first && std::string::npos != last) {
                value = value.substr(first, (last - first + 1));
            }
        } else if constexpr (std::is_arithmetic_v<T>) {
            iss >> value;
            if (iss.fail()) {
                throw std::runtime_error("Failed to convert string to number");
            }
        } else {
            throw std::runtime_error("Unsupported type for valueFromString");
        }
        return value;
    }
};

#endif // !UFO_ARRAY_H