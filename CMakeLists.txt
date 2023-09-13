cmake_minimum_required(VERSION 3.15)  # Versão mínima do CMake

project(MyAPIProject)  # Nome do seu projeto

set(CMAKE_CXX_STANDARD 17)  # Versão do C++

# Importe as configurações do Conan
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Adicione os arquivos do seu projeto aqui
add_executable(MyAPIProject src/main.cpp src/api_handlers.cpp)

# Link com as bibliotecas do Conan
target_link_libraries(MyAPIProject ${CONAN_LIBS})
