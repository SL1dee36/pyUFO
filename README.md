# Документация к библиотеки `UFO`

## Общее описание

Класс `UFOArray` предназначен для хранения пар ключ-значение, организованных по классам. Он предоставляет функционал для добавления, сохранения и загрузки данных, а также извлечения элементов по имени класса. Класс поддерживает обобщённый тип, что позволяет хранить значения различных типов, предоставляемых пользователем.

## Основные функции и методы

### 1. Конструкторы
- **UFOArray()**: Конструктор по умолчанию, инициализирующий объект класса.

### 2. Установка имени класса
- **setClassName(const std::string& className)**: Устанавливает имя класса для текущего объекта. Возвращает ссылку на текущий объект `UFOArray`, что позволяет использовать метод в цепочке вызовов.

### 3. Добавление элементов
- **add(const std::string& key, const T& value)**: Добавляет одну пару ключ-значение в текущий класс. Ключ - это строка, а значение - обобщённый тип T.
- **add(const std::string& key, const T& value, Args... args)**: Перегруженный метод для добавления нескольких пар ключ-значение за один вызов.

### 4. Преобразование в строку
- **toString()**: Возвращает строковое представление данных текущего класса в формате JSON-подобной структуры.

### 5. Сохранение и загрузка данных
- **save(const std::string& filename, bool append = true)**: Сохраняет данные текущего класса в файл. Можно выбрать, добавлять ли данные в конец файла или перезаписывать его.
- **load(const std::string& filename)**: Загружает данные из указанного файла. Очищает текущее хранилище данных перед загрузкой.

### 6. Извлечение элементов
- **getAllByClass(const std::string& className)**: Возвращает все элементы для указанного класса в виде словаря (map).
- **getFirstByClass(const std::string& className)**: Возвращает первую пару ключ-значение для указанного класса.
- **getLastByClass(const std::string& className)**: Возвращает последнюю пару ключ-значение для указанного класса.
- **getByIndexAndClass(const std::string& className, size_t index)**: Возвращает элемент по индексу для указанного класса. Индексы начинаются с 0.

### 7. Открытие файла
- **openFile(const std::string& filename)**: Открывает указанный файл в приложении по умолчанию (например, в текстовом редакторе).

## Пример использования

```cpp
#include <iostream>
#include "ufo_array.h"

int main() {
    UFOArray<std::string> ufo;

    std::string filename = "путь/к/файлу/имяфайла.ufo";

    // Установка имени класса и добавление данных
    ufo.setClassName("box").add("1", "Hello", "2", "World", "3", "Programm") // Ключ - Значение должны быть в равном количестве
       

    // Сохранение данных в файл
    ufo.save(filename);

    // Загрузка данных из файла
    ufo.load(filename);

    //Открытие файла
    ufo.open(filename);

    // Извлечение данных
    auto allItems = ufo.getAllByClass("box");
    auto firstItem = ufo.getFirstByClass("box");
    auto lastItem = ufo.getLastByClass("box");
    auto itemAtIndex = ufo.getByIndexAndClass("box", 1);

    // Вывод результатов
    std::cout << "Все элементы:\n";
    for (const auto& [key, value] : allItems) {
        std::cout << key << ": " << value << std::endl;
    }

    std::cout << "Первый элемент: " << firstItem.first << ": " << firstItem.second << std::endl;
    std::cout << "Последний элемент: " << lastItem.first << ": " << lastItem.second << std::endl;
    std::cout << "Элемент под индексом 1: " << itemAtIndex.first << ": " << itemAtIndex.second << std::endl;

    return 0;
}
```
