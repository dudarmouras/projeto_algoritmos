#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// VARIÁVEIS GLOBAIS:--------------------------------------

vector<class Sala*> g_salas;           // lista global de salas
vector<int*> g_examplePointers;        // exemplo de ponteiros globais
const string g_defaultFileName = "agendamentos.txt"; // nome padrão de arquivo

// TIPOS COMPOSTOS------------------------------------------------
// Representação alternativa de tempo (com union)
union TimeRep {
    int minutos;
    char hhmm[6]; // "HH:MM\0"
    TimeRep() { minutos = 0; hhmm[0] = '\0'; }
};

// Estrutura principal de agendamento
struct Agendamento {
    TimeRep inicio;
    TimeRep fim;
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
    NoAgendamento* head;
    Sala(int ident) : id(ident), head(nullptr) {}
};

// FUNÇÕES DE TEMPO-------------------------------------------
int ConverterHorarioParaMinutos(const string& hhmm) {
    int h, m; char sep;
    istringstream ss(hhmm);
    if (!(ss >> h >> sep >> m)) return -1;
    if (sep != ':' || h < 0 || h > 23 || m < 0 || m > 59) return -1;
    return h * 60 + m;
}

string ConverterMinutosParaHHMM(int minutos) {
    int h = minutos / 60;
    int m = minutos % 60;
    ostringstream o;
    o << setw(2) << setfill('0') << h << ":" << setw(2) << setfill('0') << m;
    return o.str();
}

bool TemSobreposicao(int s1, int f1, int s2, int f2) {
    return (s1 < f2) && (s2 < f1);
}

// FUNÇÕES PRINCIPAIS DE SALAS E AGENDAMENTOS --------------------------
// Cria ou retorna uma sala existente
Sala* ObterOuCriarSalaGlobal(int id) {
    for (Sala* s : g_salas) if (s->id == id) return s;
    Sala* nova = new Sala(id);
    g_salas.push_back(nova);
    return nova;
}

// Verifica se horário está livre
bool SalaEstaDisponivel(const Sala& sala, const Agendamento& novo) {
    NoAgendamento* cur = sala.head;
    while (cur) {
        int s = cur->ag.inicio.minutos;
        int f = cur->ag.fim.minutos;
        if (TemSobreposicao(s, f, novo.inicio.minutos, novo.fim.minutos))
            return false;
        cur = cur->prox;
    }
    return true;
}

// Insere agendamento ordenado
bool InserirAgendamentoNaSala(Sala& sala, const Agendamento& novo) {
    if (!SalaEstaDisponivel(sala, novo)) return false;

    NoAgendamento* novoNo = new NoAgendamento(novo);

    if (!sala.head || novo.inicio.minutos < sala.head->ag.inicio.minutos) {
        novoNo->prox = sala.head;
        sala.head = novoNo;
        return true;
    }

    NoAgendamento* cur = sala.head;
    while (cur->prox && cur->prox->ag.inicio.minutos <= novo.inicio.minutos)
        cur = cur->prox;

    novoNo->prox = cur->prox;
    cur->prox = novoNo;
    return true;
}

// Lista agendamentos
void ListarAgendamentosDaSala(const Sala& sala) {
    cout << "\nAgendamentos da sala " << sala.id << ":\n";
    if (!sala.head) { cout << "  (vazia)\n"; return; }
    NoAgendamento* cur = sala.head;
    while (cur) {
        cout << "  " << ConverterMinutosParaHHMM(cur->ag.inicio.minutos)
             << " - " << ConverterMinutosParaHHMM(cur->ag.fim.minutos)
             << " | " << cur->ag.disciplina
             << " | Prof: " << cur->ag.professor << "\n";
        cur = cur->prox;
    }
}

// Libera memória da sala
void LiberarNosDaSala(Sala& sala) {
    NoAgendamento* cur = sala.head;
    while (cur) {
        NoAgendamento* tmp = cur;
        cur = cur->prox;
        delete tmp;
    }
    sala.head = nullptr;
}

// Libera todas as salas
void LiberarTodasAsSalas() {
    for (Sala* s : g_salas) {
        LiberarNosDaSala(*s);
        delete s;
    }
    g_salas.clear();
}

// FUNÇÕES DE ARQUIVOS
bool SalvarAgendamentos(const string& caminho) {
    ofstream out(caminho);
    if (!out.is_open()) return false;
    for (Sala* s : g_salas) {
        NoAgendamento* cur = s->head;
        while (cur) {
            out << s->id << ";"
                << ConverterMinutosParaHHMM(cur->ag.inicio.minutos) << ";"
                << ConverterMinutosParaHHMM(cur->ag.fim.minutos) << ";"
                << cur->ag.professor << ";"
                << cur->ag.disciplina << "\n";
            cur = cur->prox;
        }
    }
    out.close();
    return true;
}

bool CarregarAgendamentos(const string& caminho) {
    ifstream in(caminho);
    if (!in.is_open()) return false;
    string linha;
    while (getline(in, linha)) {
        if (linha.empty()) continue;
        vector<string> partes;
        string token;
        istringstream ss(linha);
        while (getline(ss, token, ';')) partes.push_back(token);
        if (partes.size() < 5) continue;

        int salaId = stoi(partes[0]);
        int sMin = ConverterHorarioParaMinutos(partes[1]);
        int fMin = ConverterHorarioParaMinutos(partes[2]);
        string prof = partes[3];
        string disc = partes[4];

        if (sMin < 0 || fMin <= sMin) continue;

        Agendamento ag;
        ag.inicio.minutos = sMin;
        ag.fim.minutos = fMin;
        ag.armazenadoComoMinutos = true;
        ag.professor = prof;
        ag.disciplina = disc;

        Sala* sala = ObterOuCriarSalaGlobal(salaId);
        InserirAgendamentoNaSala(*sala, ag);
    }
    in.close();
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
            int id; string inicioStr, fimStr, professor, disciplina;
            cout << "Numero da sala: "; cin >> id;
            cout << "Horario inicio (HH:MM): "; cin >> inicioStr;
            cout << "Horario fim (HH:MM): "; cin >> fimStr;
            cin.ignore();
            cout << "Professor: "; getline(cin, professor);
            cout << "Disciplina: "; getline(cin, disciplina);

            int sMin = ConverterHorarioParaMinutos(inicioStr);
            int fMin = ConverterHorarioParaMinutos(fimStr);
            if (sMin < 0 || fMin <= sMin) {
                cout << "Horário inválido!\n"; continue;
            }

            Agendamento ag;
            ag.inicio.minutos = sMin;
            ag.fim.minutos = fMin;
            ag.professor = professor;
            ag.disciplina = disciplina;
            ag.armazenadoComoMinutos = true;

            Sala* sala = ObterOuCriarSalaGlobal(id);
            if (InserirAgendamentoNaSala(*sala, ag)) cout << "Agendamento inserido.\n";
            else cout << "Conflito de horário.\n";

        } else if (opcao == 2) {
            int id; cout << "Numero da sala: "; cin >> id;
            Sala* s = nullptr;
            for (Sala* sala : g_salas) if (sala->id == id) s = sala;
            if (!s) cout << "Sala nao encontrada.\n";
            else ListarAgendamentosDaSala(*s);

        } else if (opcao == 3) {
            string caminho;
            cout << "Arquivo para salvar (Enter para default): ";
            cin.ignore();
            getline(cin, caminho);
            if (caminho.empty()) caminho = g_defaultFileName;
            if (SalvarAgendamentos(caminho)) cout << "Salvo em " << caminho << "\n";
            else cout << "Erro ao salvar.\n";

        } else if (opcao == 4) {
            string caminho;
            cout << "Arquivo para carregar (Enter para default): ";
            cin.ignore();
            getline(cin, caminho);
            if (caminho.empty()) caminho = g_defaultFileName;
            if (CarregarAgendamentos(caminho)) cout << "Carregado com sucesso.\n";
            else cout << "Erro ao carregar.\n";

        } else if (opcao == 5) {
            cout << "Salas existentes:\n";
            if (g_salas.empty()) cout << "  (nenhuma)\n";
            for (Sala* s : g_salas) cout << "  Sala " << s->id << "\n";

        } else if (opcao == 0) {
            cout << "Encerrando...\n";
        } else {
            cout << "Opção inválida.\n";
        }
    }

    // Libera memória
    LiberarTodasAsSalas();
    for (int* p : g_examplePointers) delete p;
    g_examplePointers.clear();
    return 0;
}
