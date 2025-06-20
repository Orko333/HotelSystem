cmake_minimum_required(VERSION 3.14)
project(HotelManagementSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Підключаємо FetchContent для управління залежностями
include(FetchContent)

# Встановлення кодування для Windows
if(WIN32)
    add_compile_options(-D_UNICODE -DUNICODE)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# --- Завантаження та конфігурація залежностей ---

# Дозволяємо SQLiteCpp самостійно завантажувати та компілювати sqlite3.
# Це найнадійніший спосіб уникнути конфліктів.
set(SQLITECPP_INTERNAL_SQLITE ON CACHE BOOL "Let SQLiteCpp fetch and build its own sqlite3" FORCE)
set(SQLITECPP_BUILD_EXAMPLES OFF CACHE BOOL "Build SQLiteCpp examples" FORCE)
set(SQLITECPP_BUILD_TESTS OFF CACHE BOOL "Build SQLiteCpp tests" FORCE)

FetchContent_Declare(
    SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
    GIT_TAG        3.3.1 # Використовуємо стабільний тег
)
FetchContent_MakeAvailable(SQLiteCpp)

# --- Налаштування проєкту ---

# Включення заголовкових файлів
include_directories(include)

# Створення списку всіх вихідних файлів .cpp
set(SOURCES
    src/main.cpp
    src/utils.cpp
    src/db/DatabaseManager.cpp
    src/dao/UserDAO.cpp
    src/dao/RoomDAO.cpp
    src/dao/ClientDAO.cpp
    src/dao/BookingDAO.cpp
    src/logic/AuthenticationManager.cpp
    src/logic/HotelManager.cpp
    src/ui/Menu.cpp
)

# Додавання виконуваного файлу
add_executable(HotelSystem ${SOURCES})

# Зв'язування бібліотек
# SQLiteCpp автоматично підключить sqlite3, коли SQLITECPP_INTERNAL_SQLITE=ON
target_link_libraries(HotelSystem
    PRIVATE
    SQLiteCpp
)

# Для правильного відображення кирилиці в консолі Windows
if(WIN32)
    target_link_libraries(HotelSystem PRIVATE kernel32)
endif() 