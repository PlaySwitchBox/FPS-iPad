cmake_minimum_required(VERSION 3.21)
project(FpsMod VERSION 1.0.0 LANGUAGES CXX)

# Wir setzen C++17, da das Geode SDK moderne C++ Features nutzt
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wir nutzen FetchContent, um das Geode SDK herunterzuladen,
# ohne es manuell installieren zu müssen.
include(FetchContent)
FetchContent_Declare(
    geode-sdk
    URL "https://github.com/geode-sdk/geode/releases/download/v1.7.0/Geode-SDK-1.7.0.zip"
)

# Wir holen das SDK in den Build-Ordner.
FetchContent_MakeAvailable(geode-sdk)

# --- Moduel-Datei (dein Quellcode) ---
add_library(${PROJECT_NAME} MODULE)

# Wir fügen die Quelldateien hinzu.
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)

# --- Geode SDK Einstellungen ---

# Wir verbinden unser Modul mit der Geode Bibliothek.
# Das fügt automatisch die Header-Pfade und Linker-Flags hinzu.
geode_init_mod(${PROJECT_NAME})

# Wenn du auf macOS kompilierst, müssen wir für iOS spezifische Flags setzen,
# damit die Ausgabe als .dylib (bzw .a für geode) compiliert wird.
if(CMAKE_SYSTEM_NAME MATCHES "iOS")
    set(CMAKE_SYSTEM_PROCESSOR ARM64)
endif()

# --- Ausgabedatei ---
# Dies legt fest, wie die fertige Datei heißt.
set_target_properties(${PROJECT_NAME} PROPERTIES
    # iOS benötigt .dylib für dynamisches Laden, Geode packt das später in .geode
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    OUTPUT_NAME "custom_fps"
    SUFFIX ".dylib"
)
