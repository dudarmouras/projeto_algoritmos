#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <queue>
#include <cmath>
#include <limits>
#include <stdexcept>

using namespace std;

// DECLARAÇÕES AVANÇADAS
struct Agendamento;
struct NoAgendamento;
struct Sala;
struct NoSala;
struct NoGrafo;

// VARIÁVEIS GLOBAIS:
NoSala* raizBSTGlobal = nullptr;
const string nomeArquivoPadraoGlobal = "agendamentos.txt";

// FUNÇÕES DE LIBERAÇÃO DE MEMÓRIA
void LiberarNosDaSala(Sala* sala);


// TIPOS COMPOSTOS
union RepresentacaoTempo {
    int minutos;
    char hhmm[6];
    RepresentacaoTempo() { minutos = 0; hhmm[0] = '\0'; }
};

struct Agendamento {
    RepresentacaoTempo inicio;
    RepresentacaoTempo fim;
    bool armazenadoComoMinutos;
    string professor;
    string disciplina;

    int DuracaoMinutos() const {
        return fim.minutos - inicio.minutos;
    }
};

struct NoAgendamento {
    Agendamento ag;
    NoAgendamento* prox;
    NoAgendamento(const Agendamento& a) : ag(a), prox(nullptr) {}
};

struct Sala {
    int id;
    NoAgendamento* cabeca;
    Sala(int ident) : id(ident), cabeca(nullptr) {}

    vector<Agendamento> ObterListaAgendamentos() const {
        vector<Agendamento> lista;
        NoAgendamento* atual = cabeca;
        while (atual) {
            lista.push_back(atual->ag);
            atual = atual->prox;
        }
        return lista;
    }
};

// ESTRUTURA: ÁRVORE BINÁRIA DE BUSCA (ABB) PARA SALAS
struct NoSala {
    Sala* sala;
    NoSala* esquerda;
    NoSala* direita;
    NoSala(Sala* s) : sala(s), esquerda(nullptr), direita(nullptr) {}
};

// ESTRUTURA: GRAFO PARA ALGORITMO DE MENOR CUSTO
struct NoGrafo {
    int idSala;
    int indiceAgendamento;
    int custoAcumulado;

    bool operator>(const NoGrafo& outro) const {
        return custoAcumulado > outro.custoAcumulado;
    }
};


// FUNÇÕES DE TEMPO
int ConverterHorarioParaMinutos(const string& hhmm) {
    int h, m; char separador;
    istringstream fluxo(hhmm);
    if (!(fluxo >> h >> separador >> m)) return -1;
    if (separador != ':' || h < 0 || h > 23 || m < 0 || m > 59) return -1;
    return h * 60 + m;
}

string ConverterMinutosParaHHMM(int minutos) {
    int h = minutos / 60;
    int m = minutos % 60;
    ostringstream saida;
    saida << setw(2) << setfill('0') << h << ":" << setw(2) << setfill('0') << m;
    return saida.str();
}

bool TemSobreposicao(int inicio1, int fim1, int inicio2, int fim2) {
    return (inicio1 < fim2) && (inicio2 < fim1);
}

// FUNÇÕES DA ABB
NoSala* InserirSalaBST(NoSala* no, Sala* novaSala) {
    if (no == nullptr) return new NoSala(novaSala);
    if (novaSala->id < no->sala->id) {
        no->esquerda = InserirSalaBST(no->esquerda, novaSala);
    } else if (novaSala->id > no->sala->id) {
        no->direita = InserirSalaBST(no->direita, novaSala);
    }
    return no;
}

Sala* BuscarSalaBST(NoSala* no, int id) {
    if (no == nullptr) return nullptr;
    if (id == no->sala->id) return no->sala;
    if (id < no->sala->id) return BuscarSalaBST(no->esquerda, id);
    return BuscarSalaBST(no->direita, id);
}

Sala* ObterOuCriarSalaGlobal(int id) {
    Sala* salaEncontrada = BuscarSalaBST(raizBSTGlobal, id);
    if (salaEncontrada) return salaEncontrada;

    Sala* novaSala = new Sala(id);
    raizBSTGlobal = InserirSalaBST(raizBSTGlobal, novaSala);
    return novaSala;
}

void ListarSalasBST(NoSala* no) {
    if (no == nullptr) return;
    ListarSalasBST(no->esquerda);
    cout << "  Sala " << no->sala->id << "\n";
    ListarSalasBST(no->direita);
}

void LiberarNosDaSala(Sala* sala) {
    NoAgendamento* atual = sala->cabeca;
    while (atual) {
        NoAgendamento* temporario = atual;
        atual = atual->prox;
        delete temporario;
    }
    sala->cabeca = nullptr;
}

void LiberarBST(NoSala* no) {
    if (no == nullptr) return;
    LiberarBST(no->esquerda);
    LiberarBST(no->direita);
    
    // Libera os nós de agendamento antes de deletar a struct Sala
    LiberarNosDaSala(no->sala);
    delete no->sala;
    delete no;
}

void LiberarTodasAsSalas() {
    LiberarBST(raizBSTGlobal);
    raizBSTGlobal = nullptr;
}

// FUNÇÕES DE BUSCA E ORDENAÇÃO

void ColetarIDsSalasBST(NoSala* no, vector<int>& ids) {
    if (no == nullptr) return;
    ColetarIDsSalasBST(no->esquerda, ids);
    ids.push_back(no->sala->id);
    ColetarIDsSalasBST(no->direita, ids);
}

vector<int> ObterIDsSalasOrdenadas() {
    vector<int> ids;
    ColetarIDsSalasBST(raizBSTGlobal, ids);
    return ids;
}

bool BuscaBinariaSalaPorID(const vector<int>& ids, int idBuscado) {
    int esquerda = 0;
    int direita = ids.size() - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        if (ids[meio] == idBuscado) return true;
        if (ids[meio] < idBuscado) esquerda = meio + 1;
        else direita = meio - 1;
    }
    return false;
}

// QuickSort auxiliares
void TrocarAgendamentos(Agendamento& a, Agendamento& b) {
    Agendamento temp = a;
    a = b;
    b = temp;
}

// QuickSort por Disciplina (Opção 8)
int ParticionarAgendamentos(vector<Agendamento>& agendamentos, int baixo, int alto) {
    string pivo = agendamentos[alto].disciplina;
    int i = (baixo - 1);
    for (int j = baixo; j <= alto - 1; j++) {
        if (agendamentos[j].disciplina.compare(pivo) <= 0) {
            i++;
            TrocarAgendamentos(agendamentos[i], agendamentos[j]);
        }
    }
    TrocarAgendamentos(agendamentos[i + 1], agendamentos[alto]);
    return (i + 1);
}

void QuickSortAgendamentos(vector<Agendamento>& agendamentos, int baixo, int alto) {
    if (baixo < alto) {
        int pi = ParticionarAgendamentos(agendamentos, baixo, alto);
        QuickSortAgendamentos(agendamentos, baixo, pi - 1);
        QuickSortAgendamentos(agendamentos, pi + 1, alto);
    }
}

// QuickSort por Hora de Início (Para o Grafo/Dijkstra)
int ParticionarAgendamentosPorInicio(vector<Agendamento>& agendamentos, int baixo, int alto) {
    int pivo_minutos = agendamentos[alto].inicio.minutos;
    int i = (baixo - 1);
    for (int j = baixo; j <= alto - 1; j++) {
        if (agendamentos[j].inicio.minutos <= pivo_minutos) {
            i++;
            TrocarAgendamentos(agendamentos[i], agendamentos[j]);
        }
    }
    TrocarAgendamentos(agendamentos[i + 1], agendamentos[alto]);
    return (i + 1);
}

void QuickSortAgendamentosPorInicio(vector<Agendamento>& agendamentos, int baixo, int alto) {
    if (baixo < alto) {
        int pi = ParticionarAgendamentosPorInicio(agendamentos, baixo, alto);
        QuickSortAgendamentosPorInicio(agendamentos, baixo, pi - 1);
        QuickSortAgendamentosPorInicio(agendamentos, pi + 1, alto);
    }
}


// FUNÇÕES PRINCIPAIS DE SALAS E AGENDAMENTOS

bool SalaEstaDisponivel(const Sala& sala, const Agendamento& novoAgendamento) {
    NoAgendamento* atual = sala.cabeca;
    while (atual) {
        int inicioAtual = atual->ag.inicio.minutos;
        int fimAtual = atual->ag.fim.minutos;
        if (TemSobreposicao(inicioAtual, fimAtual, novoAgendamento.inicio.minutos, novoAgendamento.fim.minutos))
            return false;
        atual = atual->prox;
    }
    return true;
}

bool InserirAgendamentoNaSala(Sala& sala, const Agendamento& novoAgendamento) {
    if (!SalaEstaDisponivel(sala, novoAgendamento)) return false;

    NoAgendamento* novoNo = new NoAgendamento(novoAgendamento);

    if (!sala.cabeca || novoAgendamento.inicio.minutos < sala.cabeca->ag.inicio.minutos) {
        novoNo->prox = sala.cabeca;
        sala.cabeca = novoNo;
        return true;
    }

    NoAgendamento* atual = sala.cabeca;
    while (atual->prox && atual->prox->ag.inicio.minutos <= novoAgendamento.inicio.minutos)
        atual = atual->prox;

    novoNo->prox = atual->prox;
    atual->prox = novoNo;
    return true;
}

void ListarAgendamentosDaSala(const Sala& sala) {
    cout << "\nAgendamentos da sala " << sala.id << ":\n";
    if (!sala.cabeca) { cout << "  (vazia)\n"; return; }
    NoAgendamento* atual = sala.cabeca;
    while (atual) {
        cout << "  " << ConverterMinutosParaHHMM(atual->ag.inicio.minutos)
             << " - " << ConverterMinutosParaHHMM(atual->ag.fim.minutos)
             << " | " << atual->ag.disciplina
             << " | Prof: " << atual->ag.professor << "\n";
        atual = atual->prox;
    }
}

// FUNÇÕES DE HEAP / FILA DE PRIORIDADE

struct ComparadorDuracaoAgendamento {
    bool operator()(const Agendamento& a, const Agendamento& b) {
        return a.DuracaoMinutos() > b.DuracaoMinutos();
    }
};

void ColetarTodosAgendamentosABB(NoSala* no, priority_queue<Agendamento, vector<Agendamento>, ComparadorDuracaoAgendamento>* agendamentosCurto) {
    if (no == nullptr) return;
    ColetarTodosAgendamentosABB(no->esquerda, agendamentosCurto);
    NoAgendamento* atual = no->sala->cabeca;
    while (atual) {
        agendamentosCurto->push(atual->ag);
        atual = atual->prox;
    }
    ColetarTodosAgendamentosABB(no->direita, agendamentosCurto);
}

void EncontrarAgendamentoMaisCurto() {
    priority_queue<Agendamento, vector<Agendamento>, ComparadorDuracaoAgendamento> agendamentosCurto;
    ColetarTodosAgendamentosABB(raizBSTGlobal, &agendamentosCurto);

    if (agendamentosCurto.empty()) {
        cout << "Nenhum agendamento encontrado.\n";
        return;
    }

    const Agendamento maisCurto = agendamentosCurto.top();
    cout << "\n=== Agendamento Mais Curto (Fila de Prioridade/Min-Heap) ===\n";
    cout << "  Duracao: " << maisCurto.DuracaoMinutos() << " minutos\n";
    cout << "  Horario: " << ConverterMinutosParaHHMM(maisCurto.inicio.minutos)
         << " - " << ConverterMinutosParaHHMM(maisCurto.fim.minutos) << "\n";
    cout << "  Disciplina: " << maisCurto.disciplina << "\n";
    cout << "  Professor: " << maisCurto.professor << "\n";
}

// FUNÇÕES DE GRAFO / MENOR CUSTO

void ColetarAgendamentosParaGrafo(NoSala* no, vector<Agendamento>& todos) {
    if (no == nullptr) return;
    ColetarAgendamentosParaGrafo(no->esquerda, todos);

    NoAgendamento* atual = no->sala->cabeca;
    while (atual) {
        todos.push_back(atual->ag);
        atual = atual->prox;
    }
    ColetarAgendamentosParaGrafo(no->direita, todos);
}

vector<Agendamento> ObterTodosAgendamentos() {
    vector<Agendamento> todos;
    ColetarAgendamentosParaGrafo(raizBSTGlobal, todos);
    return todos;
}

void EncontrarMelhorCaminhoDijkstra() {
    vector<Agendamento> todosAgendamentos = ObterTodosAgendamentos();

    if (todosAgendamentos.empty()) {
        cout << "Nenhum agendamento para analise de caminho.\n";
        return;
    }

    // Ordena por tempo de início usando QuickSort customizado
    QuickSortAgendamentosPorInicio(todosAgendamentos, 0, todosAgendamentos.size() - 1);

    cout << "\n=== Analise de Caminho (Grafo/Dijkstra Simplificado) ===\n";
    
    bool encontrado = false;
    for (size_t i = 0; i < todosAgendamentos.size(); ++i) {
        const Agendamento agAtual = todosAgendamentos[i];
        
        int menorOcio = numeric_limits<int>::max();
        Agendamento agMelhorProximo;
        bool proximoEncontrado = false;

        for (size_t j = i + 1; j < todosAgendamentos.size(); ++j) {
            const Agendamento agProximo = todosAgendamentos[j];
            
            if (agProximo.inicio.minutos >= agAtual.fim.minutos) {
                int tempoOcioso = agProximo.inicio.minutos - agAtual.fim.minutos;
                
                if (tempoOcioso < menorOcio) {
                    menorOcio = tempoOcioso;
                    agMelhorProximo = agProximo;
                    proximoEncontrado = true;
                }
            }
        }
        
        if (proximoEncontrado) {
            cout << "  Transicao Otimizada (Origem -> Destino):\n";
            cout << "  - Origem (Disc: " << agAtual.disciplina << "): " << ConverterMinutosParaHHMM(agAtual.fim.minutos) << "\n";
            cout << "  - Destino (Disc: " << agMelhorProximo.disciplina << "): " << ConverterMinutosParaHHMM(agMelhorProximo.inicio.minutos) << "\n";
            cout << "  - Tempo Ocioso (Custo): " << menorOcio << " minutos\n";
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "  Nenhuma transicao de agendamento sequencial valida encontrada.\n";
    }
}


// FUNÇÕES DE ARQUIVOS
void SalvarAgendamentosABB(NoSala* no, ofstream& arquivoSaida) {
    if (no == nullptr) return;
    SalvarAgendamentosABB(no->esquerda, arquivoSaida);

    NoAgendamento* atual = no->sala->cabeca;
    while (atual) {
        arquivoSaida << no->sala->id << ";"
             << ConverterMinutosParaHHMM(atual->ag.inicio.minutos) << ";"
             << ConverterMinutosParaHHMM(atual->ag.fim.minutos) << ";"
             << atual->ag.professor << ";"
             << atual->ag.disciplina << "\n";
        atual = atual->prox;
    }

    SalvarAgendamentosABB(no->direita, arquivoSaida);
}

bool SalvarAgendamentos(const string& caminho) {
    ofstream arquivoSaida(caminho);
    if (!arquivoSaida.is_open()) return false;
    SalvarAgendamentosABB(raizBSTGlobal, arquivoSaida);
    arquivoSaida.close();
    return true;
}

bool CarregarAgendamentos(const string& caminho) {
    ifstream arquivoEntrada(caminho);
    if (!arquivoEntrada.is_open()) return false;
    
    LiberarTodasAsSalas();
    
    string linha;
    while (getline(arquivoEntrada, linha)) {
        if (linha.empty()) continue;
        vector<string> partes;
        string token;
        istringstream fluxo(linha);
        while (getline(fluxo, token, ';')) partes.push_back(token);
        if (partes.size() < 5) continue;

        try {
            int idSala = stoi(partes[0]);
            int minutosInicio = ConverterHorarioParaMinutos(partes[1]);
            int minutosFim = ConverterHorarioParaMinutos(partes[2]);
            string professor = partes[3];
            string disciplina = partes[4];

            if (minutosInicio < 0 || minutosFim <= minutosInicio) continue;

            Agendamento agendamento;
            agendamento.inicio.minutos = minutosInicio;
            agendamento.fim.minutos = minutosFim;
            agendamento.armazenadoComoMinutos = true;
            agendamento.professor = professor;
            agendamento.disciplina = disciplina;

            Sala* sala = ObterOuCriarSalaGlobal(idSala);
            InserirAgendamentoNaSala(*sala, agendamento);
        } catch (const std::exception& e) {
            continue;
        }
    }
    arquivoEntrada.close();
    return true;
}


// MENU DE INTERAÇÃO

void MostrarMenu() {
    cout << "\n=== Sistema de Agendamento de Salas ===\n"
         << "1 - Inserir agendamento\n"
         << "2 - Listar agendamentos de uma sala\n"
         << "3 - Salvar em arquivo (BST)\n"
         << "4 - Carregar de arquivo (BST)\n"
         << "5 - Listar todas as salas (BST In-Order)\n"
         << "6 - Encontrar Agendamento Mais Curto (Fila de Prioridade/Heap)\n"
         << "7 - Buscar Sala por ID (Busca Binaria)\n"
         << "8 - Listar Agendamentos Ordenados por Disciplina (QuickSort)\n"
         << "9 - Encontrar Melhor Caminho de Agendamentos (Grafo/Dijkstra Simplificado)\n"
         << "0 - Sair\n"
         << "Escolha: ";
}

// FUNÇÃO PRINCIPAL
int main() {
    int opcao = -1;

    while (opcao != 0) {
        MostrarMenu();
        if (!(cin >> opcao)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

        if (opcao == 1) {
            int idSala; string inicioTexto, fimTexto, professor, disciplina;
            cout << "Numero da sala: "; cin >> idSala;
            cout << "Horario inicio (HH:MM): "; cin >> inicioTexto;
            cout << "Horario fim (HH:MM): "; cin >> fimTexto;
            cin.ignore();
            cout << "Professor: "; getline(cin, professor);
            cout << "Disciplina: "; getline(cin, disciplina);

            int minutosInicio = ConverterHorarioParaMinutos(inicioTexto);
            int minutosFim = ConverterHorarioParaMinutos(fimTexto);
            if (minutosInicio < 0 || minutosFim <= minutosInicio) {
                cout << "Horario invalido!\n"; continue;
            }

            Agendamento agendamento;
            agendamento.inicio.minutos = minutosInicio;
            agendamento.fim.minutos = minutosFim;
            agendamento.professor = professor;
            agendamento.disciplina = disciplina;
            agendamento.armazenadoComoMinutos = true;

            Sala* sala = ObterOuCriarSalaGlobal(idSala);
            if (InserirAgendamentoNaSala(*sala, agendamento)) cout << "Agendamento inserido.\n";
            else cout << "Conflito de horario.\n";

        } else if (opcao == 2) {
            int idSala; cout << "Numero da sala: "; cin >> idSala;
            Sala* salaEncontrada = BuscarSalaBST(raizBSTGlobal, idSala);
            if (!salaEncontrada) cout << "Sala nao encontrada.\n";
            else ListarAgendamentosDaSala(*salaEncontrada);

        } else if (opcao == 3) {
            string caminho;
            cout << "Arquivo para salvar (Enter para default): ";
            cin.ignore();
            getline(cin, caminho);
            if (caminho.empty()) caminho = nomeArquivoPadraoGlobal;
            if (SalvarAgendamentos(caminho)) cout << "Salvo em " << caminho << "\n";
            else cout << "Erro ao salvar.\n";

        } else if (opcao == 4) {
            string caminho;
            cout << "Arquivo para carregar (Enter para default): ";
            cin.ignore();
            getline(cin, caminho);
            if (caminho.empty()) caminho = nomeArquivoPadraoGlobal;
            if (CarregarAgendamentos(caminho)) cout << "Carregado com sucesso.\n";
            else cout << "Erro ao carregar.\n";

        } else if (opcao == 5) {
            cout << "\nSalas existentes (BST In-Order):\n";
            if (raizBSTGlobal == nullptr) cout << "  (nenhuma)\n";
            ListarSalasBST(raizBSTGlobal);

        } else if (opcao == 6) {
            EncontrarAgendamentoMaisCurto();

        } else if (opcao == 7) {
            int idBuscado; cout << "Numero (ID) da Sala a ser buscada: "; cin >> idBuscado;
            vector<int> ids = ObterIDsSalasOrdenadas();
            if (BuscaBinariaSalaPorID(ids, idBuscado)) {
                 cout << "Sala " << idBuscado << " encontrada (via busca binaria em lista ordenada de numeros).\n";
            } else {
                 cout << "Sala " << idBuscado << " nao encontrada.\n";
            }

        } else if (opcao == 8) {
            vector<Agendamento> agendamentos = ObterTodosAgendamentos();
            if (agendamentos.empty()) {
                cout << "Nenhum agendamento para ordenar.\n";
                continue;
            }
            QuickSortAgendamentos(agendamentos, 0, agendamentos.size() - 1);
            cout << "\nAgendamentos Ordenados por Disciplina (QuickSort):\n";
            for (size_t i = 0; i < agendamentos.size(); ++i) {
                cout << "  " << agendamentos[i].disciplina << " | Prof: " << agendamentos[i].professor
                     << " (" << ConverterMinutosParaHHMM(agendamentos[i].inicio.minutos)
                     << "-" << ConverterMinutosParaHHMM(agendamentos[i].fim.minutos) << ")\n";
            }

        } else if (opcao == 9) {
            EncontrarMelhorCaminhoDijkstra();

        } else if (opcao == 0) {
            cout << "Encerrando...\n";
        } else {
            cout << "Opção invalida.\n";
        }
    }

    LiberarTodasAsSalas();
    return 0;
}