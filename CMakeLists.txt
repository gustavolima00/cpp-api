cmake_minimum_required(VERSION 3.15)
project(MyAPIProject)

# Integre Conan com CMake
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS) # Modifique para usar o modo TARGETS do conan_basic_setup

# Especifique a versão do C++ (por exemplo, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ativar todos os warnings para o seu projeto
add_compile_options(-Wall -Wextra -Wpedantic)

# Adicione todos os arquivos .cpp na pasta src
file(GLOB SOURCES "src/*.cpp")
# Crie o executável
add_executable(MyAPIProject ${SOURCES})

# Link Conan libs
target_link_libraries(MyAPIProject PRIVATE CONAN_PKG::restinio)
target_link_libraries(MyAPIProject PRIVATE CONAN_PKG::json_dto)

# Incluir o diretório onde api_handlers.h está, para o seu código
target_include_directories(MyAPIProject PUBLIC include/)
