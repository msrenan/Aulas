
# Tópicos:
- Sockets
- OSI model | TCP/IP - Eu
- Protocolos
- Pacotes - Eu
- Http
- Live Coding - Eu 

---
# OSI model
_Open Systems Interconnection_

É uma estrutura conceitual que divide a comunicação via rede em 7 camadas, facilitando a interoperabilidade entre sistemas de hardware e software.

### 7ª Camada (Aplicação)
É onde há a interação do usuário. Não consiste no navegador em si, mas sim nos protocolos (HTTP e etc) que a aplicação utiliza para enviar uma requisição/pacote.

### 6ª Camada (Apresentação)
É onde a informação é tratada antes de ser enviada, como formatação de caracteres ou até mesmo criptografia.

### 5ª Camada (Sessão)
É onde a conexão entre dois dispositivos é gerenciada, onde a conexão é aberta, mantida e fechada.

### 4ª Camada (Transport)
É onde o envio ocorre, nessa camada agentes responsáveis por cuidar da integridade do que está sendo enviado e do sucesso do envio existem.

### 3ª Camada (Rede)
É onde acontece o roteamento e o endereçamento lógico (usando endereço IP) de cada dispositivo.

### 2ª Camada (Data Link)
É onde se estabelece a conexão física entre dois dispositivos. É nela que endereços físicos (MAC addresses) são utilizados.

### 1ª Camada (Física)
É a parte tangível da internet, a parte mais física de todas.

Por onde circula o que foi pedido na 7ª camada, forjado na 6ª, enviado pela 4ª, usando a conexão aberta pela 5ª,endereçada ao dispositivo roteado a 3ª camada, que circulou usando a conexão física oriunda da 2ª camada e a infraestrutura da internet da 1ª camada.


O OSI model foi pensado para facilitar o desenvolvimento e troca de informações entre dois dispositivos por meio da internet. Mas no fundo, ele é um mapa conceitual para o desenvolvimento.
Na prática nos temos o modelo **TCP/IP**:

## TCP/IP
O modelo TCP/IP é a aplicação prática do OSI model. Mas com algumas divergências, ele é dividido em camadas mais abrangentes, constituindo um mapa de 4 camadas:
### 4ª Camada (Aplicação)
Abrangindo as camdas 5, 6 e 7 do OSI model, é aqui a interação do usuário, tratamento de dados e gerenciamento de conexões ocorre.

### 3ª Camada (Transporte)
É equivalente a camada 4 do OSI model, aqui o envio é tratado, onde se garante a integridade do que está sendo enviado e o sucesso do envio. É onde entram os protocolos TCP e UDP.

### 2ª Camada (Rede)
É equivalente a camada 3 do OSI, sendo o coração do modelo TCP/IP. É aqui onde o roteamento acontece. É nesta camada que os segmentos de informação são embalados em pacotes e endereçados a um IP específico. Não importa se a informação vai chegar ou não, pois isso é trabalho da terceira camada.

### 1ª Camada (Interface de Rede)
Engloba as camdas 1 e 2 do OSI model, é onde se define como os dados serão enviados físicamente através da rede. É aqui que o mapeamento entre o endereço lógico (IP) e o endereço físico (MAC) ocorre.

Pense nesses dois modelos como o passo a passo de envio e recebimento de dados pela rede. Se você ler no sentido 1ª -> 7ª/4ª ou 7ª/4ª -> 1, você consegue ter um vislumbe de como dois computadores podem se comunicar via rede.

---
# Pacotes

O pacote é a forma como nos referimos ao modelo de estrutura de dados que corre pela internet. As requests não são só jogadas na rede. Elas são encapsuladas em diversas camadas que revelam muito sobre o pacote.

Imagine que cada uma das 4 camadas do TCP/IP pega o que o usuário enviou, embrulha numa sacola de presente e coloca uma etiqueta com algumas informações importantes. É assim que funciona.

Em resumo, o pacote tem algumas partes:
- **Cabeçalhos (headers)**: Contém instruções de navegação (endereços, portas, protocolos, etc,)
- **Payload**: É a carga útil (o texto ou a requisição)
- **Rodapé (trailer)**: Geralmente contém códigos de verificação de integridade.

## Análise camada por camada:

- Os primeiros bytes de um pacote contém os invólucros da camada 2: Contendo endereços MAC de destino e origem e o *etherType* (Um código que avisa "O que tem aqui é um pacote IP")
- Depois vêm os bytes da camada 3: Contendo endereços de IPv4 ou IPv6 de origem e de destino, além de conter o *TTL* (Time to live - um contador que diminui a cada roteador que o pacote passa. Se chegar a 0, o pacote é descartado. Serve para evitar que pacotes fiquem rodando na rede infinitamente).
- Depois vêm as portas de origem e de destino, e informações relacionais que podem aparecer a depender do protocolo utilizado (TCP/UDP), como algumas flags e número de sequência.
- Depois de todos os headers, vem o payload, que é a requisição bruta, ou a mensagem enviada no pacote.

## Captação de pacotes
Neste exato momento milhares de pacotes estão sendo transmitidos para todos os nossos aparelhos pela internet, mas a placa de rede dos nossos notebooks/celulares apenas olha o conteúdo dos pacotes endereçados a ela (via MAC address). Então, a placa converte os sinais para bits, que são armazenados em um buffer. Então a placa de rede gera uma interrupção no processador avisando que informações novas chegaram pela rede, e cabe ao sistema operacional decidir o que fazer. Por fim, o driver da placa de rede copia o conteúdo dessa placa para o kernel, para que nós possamos inspecionar os pacotes com ferramentas, como *wireshark*.

## Interpretação camada por camada
- **Camada de Enlance (Cabeçalho ETHERNET)**: envelope externo, contém o MAC adress de destino, origem e o tipo (geralmente indicando se o conteúdo do pacote é um IPv4, por exemplo), nesta ordem.
- **Camada de Rede (Cabeçalho IP)**: Geralmente é o cabeçalho IPv4, que contém 20 bytes por padrão: Version, Header Length, Total Length (20b de IP + 20b de TCP + Xb do payload), TTL (Time to Live), protocol, source IP, destination IP, nessa ordem.
- **Camada de transporte (Cabeçalho TCP/UDP)**: Pode ser TCP/UDP dependendo do protocolo utilizado. O TCP normalmente possuí 20 bytes: Source Port, Destination Port, Sequence number, Ack number, flags, WindowSize (Quanto de dados a máquina aguenta recceber de volta)
- **Payload**: Carga útil, a request ou mensagem que de fato está sendo enviada pela rede.

#### Algumas considerações
1. TTL: Tempo de vida do pacote. A cada roteador que este pacote para na rede sem ser aberto, ele decrementa um do TTL, se chegar a 0, o pacote é descartado.
2. Protocol: Defini se o conteúdo da próxima camada é TCP/UDP ou outros.
3. Sequence Number: É possível que uma requisição tcp seja grande demais para caber em apenas um pacote. Nesse caso, vários pacotes são enviados em ordem, contendo partes da requisição. O Sequence Number indica qual a ordem do pacote na hora de "remontar" a request.
4. Ack Number: Indica o recebimento ou não do pacote anterior do receptor (provavelmente um SYN)
5. Flags: Podem conter diversas informações como PSH (PUSH), que diz ao receptor para não esperar o buffer encher antes de enviar os dados à aplicação, e ACK, indicando a confirmação de recebimento.
6. O conteúdo do payload é em hexadecimal.

## Dissecando um pacote na prática

```
BIT: 0               8               16              24              31
     +---------------+---------------+---------------+---------------+
     |        Endereço MAC de Destino (Primeiros 32 bits)            | 
     +---------------+---------------+---------------+---------------+ -- [0 a 47]
     | MAC Dest (cont)               |  MAC Origem (Início)          |    MAC DEST/ORIG
     +---------------+---------------+---------------+---------------+ -- [48 a 95]
     |              Endereço MAC de Origem (Final)                   |
     +---------------+---------------+---------------+---------------+ -- [96 a 111]
     | Type: 0x0800 (IPv4) [112-127] |Ver:4|IHL:5|  ToS  | Length: 69|
     +---------------+---------------+---------------+---------------+ -- [128 a 159] IP HEADER
     |      Identification: 0xXXXX   |Flags|   Fragment Offset: 0    |
     +---------------+---------------+---------------+---------------+ -- [160 a 191]
     |    TTL: 64    |  Protocol: 6  |    Header Checksum: 0xXXXX    |
     +---------------+---------------+---------------+---------------+ -- [192 a 223]
     |                    Source IP: 127.0.0.1                       |
     +---------------+---------------+---------------+---------------+ -- [224 a 255]
     |                 Destination IP: 127.0.0.1                     |
     +---------------+---------------+---------------+---------------+ -- [256 a 287] TCP HEADER
     |     Source Port: 54321        |    Destination Port: 8080     |
     +---------------+---------------+---------------+---------------+ -- [288 a 319]
     |                  Sequence Number: 0xXXXXXXXX                  |
     +---------------+---------------+---------------+---------------+ -- [320 a 351]
     |                Acknowledgment Number: 0xXXXXXXXX              |
     +---------------+---------------+---------------+---------------+ -- [352 a 383]
     |Offset| Rsrv |Flags|  Win Size |    Checksum   | Urgent Pointer|
     +---------------+---------------+---------------+---------------+ -- [384 a 415] PAYLOAD
     |   'T' (0x54)  |   'e' (0x65)  |   's' (0x73)  |   't' (0x74)  |
     +---------------+---------------+---------------+---------------+ -- [416 a 447]
     |   'a' (0x61)  |   'n' (0x6e)  |   'd' (0x64)  |   'o' (0x6f)  |
     +---------------+---------------+---------------+---------------+ -- [448 a 479]
     |   \n (0x0a)   |
     +---------------+
```

Suponha que rodemos o comando: `echo "Testando" | nc -w 1 127.0.0.1 8080` , vamos analisar o pacote que receberemos.

O pacote se assemelharia com isso:

```
Offset   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f   ASCII
-------------------------------------------------------------------------------
0000     00 00 00 00 00 00 00 00 00 00 00 00 08 00 45 00   ..............E.
0010     00 45 00 01 00 00 40 06 00 00 7f 00 00 01 7f 00   .E....@.........
0020     00 01 d4 31 1f 90 00 00 00 01 00 00 00 01 50 18   ...1..........P.
0030     ff ff d4 e2 00 00 54 65 73 74 61 6e 64 6f 0a      ......Testando.
```
-- Camada 1
1. Os primeiros 6 bytes: `00 00 00 00 00 00` representam o MAC de destino, como é um loopback, é tudo 0.
2. Os próximos 6 bytes: `00 00 00 00 00 00`representam o MAC de origem, como é um loopback, é tudo 0.
3. Os próximos 2 bytes: `08 00` representam o Type, e esse valor significa que o que vem a seguir é um pacote IPv4.
-- Camada 2
4. O próximo byte (`45`) é dividido ao meio:
	1. Metade (`4`) dele representa a verção do IP, nesse caso: IPv4.
	2. A outra (`5`) metade representa o Header Length, em que o 5 indica que o cabeçalho tem 5 palavras de 32 bits (20 bytes).
5. O próximo byte: `00` define propriedades, normalmente é 0.
6. Os próximos 2 bytes: `00 45` representam a Total Length, nesse caso, 69 bytes.
7. Os próximos 2 bytes: `00 01` representam um ID para ajudar na remontagem se o pacote for fragmentado.
8. Os próximos 2 bytes: `00 00` indicam se o pacote está fragmentado, neste caso, não está.
9. O próximo byte: `40` representa o TTL, nesse caso, o valor é 64.
10. O próximo byte: `06` representa o protocolo, nesse caso, TCP.
11. Os próximos 2 bytes: `00 00` representam o **header checksum**.
12. Os próximos 4 bytes: `7f 00 00 01` representam o IP de origem, nesse caso, 127.0.0.1 .
13. Os próximos 4 bytes: `7f 00 00 01` representam o IP de destino, nesse caso, 127.0.0.1 .
-- Camada 3
14. Os próximos 2 bytes: `d4 31` representam a porta de origem, nesse caso: 54321.
15. Os próximos 2 bytes: `1f 90` representam a porta de destino, nesse caso: 8080.
16. Os próximos 4 bytes: `00 00 00 01` representam o Sequence Number.
17. Os próximos 4 bytes: `00 00 00 01`representam o Ack number, 1 significa confirmação de recebimento.
18. O próximo byte: `50` representa um Data Offset, em que o 5 indica que o cabeçalho TCP tem 20 bytes.
19. O próximo byte: `18` representam as flags, em que o 1 no bit 4 significa ACK e no bit 3 significa PSH, ou seja, confirma o recebimento do pacote e pede para o receptor processar os dados imediatamente.
20. Os próximos 2 bytes: `ff ff` representam o Window Size, quanto espaço em buffer o emissor ter disponível.
21. Os próximos 2 bytes: `d4 e2` representam o **TCP checksum**
22. Os próximos 2 bytes: `00 00` representam o Urgent Pointer, que é usado somente se a flag URG estiver ativa, o que não é o caso.
-- Payload 
23. Por fim, todos os bytes restantes: `54 65 73 74 61 6e 64 6f 0a` representam o payload:
	1. `54` -> 'T'
	2. `65` -> 'e'
	3. `73` -> 's'
	4. `74` -> 't'
	5. `61` -> 'a'
	6. `6e` -> 'n'
	7. `64` -> 'd'
	8. `6f` -> 'o'
	9. `a0` -> '\n'

## Como forjar um pacote
Para forjar um pacote, é bem simples uma vez que você sabe como dissecar um, basta rearranjar as informações da sua máquina para o envio, e mandar para a placa de rede.

```C
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

// Estrutura para o Pseudo-cabeçalho (necessário para o cálculo da Checksum do TCP)
struct pseudo_header {
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
};

// Função padrão de Checksum (Complemento de Um)
unsigned short checksum(unsigned short *ptr, int nbytes) {
    long sum;
    unsigned short oddbyte;
    short answer;

    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char *)&oddbyte) = *(u_char *)ptr;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    answer = (short)~sum;
    return (answer);
}

int main() {
    // 1. Criar um Raw Socket
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if (s == -1) {
        perror("Erro ao abrir socket (use sudo)");
        return 1;
    }

    // Buffer para o pacote
    char datagram[4096], source_ip[32], *data, *pseudogram;
    memset(datagram, 0, 4096);

    // Ponteiros para os cabeçalhos
    struct iphdr *iph = (struct iphdr *) datagram;
    struct tcphdr *tcph = (struct tcphdr *) (datagram + sizeof(struct ip));
    struct sockaddr_in sin;
    struct pseudo_header psh;

    // Dados (Payload)
    data = datagram + sizeof(struct iphdr) + sizeof(struct tcphdr);
    strcpy(data, "Testando\n");

    // Configurações de endereço
    strcpy(source_ip, "127.0.0.1");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8080);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 2. Preencher Cabeçalho IP (Camada 3)
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data);
    iph->id = htons(1);
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0; // Deixe zero para calcular depois
    iph->saddr = inet_addr(source_ip);
    iph->daddr = sin.sin_addr.s_addr;
    iph->check = checksum((unsigned short *) datagram, iph->tot_len);

    // 3. Preencher Cabeçalho TCP (Camada 4)
    tcph->source = htons(54321); // Porta de origem
    tcph->dest = htons(8080);    // Porta de destino
    tcph->seq = htonl(1);
    tcph->ack_seq = htonl(1);    // Simulando o ACK do handshake
    tcph->doff = 5;               // Tamanho do cabeçalho TCP
    tcph->fin = 0;
    tcph->syn = 0;
    tcph->rst = 0;
    tcph->psh = 1;               // Flag PUSH ativa
    tcph->ack = 1;               // Flag ACK ativa
    tcph->urg = 0;
    tcph->window = htons(65535);
    tcph->check = 0;             // Checksum será calculada com pseudo-header
    tcph->urg_ptr = 0;

    // 4. Calcular Checksum do TCP (necessita do pseudo-header)
    psh.source_address = inet_addr(source_ip);
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(data));

    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(data);
    pseudogram = malloc(psize);
    memcpy(pseudogram, (char*) &psh, sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header), tcph, sizeof(struct tcphdr) + strlen(data));
    tcph->check = checksum((unsigned short*) pseudogram, psize);

    // 5. Informar ao kernel para NÃO preencher o cabeçalho IP (nós já fizemos isso)
    int one = 1;
    const int *val = &one;
    if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0) {
        perror("Erro ao definir IP_HDRINCL");
        return 1;
    }

    // 6. Enviar o pacote
    if (sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("Erro no envio");
    } else {
        printf("Pacote forjado enviado com sucesso!\n");
    }

    free(pseudogram);
    close(s);
    return 0;
}
```


Contudo, existe um detalhe que é crucial para a forja de um pacote: **checksums**.

### O que é uma checksum
É uma soma de verificação de um pacote de dados. É um jeito de informar que o pacote não perdeu integridade durante o envio e garantir que o receptor saiba que recebeu os dados corretos. O valor da checksum é calculado com base nos bits do pacote, ou seja, se um único bit mudar durante a viagem (seja por interferência no cabo ou falha de hardware), assim que o receptor receber o pacote e recalcular a checksum (pra verificar se nada se alterou), ele descarta o pacote imediatamente. Existem 2 checksums em um pacote IPv4:

- **Header checksum**: serve para verificar a integridade apenas do **cabeçalho IP**, cobrindo os 20 bytes do cabeçalho IP e ignorando completamente o que vem depois. Uma particularidade, é que em cada salto do pacote em um roteador, o valor do TTL é diminuído, obrigando o roteador a recalcular o header checksum do pacote antes de propagá-lo. Uma curiosidade, é que no IPv6, essa checksum foi removida, pois os engenheiros perceberam que as camadas 2 e 4 já faziam um trabalho bom o suficiente para garantir a integridade dos pacotes, e essa remoção acelerou o processamento dos roteadores.
- **TCP checksum**: serve para verificar a integridade do **cabeçalho TCP, dos dados e do pseudo-header**. Para calcular sua checksum, ele olha para o IP de origem e o de destino (que tecnicamente estão na camada de baixo (a do cabeçalho IP), por isso um pseudo-header) apenas. Isso garante que se o pacote for entregue ao IP errado devido a um erro de roteamento, a checksum do TCP vai falhar e o pacote será descartado. Uma particularidade é que o roteador não acessa essa camada do pacote, então ela serve como uma verificação ponta-a-ponta, apenas o emissor e o receptor a calculam.

O cálculo matemático das checksums envolve a aritmética de complemento de 1 de 16 bits. Basicamente o protocolo divide o pacote em vlocos de 16bits e os soma.

## Pacotes especiais (SYN, SYN-ACK, ACK)
Diferentemente do UDP que apenas joga os dados na rede sem se importar se o receptor irá recebê-los, o TCP zela pelo bom recebimento dos dados enviados. Para isso, o protocolo TCP exige um 3-way Handshake. Funciona da seguinte forma:

1. **SYN**
	1. É o primeiro pacote, enviado pelo emissor (client), com a flag *SYN* ativa. Ele significa basicamente um "Oi, quer conversar?".
	2. O conteúdo dele contém informações para sincronização de números de sequência, porta de origem e destino.
2. **SYN-ACK**
	1. É o segundo pacote, enviado pelo receptor (server), com as flags *SYN* e *ACK* ativas. Significa basicamente um "Oi! Eu quero, reccebi sua mensagem.".
	2. A flag *ACK* confirma o recebimento.
	3. A flag *SYN* informa o número de sequência do servidor, para sincronização.
3. **ACK**
	1. É o último pacote do handshake, enviado pelo emissor (client), apenas com a flag *ACK* ativa. Significa um "Show de bola! Vamos começar!".
	2. Contém a confirmação de recebimento, encerrando o handshake e firmando a conexão.
	3. Somente depois deste pacote que os dados desejados são enviados e recebidos.


---
# Live Coding \[Slides done]
Faremos um servidor http simples em rust. Mostrando como instanciar um socket, ouvir uma stream, receber e responder uma request. Vamos ensinar uma rota só. Buffer estático. Não vamos fazer parsing nem routing.

#### Primeiros passos
Primeiro, criamos nosso projeto com `cargo new server` e inicializamos a função main.

```rust

fn main() {

}

```

Depois explico como criar um socket usando rust e atrelar este socket a um IP, explicando que em Rust, os sockets ouvem streams, que é basicamente a corrente de dados, em looping.

```rust

use std::net::{TcpStream, TcpListener}

fn main() {
	
	let listener = TcpListener::bind("0.0.0.0:8080");
	
	for stream in listener.incoming() {
		let stream = stream.unwrap();
	}
	
}

```

Nessa altura do campeonato, o nosso servidor já roda localmente e já consegue escutar pacotes, mas nós não estamos fazendo nada com eles.
Vamos então criar uma função que recebe uma conexão e lê seu conteúdo. Para isso, declaramos um buffer de tamanho fixo (existe como fazer de tamanhos variados, mas não iremos abordar isso) para ler os bytes da stream.

```rust
fn handle_connection(mut stream: TcpStream) {
	let mut buffer = [0; 8192];
	(&mut stream).read(&mut buffer).unwrap();
	
	let request = String::from_utf8_lossy(&buffer[..]);
	
	println!("Request:\n{}", request);
}
```

Agora chamamos essa função dentro do nosso for loop e passamos a stream como parâmetro:

```rust
fn main() {
	
	let listener = TcpListener::bind("0.0.0.0:8080");
	
	for stream in listener.incoming() {
		let stream = stream.unwrap();
		handle_connection(stream);
	}
	
}
```

Nesse ponto, toda request que recebemos é lida e printada na tela. Isso significa que já poderíamos manipular a request se nós desejássemos. Contudo, não faremos nenhuma manipulação complexa. Apenas uma rota para exemplificação.

Para isso, vamos alterar a função `handle_connection()` para que ela responda um código 200 para todas as requests e responda um código 200 com corpo para a rota `/teste`

```rust
fn handle_connection(mut stream: TcpStream) {
	let mut buffer = [0; 8192];
	(&mut stream).read(&mut buffer).unwrap();
	
	let request = String::from_utf8_lossy(&buffer[..]);
	
	println!("Request:\n{}", request);
	
	let response;
	
	if (request.contains("/teste")) {
		
		let body = format!("Testado.");
		
		response = format!(
			"HTTP/1.1 200 OK\r\n\
			Content-type: text/html\r\n\
			Content-length: {}\r\n\
			\r\n\
			{}
			",
			body.len(),
			body
		);
	} else {
		response = format!(
			"HTTP/1.1 200 OK\r\n\r\n"
		);
	}
	
	(&mut stream).write(response.as_bytes()).unwrap;
	(&mut stream).flush();
}
```

Com isso, temos uma rota feita em nosso servidor. Se enviarmos uma request (pelo navegador) /teste, receberemos nossa mensagem de testado corretamente!

Código completo: 
```rust
use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};

fn handle_connection(mut stream: TcpStream) {
    let mut buffer = [0; 8192];

    stream.read(&mut buffer).unwrap();

    let request = String::from_utf8_lossy(&buffer[..]);

    println!("Request: {}", request);
    let mut response;
    if request.contains("/teste") {

        let resp_text = format!("Hello, world!");

        response = format!(
            "HTTP/1.1 200 OK\r\n\
            Content-type: text/html\r\n\
            Content-length: {}\r\n\
            \r\n\
            {}",
            resp_text.len(),
            resp_text
        );
    } else {
        response = format!("HTTP/1.1 200 OK\r\n\r\n");
    }

    stream.write(response.as_bytes()).unwrap();
    stream.flush().unwrap();

}

fn main() {
    let listener = TcpListener::bind("0.0.0.0:8080").unwrap();

    for stream in listener.incoming() {
        let stream = stream.unwrap();
        println!("Conex�o recebida.");
        handle_connection(stream);
    }
}
```

