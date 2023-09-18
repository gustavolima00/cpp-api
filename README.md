# API em C++ com banco de dados

## Descrição

Este projeto é uma API desenvolvida em C++ para a [rinha de backend](https://github.com/zanfranceschi/rinha-de-backend-2023-q3)

## Pré-requisitos

- CMake >= 3.15
- Compilador C++ compatível com C++17 (GCC, Clang, MSVC, etc.)
- Conan

OU

- Docker e Docker Compose instalados

## Executando o Projeto sem docker

### Instalando pré requisitos

Primeiro instale o conan para gerenciar as dependências do projeto.

```bash
pip install conan==1.61.0
```

### Executando o projeto

Antes de executar o projeto é importante configurar um banco de dados postgres e atualizar a string de conexção através da variável de ambiente `DATABASE_CONNECTION_STRING`.

#### Compilação

Para compilar o projeto, siga os passos abaixo:

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

#### Execução

Após compilado o projeto será gerado um executável na pasta bin. Para executar o projeto siga os passos abaixo: 6. Execute o projeto:

```bash
./bin/ApiProject
```

### Executando o projeto com docker

```bash
docker-compose up --build
```

**Observação**: O comando acima iniciará o processo de construção e execução dos containers para a API e o banco de dados PostgreSQL.

#### Acesse a API

Após a conclusão do passo 3, a API estará acessível através da porta 9999.

#### Parar e remover os containers

Para parar e remover todos os containers, execute o comando:

```bash
docker-compose down
```

## Tecnologias Utilizadas

- C++
- PostgreSQL
- Docker
