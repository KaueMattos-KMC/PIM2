#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "CRUD_Diario.h"
#include "CRUD_Turmas.h"     // Necessário para acessar a estrutura Turma
#include "CRUD_Cadastro.h"   // Necessário para acessar a estrutura Registro e listar alunos
#include "CRUD_Atividades.h" // Necessário para listar as atividades

// Assumindo extern das funções essenciais
extern void limpar_buffer();
extern int carregarTurmas(Turma turmas[]);
extern int carregar(Registro lista[], const char *arquivo);
extern int carregarAtividades(Atividade atividades[]);
// Linha removida: extern const char *ARQ_ALUNOS;

// --- Funções Auxiliares de Arquivo ---

// Salvar/Carregar Notas
int carregarNotas(Nota notas[]) {
    FILE *fp = fopen(ARQ_NOTAS, "r");
    if (!fp) return 0;
    int qtd = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), fp) != NULL && qtd < MAX_NOTAS) {
        linha[strcspn(linha, "\r\n")] = 0;
        if (sscanf(linha, "%9[^;];%19[^;];%9[^;];%f",
                   notas[qtd].atividadeID, notas[qtd].alunoRA, notas[qtd].turmaCodigo, &notas[qtd].valorNota) == 4) {
            qtd++;
        }
    }
    fclose(fp);
    return qtd;
}
void salvarNotas(Nota notas[], int qtd) {
    FILE *fp = fopen(ARQ_NOTAS, "w");
    if (!fp) { printf("Erro ao salvar notas.\n"); return; }
    for (int i = 0; i < qtd; i++) {
        fprintf(fp, "%s;%s;%s;%.2f\n",
                notas[i].atividadeID, notas[i].alunoRA, notas[i].turmaCodigo, notas[i].valorNota);
    }
    fclose(fp);
}

// Salvar/Carregar Frequências
int carregarFrequencias(Frequencia frequencias[]) {
    FILE *fp = fopen(ARQ_FREQUENCIAS, "r");
    if (!fp) return 0;
    int qtd = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), fp) != NULL && qtd < MAX_FREQUENCIAS) {
        linha[strcspn(linha, "\r\n")] = 0;
        // %c para ler o status
        if (sscanf(linha, "%9[^;];%10[^;];%19[^;];%c",
                   frequencias[qtd].turmaCodigo, frequencias[qtd].data, frequencias[qtd].alunoRA, &frequencias[qtd].status) == 4) {
            qtd++;
        }
    }
    fclose(fp);
    return qtd;
}
void salvarFrequencias(Frequencia frequencias[], int qtd) {
    FILE *fp = fopen(ARQ_FREQUENCIAS, "w");
    if (!fp) { printf("Erro ao salvar frequências.\n"); return; }
    for (int i = 0; i < qtd; i++) {
        fprintf(fp, "%s;%s;%s;%c\n",
                frequencias[i].turmaCodigo, frequencias[i].data, frequencias[i].alunoRA, frequencias[i].status);
    }
    fclose(fp);
}


// --- Funções do Professor ---

void lancarNota(const char *professorRA) {
    Nota notas[MAX_NOTAS];
    int qtd_notas = carregarNotas(notas);

    Atividade atividades[MAX_ATIVIDADES];
    int qtd_atividades = carregarAtividades(atividades);

    Turma turmas[MAX_TURMAS];
    int qtd_turmas = carregarTurmas(turmas);

    char id_atividade[10], aluno_ra[20];
    int atividade_idx = -1;
    float nota_lida;
    
    // 1. Mostrar Atividades criadas pelo professor
    printf("\n--- Suas Atividades Abertas ---\n");
    for (int i = 0; i < qtd_atividades; i++) {
        if (strcmp(atividades[i].professorRA, professorRA) == 0) {
            printf(" ID: %s | Turma: %s | Matéria: %s | Título: %s\n", 
                   atividades[i].id, atividades[i].turmaCodigo, atividades[i].materia, atividades[i].tipo);
        }
    }
    printf("--------------------------------\n");

    // 2. Coletar dados da nota
 printf("Digite o ID da Atividade: ");
    scanf(" %9[^\n]", id_atividade);
    for (int i = 0; i < qtd_atividades; i++) {
        if (strcmp(atividades[i].id, id_atividade) == 0 &&
            strcmp(atividades[i].professorRA, professorRA) == 0) {
            atividade_idx = i;
            break;
        }
    }
    if (atividade_idx == -1) {
        printf("Erro: Atividade não encontrada ou não é sua.\n");
        return;
    }

    printf("Digite o RA do aluno: ");
    scanf(" %19[^\n]", aluno_ra);
    printf("Digite a Nota (ex: 8.5): ");
    if (scanf("%f", &nota_lida) != 1) {
        printf("Erro: Nota inválida.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    // 3. Verificar se o aluno pertence à turma
    int aluno_pertence = 0;
    const char *cod_turma = atividades[atividade_idx].turmaCodigo;
    for (int k = 0; k < qtd_turmas; k++) {
        if (strcmp(turmas[k].codigo, cod_turma) == 0) {
            for (int l = 0; l < turmas[k].totalAlunos; l++) {
                if (strcmp(turmas[k].alunos[l], aluno_ra) == 0) {
                    aluno_pertence = 1;
                    break;
                }
            }
        }
    }

    if (!aluno_pertence) {
        printf("Erro: Aluno não está matriculado na turma %s.\n", cod_turma);
        return;
    }
    
    // 4. Lançar ou Atualizar Nota
    Nota nova_nota;
    strcpy(nova_nota.atividadeID, id_atividade);
    strcpy(nova_nota.alunoRA, aluno_ra);
    strcpy(nova_nota.turmaCodigo, cod_turma);
    nova_nota.valorNota = nota_lida;
    
    int nota_existente = 0;
    for (int i = 0; i < qtd_notas; i++) {
        if (strcmp(notas[i].alunoRA, aluno_ra) == 0 && strcmp(notas[i].atividadeID, id_atividade) == 0) {
            notas[i] = nova_nota; // Atualiza a nota existente
            nota_existente = 1;
            break;
        }
    }
    
    if (!nota_existente) {
        if (qtd_notas >= MAX_NOTAS) {
            printf("Limite de notas atingido.\n");
            return;
        }
        notas[qtd_notas++] = nova_nota;
    }

    salvarNotas(notas, qtd_notas);
    printf("Nota %.2f lançada/atualizada com sucesso para o aluno %s!\n", nota_lida, aluno_ra);
}

void registrarFrequencia(const char *professorRA) {
    Frequencia frequencias[MAX_FREQUENCIAS];
    int qtd_frequencias = carregarFrequencias(frequencias);
    
    Turma turmas[MAX_TURMAS];
    int qtd_turmas = carregarTurmas(turmas);

    char cod_turma[10], data_chamada[11], status_lido[5];
    int turma_idx = -1;
    
    // 1. Selecionar a Turma do Professor
    printf("\n--- Suas Turmas ---\n");
    for (int i = 0; i < qtd_turmas; i++) {
        if (strcmp(turmas[i].professorRA, professorRA) == 0) {
            printf(" [%s] %s\n", turmas[i].codigo, turmas[i].nome);
        }
    }
    printf("--------------------\n");
    
    printf("Digite o Código da Turma para a Chamada: ");
    scanf(" %9[^\n]", cod_turma);

    for (int i = 0; i < qtd_turmas; i++) {
        if (strcmp(turmas[i].codigo, cod_turma) == 0 &&
            strcmp(turmas[i].professorRA, professorRA) == 0) {
            turma_idx = i;
            break;
        }
    }
    if (turma_idx == -1) {
        printf("Erro: Turma não encontrada ou não é sua.\n");
        return;
    }
    
    printf("Digite a Data da Chamada (DD/MM/AAAA): ");
    scanf(" %10[^\n]", data_chamada);

    // 2. Processar a chamada para cada aluno da turma
    printf("\n-- Chamada da Turma %s em %s --\n", cod_turma, data_chamada);
    
    Turma *t = &turmas[turma_idx];
    
    for (int i = 0; i < t->totalAlunos; i++) {
        char aluno_ra[20];
        strcpy(aluno_ra, t->alunos[i]);
        
        printf("Status de %s (P/F): ", aluno_ra);
        scanf(" %1[^\n]", status_lido);
        
        char status_final = toupper(status_lido[0]);

        if (status_final != 'P' && status_final != 'F') {
            printf("Status inválido. Ignorado.\n");
            continue;
        }
        
        // 3. Registrar ou Atualizar Frequência
        Frequencia nova_frequencia;
        strcpy(nova_frequencia.turmaCodigo, cod_turma);
        strcpy(nova_frequencia.data, data_chamada);
        strcpy(nova_frequencia.alunoRA, aluno_ra);
        nova_frequencia.status = status_final;
        
        int freq_existente = 0;
        for (int j = 0; j < qtd_frequencias; j++) {
            if (strcmp(frequencias[j].alunoRA, aluno_ra) == 0 && 
                strcmp(frequencias[j].data, data_chamada) == 0) {
                frequencias[j] = nova_frequencia; // Atualiza
                freq_existente = 1;
                break;
            }
        }
        
        if (!freq_existente) {
            if (qtd_frequencias >= MAX_FREQUENCIAS) {
                printf("Limite de frequências atingido. Não foi possível registrar %s.\n", aluno_ra);
                continue;
            }
            frequencias[qtd_frequencias++] = nova_frequencia;
        }
    }

    salvarFrequencias(frequencias, qtd_frequencias);
    printf("Registro de frequência concluído para %s!\n", cod_turma);
}

void listarDiarioProfessor(const char *professorRA) {
    printf("\n--- Consulta Rápida: Notas e Frequências ---\n");
    // Esta função pode ser complexa. Para simplicidade, apenas listaremos as turmas do professor.
    
    Nota notas[MAX_NOTAS];
    int qtd_notas = carregarNotas(notas);

    Turma turmas[MAX_TURMAS];
    int qtd_turmas = carregarTurmas(turmas);
    
    printf("\n--- Suas Turmas --- \n");
    int encontrou_turma = 0;
    for(int i = 0; i < qtd_turmas; i++) {
        if(strcmp(turmas[i].professorRA, professorRA) == 0) {
            printf("[%s] %s\n", turmas[i].codigo, turmas[i].nome);
            encontrou_turma = 1;
        }
    }
    
    if(!encontrou_turma) {
        printf("Você não possui turmas cadastradas.\n");
        return;
    }
    
    char cod_turma[10];
    printf("Digite o Código da Turma para Detalhes (exibir notas/faltas): ");
    scanf(" %9[^\n]", cod_turma);

    // Simplificação: apenas exibe todas as notas daquela turma
    printf("\n--- Notas da Turma %s ---\n", cod_turma);
    int encontrou_nota = 0;
    for(int i = 0; i < qtd_notas; i++) {
        if(strcmp(notas[i].turmaCodigo, cod_turma) == 0) {
            printf("Aluno: %s | Atividade ID: %s | Nota: %.2f\n", 
                   notas[i].alunoRA, notas[i].atividadeID, notas[i].valorNota);
            encontrou_nota = 1;
        }
    }
    if(!encontrou_nota) printf("Nenhuma nota lançada nesta turma.\n");

    // Simplificação: apenas exibe a frequência resumida
    Frequencia frequencias[MAX_FREQUENCIAS];
    int qtd_freq = carregarFrequencias(frequencias);
    
    printf("\n--- Resumo de Frequência (Turma %s) ---\n", cod_turma);
    int total_alunos = 0;
    int total_chamadas = 0;
    int total_faltas = 0;
    
    for(int i = 0; i < qtd_turmas; i++) {
        if(strcmp(turmas[i].codigo, cod_turma) == 0) {
            total_alunos = turmas[i].totalAlunos;
            break;
        }
    }
    
    for(int i = 0; i < qtd_freq; i++) {
        if(strcmp(frequencias[i].turmaCodigo, cod_turma) == 0) {
            total_chamadas++;
            if(frequencias[i].status == 'F') {
                total_faltas++;
            }
        }
    }
    
    if(total_alunos > 0) {
        printf("Total de Alunos: %d\n", total_alunos);
        printf("Total de Registros de Frequência: %d\n", total_chamadas);
        // Nota: Um sistema real precisaria agrupar por aluno para calcular faltas por aluno.
    } else {
        printf("Nenhum registro de frequência ou turma encontrada.\n");
    }
}

void menuDiario(const char *professorRA) {
    int opcao;
    do {
        printf("\n--- Diário Eletrônico (Professor) ---\n");
        printf("1. Lançar/Atualizar Nota\n");
        printf("2. Registrar Frequência\n");
        printf("3. Consultar Notas e Frequências\n");
        printf("4. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1: lancarNota(professorRA); break;
            case 2: registrarFrequencia(professorRA); break;
            case 3: listarDiarioProfessor(professorRA); break;
        }
    } while (opcao != 4);
}

// --- Função do Aluno ---

void consultarDesempenho(const char *alunoRA) {
    Nota notas[MAX_NOTAS];
    int qtd_notas = carregarNotas(notas);

    Frequencia frequencias[MAX_FREQUENCIAS];
    int qtd_freq = carregarFrequencias(frequencias);

    printf("\n--- Meu Desempenho (%s) ---\n", alunoRA);

    // 1. Consulta de Notas
    printf("\n[ Notas Lançadas ]\n");
    int encontrou_nota = 0;
    for (int i = 0; i < qtd_notas; i++) {
        if (strcmp(notas[i].alunoRA, alunoRA) == 0) {
            printf(" Turma: %s | Atividade ID: %s | Nota: %.2f\n",
                   notas[i].turmaCodigo, notas[i].atividadeID, notas[i].valorNota);
            encontrou_nota = 1;
        }
    }
    if (!encontrou_nota) {
        printf(" Nenhuma nota encontrada.\n");
    }

    // 2. Consulta de Frequência (Resumo de Faltas)
    printf("\n[ Resumo de Faltas ]\n");
    int total_faltas = 0;
    int total_presencas = 0;

    // Usaremos um resumo simples por turma
    char turmas_ativas[MAX_TURMAS][10];
    int qtd_turmas_ativas = 0;

    for (int i = 0; i < qtd_freq; i++) {
        if (strcmp(frequencias[i].alunoRA, alunoRA) == 0) {
            if (frequencias[i].status == 'F') {
                total_faltas++;
            } else if (frequencias[i].status == 'P') {
                total_presencas++;
            }
            
            // Lógica para registrar turmas ativas (para futura melhoria)
            int turma_nova = 1;
            for(int j = 0; j < qtd_turmas_ativas; j++) {
                if(strcmp(turmas_ativas[j], frequencias[i].turmaCodigo) == 0) {
                    turma_nova = 0;
                    break;
                }
            }
            if (turma_nova) {
                if (qtd_turmas_ativas < MAX_TURMAS) {
                    strcpy(turmas_ativas[qtd_turmas_ativas++], frequencias[i].turmaCodigo);
                }
            }
        }
    }
    
    printf(" Presenças Registradas: %d\n", total_presencas);
    printf(" Total de Faltas: %d\n", total_faltas);
    
    if (total_faltas == 0 && total_presencas == 0) {
        printf(" Nenhum registro de frequência encontrado.\n");
    }
}
