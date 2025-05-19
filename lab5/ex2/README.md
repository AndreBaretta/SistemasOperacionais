# `ex2` - Programa de Entrada Interativa com Salvamento em Arquivo

Este programa em C lê entradas de texto do usuário via terminal e grava cada linha digitada em um arquivo chamado `data.txt`. A execução pode ser interrompida com `Ctrl+C` (SIGINT), e o programa realiza uma finalização limpa.

## Compilação

Para compilar o programa, você pode simplesmente rodar:

```bash
make
```

Isso irá gerar o executável chamado `ex2` a partir do código-fonte `ex2.c`.

## Instalação

Para instalar o programa no diretório padrão (`/usr/local/bin`), execute:

```bash
make install
```

> Isso requer permissões de superusuário, pois o destino é um diretório do sistema.

## Desinstalação

Para remover o executável instalado, execute:

```bash
make uninstall
```

## Limpeza

Para remover o executável gerado localmente:

```bash
make clean
```

## Uso

Após compilar (ou instalar) o programa, você pode executá-lo com:

```bash
./ex2
```

ou, se instalado:

```bash
ex2
```

### Funcionamento

- O programa solicita entradas de texto do usuário no terminal.
- Cada entrada é gravada no arquivo `data.txt` no mesmo diretório em que o programa é executado.
- A numeração das entradas começa em `-1`.
- Para finalizar, pressione `Ctrl+C`. O programa capturará o sinal e encerrará de forma segura, fechando o arquivo.

### Exemplo de uso:

```text
Digite textos para serem gravados em 'data.txt'. Pressione Ctrl+C para sair.
Entrada -1: Olá, mundo!
Entrada 0: Outra linha qualquer
Entrada 1: Ctrl+C
Sinal 2 recebido, finalizando...
[INFO] Fechando o arquivo e encerrando...
```

O conteúdo do arquivo `data.txt` após a execução será semelhante a:

```
Linha -1: Olá, mundo!
Linha 0: Outra linha qualquer
Linha 1: Ctrl+C
```
