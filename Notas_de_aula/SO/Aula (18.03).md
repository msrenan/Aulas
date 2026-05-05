
## Funcionamento da CPU e interrupções

O processador funciona em um looping de busca e execução de instruções e dados da memória, e também pode tratar uma interrupção ou exceção.

#### Interrupção: 
Operações como recebimento de pacotes de dados, I/O de periféricos e transferência de dados de memória secundária, em geral são eventos externos.

#### Exceção:
Erros que geram a parada de programas em execução.

#### Instrução de Interrupção
* INT \[NUM] - Interrompe propositalmente a execução do programa e faz o processador apontar para o endereço de tratamento de exceção contido no índice NUM do vetor de interrupções. É uma das formas de chamar o SO, antigamente era usada para fazer syscalls.

#### Tratamento de Interrupções e Exceções
Ambos forçam um desvio no fluxo de dados da CPU, fazendo com que ele aponte imediatamente para o vetor de interrupções - um vetor contendo endereços de rotinas de tratamento de interrupções e erros. Este vetor é carregado durante o boot do SO. As rotinas de tratamento vem dos drivers de cada periférico ou afins, e o SO, durante o boot, carrega essas rotinas na memória e salva o endereço delas no vetor de interrupções.

#### O que a CPU faz em uma interrupção?
Quando uma interrupção ocorre, o CPU salva o PC e as flags atuais no stack, e em seguida sobrescreve o endereço do PC por um endereço do vetor de interrupções, o SO **não age no tratamento de interrupções**, o SO apenas carrega o vetor com as rotinas de tratamento.

#### Como o SO funciona em relação a execução de um programa
Quando a CPU executa um programa, o SO sai de cena e só pode retornar a atuar sobre o CPU quando há uma interrupção. Para evitar que algum programa monopolize o uso do processador, quebrando outras partes do próprio SO, existe um sistema de interrupções programadas no próprio SO e na placa-mãe do computador.

Pode ocorrer também que um programa precise dos serviços do SO e interrompa premeditadamente sua própria execução para que o SO possa fornecer o serviço que ele requer para continuar sua execução, esse processo é conhecido como Syscalls ou chamadas de sistema.

## Anéis de privilégios

Ferramentas de proteção para que apenas softwares específicos possam fazer contato mais direto com o hardware, geralmente representados de números por 0 a 3, em que 0 indica acesso direto ao hardware e 3 indica uma restrição para um programa interagir com o hardware.

(Pergunta: Como malwares conseguem permissão pra se replicar no Disco, considerando os anéis de privilégios?)

