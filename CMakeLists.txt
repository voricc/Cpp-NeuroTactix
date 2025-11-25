cmake_minimum_required(VERSION 3.5.0)
project(NeuroTacticx VERSION 0.1.0)

# Setzt den Ausgabeordner auf /bin (damit alles sauber bleibt)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)

# ------------------------------------------------------------------
# SFML EINBINDEN
# ------------------------------------------------------------------
# Option A: Wenn du SFML im Ordner "vendors/sfml" liegen hast (dein Template):
add_subdirectory(vendors/sfml/)

# Option B: Falls du SFML installiert hast und NICHT im vendors ordner hast,
# dann kommentiere "add_subdirectory" oben aus und nutze stattdessen:
# find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

# ------------------------------------------------------------------
# QUELLCODE FINDEN
# ------------------------------------------------------------------
# WICHTIG: Mein Code nutzt .h, dein Template suchte nach .hpp.
# Habe das hier auf .h geändert:
file(GLOB_RECURSE ALL_SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE ALL_HEADER_FILES "${CMAKE_SOURCE_DIR}/include/*.h")

# Executable erstellen
add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES} ${ALL_HEADER_FILES}
        main.cpp)

# ------------------------------------------------------------------
# SETTINGS & INCLUDES
# ------------------------------------------------------------------
# Wir nutzen C++20 für die KI-Logik
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

# Include Verzeichnisse
target_include_directories(
        ${PROJECT_NAME}
        PRIVATE ${PROJECT_SOURCE_DIR}/include/
        # Falls SFML via vendors gebaut wird:
        PRIVATE ${PROJECT_SOURCE_DIR}/vendors/sfml/include/
)

# Bibliotheken linken
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio)

# ------------------------------------------------------------------
# RESSOURCEN KOPIEREN
# ------------------------------------------------------------------
# Kopiert den resources Ordner (falls vorhanden)
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}/bin)

# WICHTIG: Kopiert die arial.ttf direkt zum Executable,
# damit das Spiel nicht abstürzt ("Font not found")
file(GLOB FONT_FILES "${CMAKE_SOURCE_DIR}/*.ttf")
file(COPY ${FONT_FILES} DESTINATION ${CMAKE_BINARY_DIR}/bin)