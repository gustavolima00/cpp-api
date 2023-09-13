Claro! Abaixo está o seu arquivo `README.md` atualizado para incluir informações sobre como instalar dependências via Conan.

---

# My API Project

Este é um projeto de exemplo que demonstra uma API simples escrita em C++. O projeto inclui exemplos de inicialização de uma API e manipulação de uma requisição HTTP fictícia.

## Pré-requisitos

- Um compilador C++ (g++, clang, etc.)
- GNU Make
- Terminal para executar comandos de shell
- Conan (Gerenciador de Pacotes para C++)

## Instalação de Dependências com Conan

Para instalar as dependências necessárias para este projeto, você precisará usar o Conan, um gerenciador de pacotes para C++. Se você ainda não tem Conan instalado, você pode instalá-lo usando pip:

```bash
pip3 install conan==1.61.0
```

Depois de instalar o Conan, navegue até o diretório raiz do projeto e execute o seguinte comando para instalar as dependências:

```bash
conan install . --build=missing
```

Isso instalará todas as bibliotecas necessárias que estão especificadas no arquivo `conanfile.txt` do projeto.

## Como Compilar e Executar

1. Abra o Terminal e navegue até o diretório onde você salvou o projeto.
2. Configure um profile para o conan caso não exista um

   ```bash
   conan profile detect
   ```

3. Instale as dependências utilizando Conan:

   ```bash
   conan install . --build=missing
   ```

4. Execute o comando `make` para compilar o projeto. Isso criará um diretório `build/` contendo o executável `my_api_project`.
5. Para executar o projeto, execute o seguinte comando:

   ```bash
   ./build/my_api_project
   ```
