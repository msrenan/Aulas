
# Banco de dados

É uma coleção de dados relacionados. Nome, idade, etc. Os dados podem possuir **granularidades** variadas, ex: O campo idade pode ser em Anos, Anos + Meses, Anos + Meses + Dias, e assim vai. Em suma, é o nível de *detalhe* que cada dado exige.

**SGBD**: *Sistema de Gerenciamento de Banco de Dados* - Sistema computacional que gerencia a criação e manutenção de bancos de dados.

Na maioria das vezes, utilizar um SGBD é extremamente **benéfico** e facilita muito a produção e manutenção do banco de dados.

Contudo, em alguns casos, a utilização de um SGBD pode ser mais prejudicial do que benéfica, isso pois:
- Alguns **SGBDs** são extremamente caros, e exigem alto investimento para serem utilizados. Além disso, deve-se levar em conta a aquisição de hardware necessário para comportar e rodar o banco de dados, além de pagar um profissional especializado na manipulação do banco de dados, bem como seu treinamento.
- Se a necessidade do usuário exigir um **baixo tempo de resposta**, o SGBD pode ser uma barreira comprometedora das expectativas do usuário. 
- Se os dados são extremamente fixos e bem definidos - de modo que poucas ou nenhuma mudança é esperada em sua estrutura - pode ser benéfico desistir do uso de SGBDs.
- Em sistemas embarcados, cujo armazenamento é limitado, o SGBD ocupa espaço desnecessário.
- Caso seja uma aplicação muito simples e que não vão ocorrer acessos de múltiplos usuários aos dados, o SGBD também é dispensável.
- **Alguns setores, como o setor bancário, não utiliza SGBDs genéricos como Postgress. Eles, muitas vezes, desenvolvem os seus PRÓPRIOS SGBDs por questões de *segurança*.***

# \[Ler slides de conceitos de Banco de Dados e Arquitetura.]