cmake_minimum_required(VERSION 3.22)

project(Proyecto_1 VERSION 1.0)

# Enable C++20 features
set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

# Set SDL2_DIR if SDL2 is installed in a non-standard location
set(SDL2_DIR "D:/SDL2/cmake") # Replace with the actual path

# Set GLM
set(glm_DIR "D:/glm/cmake/glm")

# Find SDL2
find_package(SDL2 REQUIRED PATHS "D:/SDL2/")

# Find GLM
find_package(glm REQUIRED PATHS "D:/glm/")

# Find OpenMP
find_package(OpenMP REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    ${SOURCES}
)

# Link against SDL2 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)


