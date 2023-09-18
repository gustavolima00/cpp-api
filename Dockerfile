# Use uma imagem base com CMake e um compilador C++
FROM ubuntu:latest

# Instale pacotes necessários
RUN apt-get update && apt-get install -y \
  build-essential \
  cmake \
  git \
  python3 \
  python3-pip

# Instale o Conan
RUN pip3 install conan==1.61.0

# Copie o projeto para o container
COPY . /app
WORKDIR /app

# Configure o Conan
RUN conan profile new default --detect && conan profile update settings.compiler.libcxx=libstdc++11 default

# Instale as dependências e compile o projeto
RUN rm -rf build && \
  mkdir build && \
  cd build && \
  conan install .. --build=missing && \
  cmake .. && \
  make

# Comando para executar o projeto
CMD ["./build/bin/ApiProject"]
