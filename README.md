Demonstração de Run Condition
=====================================

Nesta demonstração de como é um run condition, foi realizado um prompt para uma IA generativa para que se crie um código funcional e mostrar o problema de sincronismo de utilização de recursos compartilhados que leva a condição de corrida (Race Condition). Foi utilizado um exemplo real para demonstrar esta situação, tal qual foi tratado um saldo bancário realizando duas compras separadas e simultâneas, no entanto, as compras compartilham a mesma variável mas não se comunicam entre si, acarretando em um erro estrutural lógico acarretado por Race condition.

```
O prompt realizado para gerar o código solicitado e a situação do Race Codition foi o seguinte: (Feito para a IA do Gemini)
"
Estou realizando uma atividade de microcontroladores com o propósito de obter um código que demonstre um "Race Condition", isto é, demonstrar o problema de sincronismo de utilização de recursos compartilhados que leva a condição de corrida. 
Assim, vamos se apropriar de um exemplo real: Saldo bancário e compras simultâneas, aonde nesta situação ocorre as seguintes:
- O que acontece: Você tem um saldo de R$ 2.000,00. Ao mesmo tempo, você faz duas compras: uma de R$ 1.500,00 e outra de R$ 1.700,00 em sites ou terminais diferentes
- A falha (Check-then-act): Ambos os sistemas checam o saldo de R$ 2.000,00, validam que há dinheiro suficiente para cada compra de forma isolada, e autorizam a transação.
- O resultado: O saldo final fica negativo em R$ -1.200,00 porque os processos não se comunicaram para atualizar o valor entre a leitura e a escrita.

Desta forma, crie um código que replique essa situação, e que demonstre a situação de "Race Codition", faça com seja impresso no terminal os resultados para monstrar isso.

A respeito sobre o ambiente de código e o tipo de estrutura, será feito no VScode sob a estrutura do Zephyr RTOS no microcontrolador Freedom KL25Z, sobretudo, no ambiente da platformio.io (faça neste ambiente se acha necessário). Forneça o código de forma funcional que satisfaça todas estas condições.
"
tal qual foi fornecido o código para se colocado na "src/main.c", "zephyr/prj.conf" e "platformio.ini". 

No entanto, a sáida visual no terminal serial monitor não deixava claro que a situação de race condition estava de fato ocorrendo, então foi feito outro prompt para que deixe mais visível as situações que deveriam ocorrer:
"
no monitor serial, ocorreu a o que foi comentado na nota de execução, não aparecendo os -1200. Desta vez, force isso ocorrer. Deixe o mais conveniente possível de forma visual que está ocorrendo um Run Condition
"

A resposta dada foi o código atual e anexado no "src/main.c"

O resultado dado no terminal do monitor serial foi o seguinte abaixo, obtido toda vez que se apertou o botão de reset da placa freedom KL25Z conectada ao notebook 
```
