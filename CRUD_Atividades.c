#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CRUD_Atividades.h"
#include "CRUD_Turmas.h" // Necessário para acessar a estrutura Turma
#include <ctype.h> // Necessário para strcspn

// Funções de utilidade e constantes assumidas do sistema
extern void limpar_buffer();
extern int carregarTurmas(Turma turmas[]); // Garantindo acesso a esta função


// --- Funções Auxiliares de Arquivo ---

void salvarAtividades(Atividade atividades[], int qtd) {
    FILE *fp = fopen(ARQ_ATIVIDADES, "w");
    if (!fp) {
        printf("Erro ao abrir arquivo de atividades.\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        // Agora salva 7 campos (incluindo a data)
        fprintf(fp, "%s;%s;%s;%s;%s;%s;%s\n", 
                atividades[i].id, atividades[i].professorRA, atividades[i].turmaCodigo,
                atividades[i].tipo, atividades[i].materia, atividades[i].descricao, atividades[i].data);
    }
    fclose(fp);
}

int carregarAtividades(Atividade atividades[]) {
    FILE *fp = fopen(ARQ_ATIVIDADES, "r");
    if (!fp) return 0;

    int qtd = 0;
    char linha[512];
    
    // A constante MAX_ATIVIDADES deve ser definida em CRUD_Atividades.h
    while (fgets(linha, sizeof(linha), fp) != NULL && qtd < MAX_ATIVIDADES) {
        // Remove caracteres de nova linha e retorno de carro
        linha[strcspn(linha, "\r\n")] = 0; 
        
        char *token;
        int campo = 0;
        
        token = strtok(linha, ";");
        // A leitura espera 7 campos
        while (token != NULL && campo < 7) { 
            switch (campo) {
                case 0: strncpy(atividades[qtd].id, token, sizeof(atividades[qtd].id) - 1); atividades[qtd].id[sizeof(atividades[qtd].id) - 1] = '\0'; break;
                case 1: strncpy(atividades[qtd].professorRA, token, sizeof(atividades[qtd].professorRA) - 1); atividades[qtd].professorRA[sizeof(atividades[qtd].professorRA) - 1] = '\0'; break;
                case 2: strncpy(atividades[qtd].turmaCodigo, token, sizeof(atividades[qtd].turmaCodigo) - 1); atividades[qtd].turmaCodigo[sizeof(atividades[qtd].turmaCodigo) - 1] = '\0'; break;
                case 3: strncpy(atividades[qtd].tipo, token, sizeof(atividades[qtd].tipo) - 1); atividades[qtd].tipo[sizeof(atividades[qtd].tipo) - 1] = '\0'; break;
                case 4: strncpy(atividades[qtd].materia, token, sizeof(atividades[qtd].materia) - 1); atividades[qtd].materia[sizeof(atividades[qtd].materia) - 1] = '\0'; break;
                case 5: strncpy(atividades[qtd].descricao, token, sizeof(atividades[qtd].descricao) - 1); atividades[qtd].descricao[sizeof(atividades[qtd].descricao) - 1] = '\0'; break;
                case 6: strncpy(atividades[qtd].data, token, sizeof(atividades[qtd].data) - 1); atividades[qtd].data[sizeof(atividades[qtd].data) - 1] = '\0'; break; // Campo data
            }
            campo++;
            token = strtok(NULL, ";");
        }
        
        if (campo == 7) { // Espera 7 campos
            qtd++;
        }
    }
    
    fclose(fp);
    return qtd;
}

// --- Funções do Professor ---

void criarAtividade(Atividade atividades[], int *qtd, const char *professorRA) {
    if (*qtd >= MAX_ATIVIDADES) {
        printf("Limite de atividades atingido.\n");
        return;
    }

    // A constante MAX_TURMAS deve ser definida em CRUD_Turmas.h
    Turma turmas[MAX_TURMAS];
    int qtd_turmas = carregarTurmas(turmas);
    
    if (qtd_turmas == 0) {
        printf("Nenhuma turma cadastrada. Crie uma turma primeiro.\n");
        return;
    }

    Atividade a;
    strcpy(a.professorRA, professorRA);
    
    // Geração de ID simples
    int next_id = *qtd + 1;
    if (next_id > 99999999) {
        printf("Aviso: Limite de IDs excedido.\n");
        return;
    }
    if (next_id < 1000) { 
        sprintf(a.id, "T%03d", next_id); // Usando 'T' para Tarefa/Trabalho
    } else {
        sprintf(a.id, "T%d", next_id); 
    }

    char codigo_turma[10];
    int turma_encontrada = 0;

    // Listar turmas do professor para facilitar a escolha
    printf("\n--- Suas Turmas ---\n");
    for (int i = 0; i < qtd_turmas; i++) {
        if (strcmp(turmas[i].professorRA, professorRA) == 0) {
            printf("  [%s] %s\n", turmas[i].codigo, turmas[i].nome);
        }
    }
    printf("--------------------\n");

    printf("Código da turma para a atividade: ");
    scanf(" %9[^\n]", codigo_turma);

    // Validação de que a turma existe e pertence ao professor
    for (int i = 0; i < qtd_turmas; i++) {
        if (strcmp(turmas[i].codigo, codigo_turma) == 0 &&
            strcmp(turmas[i].professorRA, professorRA) == 0) {
            turma_encontrada = 1;
            break;
        }
    }

    if (!turma_encontrada) {
        printf("Erro: Turma não encontrada ou não pertence a você.\n");
        return;
    }

    strcpy(a.turmaCodigo, codigo_turma);
    
    printf("Tipo da Atividade (Max 29): ");
    scanf(" %29[^\n]", a.tipo);
    printf("Matéria (Max 49): ");
    scanf(" %49[^\n]", a.materia);
    printf("Descrição (Max 255): ");
    scanf(" %255[^\n]", a.descricao);
    
    // NOVO: Solicita a data de entrega
    printf("Data de Entrega (DD/MM/AAAA): ");
    scanf(" %10[^\n]", a.data);

    atividades[*qtd] = a;
    (*qtd)++;
    salvarAtividades(atividades, *qtd);
    
    // Melhoria de usabilidade: Exibe o ID gerado para que o professor possa excluí-lo
    printf("Atividade criada com sucesso (ID: %s) e enviada para a turma %s!\n", a.id, a.turmaCodigo);
}

void excluirAtividade(Atividade atividades[], int *qtd, const char *professorRA) {
    char id[10];
    printf("Digite o ID da atividade a excluir: ");
    scanf(" %9[^\n]", id);

    int pos = -1;
    for (int i = 0; i < *qtd; i++) {
        // Verifica se o ID corresponde E se a atividade pertence ao professor logado
        if (strcmp(atividades[i].id, id) == 0 &&
            strcmp(atividades[i].professorRA, professorRA) == 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        printf("Atividade não encontrada ou você não tem permissão para excluí-la.\n");
        return;
    }

    // Move os itens subsequentes para trás para preencher o buraco
    for (int i = pos; i < *qtd - 1; i++) {
        atividades[i] = atividades[i + 1];
    }
    (*qtd)--;
    salvarAtividades(atividades, *qtd);
    printf("Atividade removida com sucesso!\n");
}


void listarAtividadesProfessor(Atividade atividades[], int qtd, const char *professorRA) {
    printf("\n--- Minhas Atividades ---\n");
    int encontrou = 0;
    for (int i = 0; i < qtd; i++) {
        if (strcmp(atividades[i].professorRA, professorRA) == 0) {
            // Exibindo a Data
            printf("ID: %s | Data: %s | Matéria: %s | Tipo: %s\n",
                   atividades[i].id, atividades[i].data, atividades[i].materia, atividades[i].tipo);
            printf("  Turma: %s | Descrição: %s\n", atividades[i].turmaCodigo, atividades[i].descricao);
            encontrou = 1;
        }
    }
    if (!encontrou) {
        printf("Nenhuma atividade encontrada.\n");
    }
}

// --- Menu de Atividades do Professor ---

void menuAtividades(const char *professorRA) {
    Atividade atividades[MAX_ATIVIDADES];
    int qtd = carregarAtividades(atividades);
    int opcao;
    
    do {
        printf("\n--- Gerenciamento de Atividades ---\n");
        printf("1. Criar Nova Atividade\n");
        printf("2. Listar Minhas Atividades\n");
        printf("3. Excluir Atividade\n");
        printf("4. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: criarAtividade(atividades, &qtd, professorRA); break;
            case 2: listarAtividadesProfessor(atividades, qtd, professorRA); break;
            case 3: excluirAtividade(atividades, &qtd, professorRA); break;
        }
    } while (opcao != 4);
}

// --- Função do Aluno ---

void listarAtividadesAluno(const char *alunoRA) {
    Atividade atividades[MAX_ATIVIDADES];
    int qtd_atividades = carregarAtividades(atividades);

    Turma turmas[MAX_TURMAS];
    int qtd_turmas = carregarTurmas(turmas);
    
    int encontrou_atividade = 0;

    printf("\n--- Minhas Atividades (%s) ---\n", alunoRA);

    // Iterar sobre todas as atividades
    for (int i = 0; i < qtd_atividades; i++) {
        const char *codigo_atividade = atividades[i].turmaCodigo;
        
        // Iterar sobre TODAS as turmas carregadas
        for (int k = 0; k < qtd_turmas; k++) {
            if (strcmp(turmas[k].codigo, codigo_atividade) == 0) {
                // Turma da atividade encontrada. Agora, verificar se o aluno está nela
                for (int l = 0; l < turmas[k].totalAlunos; l++) {
                    if (strcmp(turmas[k].alunos[l], alunoRA) == 0) {
                        // Aluno pertence à turma
                        // Exibindo a Data
                        printf("ID: %s | Data: %s | Matéria: %s | Tipo: %s\n",
                               atividades[i].id, atividades[i].data, atividades[i].materia, atividades[i].tipo);
                        printf("  Turma: %s | Descrição: %s\n", atividades[i].turmaCodigo, atividades[i].descricao);
                        encontrou_atividade = 1;
                        goto next_atividade; // Pula para a próxima atividade
                    }
                }
            }
        }
        next_atividade:; // Rótulo para o goto
    }

    if (!encontrou_atividade) {
        printf("Nenhuma atividade encontrada para você.\n");
    }
}
