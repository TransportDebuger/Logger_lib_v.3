/**
 * @example main.cpp
 * 
 * Описание работы:
- Конструктор открывает файл `app.log` (или логгирует только в консоль).
- Метод `log` пишет в `std::cout`.
- Метод `logToFile` пишет в файл.

Скомпилировать и запустить:<br>
    mkdir build && cd build<br>
    cmake .. -DBUILD_EXAMPLES=ON<br>
    make basic_usage<br>
    ./bin/basic_usage/basic_usage
 */

#include "logger/simple_logger.hpp"

int main() {
    stc_logger::SimpleLogger logger("app.log");
    logger.log("Hello, console");
    logger.logToFile("Hello, file");
    return 0;
}