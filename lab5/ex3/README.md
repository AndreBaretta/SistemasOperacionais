# `ex3` - Soma Vetorial com Memória Compartilhada e Processos

Este programa em C realiza a soma de dois vetores (`v1` e `v2`) usando múltiplos processos e memória compartilhada. Cada processo filho calcula uma parte da soma e armazena o resultado no vetor `v3`.

## Compilação

Para compilar o programa, execute:

```bash
make
```

Isso irá gerar o executável chamado `ex3` a partir do código-fonte `ex3.c`.

## Instalação

Para instalar o programa no diretório padrão (`/usr/local/bin`), execute:

```bash
make install
```

> Isso requer permissões de superusuário.

## Desinstalação

Para remover o executável instalado:

```bash
make uninstall
```

## Limpeza

Para remover o executável gerado localmente:

```bash
make clean
```

## Uso

Após compilar (ou instalar) o programa, execute com dois argumentos:

```bash
./ex3 <num_elements> <num_processes>
```

Ou, se instalado:

```bash
ex3 <num_elements> <num_processes>
```

### Parâmetros

- `<num_elements>`: número total de elementos nos vetores.
- `<num_processes>`: número de processos a serem criados para realizar o cálculo.

### Funcionamento

1. O programa aloca quatro áreas de memória compartilhada para os vetores `v1`, `v2`, `v3` e o vetor de controle `done`.
2. Inicializa `v1[i] = i` e `v2[i] = i * 2`.
3. Divide o trabalho entre os processos filhos, que somam elementos `v1[i] + v2[i]` e armazenam em `v3[i]`.
4. Após o término de todos os processos, o vetor `v3` é impresso.

### Exemplo de uso

```bash
./ex3 10 2
```

Saída esperada:

```
Resultado da soma:
V3[0] = 0
V3[1] = 3
V3[2] = 6
V3[3] = 9
V3[4] = 12
V3[5] = 15
V3[6] = 18
V3[7] = 21
V3[8] = 24
V3[9] = 27
```

## Observações

- O programa utiliza `shmget`, `shmat`, `shmdt` e `shmctl` para gerenciar memória compartilhada.
- A comunicação entre processos pai e filho é feita por meio de pipes (`pipe()`).
- É importante garantir que a soma de `num_elements` seja divisível por `num_processes` para divisão igual do trabalho, ou o último processo pode receber mais elementos.

