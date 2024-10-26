# Документация к библиотеке `UFO` (Python)

## Общее описание

Класс `UFOArrayPy` предназначен для хранения пар ключ-значение, организованных по классам. Он предоставляет функционал для добавления, сохранения и загрузки данных в формате, совместимом с C++ версией библиотеки.  Класс реализован на Python и использует JSON для сериализации данных.

## Основные функции и методы

### 1. Конструктор
- **UFOArrayPy(class_name)**: Конструктор, принимающий имя класса в качестве аргумента и инициализирующий объект.

### 2. Добавление элементов
- **add(key, value)**: Добавляет одну пару ключ-значение в текущий класс.  Возвращает ссылку на текущий объект `UFOArrayPy`, что позволяет использовать метод в цепочке вызовов.

### 3. Преобразование в строку
- **to_string()**: Возвращает строковое представление данных текущего класса в формате JSON-подобной структуры, совместимой с C++ версией.

### 4. Сохранение и загрузка данных
- **save(filename, append=True)**: Сохраняет данные текущего класса в файл.  Если `append=True`, данные добавляются в конец файла. В противном случае файл перезаписывается.  Использует UTF-8 кодировку для совместимости с C++.
- **load(filename)**: Загружает данные из указанного файла.  Очищает текущее хранилище данных перед загрузкой.  Поддерживает чтение UTF-8 кодировки.

## Пример использования

```python
from pyufo import UFOArrayPy  # Assuming you've saved the Python code as pyufo.py

ufo = UFOArrayPy("box")

filename = "data.ufo"

# Добавление данных
ufo.add("1", "Hello").add("2", "World").add("3", "Program")

# Сохранение данных в файл
ufo.save(filename)


# Загрузка данных из файла
ufo_loaded = UFOArrayPy("") # Create a dummy instance; classname will be loaded from the file.
ufo_loaded.load(filename)

# Вывод результатов (using the loaded object)
print(ufo_loaded.to_string())

# Accessing individual elements after loading:
print(ufo_loaded.data["1"]) # Prints "Hello"
print(ufo_loaded.class_name) # Prints "box"

# Example of creating and saving an empty object
empty_ufo = UFOArrayPy("EmptyClass")
empty_ufo.save("empty.ufo")

empty_ufo_loaded = UFOArrayPy("")
empty_ufo_loaded.load("empty.ufo")
print(empty_ufo_loaded.to_string()) # Prints the correctly formatted empty object.



```


##  Замечания о совместимости

Python версия `UFOArrayPy` предназначена для взаимодействия с C++ версией `UFOArray`.  Формат сохраняемых файлов совместим между двумя реализациями.  Обратите внимание, что  Python версия использует `json.dumps` и `json.loads` для сериализации и десериализации данных.  C++ версия использует потоковый ввод/вывод.

## Отличия от C++ версии

* Отсутствуют методы `getAllByClass`, `getFirstByClass`, `getLastByClass`, `getByIndexAndClass` и `openFile`.  После загрузки данных из файла, вы можете  получать доступ к данным напрямую через словарь `ufo_array.data` как показано в примере.
*  Тип данных не задаётся явно как шаблонный параметр. Python динамически типизирован, поэтому `value` в `add(key, value)` может быть любого типа. Однако, для совместимости с C++ версией, убедитесь, что типы данных, которые вы используете, могут быть представлены в C++ (например, числовые типы, строки).
