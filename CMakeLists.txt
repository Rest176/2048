cmake_minimum_required(VERSION 3.20)
project(2048 LANGUAGES CXX)

# --- Настройка стандарта C++20 ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Поиск установленного SFML 3.x ---
# Важно: В SFML 3 компоненты пишутся с ЗАГЛАВНОЙ буквы!
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# --- Создание исполняемого файла ---
# Оставляем только ОДИН вызов add_executable
add_executable(2048 MACOSX_BUNDLE WIN32 main.cpp
        block.h
        block.cpp
        Game.h
        Game.cpp
        keyboard.h
        keyboard.cpp)

# --- Линковка библиотек SFML к вашему проекту ---
# Линкуем именно к TestSFML_1, а не к main
target_link_libraries(2048 PRIVATE SFML::Graphics SFML::Window SFML::System)