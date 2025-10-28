#ifndef CRUD_ATIVIDADES_H
#define CRUD_ATIVIDADES_H

#define MAX_ATIVIDADES 100
#define ARQ_ATIVIDADES "atividades.txt"

typedef struct {
    char id[10];
    char turmaCodigo[10];
    char professorRA[20];
    // char tipo[30];          // CAMPO REMOVIDO: Tipo (Prova, Trabalho, etc.)
    char materia[50];       // Matéria relacionada (Ex: Matemática, História)
    char descricao[256];    // Conteúdo/Descrição da atividade
    char data[11];          // Data de entrega (DD/MM/AAAA)
} Atividade;

// Protótipos das funções
void salvarAtividades(Atividade atividades[], int qtd);
int carregarAtividades(Atividade atividades[]);

void criarAtividade(Atividade atividades[], int *qtd, const char *professorRA);
void excluirAtividade(Atividade atividades[], int *qtd, const char *professorRA);
void listarAtividadesProfessor(Atividade atividades[], int qtd, const char *professorRA);
void menuAtividades(const char *professorRA);

void listarAtividadesAluno(const char *alunoRA);

#endif// CRUD_ATIVIDADES_H