#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "CRUD_Aulas.h"
#include "CRUD_Turmas.h" // Necessário para: Turma, MAX_TURMAS, MAX_ALUNOS_TURMA, carregarTurmas

// Funções de utilidade assumidas (protótipos devem estar em algum .h)
extern void limpar_buffer();




// --- Funções Auxiliares de Data ---

/*
 * Retorna 1 se a data da aula (DD/MM/AAAA) cai na semana atual.
 * Baseia-se no número da semana (tm_wday) do sistema.
 */
int is_this_week(const char *aula_date_str) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    // 1. Calcula o início e fim da semana atual (domingo a sábado)
    // local->tm_wday é o dia da semana (0 = domingo, 6 = sábado)
    
    // Calcula o dia de hoje em segundos
    time_t current_day_time = mktime(local); 
    
    // Calcula o tempo que passou desde o domingo (início da semana)
    time_t sunday_start_time = current_day_time - (local->tm_wday * 24 * 60 * 60);

    // Calcula o tempo até o próximo sábado (fim da semana)
    // 7 - local->tm_wday é o número de dias restantes até o próximo domingo,
    // então usamos (7 - local->tm_wday) * 24 * 60 * 60 para pegar até o final de sábado
    time_t saturday_end_time = sunday_start_time + (7 * 24 * 60 * 60); 

    // 2. Converte a string da data da aula (DD/MM/AAAA) para time_t
    struct tm aula_tm = {0};
    // NOVO FORMATO DE PARSING: DD/MM/AAAA
    if (sscanf(aula_date_str, "%d/%d/%d", 
               &aula_tm.tm_mday, &aula_tm.tm_mon, &aula_tm.tm_year) != 3) {
        return 0; // Erro de formato
    }

    aula_tm.tm_year -= 1900; // Anos são contados a partir de 1900
    aula_tm.tm_mon -= 1;     // Meses são 0 a 11
    
    // Para mktime funcionar corretamente, precisamos inicializar outros campos
    aula_tm.tm_hour = 0;
    aula_tm.tm_min = 0;
    aula_tm.tm_sec = 0;

    time_t aula_time = mktime(&aula_tm);

    // 3. Compara se a data da aula está entre o início do domingo e o fim do sábado
    //  A hora 00:00:00 de domingo até 23:59:59 de sábado
    if (aula_time >= sunday_start_time && aula_time < saturday_end_time) {
        return 1;
    }
    return 0;
}


// --- CRUD Básico ---

void salvarAulas(Aula aulas[], int qtd) {
    FILE *fp = fopen(ARQ_AULAS, "w");
    if (!fp) {
        printf("Erro ao abrir arquivo de aulas.\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        fprintf(fp, "%s;%s;%s;%s;%s;%s\n",
                aulas[i].id, aulas[i].professorRA, aulas[i].turmaCodigo,
                aulas[i].titulo, aulas[i].conteudo, aulas[i].data);
    }
    fclose(fp);
}

int carregarAulas(Aula aulas[]) {
    FILE *fp = fopen(ARQ_AULAS, "r");
    if (!fp) return 0;

    int qtd = 0;
    char linha[512];
    
    while (fgets(linha, sizeof(linha), fp) != NULL && qtd < MAX_AULAS) {
        linha[strcspn(linha, "\r\n")] = 0; 
        
        char *token;
        int campo = 0;
        
        token = strtok(linha, ";");
        while (token != NULL && campo < 6) {
            switch (campo) {
                case 0: strncpy(aulas[qtd].id, token, sizeof(aulas[qtd].id) - 1); aulas[qtd].id[sizeof(aulas[qtd].id) - 1] = '\0'; break;
                case 1: strncpy(aulas[qtd].professorRA, token, sizeof(aulas[qtd].professorRA) - 1); aulas[qtd].professorRA[sizeof(aulas[qtd].professorRA) - 1] = '\0'; break;
                case 2: strncpy(aulas[qtd].turmaCodigo, token, sizeof(aulas[qtd].turmaCodigo) - 1); aulas[qtd].turmaCodigo[sizeof(aulas[qtd].turmaCodigo) - 1] = '\0'; break;
                case 3: strncpy(aulas[qtd].titulo, token, sizeof(aulas[qtd].titulo) - 1); aulas[qtd].titulo[sizeof(aulas[qtd].titulo) - 1] = '\0'; break;
                case 4: strncpy(aulas[qtd].conteudo, token, sizeof(aulas[qtd].conteudo) - 1); aulas[qtd].conteudo[sizeof(aulas[qtd].conteudo) - 1] = '\0'; break;
                case 5: strncpy(aulas[qtd].data, token, sizeof(aulas[qtd].data) - 1); aulas[qtd].data[sizeof(aulas[qtd].data) - 1] = '\0'; break;
            }
            campo++;
            token = strtok(NULL, ";");
        }
        
        if (campo == 6) {
            qtd++;
        }
    }
    
    fclose(fp);
    return qtd;
}

// --- Funções de Criação e Listagem ---

void criarAula(Aula aulas[], int *qtd, const char *professorRA) {
    if (*qtd >= MAX_AULAS) {
        printf("Limite de aulas atingido.\n");
        return;
    }

    Turma turmas[MAX_TURMAS];
    int qtd_turmas = carregarTurmas(turmas);
    
    if (qtd_turmas == 0) {
        printf("Nenhuma turma cadastrada. Crie uma turma primeiro.\n");
        return;
    }

    Aula a;
    strcpy(a.professorRA, professorRA);
    
    // Geração de ID simples (usando o contador atual)
    sprintf(a.id, "A%03d", *qtd + 1);

    char codigo_turma[10];
    int turma_encontrada = 0;
    char buffer_entrada[256]; // Buffer seguro para entrada

    printf("\nDigite '0' para cancelar a operação a qualquer momento.\n"); // AVISO DE CANCELAMENTO

    // Listar turmas do professor para facilitar a escolha
    printf("--- Turmas do Professor %s ---\n", professorRA);
    for (int i = 0; i < qtd_turmas; i++) {
        if (strcmp(turmas[i].professorRA, professorRA) == 0) {
            printf("  [%s] %s\n", turmas[i].codigo, turmas[i].nome);
        }
    }
    printf("-----------------------------------\n");

    // 1. Entrada: Código da Turma
    printf("Código da turma para a aula: ");
    scanf(" %9[^\n]", buffer_entrada);
    limpar_buffer();
    if (strcmp(buffer_entrada, "0") == 0) {
        printf("Criação de aula cancelada.\n");
        return;
    }
    strncpy(codigo_turma, buffer_entrada, sizeof(codigo_turma) - 1);
    codigo_turma[sizeof(codigo_turma) - 1] = '\0';


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
    
    // 2. Entrada: Título da Aula
    printf("Título da Aula (Máx. 49): ");
    scanf(" %49[^\n]", buffer_entrada);
    limpar_buffer();
    if (strcmp(buffer_entrada, "0") == 0) {
        printf("Criação de aula cancelada.\n");
        return;
    }
    strcpy(a.titulo, buffer_entrada);
    
    // 3. Entrada: Conteúdo da Aula
    printf("Conteúdo da Aula (Máx. 255): ");
    scanf(" %255[^\n]", buffer_entrada);
    limpar_buffer();
    if (strcmp(buffer_entrada, "0") == 0) {
        printf("Criação de aula cancelada.\n");
        return;
    }
    strcpy(a.conteudo, buffer_entrada);
    
    // 4. Entrada: Data da Aula
    printf("Data da Aula (Formato DD/MM/AAAA): ");
    scanf(" %10[^\n]", buffer_entrada);
    limpar_buffer();
    if (strcmp(buffer_entrada, "0") == 0) {
        printf("Criação de aula cancelada.\n");
        return;
    }
    strcpy(a.data, buffer_entrada);

    // Se chegou até aqui, salva a aula
    aulas[*qtd] = a;
    (*qtd)++;
    salvarAulas(aulas, *qtd);
    printf("Aula criada com sucesso e enviada para a turma %s!\n", a.turmaCodigo);
}

void listarAulasProfessor(Aula aulas[], int qtd, const char *professorRA) {
    printf("\n--- Minhas Aulas Criadas ---\n");
    int encontrou = 0;
    for (int i = 0; i < qtd; i++) {
        if (strcmp(aulas[i].professorRA, professorRA) == 0) {
            // SAÍDA NO FORMATO DD/MM/AAAA
            printf("ID: %s | Turma: %s | Data: %s | Título: %s\n",
                   aulas[i].id, aulas[i].turmaCodigo, aulas[i].data, aulas[i].titulo);
            encontrou = 1;
        }
    }
    if (!encontrou) {
        printf("Nenhuma aula encontrada.\n");
    }
}

// --- Função Aluno ---

/*
 * Permite ao aluno logado listar todas as aulas da semana atual
 * que foram atribuídas a qualquer turma em que ele esteja matriculado.
 */
void listarAulasAluno(const char *alunoRA) {
    Aula aulas[MAX_AULAS];
    int qtd_aulas = carregarAulas(aulas);

    Turma turmas[MAX_TURMAS];
    int qtd_turmas = carregarTurmas(turmas);
    
    int encontrou_aula_semana = 0;

    printf("\n--- Aulas da Semana Atual (%s) ---\n", alunoRA);

    for (int i = 0; i < qtd_aulas; i++) {
        // 1. Verificar se a aula é desta semana
        if (!is_this_week(aulas[i].data)) {
            continue; // Pula se a aula não for da semana
        }

        // 2. Verificar se o aluno está matriculado na turma desta aula
        const char *codigo_aula = aulas[i].turmaCodigo;
        
        // Iterar sobre TODAS as turmas carregadas
        for (int k = 0; k < qtd_turmas; k++) {
            if (strcmp(turmas[k].codigo, codigo_aula) == 0) {
                // Turma da aula encontrada. Agora, verificar se o aluno está nela
                for (int l = 0; l < turmas[k].totalAlunos; l++) {
                    if (strcmp(turmas[k].alunos[l], alunoRA) == 0) {
                        // Aluno pertence à turma E a aula é desta semana
                        // SAÍDA NO FORMATO DD/MM/AAAA
                        printf("Data: %s | Turma: %s | Título: %s\n",
                               aulas[i].data, aulas[i].turmaCodigo, aulas[i].titulo);
                        printf("    Conteúdo: %s\n", aulas[i].conteudo);
                        encontrou_aula_semana = 1;
                        goto next_aula; // Pula para a próxima aula
                    }
                }
            }
        }
        next_aula:; // Rótulo para o goto
    }

    if (!encontrou_aula_semana) {
        printf("Nenhuma aula agendada para você nesta semana.\n");
    }
}


// --- Menu Professor ---

void menuAulas(const char *professorRA) {
    Aula aulas[MAX_AULAS];
    int qtd = carregarAulas(aulas);
    int opcao;
    
    do {
        printf("\n--- Menu Gerenciamento de Aulas ---\n");
        printf("1. Criar Nova Aula\n");
        printf("2. Listar Minhas Aulas\n");
        // Opções de Edição/Exclusão podem ser adicionadas aqui
        printf("3. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: criarAula(aulas, &qtd, professorRA); break;
            case 2: listarAulasProfessor(aulas, qtd, professorRA); break;
            default:
                if (opcao != 3) {
                    printf("Opção inválida. Tente novamente.\n");
                }
                break;
        }
    } while (opcao != 3);
}