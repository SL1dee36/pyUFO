#ifndef UFO_ARRAY_H
#define UFO_ARRAY_H

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <iomanip>
#include <Windows.h>
#include <iostream>

/**
 * @brief ����� UFOArray ������������ ��� �������� ��� ����-��������,
 * �������������� �� �������, � ������������� ������ ��� ������ � ����� �������.
 *
 * @tparam T ��� ��������, ����������� � �������.
 */
template <typename T>
class UFOArray {
public:
    /**
     * @brief ����������� �� ���������.
     */
    UFOArray() = default;

    /**
     * @brief ��������� ����� ������.
     *
     * @param className ��� ������, ������� ����� �����������.
     * @return ������ �� ������� ������ UFOArray.
     */
    UFOArray<T>& setClassName(const std::string& className) {
        class_name = className;
        return *this;
    }

    /**
     * @brief ����� ��� ���������� ����� ���� ����-��������.
     *
     * @param key ���� ��� �������� ��������.
     * @param value ��������, ��������� � ������.
     * @return ������ �� ������� ������ UFOArray.
     */
    UFOArray& add(const std::string& key, const T& value) {
        data[class_name][key] = value; // ��������� �� ����� ������
        return *this;
    }

    /**
     * @brief ����� ��� ���������� ���������� ��� ����-��������.
     *
     * @param key ���� ��� ������� ��������.
     * @param value �������� ��� ������� �����.
     * @param args ��������� ���� ����-��������.
     * @return ������ �� ������� ������ UFOArray.
     */
    template <typename... Args>
    UFOArray& add(const std::string& key, const T& value, Args... args) {
        add(key, value);
        return add(args...);
    }

    /**
     * @brief �������������� � ������ ��� ������.
     *
     * @return ��������� ������������� ������ �������� ������.
     */
    std::string toString() const {
        std::ostringstream result;
        result << "(" << class_name << ")\n" << "{\n";
        for (auto it = data.at(class_name).begin(); it != data.at(class_name).end(); ++it) {
            result << "\t\"" << it->first << "\": " << it->second;
            if (std::next(it) != data.at(class_name).end()) {
                result << ",\n";
            }
        }
        result << "\n}";
        return result.str();
    }

    /**
     * @brief ���������� ������ � ����.
     *
     * @param filename ���� � ����� ��� ����������.
     * @param append ���������, ��������� �� ������ � ����� ����� (�� ��������� true).
     * @throw std::runtime_error ���� �� ������� ������� ���� ��� ������.
     */
    void save(const std::string& filename, bool append = true) const {
        std::unique_ptr<std::ofstream> outFile(new std::ofstream(filename, append ? std::ios::app : std::ios::out));

        if (!*outFile) {
            throw std::runtime_error("���������� ������� ���� ��� ������");
        }

        *outFile << toString() << "\n";
    }

    /**
     * @brief �������� ������ �� �����.
     *
     * @param filename ���� � ����� ��� �������� ������.
     * @throw std::runtime_error ���� �� ������� ������� ���� ��� ������.
     */
    void load(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            throw std::runtime_error("���������� ������� ���� ��� ������");
        }

        data.clear(); // ������� �������� ���������
        std::string line;
        std::string currentClass;

        while (std::getline(inFile, line)) {
            // ��������� ������ ��� ��������� ����� ������
            if (line.find("(") != std::string::npos) {
                currentClass = line.substr(1, line.find(")") - 1); // ��������� ����� ������
                continue;
            }
            // ��������� ������ � �������
            if (line.find(":") != std::string::npos && !currentClass.empty()) {
                size_t pos = line.find(":");
                std::string key = line.substr(1, pos - 3); // ���������� �����
                T value = valueFromString(line.substr(pos + 1)); // ���������� ��������
                data[currentClass][key] = value; // ���������� �� ����� ������
            }
        }
    }

    /**
     * @brief ��������� ���� ��������� �� ������.
     *
     * @param className ��� ������ ��� ���������� ������.
     * @return ������� (map) ��� ����-�������� ������� ������.
     * @throw std::runtime_error ���� ����� �� ������.
     */
    std::map<std::string, T> getAllByClass(const std::string& className) const {
        if (data.find(className) != data.end()) {
            return data.at(className);
        }
        throw std::runtime_error("����� �� ������");
    }

    /**
     * @brief ��������� ������� �������� �� ������.
     *
     * @param className ��� ������ ��� ���������� ������.
     * @return ���� ����-�������� ������� �������� ������.
     * @throw std::runtime_error ���� ��� ��������� ��� ����������.
     */
    std::pair<std::string, T> getFirstByClass(const std::string& className) const {
        if (data.find(className) != data.end() && !data.at(className).empty()) {
            return *data.at(className).begin();
        }
        throw std::runtime_error("��� ��������� ��� ����������");
    }

    /**
     * @brief ��������� ���������� �������� �� ������.
     *
     * @param className ��� ������ ��� ���������� ������.
     * @return ���� ����-�������� ���������� �������� ������.
     * @throw std::runtime_error ���� ��� ��������� ��� ����������.
     */
    std::pair<std::string, T> getLastByClass(const std::string& className) const {
        if (data.find(className) != data.end() && !data.at(className).empty()) {
            return *data.at(className).rbegin();
        }
        throw std::runtime_error("��� ��������� ��� ����������");
    }

    /**
     * @brief ��������� �������� �� ������� � ������.
     *
     * @param className ��� ������ ��� ���������� ������.
     * @param index ������ ��������.
     * @return ���� ����-�������� �������� �� �������.
     * @throw std::out_of_range ���� ������ ��� ��������� ��� ����� �� ������.
     */
    std::pair<std::string, T> getByIndexAndClass(const std::string& className, size_t index) const {
        if (data.find(className) != data.end() && index < data.at(className).size()) {
            auto it = std::next(data.at(className).begin(), index);
            return *it;
        }
        throw std::out_of_range("������ ��� ��������� ��� ����� �� ������");
    }

    /**
     * @brief �������� ����� � ����������.
     *
     * @param filename ���� � ����� ��� ��������.
     */
    void openFile(const std::string& filename) const {
        std::string command = "start " + filename;
        system(command.c_str());
    }

private:
    std::map<std::string, std::map<std::string, T>> data; // ��������� ��� ����-�������� �� �������
    std::string class_name; // ��� ������

    /**
     * @brief �������������� ������ � �������� ���� T.
     *
     * @param str ������ ��� ��������������.
     * @return �������� ���� T.
     */
    T valueFromString(const std::string& str) {
        T value;
        std::istringstream iss(str);
        iss >> value; // ����� ��������� ��������� � ����������� �� ���� T
        return value;
    }
};

#endif // !UFO_ARRAY_H
