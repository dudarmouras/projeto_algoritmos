# projeto_algoritmos
Project intended for renting classrooms in the university

1. Descrição do problema e objetivos
O projeto consiste no desenvolvimento de um sistema para gerenciar o agendamento de salas de aula em uma instituição de ensino superior. O objetivo principal é permitir que múltiplos agendamentos sejam feitos para diferentes horários em uma mesma sala, garantindo que não haja sobreposição de horários. O sistema deve possibilitar a criação, consulta e controle dos agendamentos, assegurando a organização eficiente das salas para as atividades acadêmicas.

2. Explicação das estruturas e funções implementadas
Para representar os agendamentos, utilizamos estruturas compostas (structs) que armazenam informações como horário de início e fim, professor responsável e disciplina. Cada sala possui uma lista encadeada dinâmica que guarda seus agendamentos ordenados por horário, facilitando a busca e inserção.
As funções principais implementadas são:
Verificação de horário livre: percorre os agendamentos da sala para garantir que o novo horário não se sobreponha a nenhum existente.

- Inserção de agendamento: adiciona um novo agendamento na lista da sala, mantendo a ordem cronológica.
- Listagem de agendamentos: exibe os horários reservados para cada sala.
- Leitura e gravação em arquivo: para persistência dos dados entre execuções do programa.

Essas funções garantem modularidade, manipulação eficiente de memória via ponteiros, e organização clara dos dados.

3. Exemplos de execução do programa
Ao tentar agendar a sala 5 das 9h às 10h para a disciplina “Algoritmos” com o professor João, o sistema verifica os agendamentos existentes e confirma a disponibilidade, realizando a reserva com sucesso.

Caso haja um agendamento das 9h às 10h e outro das 15h às 16h na sala 5, uma nova tentativa de agendar das 9h30 às 10h30 será rejeitada por conflito, enquanto um pedido para o horário das 10h às 11h será aceito.

O usuário pode listar todos os agendamentos da sala 5 para consultar horários disponíveis e ocupados.

Especificações:
• Tipos compostos: registros e/ou uni ̃oes.
• Vari ́aveis globais e locais.
• Modulariza ̧c ̃ao com base em fun ̧c ̃oes.
• Ponteiros e aloca ̧c ̃ao dinˆamica de mem ́oria.
• Entrada e sa ́ıda: fluxos padr ̃ao e arquivos.

Onde há no nosso código:
....