# Meu Projeto de API em C++

## Descrição

Este projeto é uma API desenvolvida em C++ para manipular um catálogo de livros. Utiliza Docker para facilitar o processo de desenvolvimento e produção, incluindo a configuração do banco de dados PostgreSQL.

## Pré-requisitos

- Docker e Docker Compose instalados

## Como rodar o projeto

### Inicie os containers do Docker

```bash
docker-compose up --build
```

**Observação**: O comando acima iniciará o processo de construção e execução dos containers para a API e o banco de dados PostgreSQL.

### Passo 4: Acesse a API

Após a conclusão do passo 3, a API estará acessível através da porta 8080. Por exemplo:

- Lista de livros: `http://localhost:8080/`
- Livro específico: `http://localhost:8080/:id`

### Passo 5: Parar e remover os containers

Para parar e remover todos os containers, execute o comando:

```bash
docker-compose down
```

## Estrutura do Projeto

- `/api` - Contém o código-fonte da API em C++
- `/database` - Contém o script SQL de inicialização do banco de dados
- `Dockerfile` - Define como construir a imagem Docker para a API
- `docker-compose.yml` - Define os serviços, redes e volumes do Docker

## Tecnologias Utilizadas

- C++
- PostgreSQL
- Docker
