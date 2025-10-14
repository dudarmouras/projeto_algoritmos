# Sistema de Agendamento de Salas

## Descrição
Este projeto para a disciplina Algoritmos 25.2 (CIN0140).
O projeto consiste no desenvolvimento de um sistema para gerenciar o agendamento de salas de aula em uma instituição de ensino superior. O objetivo principal é permitir que múltiplos agendamentos sejam feitos para diferentes horários em uma mesma sala, garantindo que não haja sobreposição de horários. O sistema deve possibilitar a criação, consulta e controle dos agendamentos, assegurando a organização eficiente das salas para as atividades acadêmicas.

### O sistema opera por meio de um menu textual interativo, possibilitando ao usuário:
- Inserir novos agendamentos;
- Listar os agendamentos de uma sala;
- Salvar e carregar os dados em arquivo texto;
- Exibir todas as salas cadastradas.

## 🧩 Funcionalidades Principais

✅ Controle de conflitos de horário — impede reservas sobrepostas, percorrendo os agendamentos da sala para garantir que o novo horário não se sobreponha a nenhum existente.

📖 Listagem de agendamentos — exibe os horários reservados para cada sala.

🎁Inserção de agendamento — adiciona um novo agendamento na lista da sala, mantendo a ordem cronológica.

💾 Persistência de dados — leitura e gravação em arquivo (agendamentos.txt).

🧠 Estrutura dinâmica — cada sala possui uma lista encadeada de agendamentos.

⚙️ Modularização completa — funções independentes e reutilizáveis.

💬 Interface textual simples — interação direta via terminal.  
    
## 🏗️ Estrutura de Dados Utilizada

Para representar os agendamentos, utilizamos estruturas compostas (structs) que armazenam informações como horário de início e fim, professor responsável e disciplina. Cada sala possui uma lista encadeada dinâmica que guarda seus agendamentos ordenados por horário, facilitando a busca e inserção, além de ponteiros e alocação dinâmica de memória.

### De que forma todos os requisitos do projeto foram cumpridos 
| Requisito                         | Onde foi implementado no código                                           |
|----------------------------------|--------------------------------------------------------------------------|
| Tipos compostos                   | `struct Agendamento`, `struct Sala`, `struct NoAgendamento`, `union RepresentacaoTempo` |
| Variáveis Globais                 | `salasGlobais`, `nomeArquivoPadraoGlobal`, `ponteirosExemploGlobais`     |
| Variáveis Locais                  | `opcao`, `professor`, `disciplina`, `idSala`                              |
| Ponteiros e Alocação Dinâmica     | Listas encadeadas: `new Sala`, `new NoAgendamento`, `delete`; <br> Ponteiros: `NoAgendamento* prox`, `Sala*` |
| Modularização com base em funções | `ConverterHorarioParaMinutos`, `InserirAgendamentoNaSala`, `CarregarAgendamentos`, `ObterOuCriarSalaGlobal`, `ListarAgendamentosDaSala`, `SalvarAgendamentos` |
| Entrada e Saída: fluxos padrão e arquivos:                   |  `cin`, `cout`, `ofstream`, `ifstream`, `getline` |

## Exemplos de execução do programa
**Caso 1: Agendamento bem-sucedido**

Ao tentar agendar a sala 5 das 9h às 10h para a disciplina “Algoritmos” com o professor João, o sistema verifica os agendamentos existentes e confirma a disponibilidade, realizando a reserva com sucesso.

**Caso 2: Conflito de horário**

Caso haja um agendamento das 9h às 10h e outro das 15h às 16h na sala 5, uma nova tentativa de agendar das 9h30 às 10h30 será rejeitada por conflito, enquanto um pedido para o horário das 10h às 11h será aceito.

**Caso 3: Consulta de horários disponíveis**

O usuário pode listar todos os agendamentos da sala 5 para consultar horários disponíveis e ocupados.





## Como executar o código
**1.Compile o código**

`g++ main.cpp -o agendamento`

**2.Execute o programa** 

`./agendamento`

**3.Utilize o menu para interagir**






## Equipe
[Eduarda Rodrigues de Moura Santana](https://github.com/dudarmouras)

[Isaac Manoel Teixeira da Silva](https://github.com/isaacteixeira06)

[Isabelle Tenorio Vaz Bezerra](https://github.com/IsabelleTenorio)

[Júlia Maria Campelo Bezerra](https://github.com/juliamcbezerra)

[Millena Ellen da Silva ](https://github.com/ellenpry)
