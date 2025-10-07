#ifndef CRUD_AULAS_H
#define CRUD_AULAS_H

#define MAX_AULAS 100
#define ARQ_AULAS "aulas.txt"

// Estrutura da Aula
typedef struct {
    char id[10];            // Identificador único da aula
    char professorRA[20];
    char turmaCodigo[10];
    char titulo[50];
    char conteudo[256];
    char data[11];          // Formato: YYYY-MM-DD
} Aula;

// Protótipos das funções de gerenciamento de aulas
void salvarAulas(Aula aulas[], int qtd);
int carregarAulas(Aula aulas[]);
void menuAulas(const char *professorRA);

// Funções específicas:
void criarAula(Aula aulas[], int *qtd, const char *professorRA);
void listarAulasProfessor(Aula aulas[], int qtd, const char *professorRA);

// Função para o aluno
void listarAulasAluno(const char *alunoRA);

#endif // CRUD_AULAS_H
