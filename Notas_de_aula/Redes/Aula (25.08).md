
# Introdução

## Comutação por pacotes x Comutação por circuitos
- **Por Circuitos**: Utilizada na telefonia, a ideia inicial de redes era fazerem os computadores se comunicarem da mesma maneira que os telefones faziam. Utilizava *Frequency Division Multiplexing* para conseguir fazer várias linhas telefônicas se comunicarem a partir de um único fio de cobre - economizando custos. A FDM funcionava reduzindo a frequência de som que era transmitida por cada linha, para conseguir fazer mais de uma caber em um fio de cobre. Em 1960 a FDM foi substituída pela *Time Division Multiplexing*, um método que dividia as linhas em fatias de tempos igualitárias, que se conectavam por meio de um multiplexador. A comutação por circuítos, independente da tecnologia, depende de definir uma conexão física entre as duas pontas que vão se comunicar, **antes da comunicação iniciar** - o circuito - além de requerer componentes físicos e uma grande infraestrutura para funcionar, ex: Cabos de Cobre / Frequência do cabo de cobre / Fatia de tempo.
- **Por Pacotes**: Inspirações nos Correios: para mandar uma carta, você apenas escreve a carta, coloca em um envelope indicando os dados de remetente e destinatário. Você não precisa reservar componentes físicos, nem estabelecer o circuíto. Você indica, o CEP do destinatário, que foi projetado para dividir o país em regiões, cidades e barrios. O centro dos correios recebe a carta e distribuí de acordo com o CEP, enviando a carta de centro em centro dos correios, até que ela fique em um centro próximo o suficiente do local de entrega. O sistema de redes moderno funciona de maneira análoga aos correios. É estatisticamente mais eficiente que a comutação por circuítos, já que nem sempre a rede estará sendo utilizada por todos, o que permite uma maior taxa de transmissão por conexão, o que jamais seria possível pela comutação por circuitos.
	- **ARPANET** - *Advanced Research Projects Agency Network*: A primeira rede de longo alcance a funcionar por meio de comutação de pacotes. A ideia era produzir uma rede de troca de informações descentralizada, para caso fosse derrubada, pudesse ser reerguida. Contava com grande infraestrutura física, que foi reaproveitada posteriormente, no nascimento da internet moderna.
	- **CYCLADES**: Projeto francês parecido com a ARPANET situado no mesmo espaço temporal, porém menor e com algumas diferenças. Funcionalidades e ideias nascidas neste projeto foram reaproveitadas, também, na internet moderna. O modelo dos correios para a internet foi amplamente desenvolvido e expandido nesse projeto.

## Modelo OSI
- 7 - Aplicação: **HTTP**, DNS e DHCP, SSH, IMAP/POP3, SMTP, FTP, IRC, etc. são alguns protocolos dessa camada. A finalidade é prover funcionalidade ao usuário.
- 6 - Apresentação: A finalidade dessa camada é prover criptografia, compressão ou conversão de formato/codificação.
- 5 - Sessão:  A finalidade é prover restauração de sessão.
- 4 - Transporte: A funcionalidade básica é entregar dados para o processo correto, o UDP. Se precisar garantir a entrega dos dados, você utiliza o TCP.
- 3 - Rede: Protocolo **IP** é o principal. Sua finalidade é levar o pacote da origem até o destino. Existe em duas versões principais: IPv4 e IPv6.
- 2 - Enlace: Finalidade básica é delimitar pacotes - indicando onde começa e onde termina um pacote. **Ethernet, SLIP,  Wifi e PPP**.
- 1 - Física: Mapeia grandeza física (tensão) em bits e vice-versa. **Ethernet e Wifi**.

