# Microcontroladores

Este repositório reúne todas as atividades desenvolvidas durante o laboratório da disciplina de Microcontroladores, incluindo exercícios práticos, implementações em linguagem C embarcada e o projeto final desenvolvido ao longo do curso.

## Conteúdo do repositório

* Atividades realizadas em laboratório;
* Exercícios práticos utilizando microcontroladores;
* Códigos desenvolvidos durante as aulas;
* Projeto final da disciplina;
* Materiais e arquivos de apoio utilizados durante o semestre.

## Projeto Final – Sistema de Monitoramento de Sinais Vitais

O projeto consiste no desenvolvimento de um sistema embarcado para monitoramento de sinais vitais utilizando um microcontrolador AVR. O sistema realiza a leitura de sensores por meio do conversor ADC (Conversor Analógico-Digital) para acompanhar dois parâmetros principais:

* **Temperatura corporal**
* **Frequência cardíaca (BPM – Batimentos por Minuto)**

Com base nos valores capturados, o sistema classifica automaticamente o estado do paciente em diferentes condições:

### Estados do sistema

* **NORMAL:** temperatura e frequência cardíaca dentro dos limites definidos;
* **ALERTA:** quando apenas um dos parâmetros apresenta alteração;
* **CRÍTICO:** quando temperatura e frequência cardíaca estão simultaneamente fora da faixa segura;
* **SILENCIADO:** modo temporário para desativação do alarme.

## Funcionalidades implementadas

* Leitura de sensores utilizando **ADC**;
* Comunicação serial **UART** para exibição dos dados em monitor serial;
* Controle de tempo utilizando **interrupções (Timer2)**;
* Geração de sinais sonoros através de **PWM** para acionamento do buzzer;
* Sistema de alarmes visuais com **LED**;
* Botão de interrupção externa para silenciar alarmes temporariamente;
* Máquina de estados para gerenciamento inteligente das condições monitoradas.

O sistema altera automaticamente o comportamento do LED e do buzzer conforme a gravidade da situação, além de exibir continuamente os valores lidos e o estado atual do monitoramento via comunicação serial.

Este projeto teve como objetivo aplicar conceitos de sistemas embarcados, manipulação de registradores, interrupções, temporizadores, comunicação serial, leitura de sensores e controle de periféricos em um cenário prático de monitoramento.
