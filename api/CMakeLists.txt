cmake_minimum_required(VERSION 3.15)
project(ApiProject)

# Install packages
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# Especifique a versão do C++ (por exemplo, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ativar todos os warnings para o seu projeto
add_compile_options(-Wall -Wextra -Wpedantic -O2)

# Adicione todos os arquivos .cpp na pasta src e src/models
set(SOURCES "src/models/pessoa.cpp" ${SOURCES})
set(SOURCES "src/database/database.cpp" ${SOURCES})
set(SOURCES "src/repositories/pessoas_repository.cpp" ${SOURCES})
set(SOURCES "src/requests/base_request_handler.cpp" ${SOURCES})
set(SOURCES "src/requests/pessoas_requests_handler.cpp" ${SOURCES})
set(SOURCES "src/requests/pessoas_router.cpp" ${SOURCES})
set(SOURCES "src/main.cpp" ${SOURCES})

# Crie o executável
add_executable(ApiProject ${SOURCES})

# Link Conan libs
target_link_libraries(ApiProject PRIVATE CONAN_PKG::restinio)
target_link_libraries(ApiProject PRIVATE CONAN_PKG::json_dto)
target_link_libraries(ApiProject PRIVATE CONAN_PKG::libpqxx)

# Incluir diretórios
target_include_directories(ApiProject PUBLIC ${PROJECT_SOURCE_DIR}/include)
