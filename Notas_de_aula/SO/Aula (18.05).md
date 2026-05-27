
# Disco
- Cilindros
- Cabeças
- Setores por trilha (512 bytes por setor)
- LBA (Logical blocking processing)
- Linearização dos blocos

O Disco é essencialmente um dispositivo de **blocos**, não se pode ler o escrever bits, e sim apenas blocos de bytes. Um bloco (ou setor) possuí cerca de 512 bytes. Se o SO utiliza 8 blocos para uma ação,
ele está utilizando exatamente 1 página (4096 bytes). Existe uma fragmentação do último bloco - sempre sobra mais espaço do que é utilizado - que o SO pode mitigar utilizando algumas estratégias. Quanto menor o tamanho do bloco, **maior a quantidade de ponteiros e variáveis de controle necessárias para garantir uma boa transferência de dados.** O SO usa um processamento lógico de blocos (LBA) para conseguir identificar e manipular cada bloco do disco de maneira independente. Para que o SO possa gerenciar os espaços livres no disco, ele utiliza do **Sistema de Arquivos**.
# Sistema de Arquivos
- Superbloco
- Gerenciamento de blocos livres
- I-Nodes
- Diretório Raíz
- Arquivos e diretórios
- Blocos livres e ocupados

O Sistema de Arquivos é o artifício que o SO utiliza para gerenciar e manipular os blocos vazios em disco. O disco é tratado como um bloco dentro do sistema de arquivos, que é sub-dividido em **partições**, em que cada partição abriga um sistema de arquivos diferente. Cada partição é sub-dividida em:
- Blocos de inicialização - *Incluem informações sobre como iniciar o sistema operacional dessa partição.*
- Superbloco - *Conjunto de metadados relacionados ao sistema de arquivos: data de criação, tamanho dos blocos já organizados, número de I-Nodes reservados, espaço de blocos - informações que ajudam o SO a entender como os dados dessa partição estão organizados.*
- Gerenciamento de espaço livre - *Indica o que sobra após a inicialização, sendo basicamente um **bitmap** que indica quais blocos estão ocupados s livres*.
- I-Nodes - *Um I-Node é uma estrutura usada para indicar cada entrada no sistema de arquivos, diretórios, pipes, sockets... Contém o nome, tipo, datas, permissões e outras informações de controle para aquela entrada do sistema de arquivos. Contém também a relação entre todos os I-Nodes do SO, o que representa a organização lógica do disco. Cada I-Node também precisa conter uma indicação de qual bloco da seção de Arquivos e Diretórios eles representam. Essa divisão representa o espaço reservado para o armazenamento de todos os possíveis I-Nodes que aquela partição pode ter.*
- Diretório-raíz - *É uma indicação para qual dos I-Nodes reservados indica a raíz daquele sistema de arquivos. Pois é a partir do diretório-raíz que todos os outros diretórios daquele sistema de arquivos podem ser encontrados.*
- Arquivos e diretórios - *Todo o conjunto de blocos que não são reservados para controle - onde os dados do usuário são adicionados e removidos.*

Partições podem ser montadas de jeitos diferentes. 
# Montagem do Sistema de Arquivos
- Lê informações do Sistema de Arquivos (superbloco + raíz)
- Árvore lógica de diretórios

Montar um sistema de arquivos significa que o SO vai ler os metadados do superbloco daquele sistema de arquivos, verificar se ele tem suporte para aquele sistema de arquivos, então lê qual é o I-Node raíz para descorir seus metadados e começa a ler o conteúdo dentro do diretório-raíz e a partir da raíz, todos os arquivos do sistema são encontrados e carregados no Sistema de Arquivos, o que simboliza a montagem.
O SO vai manter uma estrutura em sua memória que é a raíz do sistema de arquivos, pois a partir dela o SO consegue acessar qualquer área do sistema de arquivos.

O comando `mount` monta um sistema de arquivos no diretório passado. Essencialmente, o SO usa um `mount <disk> /` para montar o sistema de arquivos principal. Contudo, o SO não carrega TODA a informação sobre a árvore de diretórios. Ele carrega ela sob demanda. Primeiramente carrega só a raíz, e o resto da árvore lógica de diretórios é preenchida conforme a necessidade de acesso aos diretórios e arquivos do Sistema de Arquivos.

# Processo
- `open(path, flags, node)`
- `read(fd, buf, count)`
- `write(fd, buf, count)`
- `lseek [...]`
