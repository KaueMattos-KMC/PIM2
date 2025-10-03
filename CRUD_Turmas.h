#ifndef CRUD_TURMAS_H
#define CRUD_TURMAS_H
void limpar_buffer(); 

#define MAX_TURMAS 50
#define MAX_ALUNOS_TURMA 100
#define ARQ_TURMAS "turmas.txt"

typedef struct {
    char codigo[10];
    char nome[50];
    char professorRA[20];
    char alunos[MAX_ALUNOS_TURMA][20]; // Lista de RAs dos alunos
    int totalAlunos;
} Turma;

void salvarTurmas(Turma turmas[], int qtd);
int carregarTurmas(Turma turmas[]);
void criarTurma(Turma turmas[], int *qtd, const char *professorRA);
void listarTurmas(Turma turmas[], int qtd);
void adicionarAlunoTurma(Turma turmas[], int qtd);
void menuTurmas(const char *professorRA);


// NOVO PROTÓTIPO: Lista turmas específicas para um RA de aluno
void listarTurmasAluno(const char *alunoRA);

#endif
