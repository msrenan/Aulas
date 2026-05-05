# Load Balancing
## Tópicos
- O problema e a solução
- O que é?
- Como funciona?
- Vantagens
- Jeitos de fazer o balanceamento.
- Live Coding
- E o psel?

## O problema e a solução
Bom, então vamos começar a entender o que é um Load Balancer. Imagine o seguinte problema: Eu tenho um servidor que consegue processar, teróricamente, 30 requisições ao mesmo tempo. Contudo, na prática, quando meu servidor recebe 20 requisições, ele já começa a apresentar lentidão. 

Uma das soluções possíveis pra esse problema, seria comprar mais 4 servidores que processam 30 requisições cada ao mesmo tempo, e dividir os visitantes dos sites entre os 5 servidores. Mas aí surge outro problema, desse modo, teríamos que ter 5 sites diferentes para cada servidor. Então surge o problema principal: **Como distribuir as conexões de cada cliente para os 5 servidores, fazendo com que eles acessem só um site?**

A resposta para essa pergunta é um servidor proxy. Mais específicamente um proxy reverso. Um que atue decidindo para qual dos servidores cada cliente vai ser redirecionado. Isso garante alguns benefícios: 
- Nenhum servidor vai ficar cheio ao ponto de não conseguir processar as requisições.
- Já que agora o cliente manda suas requisições no proxy, o servidor fica escondido do cliente, adicionando uma camada de segurança na sua aplicação.

Isso, senhoras e senhores, é um exemplo de um Load Balancer.

## O que é?
O Load Balancer é um tipo especializado de proxy reverso, focado na distribuição inteligente de conexões cliente -> servidor. Existem inúmeros motivos para se utilizar um load balancer e existem inumeros critérios de distribuição também, o que torna um load balancer um elemento muito versátil e necessário na infraestrutura de qualquer projeto web.

## Como ele funciona?
Existem vários algorítmos de escalonamento que um Load Balancer pode adotar, então vou abordar apenas algumas mais relevantes:
- **Redirecionamento 'burro'**: Ideal caso você esteja enfrentando um gargalo de acessos em um servidor. Você compra um segundo servidor. Quando o primeiro lotar, todas as contexões são redirecionadas para o segundo servidor. Contudo, isso implica que o primeiro servidor sempre está operando na capacidade máxima, o que não é muito bom a longo prazo, pois ele se desgasta mais rapidamente.
- **Round Robin**: Neste algoritmo o proxy realiza um rodizo dos servidores existentes para fazer o redirecionamento. A primeira requisição vai pro servidor A, a segunda para o B, a terceira para o C, a quarta para o D, a quinta pro E e a sexta para o A novamente. É um algoritmo mais eficiente em questão de utilização dos recursos dos servidores, já que eles irão ser preenchidos gradativamente, o que faz com que a aplicação precise de mais acessos para que os servidores começem a trabalhar com capacidade máxima preenchida.
- **Weighted Round Robin**: Se você sabe que o servidor A tem o dobro de poder de processamento que os demais, então você manda duas requests para o A antes de fazer o rodízio para o B. Isso garante uma certa proporção poder de processamento X requests para processar. 
- **Least Connections**: Um método inteligente, prioriza o envio de requisições sempre para o servidor com menos conexões ativas. Na prática, isso significa que todos os servidores estão igualados em questão de média de conexões ativas. É um bom jeito de garantir que nenhum servidor fique lotado antes da hora.
- **IP Hash**: Um método que possuí aplicação prática mais específica, se resume em atrelar o IP do cliente (na forma de um Hash) a um servidor específico. Dessa forma, todas as requisições daquele IP vão sempre ser redirecionadas para o mesmo servidor. Isso implica que aplicações envolvendo controle de sessão não vão se quebrar caso o cliente tenha que se reconectar.
- **Least Response Time**: Considerado o melhor método de escalonamento, redireciona as requisições para o servidor com menor tempo de resposta. Priorizando a experiência do usuário.

## Vantanges

## Maneiras de implementar

## Live coding

## E o psel?

