# SimpleDu
## Trabalho Prático nº 1 - SOPE

Uso: ./simpledu dir flag1 flag2

[comment]: <> (www.tablesgeneratormarkdown_tables#)

| Flag          | Alternativa       | Descrição                                                                        |
|---------------|-------------------|----------------------------------------------------------------------------------|
| -a            | --all             | - mostra também files                                                            |
| -b            | --bytes           | - apresenta nº de bytes em vez de blocos                                         |
| -B [SIZE]     | --count-links     | - contabiliza multiplas vezes o mesmo ficheiro                                   |
| -l            | --block-size=SIZE | - define o tamanho dos blocos                                                    |
| -L            | --dereference     | - segue links simbolicos                                                         |
| -S            | --separate-dirs   | - não inclui o tamanho dos subdirs                                               |
| --max-depth=N |                   | - limita a informação exibida a N (0,1, …) níveis de profundidade de diretórios. |                - limita a informação exibida a N (0,1, …) níveis de profundidade de diretórios.

### Features
 - Apresenta diretórios e ficheiros
 - Apresentação em números de bytes ou número de blocos
 - Definição do tamanho default dos blocos
 - Preferência para contabilização múltipla de ficheiros
 - Apresentação de forma cumulativa do tamanho de subdiretórios e ficheiros incluídos
 - Não restringe os niveis de profundidade na estrutura de diretórios
 - Aceita flags repetidas tal como du
 - Geração de registos de execução
 - Comunicação entre processos via pipes

### Erros
 - CTRL+C explicado em baixo
 - Cálculo de Blocos ligeiramente diferente de du quando a flag -B está ativa com certos valores

### Details
 - Durante os testes, usando "~/" como dir, ao premir CTRL+C o programa demora um bocado a pausar todos os processos filhos (depende do computador) e com isso apresenta a mensagem indicativa de pausa e as opções disponiveis no meio da listagem do programa. Fazendo com que quando a listagem é pausada, o programa espera o input de "Y" ou "N" sem teto indicativo.
 - Fazendo CTRL+C , 'N' de input e CTRL+C outra vez, o programa não para.
 - Em du: tendo -b -B SIZE, dará o output em block sizes e não em bytes. No simpledu, com as mesmas flags dará output em bytes. Isto é intencional para que o display de bytes seja sempre ativado (ou não) tendo em conta a presença da flag -b e não a ordem das flags -b e -B.
