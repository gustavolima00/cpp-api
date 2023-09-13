# API em C++

Este é um exemplo de projeto API em C++ que utiliza Conan para gerenciar dependências e Docker para facilitar o desenvolvimento e a implantação.

## Pré-requisitos

- CMake >= 3.15
- Compilador C++ compatível com C++17 (GCC, Clang, MSVC, etc.)
- Conan
- Docker (opcional)

## Executando o Projeto Localmente

### Usando Conan 1.61.0 para Instalar Dependências

#### Instalando o Conan

```bash
pip install conan==1.61.0
```

#### Instalando Dependências

1. Abra um terminal e navegue até o diretório do projeto.
2. Crie um diretório de build e navegue até ele:

   ```bash
   mkdir build
   cd build
   ```

3. Instale as dependências usando Conan:

   ```bash
   conan install .. --build=missing
   ```

4. Gere os arquivos de build usando CMake:

   ```bash
   cmake ..
   ```

5. Compile o projeto:

   ```bash
   make
   ```

6. Execute o projeto:

   ```bash
   ./bin/MyAPIProject
   ```

### Usando Docker

Se você preferir usar Docker, você pode construir e rodar uma imagem Docker do projeto.

1. Abra um terminal e navegue até o diretório do projeto.
2. Construa a imagem Docker:

   ```bash
   docker build -t my_api_project .
   ```

3. Rode a imagem Docker:

   ```bash
   docker run -p 8080:8080 my_api_project
   ```

Com isso, seu serviço deve estar rodando na porta `8080` e você pode acessá-lo através de `http://localhost:8080`.
