# Protocolo IRC — Guia Completo para Desenvolvimento de um Servidor

> Protocolo que funciona na camada de **Aplicação**, usado em sua maioria para **chats de texto em tempo real**.
> Modelo cliente-servidor, comunicação em texto puro sobre **TCP/IP** (porta padrão **6667**; TLS na **6697**).

---

## Sumário

1. [Visão Geral e Histórico](#1-visão-geral-e-histórico)
2. [Referências (RFCs e Padrões)](#2-referências-rfcs-e-padrões)
3. [Modelo de Arquitetura](#3-modelo-de-arquitetura)
4. [Formato das Mensagens (Gramática)](#4-formato-das-mensagens-gramática)
5. [Convenções e Conceitos-Chave](#5-convenções-e-conceitos-chave)
6. [Mensagens (Comandos) do Cliente](#6-mensagens-comandos-do-cliente)
7. [Mensagens do Servidor](#7-mensagens-do-servidor)
8. [Numerics (Respostas Numéricas)](#8-numerics-respostas-numéricas)
9. [Respostas de Erro — Tabela Completa](#9-respostas-de-erro--tabela-completa)
10. [Respostas de Sucesso e Comando — Tabela Completa](#10-respostas-de-sucesso-e-comando--tabela-completa)
11. [Respostas de RESERVED / Outros — Tabela Completa](#11-respostas-de-reserved--outros--tabela-completa)
12. [Respostas Especiais de Comando](#12-respostas-especiais-de-comando)
13. [Exemplos de Sessões Reais (Handshake → Uso → Saída)](#13-exemplos-de-sessões-reais-handshake--uso--saída)
14. [Implementação Profissional — Checklist e Boas Práticas](#14-implementação-profissional--checklist-e-boas-práticas)
15. [Extensões (IRCv3 / Modernas)](#15-extensões-ircv3--modernas)
16. [Referências para Estudo Aprofundado](#16-referências-para-estudo-aprofundado)

---

## 1. Visão Geral e Histórico

- Criado por **Jarkko Oikarinen** em **1988** (Finlândia), originalmente para substituir o programa *talk*.
- Em 1990 foi formado o **IRC Working Group** da IETF.
- É um protocolo de **texto plano**, linha-orientado, **case-insensitive** para comandos e nomes de canais.
- A comunicação é feita via **linhas de texto terminadas em `CRLF`** (`\r\n`).
- Os servidores formam uma **rede (IRC network)** conectada por links de servidor a servidor.
- Rápido, leve e simples de inspecionar com `nc`/`telnet`/`openssl s_client`.

---

## 2. Referências (RFCs e Padrões)

| Documento | Título | Observação |
|---|---|---|
| **RFC 1459** | Internet Relay Chat Protocol | Base original (1993). Ainda é a referência canônica. |
| **RFC 2810** | IRC: Architecture | Arquitetura da rede (clientes, servidores, canais). |
| **RFC 2811** | IRC: Channel Management | Nomes, modos, tipos de canal (`#`, `&`, `+`, `!`). |
| **RFC 2812** | IRC: Client Protocol | Comandos e numerics (substitui partes da 1459). |
| **RFC 2813** | IRC: Server Protocol | Comandos servidor↔servidor (`SERVER`, `SQUIT`, etc.). |
| **IRCv3** | ircv3.github.io | Extensões modernas (CAP, SASL, message-tags…). |
| **Modern IRC** | modern.ircdocs.horse | Especificação consolidada e corrigida, recomendada hoje. |

> **Dica profissional:** implemente com base no *Modern IRC* (que corrige ambiguidades da RFC 1459) e na **RFC 2812** para os numerics.

---

## 3. Modelo de Arquitetura

```
                 ┌──────────┐
                 │ Servidor │  ← nosso alvo de implementação
                 │   (hub)  │
                 └────┬─────┘
        ┌─────────────┼──────────────┐
        │             │              │
   ┌────┴─────┐  ┌────┴─────┐   ┌────┴─────┐
   │ Cliente A│  │ Cliente B│   │ Cliente C│
   └──────────┘  └──────────┘   └──────────┘
```

- **Cliente** = processo que se conecta via TCP e envia comandos.
- **Servidor** = autoridade central que gerencia estado, canais e encaminha mensagens.
- **Rede** = conjunto de servidores linkados; cada servidor conhece o estado global.
- Cada cliente recebe um **apelido (nick)** único e, opcionalmente, um **nome real** e um **usuário**.
- **Canais** = salas de conversa identificadas por `#` (público global), `&` (local ao servidor), `+` (sem moderação), `!` (com modelo de segurança).

**Registro do cliente no servidor (Registration):**
1. Conexão TCP aceita.
2. Cliente envia `NICK` e `USER` (o servidor **não** deve aceitar outros comandos até o registro completo).
3. Servidor responde com a sequência de boas-vindas `001`–`005`.
4. Cliente é considerado "registrado" e pode participar.

---

## 4. Formato das Mensagens (Gramática)

### 4.1. Sintaxe ABNF (RFC 2812)

```
message   = [ ":" prefix SPACE ] command [ params ] crlf
prefix    = servername / ( nickname [ [ "!" user ] "@" host ] )
command   = 1*letter / 3digit
params    = SPACE *14( ":" middle / params ) / SPACE ":" trailing
nospcrlfcl= %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF   ; sem espaço, ':' ou CR/LF
middle    = nospcrlfcl *( ":" / nospcrlfcl )
trailing  = *( ":" / " " / nospcrlfcl )
SPACE     = %x20
crlf      = %x0D %x0A
```

### 4.2. Regras de ouro

1. **Toda linha termina em `CRLF`** (`\r\n`). Linhas são o delimitador de mensagem.
2. **Comandos** são case-insensitive (`nick` = `NICK` = `Nick`). Parâmetros podem ser sensíveis a maiúsculas conforme o contexto.
3. **Prefixo `:`** no início indica a origem da mensagem (nick do emissor ou servidor). Só o servidor adiciona prefixos em encaminhamentos.
4. O primeiro parâmetro que começa com `:` é o **último** parâmetro (parâmetro *trailing*) e pode conter espaços — é a "mensagem" em si.
5. **Máximo de 512 bytes** por linha (incluindo CRLF), incluindo o prefixo. Mensagens maiores devem ser truncadas ou descartadas.
6. **Limite de 14 parâmetros** após o comando (sem contar o trailing).
7. Caracteres proibidos dentro de parâmetros: NUL (`0x00`), CR (`0x0D`), LF (`0x0A`), espaço (exceto no trailing) e, se for o primeiro char de um param, `:`.

### 4.3. Exemplos de linhas

| Linha enviada                                                            | Significado                                                                  |
| ------------------------------------------------------------------------ | ---------------------------------------------------------------------------- |
| `NICK rms\r\n`                                                           | Cliente define apelido `rms`.                                                |
| `USER guest 0 * :Real Name Here\r\n`                                     | Registro: user `guest`, sem senha, hostmask `*`, nome real `Real Name Here`. |
| `JOIN #redes\r\n`                                                        | Entrar no canal `#redes`.                                                    |
| `PRIVMSG #redes :Olá, pessoal!\r\n`                                      | Enviar a mensagem `Olá, pessoal!` para o canal.                              |
| `:rms!guest@host PRIVMSG #redes :Olá, pessoal!\r\n`                      | Encaminhada pelo servidor com o prefixo do remetente.                        |
| `:irc.server.net 001 rms :Welcome to the Internet Relay Network rms\r\n` | Numeric `001` com prefixo do servidor.                                       |

---

## 5. Convenções e Conceitos-Chave

### 5.1. Case-insensitivity
- **Nicks** e **canais** devem ser comparados ignorando maiúsculas/minúsculas (ASCII `a-z`/`A-Z`). RFC sugere *ascii case mapping*; o RFC 1459 é ambíguo — o padrão *Modern IRC* recomenda `rfc1459` (com `{}|^` mapeados para `[]\~`) ou `ascii`.

### 5.2. Nicks
- Máximo de **9 caracteres** (RFC 1459). Inicia com letra ou caractere especial `[]\`_^{|}`; depois pode conter dígitos e `-`.
- Regex de referência: `^[A-Za-z\[\]\\`_^{|}][A-Za-z0-9\[\]\\`_^{|}-]{0,8}$`
- Caso o cliente não envie `NICK`/`USER` em **tempo hábil** (timeout típico: 30–120 s), o servidor deve desconectar (frequentemente com `ERROR` / `PING`).

### 5.3. Canais
- Prefixos: `#` (global), `&` (local), `+` (sem modos), `!` (com *chanid*).
- Nome máximo: **50 caracteres** (excluindo o prefixo).
- Canal `#` começa com `#` e pode conter letras, dígitos e `-`/`_` entre outros; não pode conter espaço, `,`, `:` ou `Ctrl+G`.

### 5.4. Prefixos de usuário em canal
| Símbolo | Significado | Modo |
|---|---|---|
| `@` | Operador de canal (op) | `+o` |
| `%` | Half-op (semi-operador) | `+h` |
| `+` | Voz (voice) | `+v` |

### 5.5. Modos (RFC 2811)
- **Modos de usuário:** `i` (invisível), `s` (server notices), `w` (wallops), `o` (operador IRC).
- **Modos de canal:** `O` (somente oper), `o` (operador de canal), `v` (voz), `i` (somente convite), `m` (moderado), `n` (sem mensagens externas), `p` (privado), `s` (secreto), `t` (somente ops mudam tópico), `k` (chave/senha), `l` (limite de usuários), `b` (ban mask), `e` (exceção), `I` (máscara de convite).

---

## 6. Mensagens (Comandos) do Cliente

Sintaxe genérica: `COMANDO [param1] [param2] ... [:trailing]`

### 6.1. Registro e Conexão

| Comando | Sintaxe | Descrição |
|---|---|---|
| **PASS** | `PASS <senha>` | Senha de conexão (opcional, antes de NICK/USER). |
| **NICK** | `NICK <nick>` | Define ou troca apelido. |
| **USER** | `USER <user> <mode> <unused> :<realname>` | Registra nome de usuário/realname. |
| **OPER** | `OPER <nome> <senha>` | Solicita privilégios de operador de IRC. |
| **QUIT** | `QUIT [:motivo]` | Encerra conexão. |

### 6.2. Operações com Canais

| Comando | Sintaxe | Descrição |
|---|---|---|
| **JOIN** | `JOIN <canais> [<chaves>]` | Entra em um ou mais canais (`chave` para `+k`). |
| **PART** | `PART <canais> [:motivo]` | Sai de canais. |
| **MODE** | `MODE <canal/nick> [<modos> [<args>]]` | Consulta ou altera modos. |
| **TOPIC** | `TOPIC <canal> [:novo tópico]` | Consulta ou altera o tópico. |
| **NAMES** | `NAMES [<canais>]` | Lista nicks por canal. |
| **LIST** | `LIST [<canais>]` | Lista canais com tópico e nº de usuários. |
| **INVITE** | `INVITE <nick> <canal>` | Convida usuário para canal `+i`. |
| **KICK** | `KICK <canal> <usuario> [:motivo]` | Expulsa usuário do canal. |

### 6.3. Envio de Mensagens

| Comando | Sintaxe | Descrição |
|---|---|---|
| **PRIVMSG** | `PRIVMSG <alvo> <texto>` | Mensagem privada (para nick ou canal). |
| **NOTICE** | `NOTICE <alvo> <texto>` | Como PRIVMSG, mas **nunca** gera resposta automática. |
| **TAGMSG** (IRCv3) | `@tags TAGMSG <alvo>` | Envia apenas tags/mensagens. |

### 6.4. Consultas (Queries)

| Comando | Sintaxe | Descrição |
|---|---|---|
| **WHOIS** | `WHOIS [<servidor>] <nick>` | Informações detalhadas de usuário. |
| **WHOWAS** | `WHOWAS <nick> [<limite>]` | Informações de nick já desconectado. |
| **WHO** | `WHO [<nome>]` | Lista usuários/máscaras. |
| **USERHOST** | `USERHOST <nick>...` | Retorna nicks + hosts (até 5). |
| **ISON** | `ISON <nick>...` | Checa se nicks estão online. |
| **TIME** | `TIME` | Data/hora do servidor. |
| **VERSION** | `VERSION` | Versão/software do servidor. |
| **ADMIN** | `ADMIN [<servidor>]` | Info administrativa. |
| **INFO** | `INFO` | Info textual do servidor. |
| **MOTD** | `MOTD` | Mensagem do dia. |
| **LINKS** | `LINKS [<mask>]` | Lista servidores da rede. |
| **STATS** | `STATS [<query>]` | Estatísticas. |
| **LUSERS** | `LUSERS` | Nº de usuários/servidores conectados. |
| **TRACE** | `TRACE [<target>]` | Caminho até o servidor. |
| **PING** | `PING <token>` | Checa latência/keep-alive. |
| **PONG** | `PONG <token>` | Resposta ao PING. |
| **HELP** (não-RFC) | `HELP [<assunto>]` | Ajuda (comum em daemons como o UnrealIRCd). |

### 6.5. Operador / Comandos "paranormais" (usuais em servidores modernos)

| Comando | Sintaxe | Descrição |
|---|---|---|
| **KILL** | `KILL <nick> :<motivo>` | Desconecta um usuário (somente oper). |
| **SQUIT** | `SQUIT <servidor> :<motivo>` | Força saída de servidor (somente oper). |
| **CONNECT** | `CONNECT <alvo> [<porta>]` | Força link com outro servidor. |
| **WALLOPS** | `WALLOPS :<msg>` | Mensagem global para ops. |
| **AWAY** | `AWAY [:motivo]` | Marca ausência; sem parâmetro, remove. |
| **REHASH** | `REHASH` | Recarrega config. |

> **Comando obrigatório em qualquer servidor que se preze:** `PING`/`PONG` (detecção de conexões mortas).

---

## 7. Mensagens do Servidor

### 7.1. Numeric (respostas numéricas)
O servidor responde na forma:
```
:<servidor> <3 dígitos> <nick do destino> :<texto>|<parâmetros específicos>
```
Ex.: `:irc.exemplo.net 001 rms :Welcome to the Internet Relay Network rms`

### 7.2. Comandos emitidos pelo servidor
| Comando | Sintaxe | Uso |
|---|---|---|
| **PING** | `PING :<token>` | Servidor testa se cliente está vivo. |
| **ERROR** | `ERROR :<motivo>` | Erro fatal; geralmente precede desconexão. |
| **NOTICE** | `NOTICE <nick> :<texto>` | Avisos (ex.: senha incorreta). |
| **WALLOPS** | `:servidor WALLOPS :<msg>` | Difundido a usuários `+w`. |
| **MODE** | (encaminhado) | Mudanças de modo são difundidas. |

---

## 8. Numerics (Respostas Numéricas)

As respostas numéricas têm **3 dígitos** e são organizadas por faixa:
- **000–099**: conexão/resposta de erro na fase de registro.
- **200–399**: respostas de comandos (ex.: LIST, WHOIS).
- **400–599**: erros originados pelo cliente (os mais importantes para seu servidor).
- **600–999**: reservados / não padronizados (respostas extras e extensões).

> As tabelas abaixo usam `<nick>` para o alvo e seguem a RFC 2812 + RFC 1459. Ao implementar, lembre-se: o **primeiro parâmetro depois do numeric** é sempre o nick do destinatário da resposta.

---

## 9. Respostas de Erro — Tabela Completa

### 400–499 (erros genéricos de cliente/comando)

| Código | Nome | Significado / Formato |
|---|---|---|
| 401 | `ERR_NOSUCHNICK` | `:No such nick/channel` — alvo não existe. |
| 402 | `ERR_NOSUCHSERVER` | `:No such server` — servidor não existe. |
| 403 | `ERR_NOSUCHCHANNEL` | `:No such channel` — canal não existe. |
| 404 | `ERR_CANNOTSENDTOCHAN` | `:Cannot send to channel` — sem permissão de fala (ex.: `+n`, `+m`). |
| 405 | `ERR_TOOMANYCHANNELS` | `:You have joined too many channels` — limite excedido. |
| 406 | `ERR_WASNOSUCHNICK` | `:There was no such nickname` — WHOWAS sem histórico. |
| 407 | `ERR_TOOMANYTARGETS` | `:Duplicate recipients. No message delivered` — muitos alvos. |
| 408 | `ERR_NOSUCHSERVICE` | `:No such service` (RFC 2812). |
| 409 | `ERR_NOORIGIN` | `:No origin specified` — faltou parâmetro obrigatório (ex.: PING). |
| 410 | `ERR_NORECIPIENT` | `:No recipient given (PRIVMSG)` — faltou alvo. |
| 411 | `ERR_NOTEXTTOSEND` | `:No text to send` — faltou a mensagem. |
| 412 | `ERR_NOTOPLEVEL` | `:No toplevel domain specified` (máscara inválida). |
| 413 | `ERR_WILDTOPLEVEL` | `:Wildcard in toplevel domain` (ex.: `*.com` inválido). |
| 414 | `ERR_BADMASK` | `:Bad Server/host mask` — máscara malformada. |
| 415–419 | — | (indefinido/legado) |
| 421 | `ERR_UNKNOWNCOMMAND` | `:Unknown command` — comando não reconhecido. |
| 422 | `ERR_NOMOTD` | `:MOTD File is missing` — servidor sem MOTD. |
| 423 | `ERR_NOADMININFO` | `:No administrative info available` — ADMIN falhou. |
| 424 | `ERR_FILEERROR` | `:File error doing <op> on <file>` — erro interno de arquivo. |
| 431 | `ERR_NONICKNAMEGIVEN` | `:No nickname given` — NICK sem parâmetro. |
| 432 | `ERR_ERRONEUSNICKNAME` | `:Erroneous nickname` — nick com caracteres inválidos. |
| 433 | `ERR_NICKNAMEINUSE` | `:Nickname is already in use` — nick já em uso. |
| 436 | `ERR_NICKCOLLISION` | `:Nickname collision KILL` — colisão entre servidores. |
| 437 | `ERR_UNAVAILRESOURCE` | `:Nick/channel is temporarily unavailable` (RFC 2812). |
| 441 | `ERR_USERNOTINCHANNEL` | `:They aren't on that channel` — alvo não está no canal. |
| 442 | `ERR_NOTONCHANNEL` | `:You're not on that channel` — usuário não está no canal. |
| 443 | `ERR_USERONCHANNEL` | `:is already on channel` — INVITE duplicado. |
| 444 | `ERR_NOLOGIN` | `:User not logged in` (legado). |
| 445 | `ERR_SUMMONDISABLED` | `:SUMMON has been disabled`. |
| 446 | `ERR_USERSDISABLED` | `:USERS has been disabled`. |
| 451 | `ERR_NOTREGISTERED` | `:You have not registered` — comandos enviados antes de NICK/USER. |
| 461 | `ERR_NEEDMOREPARAMS` | `:<comando> :Not enough parameters` — faltaram argumentos. |
| 462 | `ERR_ALREADYREGISTRED` | `:Unauthorized command (already registered)` — segundo USER/PASS. |
| 463 | `ERR_NOPERMFORHOST` | `:Your host isn't among the privileged`. |
| 464 | `ERR_PASSWDMISMATCH` | `:Password incorrect`. |
| 465 | `ERR_YOUREBANNEDCREEP` | `:You are banned from this server`. |
| 466 | `ERR_YOUWILLBEBANNED` | (legado) |
| 467 | `ERR_KEYSET` | `:Channel key already set` — +k com chave já existente. |
| 471 | `ERR_CHANNELISFULL` | `:Cannot join channel (+l)` — canal cheio. |
| 472 | `ERR_UNKNOWNMODE` | `:<char> :is unknown mode char to me`. |
| 473 | `ERR_INVITEONLYCHAN` | `:Cannot join channel (+i)` — canal somente convite. |
| 474 | `ERR_BANNEDFROMCHAN` | `:Cannot join channel (+b)` — banido. |
| 475 | `ERR_BADCHANNELKEY` | `:Cannot join channel (+k)` — chave incorreta. |
| 476 | `ERR_BADCHANMASK` | `:Bad Channel Mask` (RFC 2812). |
| 477 | `ERR_NOCHANMODES` | `:Channel doesn't support modes` (canal `+`). |
| 478 | `ERR_BANLISTFULL` | `:<canal> <char> :Channel list is full`. |
| 481 | `ERR_NOPRIVILEGES` | `:Permission Denied- You're not an IRC operator`. |
| 482 | `ERR_CHANOPRIVSNEEDED` | `:You're not channel operator` — requer `+o`. |
| 483 | `ERR_CANTKILLSERVER` | `:You can't kill a server!`. |
| 484 | `ERR_RESTRICTED` | `:Your connection is restricted!` (RFC 2812). |
| 485 | `ERR_UNIQOPPRIVSNEEDED` | `:You're not the original channel operator` (RFC 2812). |
| 491 | `ERR_NOOPERHOST` | `:No O-lines for your host` — OPER negado. |
| 492 | `ERR_NOSERVICEHOST` | (RFC 2812). |
| 493–500 | — | (indefinido/legado) |

### 501–502 e respostas de erro residuais

| Código | Nome | Significado |
|---|---|---|
| 501 | `ERR_UMODEUNKNOWNFLAG` | `:Unknown MODE flag` — modo de usuário desconhecido. |
| 502 | `ERR_USERSDONTMATCH` | `:Cannot change mode for other users` — MODE de nick alheio. |

---

## 10. Respostas de Sucesso e Comando — Tabela Completa

| Código | Nome | Significado / Formato |
|---|---|---|
| 001 | `RPL_WELCOME` | `:Welcome to the Internet Relay Network <nick>!<user>@<host>` |
| 002 | `RPL_YOURHOST` | `:Your host is <servidor>, running version <ver>` |
| 003 | `RPL_CREATED` | `:This server was created <data>` |
| 004 | `RPL_MYINFO` | `<servidor> <ver> <modos_usr> <modos_chan> <modos_chan_params>` |
| 005 | `RPL_ISUPPORT` | `:NICKLEN=30 CHANTYPES=# ... are supported by this server` |
| 200 | `RPL_TRACELINK` | Dados de TRACE. |
| 201 | `RPL_TRACECONNECTING` | TRACE. |
| 202 | `RPL_TRACEHANDSHAKE` | TRACE. |
| 203 | `RPL_TRACEUNKNOWN` | TRACE. |
| 204 | `RPL_TRACEOPERATOR` | TRACE. |
| 205 | `RPL_TRACEUSER` | TRACE. |
| 206 | `RPL_TRACESERVER` | TRACE. |
| 207 | `RPL_TRACESERVICE` | TRACE (RFC 2812). |
| 208 | `RPL_TRACENEWTYPE` | TRACE. |
| 209 | `RPL_TRACECLASS` | TRACE (RFC 2812). |
| 210 | `RPL_TRACERECONNECT` | TRACE. |
| 211 | `RPL_STATSLINKINFO` | `<linkname> <sendq> <sent_msgs> <sent_bytes> <rcvd_msgs> <rcvd_bytes> <time_open>` |
| 212 | `RPL_STATSCOMMANDS` | `<comando> <count> <byte_count> <remote_count>` |
| 213 | `RPL_STATSCLINE` | `C <host> * <nome> <porta> <classe>` |
| 214 | `RPL_STATSNLINE` | `N <host> * <nome> <porta> <classe>` |
| 215 | `RPL_STATSILINE` | `I <host> * <host> <porta> <classe>` |
| 216 | `RPL_STATSKLINE` | `K <host> * <usuario> <porta> <classe>` |
| 217 | `RPL_STATSQLINE` | `Q <mask> :reason` (legado). |
| 218 | `RPL_STATSYLINE` | `Y <classe> <ping> <freq> <maxconn> <sendq>` |
| 219 | `RPL_ENDOFSTATS` | `<stats> :End of /STATS report` |
| 221 | `RPL_UMODEIS` | `<modos>` — modos de usuário atuais. |
| 231 | `RPL_SERVICEINFO` | (RFC 2812). |
| 232 | `RPL_ENDOFSERVICES` | (RFC 2812). |
| 233 | `RPL_SERVICE` | (RFC 2812). |
| 234 | `RPL_SERVLIST` | (RFC 2812). |
| 235 | `RPL_SERVLISTEND` | (RFC 2812). |
| 241 | `RPL_STATSLLINE` | `L <hostmask> * <server> <maxdepth>` |
| 242 | `RPL_STATSUPTIME` | `:Server Up %d days %d:%02d:%02d` |
| 243 | `RPL_STATSOLINE` | `O <hostmask> * <nome>` |
| 244 | `RPL_STATSHLINE` | `H <hostmask> * <server>` |
| 245 | `RPL_STATSSLINE` | (RFC 2812). |
| 246 | `RPL_STATSPING` | (RFC 2812). |
| 247 | `RPL_STATSBLINE` | (RFC 2812). |
| 250 | `RPL_STATSDLINE` | `:Highest connection count: N (M clients) (K connections received)` |
| 251 | `RPL_LUSERCLIENT` | `:There are <u> users and <i> invisible on <s> servers` |
| 252 | `RPL_LUSEROP` | `<n> :operator(s) online` |
| 253 | `RPL_LUSERUNKNOWN` | `<n> :unknown connection(s)` |
| 254 | `RPL_LUSERCHANNELS` | `<n> :channels formed` |
| 255 | `RPL_LUSERME` | `:I have <c> clients and <s> servers` |
| 256 | `RPL_ADMINME` | `:<server> :Administrative info` |
| 257 | `RPL_ADMINLOC1` | `:<info>` |
| 258 | `RPL_ADMINLOC2` | `:<info>` |
| 259 | `RPL_ADMINEMAIL` | `:<info>` |
| 261 | `RPL_TRACELOG` | `File <arquivo> <n>` (legado). |
| 262 | `RPL_TRACEEND` | (RFC 2812). |
| 263 | `RPL_TRYAGAIN` | `:<comando> :Please wait a while and try again` (rate limit). |
| 300 | `RPL_NONE` | (comando sem resposta definida). |
| 301 | `RPL_AWAY` | `<nick> :<mensagem de ausência>` |
| 302 | `RPL_USERHOST` | `:<replies>` — ex.: `rms=+rms@host` (`+` online / `-` ausente). |
| 303 | `RPL_ISON` | `:<nicked online>` |
| 305 | `RPL_UNAWAY` | `:You are no longer marked as being away` |
| 306 | `RPL_NOWAWAY` | `:You have been marked as being away` |
| 311 | `RPL_WHOISUSER` | `<nick> <user> <host> * :<realname>` |
| 312 | `RPL_WHOISSERVER` | `<nick> <server> :<server info>` |
| 313 | `RPL_WHOISOPERATOR` | `<nick> :is an IRC operator` |
| 314 | `RPL_WHOWASUSER` | `<nick> <user> <host> * :<realname>` |
| 315 | `RPL_ENDOFWHO` | `<mask> :End of /WHO list` |
| 316 | `RPL_WHOISCHANOP` | (legado). |
| 317 | `RPL_WHOISIDLE` | `<nick> <idle> <signon> :seconds idle, signon time` |
| 318 | `RPL_ENDOFWHOIS` | `<nick> :End of /WHOIS list` |
| 319 | `RPL_WHOISCHANNELS` | `<nick> :<@#canal +%canal +canal>` |
| 321 | `RPL_LISTSTART` | `Channel :Users Name` (legado/removido em servidores modernos). |
| 322 | `RPL_LIST` | `<canal> <nº usuários> :<tópico>` |
| 323 | `RPL_LISTEND` | `:End of /LIST` |
| 324 | `RPL_CHANNELMODEIS` | `<canal> <modos> [<args>]` |
| 325 | `RPL_UNIQOPIS` | (RFC 2812). |
| 331 | `RPL_NOTOPIC` | `<canal> :No topic is set` |
| 332 | `RPL_TOPIC` | `<canal> :<tópico>` |
| 333 | `RPL_TOPICWHOTIME` | `<canal> <nick> <timestamp>` (não-RFC, universal). |
| 341 | `RPL_INVITING` | `<canal> <nick>` |
| 342 | `RPL_SUMMONING` | `<user> :Summoning user to IRC` (legado). |
| 346 | `RPL_INVITELIST` | `<canal> <mask>` |
| 347 | `RPL_ENDOFINVITELIST` | `<canal> :End of channel invite list` |
| 348 | `RPL_EXCEPTLIST` | `<canal> <mask>` |
| 349 | `RPL_ENDOFEXCEPTLIST` | `<canal> :End of channel exception list` |
| 351 | `RPL_VERSION` | `<version>.<debuglevel> <server> :<comentários>` |
| 352 | `RPL_WHOREPLY` | `<canal> <user> <host> <server> <nick> <H/G>[*][@|+] :<hopcount> <realname>` |
| 353 | `RPL_NAMREPLY` | `= <canal> :<@nick +nick nick>` (símbolos: `=` público, `*` privado, `@` secreto) |
| 354 | `RPL_WHOSPCRPL` | (RFC 2812, WHOX). |
| 361 | `RPL_KILLDONE` | `<nick> :Killed` (legado). |
| 362 | `RPL_CLOSING` | `<cliente> :Closed` (legado). |
| 363 | `RPL_CLOSEEND` | (legado). |
| 364 | `RPL_LINKS` | `<mask> <servidor> :<hopcount> <server info>` |
| 365 | `RPL_ENDOFLINKS` | `<mask> :End of /LINKS list` |
| 366 | `RPL_ENDOFNAMES` | `<canal> :End of /NAMES list` |
| 367 | `RPL_BANLIST` | `<canal> <mask>` (pode ter quem/quando como extra) |
| 368 | `RPL_ENDOFBANLIST` | `<canal> :End of channel ban list` |
| 369 | `RPL_ENDOFWHOWAS` | `<nick> :End of WHOWAS` |
| 371 | `RPL_INFO` | `:<texto>` |
| 372 | `RPL_MOTD` | `:- <texto>` |
| 373 | `RPL_INFOSTART` | `:Server INFO` |
| 374 | `RPL_ENDOFINFO` | `:End of /INFO list` |
| 375 | `RPL_MOTDSTART` | `:- <servidor> Message of the day -` |
| 376 | `RPL_ENDOFMOTD` | `:End of /MOTD command` |
| 381 | `RPL_YOUREOPER` | `:You are now an IRC operator` |
| 382 | `RPL_REHASHING` | `<config> :Rehashing` |
| 383 | `RPL_YOURESERVICE` | `:You are service <nome>` (RFC 2812). |
| 384 | `RPL_MYPORTIS` | (legado). |
| 391 | `RPL_TIME` | `<servidor> :<data/hora>` |
| 392 | `RPL_USERSSTART` | `:UserID Terminal Host` (legado). |
| 393 | `RPL_USERS` | `:<texto>` (legado). |
| 394 | `RPL_ENDOFUSERS` | `:End of users` (legado). |
| 395 | `RPL_NOUSERS` | `:Nobody logged in` (legado). |

---

## 11. Respostas de RESERVED / Outros — Tabela Completa

> Códigos acima de **599** são reservados/legados ou específicos de implementação. Os principais que você encontrará:

| Código | Nome | Uso comum |
|---|---|---|
| 600–999 | — | Reservados (RFC 2812) / não padronizados. |
| 005 (é 004+1…) | — | Na verdade `005` é tratado como RPL_ISUPPORT (ver seção 10). |
| 008 | `RPL_SNOMASKIS` | Implementações como InspIRCd usam para *server notice masks*. |
| 010 | `RPL_BOUNCE` / `RPL_REDIR` | Servidores usam para redirecionar o cliente para outro servidor: `:Try server <nome>, port <porta>` |
| 042 | `RPL_YOURID` | InspIRCd: mostra ID do usuário. |
| 099 | `RPL_AWAY`? — não | (não usado). |
| 100–199 | — | Historicamente usados por versões antigas (não padrão). |
| 900–903 | — | SASL (IRCv3): 900 = logged in, 902 = nick in use, 903 = success. |
| 904–907 | — | SASL: falha/abort etc. |
| 908 | `RPL_SASLMECHS` | SASL mechanisms disponíveis. |
| 670 | `RPL_STARTTLS` | STARTTLS (IRCv3). |
| 691 | `ERR_STARTTLS` | Falha no STARTTLS. |
| 696 | `ERR_INVALIDMODEPARAM` | InspIRCd: parâmetro de modo inválido (amplamente adotado). |

---

## 12. Respostas Especiais de Comando

Além dos numerics, os comandos a seguir têm **formato próprio de resposta**.

| Resposta | Formato | Exemplo |
|---|---|---|
| **`JOIN`** | `:<user> JOIN <canal>` | `:rms!guest@host JOIN #redes` |
| **`PART`** | `:<user> PART <canal> :<motivo>` | `:rms!guest@host PART #redes :tchau` |
| **`QUIT`** | `:<user> QUIT :<motivo>` | `:rms!guest@host QUIT :Client Quit` |
| **`KICK`** | `:<op> KICK <canal> <user> :<motivo>` | `:op!a@b KICK #redes rms :spam` |
| **`MODE`** | `:<user> MODE <canal> <modos> [<args>]` | `:rms MODE #redes +o amigo` |
| **`TOPIC`** | `:<user> TOPIC <canal> :<tópico>` | `:rms TOPIC #redes :Novo assunto` |
| **`NICK` (mudança)** | `:<nick-antigo> NICK :<nick-novo>` | `:rms NICK :rms_away` |
| **`PING`** | `PING :<token>` | `PING :1234567890` |
| **`PONG`** | `:<user> PONG <servidor> :<token>` | `:rms PONG irc.exemplo.net :1234567890` |
| **`ERROR`** | `ERROR :<motivo>` | `ERROR :Closing Link: rms (Excess Flood)` |
| **`CAP`** (IRCv3) | `CAP <subcomando> [<caps>]` | `CAP LS 302`, `CAP REQ :sasl`, `CAP END` |

> **Observação sobre PING:** no IRC o cliente responde `PONG` **ecoando o token**. Não confundir com ping ICMP.

---

## 13. Exemplos de Sessões Reais (Handshake → Uso → Saída)

### 13.1. Registro de um novo cliente (handshake completo)

Conexão TCP em `irc.exemplo.net:6667`. As linhas marcadas com `C:` são enviadas pelo **cliente** e `S:` pelo **servidor**.

```
C: NICK rms
C: USER guest 0 * :Ronaldo Menezes da Silva
S: :irc.exemplo.net 001 rms :Welcome to the Internet Relay Network rms!guest@192.168.1.10
S: :irc.exemplo.net 002 rms :Your host is irc.exemplo.net, running version exemplo-1.0
S: :irc.exemplo.net 003 rms :This server was created Tue Sep 08 2026 at 08:00:00 BRT
S: :irc.exemplo.net 004 rms irc.exemplo.net exemplo-1.0 iow biklmnopstv bklov
S: :irc.exemplo.net 005 rms CHANTYPES=# NICKLEN=30 CHANNELLEN=50 TOPICLEN=300 AWAYLEN=160
          KICKLEN=200 MAXCHANNELS=25 MAXBANS=100 NETWORK=ExemploNet :are supported by this server
S: :irc.exemplo.net 251 rms :There are 2 users and 0 invisible on 1 servers
S: :irc.exemplo.net 252 rms 0 :operator(s) online
S: :irc.exemplo.net 254 rms 1 :channels formed
S: :irc.exemplo.net 255 rms :I have 2 clients and 1 servers
S: :irc.exemplo.net 375 rms :- irc.exemplo.net Message of the day -
S: :irc.exemplo.net 372 rms :- Bem-vindo ao servidor de exemplo!
S: :irc.exemplo.net 376 rms :End of /MOTD command
```

### 13.2. Trocas de nick: erro e sucesso

```
C: NICK rms
C: NICK rms              ← outro cliente já tomou o nick
S: :irc.exemplo.net 433 rms :Nickname is already in use
C: NICK rms2             ← tentativa com nick livre
S: :rms NICK :rms2       ← o próprio servidor ecoa a mudança (prefixo = nick antigo)
```

### 13.3. Entrando em um canal e conversando (PRIVMSG broadcast)

Dois clientes registrados: `rms` e `amigo`. O canal `#redes` não existia.

```
C(rms):   JOIN #redes
S:        :rms!guest@192.168.1.10 JOIN :#redes
S:        :irc.exemplo.net 353 rms = #redes :@rms
S:        :irc.exemplo.net 366 rms #redes :End of /NAMES list

C(amigo): JOIN #redes
S:        :amigo!usr@10.0.0.5 JOIN :#redes
S(→rms):  :amigo!usr@10.0.0.5 JOIN :#redes      ← broadcast do JOIN para os demais
S(→amigo): :irc.exemplo.net 353 amigo = #redes :@rms +amigo
S(→amigo): :irc.exemplo.net 366 amigo #redes :End of /NAMES list

C(rms):   PRIVMSG #redes :Olá, pessoal!
S(→todos no canal): :rms!guest@192.168.1.10 PRIVMSG #redes :Olá, pessoal!
```

> **Detalhe:** o JOIN do próprio cliente chega **com prefixo do usuário**, sem numeric. Os numerics 353/366 (NAMES) são enviados **apenas ao cliente que entrou**.

### 13.4. Mensagem privada (DM) + resposta de erro de alvo inexistente

```
C: PRIVMSG amigo :Você viu o material da aula?
S: :rms!guest@192.168.1.10 PRIVMSG amigo :Você viu o material da aula?
C: PRIVMSG alguem_que_nao_existe :oi
S: :irc.exemplo.net 401 rms alguem_que_nao_existe :No such nick/channel
```

### 13.5. NOTICE nunca gera resposta automática

```
C: NOTICE amigo :Estou te enviando um aviso (posso até usar CTCP VERSION)
S: :rms!guest@192.168.1.10 NOTICE amigo :Estou te enviando um aviso
```

> **CTCP:** mensagens `PRIVMSG`/`NOTICE` com `\x01` no início e fim (`\x01VERSION\x01`) são usadas para comandos CTCP (`VERSION`, `TIME`, `PING`, `ACTION` = `/me`). O `ACTION` é: `PRIVMSG #redes :\x01ACTION dá risada\x01`.

### 13.6. Operações de canal: modo, tópico e kick

```
C(op): MODE #redes +o amigo
S:     :op!o@host MODE #redes +o amigo        ← broadcast no canal

C:     TOPIC #redes
S:     :irc.exemplo.net 332 rms #redes :Protocolo IRC - guia profissional
S:     :irc.exemplo.net 333 rms #redes op!o@host 1757347200

C:     MODE #redes +m                           ← canal moderado
S:     :op!o@host MODE #redes +m
C(amigo s/ voz): PRIVMSG #redes :posso falar?
S:     :irc.exemplo.net 404 amigo #redes :Cannot send to channel

C(op): KICK #redes amigo :flood
S:     :op!o@host KICK #redes amigo :flood
```

### 13.7. WHOIS

```
C: WHOIS amigo
S: :irc.exemplo.net 311 rms amigo user 10.0.0.5 * :Amigo de Aula
S: :irc.exemplo.net 319 rms amigo :@#redes #geral
S: :irc.exemplo.net 312 rms amigo irc.exemplo.net :Servidor de Exemplo
S: :irc.exemplo.net 317 rms amigo 42 1757347200 :seconds idle, signon time
S: :irc.exemplo.net 318 rms amigo :End of /WHOIS list
```

### 13.8. AWAY e respostas automáticas (301)

```
C: AWAY :Estudando protocolo IRC
S: :irc.exemplo.net 306 rms :You have been marked as being away

C(amigo): PRIVMSG rms :bora?
S(→amigo): :irc.exemplo.net 301 amigo rms :Estudando protocolo IRC   ← resposta automática
S(→rms):   :amigo!usr@10.0.0.5 PRIVMSG rms :bora?
```

### 13.9. Keep-alive: PING/PONG entre servidor e cliente

```
S: PING :1757347200123
C: PONG :1757347200123
```

### 13.10. Saída (QUIT) e broadcast

```
C(rms): QUIT :Saindo para almoçar
S(→todos os canais do rms): :rms!guest@192.168.1.10 QUIT :Saindo para almoçar
S: (fecha o socket)
```

### 13.11. Comandos enviados fora de hora (antes do registro) → 451

```
C: PRIVMSG #redes :oi          ← sem NICK/USER ainda
S: :irc.exemplo.net 451 * :You have not registered
```

### 13.12. Senha incorreta / não registrado / requisitos de privilégios

```
C: PASS errada
C: NICK rms
C: USER guest 0 * :Ronaldo
S: :irc.exemplo.net 464 rms :Password incorrect
S: ERROR :Closing Link: rms[192.168.1.10] (Bad Password)

C: KICK #redes alguem :vai  → sem ser operador do canal
S: :irc.exemplo.net 482 rms #redes :You're not channel operator
```

### 13.13. Sessão completa com `nc` (teste manual rápido)

```bash
$ nc 127.0.0.1 6667
NICK rms
USER guest 0 * :Teste Manual
:irc.local 001 rms :Welcome to the Internet Relay Network rms!guest@127.0.0.1
...
JOIN #teste
:rms!guest@127.0.0.1 JOIN :#teste
:irc.local 353 rms = #teste :@rms
:irc.local 366 rms #teste :End of /NAMES list
PRIVMSG #teste :funciona!
:rms!guest@127.0.0.1 PRIVMSG #teste :funciona!
```

---

## 14. Implementação Profissional — Checklist e Boas Práticas

### 14.1. Núcleo do protocolo
- [ ] Parser de mensagens: suporta prefixo `:`, trailing `:`, até 512 bytes, limite de 14 params.
- [ ] Fila de envio por cliente (sendq) e **flush não-bloqueante**.
- [ ] Timeouts: registro (ex.: 30 s) e keep-alive (`PING` a cada ~60–90 s; sem `PONG` em ~2–3 min → encerra).
- [ ] Envio de `PING` automático e resposta `PONG`.
- [ ] Case-insensitive para nicks/canais (defina `rfc1459` ou `ascii` e anuncie via `005 CASEMAPPING=`).
- [ ] Validação de nick (comprimento ≤ 9 ou NICKLEN anunciado; charset).
- [ ] Registro: aceitar `PASS`/`NICK`/`USER` antes; os demais → `451`. `USER`/`NICK` duplicados → `462`.
- [ ] Lidar com `QUIT` limpo, notificando canais e amigos.

### 14.2. Estado de canais
- [ ] Estruturas: usuário, canal, associação (nick → modos no canal), banlist.
- [ ] `JOIN` com `+k` (475), `+i` (473), `+l` (471), `+b` (474).
- [ ] Limite de canais por usuário → `405`.
- [ ] Topico (`332`/`333`), modo `+t`.
- [ ] Modo `+n` (sem msg externa → `404`), `+m` (moderado → ver voz).
- [ ] Broadcast de `JOIN`/`PART`/`QUIT`/`NICK`/`TOPIC`/`MODE`/`KICK`.
- [ ] Enviar `353`+`366` ao novo membro com prefixos `@`, `%`, `+`.

### 14.3. Segurança e robustez
- [ ] **Limite de taxa (flood control):** ex. 8 msgs / 2 s. Excedeu → `ERROR :Excess Flood` + close (ou `+R`/`+q` em canais). 
- [ ] Sanitização: nunca ecoar NUL/CR/LF; usar `:` para qualquer texto.
- [ ] Não revelar IP em `WHOIS` para usuários sem privilégio (modo `+i`, `+x`).
- [ ] Validar todos os tamanhos (tópico, kick msg, away, nick) conforme `005`.
- [ ] Tratar conexões anônimas (`USERHOST`/`WHO` com host encoberto).
- [ ] Log de erros e auditoria de `OPER`/`KILL`.
- [ ] Anti-spoofing: conferir `PASS` antes de aceitar registro.

### 14.4. Compatibilidade
- [ ] Responder `005 RPL_ISUPPORT` com os recursos (essencial para clientes modernos).
- [ ] Enviar MOTD (`375`/`372`/`376` ou `422` se ausente).
- [ ] Implementar `CAP LS 302`/`CAP END` mesmo sem suportar extensões (clientes modernos exigem).
- [ ] Apoiar `NAMES`, `LIST`, `WHO`, `WHOIS`, `WHOWAS`, `ISON`, `USERHOST`.

### 14.5. Testes que você deve fazer
1. Dois clientes em canais separados; mensagem privada direta.
2. Colisão de nick, nick inválido, troca de nick dentro de canal (atualizar prefixos na lista 353).
3. Flood control e cliente que não responde PING.
4. Linha > 512 bytes, linha com `\n` solto (parser deve tolerar/descartar).
5. Rejoin após KICK; banlist com máscara `*!*@host`.
6. Modos combinados: `MODE #c +ov nick1 nick2`, `MODE #c -o+b nick mask`.

---

## 15. Extensões (IRCv3 / Modernas)

O núcleo da RFC não cobre tudo que clientes atuais esperam. As extensões mais relevantes:

| Extensão | Comando/Tag | Propósito |
|---|---|---|
| **CAP** | `CAP LS/REQ/ACK/NAK/LIST/END` | Negociação de capacidades. |
| **sasl** | `AUTHENTICATE` | Autenticação (nick/password) no registro. |
| **message-tags** | tags `@...` no início da linha | Metadados por mensagem. |
| **multi-prefix** | — | Enviar todos os prefixos no NAMES (`@+`). |
| **away-notify** | — | Avisar automaticamente entrada/saída de AWAY. |
| **account-notify / extended-join** | tags | Informações de conta no JOIN. |
| **echo-message** | — | Servidor ecoa PRIVMSG do próprio usuário. |
| **userhost-in-names** | — | Incluir userhost no NAMES. |
| **cap-notify** | — | Mudanças de CAP em runtime. |
| **server-time** | tag `time=` | Timestamp das mensagens. |
| **batch** | `BATCH` | Agrupar múltiplas mensagens. |
| **labeled-response** | tag `label=` | Associar resposta a comando. |
| **starttls** | `STARTTLS` | Upgrade para TLS na porta 6667. |
| **TAGMSG** | comando | Mensagens apenas com tags (ex.: reações). |

**Página de referência:** https://ircv3.net/

---

## 16. Referências para Estudo Aprofundado

| Recurso | URL |
|---|---|
| RFC 1459 | https://datatracker.ietf.org/doc/html/rfc1459 |
| RFC 2810–2813 | https://datatracker.ietf.org/doc/html/rfc2810 (arquitetura); .../rfc2811 (canais); .../rfc2812 (cliente); .../rfc2813 (servidor) |
| Modern IRC Docs | https://modern.ircdocs.horse/ |
| IRCv3 | https://ircv3.net/ |
| Lista de numerics | https://www.alien.net.au/irc/irc2numerics.html |

---

## Apêndice A — Fluxograma de registro simplificado

```
 TCP conectado (socket aceito)
        │
        ▼
 espera PASS? → NICK → USER
        │
        ▼
 VALIDA (nick livre? senha ok? host banido?)
  ├─ erro → numeric (433/464/465...) → ERROR → close
  └─ ok   → 001..004 → 005 → 251..255 → 375/372/376 (MOTD)
        │
        ▼
   REGISTRADO — pronto para JOIN/PRIVMSG/...
```

## Apêndice B — Formatação do primeiro comando de resposta
```c
// Conceito: toda resposta segue <prefixo> <codigo/nome> <nick> <resto>
":%s %03d %s :%s\r\n", server_name, 401, client.nick, "No such nick/channel"
```
> Use `%03d` para numerics e garanta `\r\n` no fim. NUNCA use apenas `\n`.
