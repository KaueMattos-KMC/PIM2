#ifndef CRUD_DIARIO_H
#define CRUD_DIARIO_H

#define MAX_NOTAS 500
#define MAX_FREQUENCIAS 500
#define ARQ_NOTAS "notas.txt"
#define ARQ_FREQUENCIAS "frequencias.txt"

typedef struct {
    char atividadeID[10];
    char alunoRA[20];
    char turmaCodigo[10];
    float valorNota;
} Nota;

typedef struct {
    char turmaCodigo[10];
    char data[11]; // DD/MM/AAAA
    char alunoRA[20];
    char status;   // 'P' para Presente, 'F' para Falta
} Frequencia;

// Protótipos de Funções Auxiliares (Salvar/Carregar)
int carregarNotas(Nota notas[]);
void salvarNotas(Nota notas[], int qtd);
int carregarFrequencias(Frequencia frequencias[]);
void salvarFrequencias(Frequencia frequencias[], int qtd);

// Protótipos de Funções do Professor
void lancarNota(const char *professorRA);
void registrarFrequencia(const char *professorRA);
void listarDiarioProfessor(const char *professorRA);
void menuDiario(const char *professorRA);

// Protótipos de Funções do Aluno
void consultarDesempenho(const char *alunoRA);

#endif 
