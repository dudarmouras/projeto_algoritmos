#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// VARIÁVEIS GLOBAIS:--------------------------------------

vector<class Sala*> salasGlobais;           // lista global de salas
vector<int*> ponteirosExemploGlobais;        // exemplo de ponteiros globais
const string nomeArquivoPadraoGlobal = "agendamentos.txt"; // nome padrão de arquivo

// TIPOS COMPOSTOS------------------------------------------------
// Representação alternativa de tempo (com union)
union RepresentacaoTempo {
    int minutos;
    char hhmm[6]; // "HH:MM\0"
    RepresentacaoTempo() { minutos = 0; hhmm[0] = '\0'; }
};

// Estrutura principal de agendamento
struct Agendamento {
    RepresentacaoTempo inicio;
    RepresentacaoTempo fim;
    bool armazenadoComoMinutos; // se true: usamos minutos
    string professor;
    string disciplina;
};

// Nó da lista encadeada
struct NoAgendamento {
    Agendamento ag;
    NoAgendamento* prox;
    NoAgendamento(const Agendamento& a) : ag(a), prox(nullptr) {}
};

// Estrutura da sala com lista encadeada
struct Sala {
    int id;
    NoAgendamento* cabeca;
    Sala(int ident) : id(ident), cabeca(nullptr) {}
};

// FUNÇÕES DE TEMPO-------------------------------------------
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

// FUNÇÕES PRINCIPAIS DE SALAS E AGENDAMENTOS --------------------------
// Cria ou retorna uma sala existente
Sala* ObterOuCriarSalaGlobal(int id) {
    for (Sala* sala : salasGlobais) if (sala->id == id) return sala;
    Sala* novaSala = new Sala(id);
    salasGlobais.push_back(novaSala);
    return novaSala;
}

// Verifica se horário está livre
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

// Insere agendamento ordenado
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

// Lista agendamentos
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

// Libera memória da sala
void LiberarNosDaSala(Sala& sala) {
    NoAgendamento* atual = sala.cabeca;
    while (atual) {
        NoAgendamento* temporario = atual;
        atual = atual->prox;
        delete temporario;
    }
    sala.cabeca = nullptr;
}

// Libera todas as salas
void LiberarTodasAsSalas() {
    for (Sala* sala : salasGlobais) {
        LiberarNosDaSala(*sala);
        delete sala;
    }
    salasGlobais.clear();
}

// FUNÇÕES DE ARQUIVOS
bool SalvarAgendamentos(const string& caminho) {
    ofstream arquivoSaida(caminho);
    if (!arquivoSaida.is_open()) return false;
    for (Sala* sala : salasGlobais) {
        NoAgendamento* atual = sala->cabeca;
        while (atual) {
            arquivoSaida << sala->id << ";"
                << ConverterMinutosParaHHMM(atual->ag.inicio.minutos) << ";"
                << ConverterMinutosParaHHMM(atual->ag.fim.minutos) << ";"
                << atual->ag.professor << ";"
                << atual->ag.disciplina << "\n";
            atual = atual->prox;
        }
    }
    arquivoSaida.close();
    return true;
}

bool CarregarAgendamentos(const string& caminho) {
    ifstream arquivoEntrada(caminho);
    if (!arquivoEntrada.is_open()) return false;
    string linha;
    while (getline(arquivoEntrada, linha)) {
        if (linha.empty()) continue;
        vector<string> partes;
        string token;
        istringstream fluxo(linha);
        while (getline(fluxo, token, ';')) partes.push_back(token);
        if (partes.size() < 5) continue;

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
    }
    arquivoEntrada.close();
    return true;
}

// MENU DE INTERAÇÃO------------------------------------------

void MostrarMenu() {
    cout << "\n=== Sistema de Agendamento de Salas ===\n"
         << "1 - Inserir agendamento\n"
         << "2 - Listar agendamentos de uma sala\n"
         << "3 - Salvar em arquivo\n"
         << "4 - Carregar de arquivo\n"
         << "5 - Listar todas as salas\n"
         << "0 - Sair\n"
         << "Escolha: ";
}

// FUNÇÃO PRINCIPAL-----------------------------------------------------
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
                cout << "Horário inválido!\n"; continue;
            }

            Agendamento agendamento;
            agendamento.inicio.minutos = minutosInicio;
            agendamento.fim.minutos = minutosFim;
            agendamento.professor = professor;
            agendamento.disciplina = disciplina;
            agendamento.armazenadoComoMinutos = true;

            Sala* sala = ObterOuCriarSalaGlobal(idSala);
            if (InserirAgendamentoNaSala(*sala, agendamento)) cout << "Agendamento inserido.\n";
            else cout << "Conflito de horário.\n";

        } else if (opcao == 2) {
            int idSala; cout << "Numero da sala: "; cin >> idSala;
            Sala* salaEncontrada = nullptr;
            for (Sala* sala : salasGlobais) if (sala->id == idSala) salaEncontrada = sala;
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
            cout << "Salas existentes:\n";
            if (salasGlobais.empty()) cout << "  (nenhuma)\n";
            for (Sala* sala : salasGlobais) cout << "  Sala " << sala->id << "\n";

        } else if (opcao == 0) {
            cout << "Encerrando...\n";
        } else {
            cout << "Opção inválida.\n";
        }
    }

    // Libera memória
    LiberarTodasAsSalas();
    for (int* ponteiro : ponteirosExemploGlobais) delete ponteiro;
    ponteirosExemploGlobais.clear();
    return 0;
}
