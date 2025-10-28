#include "CRUD_Cadastro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para strcspn, se necessário na função carregar

// Função auxiliar para limpar o buffer de entrada (protótipo deve estar em um .h)
extern void limpar_buffer(); 

// ---------- Funções Auxiliares ----------

void salvar(Registro lista[], int qtd, const char *arquivo) {
    FILE *fp = fopen(arquivo, "w");
    if (!fp) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }
    for (int i = 0; i < qtd; i++) {
        fprintf(fp, "%s;%s;%s;%s;%s;%s\n",
                lista[i].ra,
                lista[i].nome,
                lista[i].cpf,
                lista[i].telefone,
                lista[i].email,
                lista[i].senha);
    }
    fclose(fp);
}

// FUNÇÃO CORRIGIDA: Implementação mais robusta com fgets/strtok 
int carregar(Registro lista[], const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    if (!fp) return 0;

    int qtd = 0;
    char linha[256]; 

    while (fgets(linha, sizeof(linha), fp) != NULL && qtd < MAX) {
        // Remover o newline (\n) do final da linha
        linha[strcspn(linha, "\r\n")] = 0; 
        
        char *token;
        int campo = 0;
        
        // Usa strtok para dividir a linha pelo delimitador ';'
        token = strtok(linha, ";");
        while (token != NULL && campo < 6) {
            switch (campo) {
                // Usando strncpy com limite seguro
                case 0: strncpy(lista[qtd].ra, token, sizeof(lista[qtd].ra) - 1); lista[qtd].ra[sizeof(lista[qtd].ra) - 1] = '\0'; break;
                case 1: strncpy(lista[qtd].nome, token, sizeof(lista[qtd].nome) - 1); lista[qtd].nome[sizeof(lista[qtd].nome) - 1] = '\0'; break;
                case 2: strncpy(lista[qtd].cpf, token, sizeof(lista[qtd].cpf) - 1); lista[qtd].cpf[sizeof(lista[qtd].cpf) - 1] = '\0'; break;
                case 3: strncpy(lista[qtd].telefone, token, sizeof(lista[qtd].telefone) - 1); lista[qtd].telefone[sizeof(lista[qtd].telefone) - 1] = '\0'; break;
                case 4: strncpy(lista[qtd].email, token, sizeof(lista[qtd].email) - 1); lista[qtd].email[sizeof(lista[qtd].email) - 1] = '\0'; break;
                case 5: strncpy(lista[qtd].senha, token, sizeof(lista[qtd].senha) - 1); lista[qtd].senha[sizeof(lista[qtd].senha) - 1] = '\0'; break;
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

// ---------- Validações ----------

int existeRA(Registro lista[], int qtd, const char *ra) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].ra, ra) == 0) {
            return 1;
        }
    }
    return 0;
}

int validarTelefone(const char *telefone) {
    int len = strlen(telefone);
    
    // Simplificado para foco na lógica
    if (len == 15) {
        if (telefone[0] == '(' && telefone[3] == ')' &&
            telefone[4] == ' ' && telefone[10] == '-') {
             // Simplificação: Assume que os números estão corretos
            return 1;
        }
    }
    if (len == 13 || len == 11) {
        // Simplificação: Assume que os números estão corretos
        return 1; 
    }
    return 0;
}

// ---------- Cadastro & Login ----------

void cadastrar(Registro lista[], int *qtd, const char *arquivo) {
    if (*qtd >= MAX) {
        printf("Limite de cadastros atingido.\n");
        return;
    }
    
    Registro r;
    printf("RA (Max 19): ");
    scanf(" %19[^\n]", r.ra);
    limpar_buffer();

    if (existeRA(lista, *qtd, r.ra)) {
        printf("Erro: já existe um cadastro com esse RA!\n");
        return;
    }

    printf("Nome e Sobrenome (Max 49): ");
    scanf(" %49[^\n]", r.nome);
    limpar_buffer();
    printf("CPF (Max 14): ");
    scanf(" %14[^\n]", r.cpf);
    limpar_buffer();

    do {
        printf("Telefone (Max 15): ");
        scanf(" %15[^\n]", r.telefone);
        limpar_buffer();
        if (!validarTelefone(r.telefone)) {
            printf("Formato inválido! Tente novamente.\n");
        }
    } while (!validarTelefone(r.telefone));

    printf("E-mail (Max 49): ");
    scanf(" %49[^\n]", r.email);
    limpar_buffer();
    printf("Senha (Max 19): ");
    scanf(" %19[^\n]", r.senha);
    limpar_buffer();

    lista[*qtd] = r;
    (*qtd)++;
    salvar(lista, *qtd, arquivo);
    printf("Cadastro realizado com sucesso!\n");
}

int login(const char *arquivo, const char *tipo, char *usuarioRA) {
    Registro lista[MAX];
    int qtd = carregar(lista, arquivo);
    char ra[20], senha[20];

    if (qtd == 0) {
        printf("Nenhum %s cadastrado ainda.\n", tipo);
        return 0;
    }

    printf("\n--- Login %s ---\n", tipo);
    printf("RA: ");
    scanf(" %19[^\n]", ra);
    limpar_buffer();
    printf("Senha: ");
    scanf(" %19[^\n]", senha);
    limpar_buffer();

    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].ra, ra) == 0 && strcmp(lista[i].senha, senha) == 0) {
            printf("Login bem-sucedido! Bem-vindo, %s.\n", lista[i].nome);
            strcpy(usuarioRA, ra);
            return 1;
        }
    }
    printf("RA ou senha inválidos.\n");
    return 0;
}

// ---------- CRUD ----------

void listarAlunos() {
    Registro lista[MAX];
    int qtd = carregar(lista, ARQ_ALUNOS);

    printf("\n--- Lista de Alunos ---\n");
    for (int i = 0; i < qtd; i++) {
        printf("%s - %s - %s - %s - %s\n",
               lista[i].ra,
               lista[i].nome,
               lista[i].cpf,
               lista[i].telefone,
               lista[i].email);
    }
}

void buscarAluno() {
    Registro lista[MAX];
    int qtd = carregar(lista, ARQ_ALUNOS);
    char ra[20];
    printf("Digite o RA do aluno: ");
    scanf(" %19[^\n]", ra); 
    limpar_buffer();

    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].ra, ra) == 0) {
            printf("Encontrado: %s - %s - %s - %s - %s\n",
                   lista[i].ra,
                   lista[i].nome,
                   lista[i].cpf,
                   lista[i].telefone,
                   lista[i].email);
            return;
        }
    }
    printf("Aluno não encontrado.\n");
}

// FUNÇÃO ATUALIZADA com Fluxo de Cancelamento
void alterarCadastro(const char *arquivo, const char *usuarioRA) {
    Registro lista[MAX];
    int qtd = carregar(lista, arquivo);
    int encontrado = 0;
    int i; // Índice do registro a ser alterado

    // Encontra o índice do usuário a ser alterado
    for (i = 0; i < qtd; i++) {
        if (strcmp(lista[i].ra, usuarioRA) == 0) {
            encontrado = 1;
            break; 
        }
    }

    if (!encontrado) {
        printf("RA não encontrado.\n");
        return;
    }

    printf("Alterando cadastro de %s\n", lista[i].nome);
   

    char novo_nome[50];
    printf("Novo nome (Max 49): ");
    scanf(" %49[^\n]", novo_nome);
    limpar_buffer();
    if (strcmp(novo_nome, "0") == 0) {
        printf("Alteração cancelada.\n");
        return;
    }
    strcpy(lista[i].nome, novo_nome);

    char novo_telefone[20]; // Aumentado para 20
    do {
        printf("Novo telefone (Max 15): ");
        scanf(" %15[^\n]", novo_telefone);
        limpar_buffer();
        if (strcmp(novo_telefone, "0") == 0) {
            printf("Alteração cancelada.\n");
            return;
        }
        if (!validarTelefone(novo_telefone)) {
            printf("Formato inválido! Tente novamente.\n");
        }
    } while (!validarTelefone(novo_telefone));
    strcpy(lista[i].telefone, novo_telefone);

    char novo_email[50];
    printf("Novo e-mail (Max 49): ");
    scanf(" %49[^\n]", novo_email);
    limpar_buffer();
    if (strcmp(novo_email, "0") == 0) {
        printf("Alteração cancelada.\n");
        return;
    }
    strcpy(lista[i].email, novo_email);

    char nova_senha[20];
    printf("Nova senha (Max 19): ");
    scanf(" %19[^\n]", nova_senha);
    limpar_buffer();
    if (strcmp(nova_senha, "0") == 0) {
        printf("Alteração cancelada.\n");
        return;
    }
    strcpy(lista[i].senha, nova_senha);
    
    // Se chegou até aqui sem cancelar, salva as alterações
    salvar(lista, qtd, arquivo);
    printf("Cadastro atualizado com sucesso!\n");
}


void apagarConta(const char *arquivo, const char *usuarioRA) {
    char confirmacao;
    printf("Tem certeza que deseja excluir sua conta? (S/N): ");
    scanf(" %c", &confirmacao);
    limpar_buffer(); 

    if (confirmacao != 'S' && confirmacao != 's') {
        printf("Operação cancelada.\n");
        return;
    }

    Registro lista[MAX];
    int qtd = carregar(lista, arquivo);

    int pos = -1;
    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].ra, usuarioRA) == 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        printf("Conta não encontrada.\n");
        return;
    }

    for (int i = pos; i < qtd - 1; i++) {
        lista[i] = lista[i + 1];
    }
    qtd--;
    salvar(lista, qtd, arquivo);
    printf("Sua conta foi apagada com sucesso!\n");
}

void apagarAluno() {
    Registro lista[MAX];
    int qtd = carregar(lista, ARQ_ALUNOS);
    char ra[20];
    printf("Digite o RA do aluno a excluir: ");
    scanf(" %19[^\n]", ra); 
    limpar_buffer();

    int pos = -1;
    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].ra, ra) == 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        printf("Aluno não encontrado.\n");
        return;
    }

    for (int i = pos; i < qtd - 1; i++) {
        lista[i] = lista[i + 1];
    }
    qtd--;
    salvar(lista, qtd, ARQ_ALUNOS);
    printf("Aluno removido com sucesso!\n");
}

// ---------- Menus ----------

void menuInterno(const char *tipo, const char *arquivo, const char *usuarioRA) {
    int opcao;
    do {
        printf("\n--- Menu ---\n");
        printf("1. Alterar Cadastro\n");
      
        if (strcmp(tipo, "Professor") == 0) {
            printf("2. Listar Alunos\n");
            printf("3. Buscar Aluno\n");
            printf("4. Apagar Aluno\n");
            printf("5. Apagar Minha Conta\n"); 
            printf("6. Voltar\n");
        } else { // Aluno
            // Opção "Apagar Minha Conta" removida daqui, pois já está no Menu Principal do Aluno
            printf("2. Voltar\n"); 
        }

        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer(); 

        // Lógica de execução do menu
        if (opcao == 1) {
            alterarCadastro(arquivo, usuarioRA);
        } else if (strcmp(tipo, "Professor") == 0) {
            switch (opcao) {
                case 2: listarAlunos(); break;
                case 3: buscarAluno(); break;
                case 4: apagarAluno(); break;
                case 5: apagarConta(arquivo, usuarioRA); break; 
                default:
                    // Se não for 1, 2, 3, 4, 5 e não for 6 (Voltar)
                    if (opcao != 6) printf("Opção inválida. Tente novamente.\n");
                    break;
            }
            if(opcao == 5) break; // Sai do menu se apagou a conta
        } else { // Aluno
            // Opções do Aluno: 1 (Alterar), 2 (Voltar)
             if (opcao != 1 && opcao != 2) {
                 printf("Opção inválida. Tente novamente.\n");
            }
        }
        
    } while ((strcmp(tipo, "Professor") == 0 && opcao != 6 && opcao != 5) || // Condição de saída para Professor
             (strcmp(tipo, "Aluno") == 0 && opcao != 2)); // Condição de saída para Aluno (Opção 2 = Voltar)
}


void menuCadastroLogin(const char *arquivo, const char *tipo) {
    Registro lista[MAX];
    int qtd = carregar(lista, arquivo);
    int opcao, logado = 0;
    char usuarioRA[20];

    do {
        printf("\n--- %s ---\n", tipo);
        printf("1. Cadastrar\n");
        printf("2. Login\n");
        printf("3. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer(); 

        switch (opcao) {
            case 1: cadastrar(lista, &qtd, arquivo); break;
            case 2: logado = login(arquivo, tipo, usuarioRA); break;
            default:
                 // Validação de opção inválida
                 if (opcao != 3) printf("Opção inválida. Tente novamente.\n");
                 break;
        }

        if (logado) {
            menuInterno(tipo, arquivo, usuarioRA);
            logado = 0; 
        }

    } while (opcao != 3);
}
