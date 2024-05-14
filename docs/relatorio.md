![UC LOGO](/assets/uc-logo.png)

---
title: Trabalho Prático - SO - Relatório
subtitle: Sistemas Operativos
author: ["David Carvalheiro", "Luís Góis"]
date: \today
---

# Simulador de Sistema de Autorização de Serviços Móveis 5G

Para proteger o acesso aos recursos compartilhados, como a shared memory,
LogFile e as queues, devemos implementar mecanismos de sincronização, tais como
semáforos e mutexes.

Optou-se pela utilização de semáforos para controlar o acesso ao LogFile e à
Message Queue,uma vez que permitem múltiplas threads ou processos acessem um
mesmo recurso.

Para a shared memory, que contém a informação sobre os dados disponíveis de cada
utilizador, utilizados por diversos processos/ threads optou-se por utilizar
semáforos durante a consulta e alteração dos dados.

Por outro lado, para as queues de espera por serem acedidas apenas por várias
threads de um mesmo processo, decidiu-se usar mutexes. Uma vez que permitem o
acesso de apenas uma thread por vez.

Desta forma, mantém-se a integridade dos dados.

## Shared Memory

Utilizou-se a memória partilhada para armazenar diversos tipos de dados. 

## Message Queue

As mensagens circulantes nesta fila, derivam de uma construção com um
messageType e com um conjunto de estatísticas ou alertas, sendo a diferenciação
resultante do messageType. Não necessitam de mecanismos de sincronização porque
já são seguras por si.


![Diagrama](/assets/diagram.png)
