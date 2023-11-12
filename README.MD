
# Detector de queda de tensão

Projeto de detecção de queda de tensão na rede elétrica, motivado pelas recorrentes falhas no meu bairro onde a concessionária Neoenergia Pernambuco (antes CELPE) atua. O projeto visa o envio de SMS para telefones cadastros quando ocorrer uma queda de tensão na rede elétrica em que o detector está sendo alimentado.


## Autores

- [@renanalencar](https://www.github.com/renanalencar)


## Material necessário

- Módulo LilyGo TTGO SIM800L (ESP32)
- Sensor de corrente ACS712 de 20A
- Fios de ligação fêmea-fêmea
- Bateria de lítio de qualquer capacidade
## Esquema de montagem

- Alimentar o sensor de corrente e ligar o pino de sinal ao GPIO 34 da ESP32;
- Ligar o sensor de corrente em série com o equipamento que queremos ser notificado (executar este passo antes de iniciar o programa).

NOTA: devido à alta sensibilidade do sensor de corrente, é aconselhavel afastar o sensor do módulo ESP32, uma vez que a recepção ou envio de mensagens podem afetar o desempenho do sensor de corrente!

ATENÇÃO: Para o bom funcionamento do programa, é necessário alterar o arquivo variables_example.h renomeando para variables.h e incluir neste header tanto a mensagem para o SMS quanto os destinatários:
```cpp
const static char* MESSAGE = "<ADD HERE THE MESSAGE YOU WANT TO SEND IN THE SMS>";
const static char* NUMBERS[] = {"<RECIPIENT_1>", "<RECIPIENT_2>"}; // Add phone numbers with "+" and the contry code, e.g. +17077225041
```
## Funcionamento

- O sensor fica constantemente efetuando leituras do valor da corrente que percorre o equipamento a ele ligado.
- Detectada uma queda de tensão, a corrente será nula, ao que o sensor envia a mensagem `FOI DETECTADA UMA QUEDA DE TENSÃO!` para o(s) número(s) do celular adicionado(s) ao arquivo `varibles.h`.
## Referência

 - [Detetor de falha de energia com A ESP32 - ElectroFun.pt](https://www.electrofun.pt/blog/detetor-de-falha-de-energia-com-esp32/)
 - [Detetor de falha de energia com A ESP32 - ElectroFun.pt (YouTube)](https://youtu.be/wlmwNB2hc4Y?si=5GupkxPdPb0jFcgB)
 - [Detector de energia elétrica com Arduino (YouTube)](https://www.youtube.com/watch?v=N8Lao5FSuNA)

