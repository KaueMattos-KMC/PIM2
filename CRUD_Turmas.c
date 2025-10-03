#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para strcspn
#include "CRUD_Turmas.h"

// Função auxiliar para limpar o buffer de entrada (necessário após scanf("%d"))


void salvarTurmas(Turma turmas[], int qtd) {
    FILE *fp = fopen(ARQ_TURMAS, "w");
    if (!fp) {
        printf("Erro ao abrir arquivo de turmas.\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        // Formato: codigo;nome;professorRA;totalAlunos;RA_aluno1;RA_aluno2;\n
        fprintf(fp, "%s;%s;%s;%d;", 
                turmas[i].codigo, turmas[i].nome,
                turmas[i].professorRA, turmas[i].totalAlunos);
                
        for (int j = 0; j < turmas[i].totalAlunos; j++) {
            fprintf(fp, "%s;", turmas[i].alunos[j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// FUNÇÃO CORRIGIDA: Usa fgets e strtok para leitura robusta de CSV.
int carregarTurmas(Turma turmas[]) {
    FILE *fp = fopen(ARQ_TURMAS, "r");
    if (!fp) return 0;
    
    int qtd = 0;
    char linha[1024]; // Buffer seguro para ler a linha inteira

    while (fgets(linha, sizeof(linha), fp) != NULL && qtd < MAX_TURMAS) {
        // Remove o newline (\n) e carriage return (\r) do final
        linha[strcspn(linha, "\r\n")] = 0; 

        Turma t;
        char *token;
        int campo = 0;

        // 1. Extrai o Código
        token = strtok(linha, ";");
        if (token == NULL) continue;
        strncpy(t.codigo, token, sizeof(t.codigo) - 1);
        t.codigo[sizeof(t.codigo) - 1] = '\0';
        
        // 2. Extrai o Nome
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(t.nome, token, sizeof(t.nome) - 1);
        t.nome[sizeof(t.nome) - 1] = '\0';
        
        // 3. Extrai o professorRA
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        strncpy(t.professorRA, token, sizeof(t.professorRA) - 1);
        t.professorRA[sizeof(t.professorRA) - 1] = '\0';

        // 4. Extrai o totalAlunos
        token = strtok(NULL, ";");
        if (token == NULL) continue;
        t.totalAlunos = atoi(token);

        // 5. Extrai os RAs dos alunos
        int alunos_lidos = 0;
        for (int j = 0; j < t.totalAlunos && j < MAX_ALUNOS_TURMA; j++) {
            token = strtok(NULL, ";");
            if (token == NULL) break;
            strncpy(t.alunos[j], token, sizeof(t.alunos[j]) - 1);
            t.alunos[j][sizeof(t.alunos[j]) - 1] = '\0';
            alunos_lidos++;
        }
        t.totalAlunos = alunos_lidos; // Ajusta caso o arquivo estivesse inconsistente
        
        turmas[qtd++] = t;
    }
    
    fclose(fp);
    return qtd;
}

// FUNÇÃO CORRIGIDA: Adicionado limite de caracteres no scanf para segurança.
void criarTurma(Turma turmas[], int *qtd, const char *professorRA) {
    if (*qtd >= MAX_TURMAS) {
        printf("Limite de turmas atingido.\n");
        return;
    }
    Turma t;
    printf("Código da turma (Max 9): ");
    scanf(" %9[^\n]", t.codigo); // Limite de 9 caracteres
    printf("Nome da turma (Max 49): ");
    scanf(" %49[^\n]", t.nome); // Limite de 49 caracteres
    strcpy(t.professorRA, professorRA);
    t.totalAlunos = 0;
    turmas[*qtd] = t;
    (*qtd)++;
    salvarTurmas(turmas, *qtd);
    printf("Turma criada com sucesso!\n");
}

void listarTurmas(Turma turmas[], int qtd) {
    printf("\n--- Lista de Turmas ---\n");
    for (int i = 0; i < qtd; i++) {
        printf("Código: %s | Nome: %s | Professor: %s | Alunos: %d\n",
               turmas[i].codigo, turmas[i].nome,
               turmas[i].professorRA, turmas[i].totalAlunos);
    }
}

// FUNÇÃO CORRIGIDA: Adicionado limite de caracteres no scanf para segurança.
void adicionarAlunoTurma(Turma turmas[], int qtd) {
    char codigo[10], ra[20];
    printf("Digite o código da turma (Max 9): ");
    scanf(" %9[^\n]", codigo);

    for (int i = 0; i < qtd; i++) {
        if (strcmp(turmas[i].codigo, codigo) == 0) {
            if (turmas[i].totalAlunos >= MAX_ALUNOS_TURMA) {
                printf("Turma cheia.\n");
                return;
            }
            printf("Digite o RA do aluno a adicionar (Max 19): ");
            scanf(" %19[^\n]", ra); // Limite de 19 caracteres
            
            // Verifica se o aluno já está na turma
            for (int k = 0; k < turmas[i].totalAlunos; k++) {
                if (strcmp(turmas[i].alunos[k], ra) == 0) {
                    printf("Erro: Aluno já está nesta turma.\n");
                    return;
                }
            }
            
            strcpy(turmas[i].alunos[turmas[i].totalAlunos], ra);
            turmas[i].totalAlunos++;
            salvarTurmas(turmas, qtd);
            printf("Aluno adicionado com sucesso!\n");
            return;
        }
    }
    printf("Turma não encontrada.\n");
}
// AQUI: ADICIONE A NOVA FUNÇÃO DE ACESSO DO ALUNO
void listarTurmasAluno(const char *alunoRA) {
    Turma turmas[MAX_TURMAS];
    int qtd = carregarTurmas(turmas);
    int encontrou_turma = 0;

    printf("\n--- Minhas Turmas (%s) ---\n", alunoRA);

    for (int i = 0; i < qtd; i++) {
        // Itera sobre a lista de alunos de CADA turma
        for (int j = 0; j < turmas[i].totalAlunos; j++) {
            
            // Verifica se o RA do aluno logado (alunoRA) corresponde a algum RA na turma
            if (strcmp(turmas[i].alunos[j], alunoRA) == 0) {
                
                // Se o aluno for encontrado na turma, exibe a informação
                printf("Código: %s | Nome: %s | Professor: %s\n",
                       turmas[i].codigo, turmas[i].nome,
                       turmas[i].professorRA);
                encontrou_turma = 1;
                break; // Sai do loop interno
            }
        }
    }

    if (!encontrou_turma) {
        printf("Você não está matriculado em nenhuma turma.\n");
    }
}

// FUNÇÃO CORRIGIDA: Adicionada limpeza de buffer.
void menuTurmas(const char *professorRA) {
    Turma turmas[MAX_TURMAS];
    int qtd = carregarTurmas(turmas);
    int opcao;
    do {
        printf("\n--- Menu Turmas ---\n");
        printf("1. Criar Turma\n");
        printf("2. Listar Turmas\n");
        printf("3. Adicionar Aluno a Turma\n");
        printf("4. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer(); // <--- Limpeza de buffer após ler o inteiro

        switch (opcao) {
            case 1: criarTurma(turmas, &qtd, professorRA); break;
            case 2: listarTurmas(turmas, qtd); break;
            case 3: adicionarAlunoTurma(turmas, qtd); break;
        }
    } while (opcao != 4);

}