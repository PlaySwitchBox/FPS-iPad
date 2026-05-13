cmake_minimum_required(VERSION 3.18)
project(TargetFPS)

# Standard C++ Einstellungen
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wir fügen den Geode SDK Ordner hinzu, damit wir die Header einbinden können.
# Hinweis: Dieser Ordner muss im Hauptverzeichnis liegen (geode-sdk).
add_subdirectory(geode-sdk)

# Definiert dein Modul. 'impl' ist der Name des Moduls.
geode::mod(impl)
