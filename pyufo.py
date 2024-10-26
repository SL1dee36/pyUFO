import json

class UFOArrayPy:
    def __init__(self, class_name):
        self.class_name = class_name
        self.data = {}

    def add(self, key, value):
        self.data[key] = value
        return self

    def to_string(self):
        if not self.data:
            return f"({self.class_name})\n{{\n}}"
        json_data = json.dumps(self.data, indent=4)
        return f"({self.class_name})\n{{\n{json_data}\n}}"

    def save(self, filename, append=True):
        mode = "a" if append else "w"
        with open(filename, mode, encoding="utf-8") as f:  # Specify UTF-8 encoding
            f.write(self.to_string() + "\n")

    def load(self, filename):
       with open(filename, 'r', encoding="utf-8") as f:
            content = f.read()

       # Parse the content and update data. Simplest approach assuming a standard format.
       try:
           start_index = content.find("{") + 1
           end_index = content.rfind("}")
           json_string = content[start_index:end_index].strip()

           if json_string:  # Check if json_string is not empty
               self.data = json.loads(json_string)
           else:  # Handle the case where there's no data
                self.data = {}
       except json.JSONDecodeError: # Handle empty object
           self.data = {}

       # Extract class name
       class_name_start = content.find("(") + 1
       class_name_end = content.find(")")
       self.class_name = content[class_name_start:class_name_end]


# Example usage in Python:
ufo = UFOArrayPy("MyClass")
ufo.add("name", "John Doe").add("age", 30).add("city", "New York")
print(ufo.to_string())
ufo.save("ufo_data.txt")

ufo2 = UFOArrayPy("")
ufo2.load("ufo_data.txt")
print(ufo2.to_string())

ufo3 = UFOArrayPy("EmptyClass") # Testing an empty object
ufo3.save("empty.txt")
ufo4 = UFOArrayPy("")
ufo4.load("empty.txt")
print(ufo4.to_string())