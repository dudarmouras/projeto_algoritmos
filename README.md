# Sistema de Agendamento de Salas

## Descrição
Este projeto para a disciplina Algoritmos 25.2 (CIN0140).
O projeto consiste no desenvolvimento de um sistema para gerenciar o agendamento de salas de aula em uma instituição de ensino superior. O objetivo principal é permitir que múltiplos agendamentos sejam feitos para diferentes horários em uma mesma sala, garantindo que não haja sobreposição de horários. O sistema deve possibilitar a criação, consulta e controle dos agendamentos, assegurando a organização eficiente das salas para as atividades acadêmicas.

### O sistema opera por meio de um menu textual interativo, possibilitando ao usuário:
- Inserir novos agendamentos;
- Listar os agendamentos de uma sala;
- Salvar e carregar os dados em arquivo texto;
- Exibir todas as salas cadastradas (ordenadas usando BST);
- Encontrar o agendamento mais curto usando Fila de Prioridade/Heap;
- Buscar sala por ID usando Busca Binária;
- Listar agendamentos ordenados por disciplina usando QuickSort;
- Encontrar o melhor caminho entre agendamentos usando algoritmo de Grafo/Dijkstra Simplificado.

## 🧩 Funcionalidades Principais

✅ **Controle de conflitos de horário** — impede reservas sobrepostas, percorrendo os agendamentos da sala para garantir que o novo horário não se sobreponha a nenhum existente.

📖 **Listagem de agendamentos** — exibe os horários reservados para cada sala.

🎁 **Inserção de agendamento** — adiciona um novo agendamento na lista da sala, mantendo a ordem cronológica.

💾 **Persistência de dados** — leitura e gravação em arquivo (agendamentos.txt).

🧠 **Estrutura dinâmica** — cada sala possui uma lista encadeada de agendamentos.

🌳 **Árvore Binária de Busca (BST)** — gerencia as salas de forma eficiente, permitindo inserção, busca e listagem ordenada.

🔍 **Busca Binária** — localiza salas rapidamente em uma lista ordenada de IDs.

📊 **Fila de Prioridade (Min-Heap)** — identifica o agendamento com menor duração entre todos os cadastrados.

⚡ **QuickSort** — ordena agendamentos por disciplina ou por horário de início.

🗺️ **Algoritmo de Grafo (Dijkstra Simplificado)** — encontra a melhor transição entre agendamentos sequenciais com menor tempo ocioso.

⚙️ **Modularização completa** — funções independentes e reutilizáveis.

💬 **Interface textual simples** — interação direta via terminal.  
    
## 🏗️ Estruturas de Dados Utilizadas

O sistema utiliza múltiplas estruturas de dados para garantir eficiência e organização:

### 1. **Lista Encadeada**
Cada sala possui uma lista encadeada de agendamentos ordenados cronologicamente, permitindo inserção e busca eficientes.

### 2. **Árvore Binária de Busca (BST)**
As salas são organizadas em uma BST pelo ID, permitindo:
- Inserção em O(log n) no caso médio
- Busca eficiente de salas
- Listagem ordenada (travessia in-order)

### 3. **Fila de Prioridade (Min-Heap)**
Utilizada para encontrar o agendamento com menor duração entre todos os cadastrados, com complexidade O(1) para acessar o mínimo.

### 4. **Union (RepresentacaoTempo)**
Permite armazenar o tempo tanto como minutos (int) quanto como string formatada (HH:MM), economizando memória.

### 5. **Vector (STL)**
Usado para coletar e manipular conjuntos de agendamentos quando necessário, especialmente para algoritmos de ordenação e grafo.

## 📋 De que forma todos os requisitos do projeto foram cumpridos 

| Requisito                         | Onde foi implementado no código                                           |
|----------------------------------|--------------------------------------------------------------------------|
| **Tipos compostos**              | `struct Agendamento`, `struct Sala`, `struct NoAgendamento`, `struct NoSala`, `struct NoGrafo`, `union RepresentacaoTempo` |
| **Variáveis Globais**            | `raizBSTGlobal`, `nomeArquivoPadraoGlobal`     |
| **Variáveis Locais**             | `opcao`, `professor`, `disciplina`, `idSala`, `menorOcio`, etc.                              |
| **Ponteiros e Alocação Dinâmica** | Listas encadeadas: `new Sala`, `new NoAgendamento`, `new NoSala`; <br> Ponteiros: `NoAgendamento* prox`, `NoSala* esquerda/direita`, `Sala*`; <br> Liberação: `delete`, `LiberarBST`, `LiberarNosDaSala` |
| **Modularização (Funções)**      | `ConverterHorarioParaMinutos`, `InserirAgendamentoNaSala`, `InserirSalaBST`, `BuscarSalaBST`, `QuickSortAgendamentos`, `EncontrarAgendamentoMaisCurto`, `EncontrarMelhorCaminhoDijkstra`, `BuscaBinariaSalaPorID` |
| **Entrada e Saída**              | Fluxos padrão: `cin`, `cout`; <br> Arquivos: `ofstream`, `ifstream`, `getline` |
| **Árvore Binária de Busca**      | `NoSala`, `InserirSalaBST`, `BuscarSalaBST`, `ListarSalasBST` (travessia in-order) |
| **Busca Binária**                | `BuscaBinariaSalaPorID` (opção 7 do menu) |
| **QuickSort**                    | `QuickSortAgendamentos` (por disciplina), `QuickSortAgendamentosPorInicio` (por horário) |
| **Fila de Prioridade/Heap**      | `priority_queue<Agendamento>` com `ComparadorDuracaoAgendamento` (opção 6) |
| **Grafo/Dijkstra**               | `EncontrarMelhorCaminhoDijkstra` - encontra menor tempo ocioso entre agendamentos (opção 9) |

## 💡 Exemplos de execução do programa

### **Caso 1: Agendamento bem-sucedido**
Ao tentar agendar a sala 5 das 9h às 10h para a disciplina "Algoritmos" com o professor João, o sistema verifica os agendamentos existentes na lista encadeada da sala e confirma a disponibilidade, realizando a reserva com sucesso.

### **Caso 2: Conflito de horário**
Caso haja um agendamento das 9h às 10h e outro das 15h às 16h na sala 5, uma nova tentativa de agendar das 9h30 às 10h30 será rejeitada por conflito (sobreposição detectada), enquanto um pedido para o horário das 10h às 11h será aceito.

### **Caso 3: Consulta de horários disponíveis**
O usuário pode listar todos os agendamentos da sala 5 para consultar horários disponíveis e ocupados.

### **Caso 4: Busca de sala (BST e Busca Binária)**
Ao buscar a sala 5, o sistema utiliza a BST para localização eficiente. Na opção 7, é utilizada busca binária em uma lista ordenada de IDs.

### **Caso 5: Agendamento mais curto**
O sistema utiliza uma fila de prioridade (min-heap) para identificar qual agendamento tem a menor duração entre todos cadastrados, exibindo sala, duração e horário.

### **Caso 6: Ordenação por disciplina**
Usando QuickSort, o sistema ordena e exibe todos os agendamentos alfabeticamente por nome da disciplina.

### **Caso 7: Melhor caminho (Dijkstra)**
O algoritmo encontra a transição entre agendamentos com menor tempo ocioso, indicando as salas de origem e destino e o custo (tempo ocioso em minutos).





## 🚀 Como executar o código

**1. Compile o código**
```bash
g++ main.cpp -o agendamento
```

**2. Execute o programa**
```bash
./agendamento
```

**3. Utilize o menu interativo**

O programa apresenta um menu com as seguintes opções:
- **1** - Inserir agendamento
- **2** - Listar agendamentos de uma sala
- **3** - Salvar em arquivo (BST)
- **4** - Carregar de arquivo (BST)
- **5** - Listar todas as salas (BST In-Order)
- **6** - Encontrar Agendamento Mais Curto (Fila de Prioridade/Heap)
- **7** - Buscar Sala por ID (Busca Binária)
- **8** - Listar Agendamentos Ordenados por Disciplina (QuickSort)
- **9** - Encontrar Melhor Caminho de Agendamentos (Grafo/Dijkstra Simplificado)
- **0** - Sair






## Equipe
[Eduarda Rodrigues de Moura Santana](https://github.com/dudarmouras)

[Isaac Manoel Teixeira da Silva](https://github.com/isaacteixeira06)

[Isabelle Tenorio Vaz Bezerra](https://github.com/IsabelleTenorio)

[Júlia Maria Campelo Bezerra](https://github.com/juliamcbezerra)

[Millena Ellen da Silva ](https://github.com/ellenpry)
