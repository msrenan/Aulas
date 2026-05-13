# FileSystem

\[Olhar os slides para informações iniciais]

O sistema de arquivos é uma abstração do SO para a utilização dos blocos de informação salvos persistentemente na memória (secundária). A memória secundária funciona com base em discos que armazenam bits e controladores que escrevem ou leem bits armazenados.

Os controladores de disco, independente de ser HD ou SSD, possuem apenas duas operações:
- Leia o bloco K
- Escreva no bloco K

Diretórios, do ponto de vista do SO são "Arquivos" que conteḿ "ponteiros" para outros arquivos e algumas informações descricionais sobre o conteúdo do que possui dentro do arquivo.

