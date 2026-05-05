### Internet x Web
É importante ressaltar que ele não criou a **internet**, e sim a **web**. Enquanto a Internet é toda a infraestrutura de conexão em rede, que já existia desde a década de 60/70, a Web é camada de informação que transita nela.
# Surgimento da WEB e Tim Bernes Lee

### Quem foi Tim Bernes Lee
Tim Bernes Lee foi um Engenheiro de Software que atuava no CERN (Organização Europeia de Pesquisas Nucleares), na Suíça, no ano de 1989. Durante seu trabalho como programador, ele enfrentava um problema grave: o CERN recebia cientistas e pesquisadores de todo o mundo, cada um com sistemas e formatos de arquivos próprios, o que dificultava MUITO o compartilhamento de informações, um verdadeiro pesadelo logístico na época. Ele queria mudar isso, criando uma forma de uma pessoa qualquer, em um lugar qualquer, pudesse acessar os documentos de forma interconectada.

### A proposta
E foi assim, em 1989, que Tim escreveu a proposta de projeto: "Information Management: A Proposal", que foi recebida de maneira curiosa pelo seu chefe, o qual apenas escreveu uma nota no início do arquivo: "Vago, mas emocionante.". E foi com essa aprovação um tanto quanto ambígua que Tim Bernes Lee deu início ao seu projeto.

### Kit básico da Web
Para fazer seu projeto dar certo, Tim e seu colega Robert Cailliau desenvolveram as 3 principais tecnologias, que por sinal utilizamos até hoje:
- **HTML** - *HyperText Markup Language* (Línguagem de Marcação em Hipertexto): É a linguagem de marcação padrão para criar e estruturar páginas web, "o esqueleto do site".
- **HTTP** - *HyperText Transfer Protocol* (Protocolo de Transferência em Hipertexto): É o protocolo fundamental de transferência de dados da web, utilizado para transferir páginas HTML, imagens, vídeos e etc. entre um cliente (navegador) e um servidor.
- **URI/URL** - *Uniform Resource Identifier / Uniform Resource Locator* (Localizador/Identificador Uniforme de Recursos): Apesar de serem semelhantes, possuem uma diferença importante. Um URI é representado por uma sequência de caracteres utilizados para identificar um recurso da internet, podendo ser um nome, um local ou ambos as coisas. Um URL é um subconjunto do URI que busca um recurso, especificamente pelo seu local, geralmente é representado por um nome de domínio e uma porta.

Além disso, ele desenvolveu o primeiro navegador de internet conhecido, o WorldWideWeb e também o primeiro servidor do mundo.

### Abertura da Web
Em 1993, o CERN tomou a decisão de **liberar o código fonte da web para o domínio público, de graça**. Se Tim Bernes Lee tivesse patenteado a ideia, a Web poderia ter se tornado uma rede fragmentada, pequena e paga. Ao torná-la livre, ele permitiu que ela crescesse e alcançasse o patamar que ela tem hoje. Em 1991, o primeiro site do mundo foi ao ar, explicando justamente o que era a própria Web.

### Curiosidade
Vale ressaltar que o primeiro servidor da Web foi um computador da marca NeXT, a empresa que Steve Jobs fundou depois de ser expulso da Apple. Nele, havia uma etiqueta, escrito "Esta máquina é um servidor. NÃO DESLIGUE!" em vermelho. Jobs foi expulso da empresa que ele mesmo criou por conta de desapontamentos com o conselho administrativo depois do fracasso comercial do MacIntosh.

# Popularização da Web

### Do texto para o visual
Quando a Web surgiu, a experiência de navegar nela era bem escassa e entediante, existiam apenas textos e links azuis. Mas logo isso mudou com a criação de um novo navegador:
- **Mosaic (1993)**: Foi o primeiro navegador a exibir imagens junto ao texto dos sites. Antes disso, as imagens eram sembre abertas em janelas separadas. Isso tornou o texto dos sites mais flúidos e visualmente agradáveis.

Nessa época, os sites ainda era **estáticos**, ou seja, para mudar uma vírgula sequer no site, você tinha que editar o arquivo HTML e subir denovo via FTP. Um trabalho bem braçal mesmo.

### A era PHP
Em 1994, um programador chamado Rasmos Lerdof criou um conjunto de ferramentas para monitorar quem estava visitando o seu currículo online, o PHP/FI (*Personal Home Page / Forms Interpreter*).
Diferente do HTML, que apenas exibia o que era escrito, o PHP permitia que o servidor pensasse antes de retornar algo para o usuário.

**Qual o motivo do sucesso global disso?**
Certamente não foi por ser a linguagem mais elegante do mundo, mas sim por ser uma linguagem acessível:
- Era relativamente fácil de aprender na época, permitindo que você misturasse códigos PHP dentro das páginas HTML, sendo muito intuitivo para iniciantes.
- Dinamicidade: Com PHP e MySQL, nasceram os sites que armazenavam dados. Com isso, era possível criar sistemas de login, fóruns de discussão e posteriormente, até mesmo lojas virtuais.
-  O "Stack LAMP": O PHP era uma peça central de uma combinação muito poderosa e gratuita: **L**inux (Sistema), **A**pache (Servidor), **M**ySQL (Banco de dados) e **P**HP (linguagem). Essa combinação permitiu que diversas pessoas pudessem criar sites complexos sem gastar quase nada.

Em resumo, Tim Bernes Lee inventou o papel e a caneta (HTML/HTTP) e o PHP foi a máquina de impressão em massa que democratizou a programação de websites.

# Hospedagem dedicada
Para manter um site rodando na Web, você tinha que ter um servidor. Você podia ter você mesmo um servidor, o que era pouco viável na época, já que um computador não era a coisa mais acessível do mundo, ou você poderia buscar uma hospedagem.

A hospedagem dedicada era uma alternativa. Você pagaria um preço alto para manter seu site rodando, mas em troca você teria total controle sobre a máquina que você esta utilizando como servidor e o principal: **Exclusividade**: O servidor pelo qual você paga é somente para o seu uso, ele é _dedicado_ somenente a você.

Geralmente grandes empresas e ecommerces utilizavam este serviço, já que a hospedagem dedicada fornecia maior poder computacional e armazenamento, além de uma certa camada de segurança. Contudo, por conta do preço alto, pessoas comuns acabavam buscando uma alternativa mais viável. E assim nasceu...

# Hospedagem Compartilhada
Pense nisso como se fosse uma república onde moram 4 pessoas e só há 1 banheiro. Todo mundo usa o mesmo banheiro. Se uma pessoa fizer merda no banheiro, todas as 4 sofrem com isso.

Contudo, a vantagem monetária de uma hospedagem compartilhada era imensa, se você precisava de 200 dólares pra manter seu servidor rodando usando uma hospedagem particular, você precisaria de apenas 15 dólares para manter seu servidor rodando em uma hospedagem compartilhada, uma economia avassaladora de gastos.

#### Problemas
Por conta de todos os sites estarem hospedados em um mesmo servidor, os recursos que seu site poderia utilizar eram limitados. Caso contrário, se uma hospedagem compartilhada hospedava 100 sites, mas apenas um site gastou todos os recursos do servidor para fazer uma ação qualquer, os outros 99 sites ficariam fora do ar imediatemente.

Outro problema era a questão da segurança, já que nessa época, o controle de acesso para os conteúdos dos sites eram feitos na mão, usando o CHMOD, ou seja, se um usuário qualquer precisasse enviar um formulário para um site, a equipe técnica que mantinha o site no ar tinha que manualmente adicionar a permissão de ler, escrever e editar arquivos (CHMOD 777) para aquele usuário, um convite aberto para os cibercriminosos.

Mesmo com recursos limitados, a performance dos sites ainda era afetada pela quantidade de tráfego dos demais sites: Um site em pico de tráfego poderia desacelerar o poder computacional dos outros sites hospedados.

Por fim, um último problema que surgiu foi a limitação em configurações avançadas para o servidor e a falta de controle sobre o ambiente da hospedagem, pois se a hospedagem compartilhada desse permissão para todos os sites hospedados de mexer em requisitos técnicos do servidor e ter total controle sobre o ambiente de hospedagem, certamente o servidor viraria uma zona de guerra, em que cada site tentaria fazer uma coisa diferente para melhorar a própria situação, quebrando as configurações dos outros sites.

#### Paralelo com hoje em dia:
Hoje, apesar de ainda existirem algumas hospedagens compartilhadas, quem reina são os serviços de hospedagem e computação em nuvem. Uma alternativa muito melhor e mais poderosa do que as hospedagens compartilhadas.



---- RESTO = Marlon