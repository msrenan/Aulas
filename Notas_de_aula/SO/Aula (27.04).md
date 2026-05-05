# \[...] Continuação sobre memória

## Resumindo
Cada programa é criado usando um espaço de endereçamento virtual para mapear as áreas do programa. Isso permite que todos os programas sejam iniciados com endereçamento padronizado. Dois programas, lógicamente podem apontar para o mesmo endereço. Fisicamente, por outro lado, isso é inviável.

Para resolver este problema, o SO utiliza a paginação da memória RAM e tabelas de página para garantir que os endereços lógicos sejam traduzidos em endereços efetivos da memória física.

Uma tabela de página é parecida com:
```
----------------------------------------------------------------------------
Bit de presença | Bits sobre as permissões | Endereço efetivo do page frame
----------------------------------------------------------------------------
```

O Bit de presença indica se a página está mapeada em memória física. Os bits sobre as permissões indicam quais permissões aquela página precisa. O endereço efetivo do **page frame** (página mapeada físicamente na memória), um ponteiro.

Então o SO mantém a RAM "modularizada", já que ele divide ela em segmentos de 4KB, o qual cada pedacinho de 4KB é um page frame. Os page frames de um mesmo programa não precisam mais ser contíguos dessa maneira. E eles também não precisam estar todos carregados em memória.

Se vários programas com tamanho maior do que a RAM estão em execução ao mesmo tempo, todos eles são parcialmente carregados na RAM. Em tempo de execução, se qualquer um dos programas pedir uma pagina para a tabela que tem `0` no bit de presença (indicando que a página necessitada não está mapeada na RAM), ocorre uma **page fault**.

Existem 2 tipos de page fault. No caso supracitado, estamos falando sobre um _hard page fault_, em que a MMU (**Memory Managementi Unit**) vai precisar carregar a página que está em falta na memória e indicar sua presença na tabela de páginas. A page fault ocorre pode ocorrer em tempo de execução, sem comprometer o funcionamento geral da aplicação (SO salva contexto, inicia o processo de carregamento da página necessária na RAM, atualiza da tabela, restaura o contexto do programa e continua sua execução).

Contudo, as vezes várias instruções em sequência ocorrem dentro de uma mesma página, e assim fica custoso para o processador ficar consultando a tabela de páginas toda vez que precisar acessar instruções em sequência de uma _mesma_ página.

Para contornar isso, existe o **TLB** (Translation Lookaside Buffer), que é básicamente uma memória cache da tabela de páginas. Assim, a MMU consulta primeiro o cache para ver se o endereço requisitado está em fácil acesso. DIto isso, o **segundo tipo** de page fault, _soft page fault_ ocorre quando uma página existe na tabela de páginas mas não existe no TLB, o que requer que o acesso (que ficaria custoso se repetido várias vezes para obter o mesmo resultado) ocorra apenas uma vez:
- O SO pega as informações na tabela de páginas.
- Essas informações serão utilizadas na execução do programa, **mas antes são armazenadas no TLB para garantir a otimização**.

A tabela de página existe em memória RAM. Quando um programa é carregado na memória, o SO cria sua tabela de páginas e carrega algumas páginas para poder iniciar a execução do programa. O resto das páginas é carregada conforme a demanda dos endereços virtuais do programa.

A área virtual de um programa é mapeada na fase de compilação.
# Áreas de memória de um processo

Dentro da área de memória virtual de um processo, existem alguns segmentos:
- Segmento de código
- Segmento de dados
- Heap
- Stack

O Segmento de código contém todo o código executável de um processo, as instruções.
O Segmento de dados (globais) contém todas as variáveis globais do código.
O Heap é um segmento criado "em tempo de execução", que mapeia uma área de memória vazia para que ela se torna uma área capaz de guardar informações especificadas pelo programador.

## Pilha
A pilha de execução de um programa contém todas as chamads de funções. Cada função possuí um **stack frame**: uma área de memória que contém o endereço para a área de código da função e contém todas as variáveis e parâmetros necessários para o bom funcionamento da função. Isso ocorre fixando a base da pilha e incrementando o topo da pilha, adicionando um stack frame no topo da pilha, que basicamente contém todas as informações de uma função.
