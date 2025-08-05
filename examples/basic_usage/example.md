\page simple_logger_example Пример использования SimpleLogger

# Пример базового использования Logger_lib v3

Этот пример демонстрирует основные возможности библиотеки логирования.

## Что демонстрирует пример

### 1. Создание логгера с файловым выводом

```cpp
stc::SimpleLogger logger("app.log");
```

Создается логгер, который по умолчанию записывает в файл `app.log`.

### 2. Добавление дополнительных каналов вывода

```cpp
logger.addOutput(std::make_uniquestc::ConsoleOutput());
```

Добавляется вывод в консоль. Теперь сообщения будут дублироваться в файл и консоль.

### 3. Настройка форматирования сообщений

```cpp
auto fmt = std::make_uniquestc::StandardFormatter("[{level}] {message}");
logger.setFormatter(std::move(fmt));
```

Устанавливается пользовательский формат: `[INFO] Your message here`

### 4. Логирование сообщений разных уровней

```cpp
logger.debug("Debug message");
logger.info("Info message");
logger.warning("Warning message");
logger.error("Error message");
logger.fatal("Fatal message");
```

### 5. Система фильтрации

#### Фильтрация по уровню

```cpp
auto lvlFilter = std::make_uniquestc::LevelFilter(stc::LogLevel::Warning);
logger.addFilter(std::move(lvlFilter));
```

Пропускает только сообщения уровня Warning и выше (Warning, Error, Fatal).

#### Фильтрация по компонентам

```cpp
auto compFilter = std::make_uniquestc::ComponentFilter(stc::ComponentFilter::Mode::Whitelist);
compFilter->addComponent("Database");
logger.addFilter(std::move(compFilter));
```

В режиме Whitelist пропускает только сообщения от компонента "Database".

### 6. Chain of Responsibility в действии

```cpp
logger.log({stc::LogLevel::Info, "This info is filtered out", "Database"}); // ❌ Блокируется LevelFilter
logger.log({stc::LogLevel::Error, "Database error occurred", "Database"}); // ✅ Проходит оба фильтра
logger.log({stc::LogLevel::Error, "UI error occurred", "UI"}); // ❌ Блокируется ComponentFilter
```
## Результат выполнения

После фильтрации в файле `app.log` и консоли появится только:

```
[ERROR] Database error occurred
```

## Паттерны проектирования

- **Strategy Pattern**: Форматтеры, выходы, фильтры реализуют разные стратегии
- **Chain of Responsibility**: Фильтры образуют цепочку обработки
- **Composite Pattern**: Множественные выходы работают как единое целое

## Сборка и запуск

mkdir build && cd build
cmake .. -DBUILD_EXAMPLES=ON
make
./bin/examples/basic_usage

Содержимое примера (`examples/basic_usage/main.cpp`) можно посмотреть [здесь](main.cpp).