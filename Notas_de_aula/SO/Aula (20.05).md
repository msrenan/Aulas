# Swap
Área auxiliar da memória virtual que o SO pode armazenar áreas de um programa que não estão sendo utilizadas para liberar espaço na memória RAM para eventuais necessidades.

Se uma área de memória carregada na RAM deixa de ser usada por muito tempo por um programa, o SO pode colocar ela na área de SWAP, um intermediário que é mais rápido que a memória secundária em carregamento contudo ainda exige um acesso a memória. Isso é importante, pois dessa maneira o SO pode sempre deixar a RAM livre para eventuais necessidades, sem descartar totalmente páginas que não estão sendo utilizadas já há algum tempo.

# I-node
Estrutura que armazena os metadados, lista de blocos do disco, permissões e outras informações de uma **entrada** do Sistema de Arquivos. Uma entrada do sistema de arquivos pode ser tanto um arquivo quanto um diretório.

# Raíz do sistema de arquivos
O sistema unix começa o sistema de arquivos em "/" representando a raíz do sistema de arquivos. Então algumas pastas do sistema de arquivo, ex: /fedora, /ubuntu (exemplo genérico) podem conter diferentes sistemas de arquivos para diferentes sistemas/propósitos. Isso significa que qualquer pasta diretório dentro de um HD pode ser usado para *montar* um novo sistema de arquivos.

# Leitura de dados
Em uma syscall `read`, por exemplo, o SO utiliza a estrutura dos I-nodes para identificar o *path* para o bloco de memória necessário, e então o SO carrega um bloco inteiro (4KB) para o cache/buffer do Sistema de Arquivos e de lá, ele acessa a quantidade de dados específicada pelo usuário na syscall. Vale ressaltar que para manter esses dados em cacho/buffer, o SO coloca os dados em novas tabelas de páginas. Assim, em casos de acesso a dados dentro de um mesmo bloco, a operação de carregamento (a mais custosa) é executada apenas uma vez. Se a quantidade de informação do usuário superar um ou mais blocos, então todos são carregados para o buffer/cache.

Antes de carregar um bloco de memória em uma página de memória (repare que os blocos são separados em 4KB justamente para dar match com exatamente uma página da tabela de páginas), ele verifica se o bloco está em cache, se estiver,  faz o acesso direto pelo cache mesmo, senão, carrega o bloco

# Grevação de dados
Já na escrita de dados, a coisa muda um pouco. Se alterarmos 1 byte ou os 4KB de um bloco de memória, o SO vai fazer a transferência do **bloco inteiro** independente de termos mudado apenas 1 byte ou o bloco inteiro. Contudo, para não fazer acessos em demasia na memória secundária, o SO não escreve os dados imediatamente no disco, ele espera um timeout, ou então o arquivo ser fechado, ou então a alteração do usuário ultrapassar um bloco completo e aí sim, ele escreve esse bloco completo na memória secundária. 
> É por isso que caso a energia acabe repentinamente ou caso removamos um pen drive abruptamente de um PC, existe a chance de alguns dados ficarem incompletos/corrompidos ou algumas alterações não ficarem gravadas na memória: **As vezes, o desligamento repentino ocorrou bem no delay do SO para gravar os dados.**

