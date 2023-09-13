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
RUN rm -rf build

# Instale as dependências do projeto com Conan
RUN mkdir build && cd build && \
  conan install .. --build=missing && \
  cmake .. && \
  make

# Comando para executar o projeto
CMD ["./build/bin/MyAPIProject"]
