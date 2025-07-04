\page simple_logger_example Пример использования SimpleLogger

Ниже приведён минимальный пример использования `stc_logger::SimpleLogger`:

```cpp
#include "logger/simple_logger.hpp"
#include <iostream>

int main() {
    stc::SimpleLogger logger("app.log");
    logger.log("Hello, console");
    logger.logToFile("Hello, file");
    return 0;
}
```

Описание работы:
- Конструктор открывает файл `app.log` (или логгирует только в консоль).
- Метод `log` пишет в `std::cout`.
- Метод `logToFile` пишет в файл.

Содержимое примера (`examples/basic_usage/main.cpp`) можно посмотреть [здесь](basic_usage/main.cpp).

Скомпилировать и запустить:<br>
    mkdir build && cd build<br>
    cmake .. -DBUILD_EXAMPLES=ON<br>
    make basic_usage<br>
    ./bin/basic_usage/basic_usage