
# O protocolo HTTP



- **Cookies**: Informações inseridas pelo navegador, de maneira automática, no cabeçalho de todas as requests, para inserir informações na requisição. É útil pois possibilita inserir dados em um GET, o que não era possível anteriormente... Sendo necessário fazer um POST, mesmo se apenas fosse necessário receber os dados de uma página - por exemplo.

## HTTP/1.0 x HTTP/1.1
- **1.0**: Uma request por conexão. Ao fim do recebimento dos dados, a conexão é encerrada.
- **1.1**: Uma conexão pode suportar várias requisições. Para saber se todos os dados foram recebidos, cabeçalhos como *Content-length* e *Truncate Encoded* servem para informar o tamanho dos dados que devemos esperar!