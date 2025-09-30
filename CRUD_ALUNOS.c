


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX 100
#define ARQ_ALUNOS "alunos.txt"
#define ARQ_PROFESSORES "professores.txt"

typedef struct {
    char ra[20];
    char nome[50];
    char cpf[20];
    char telefone[20];
    char email[50];
    char senha[20];
} Registro;

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

int carregar(Registro lista[], const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    int qtd = 0;
    if (!fp) return 0;
    while (fscanf(fp, "%19[^;];%49[^;];%19[^;];%19[^;];%49[^;];%19[^\n]\n",
                  lista[qtd].ra,
                  lista[qtd].nome,
                  lista[qtd].cpf,
                  lista[qtd].telefone,
                  lista[qtd].email,
                  lista[qtd].senha) == 6) {
        qtd++;
    }
    fclose(fp);
    return qtd;
}

// ---------- Validações ----------

int existeRA(Registro lista[], int qtd, const char *ra) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].ra, ra) == 0) {
            return 1; // RA já existe
        }
    }
    return 0;
}

int validarTelefone(const char *telefone) {
    int len = strlen(telefone);

    // Caso 1: (99) 99999-9999  -> total 15 caracteres
    if (len == 15) {
        if (telefone[0] == '(' && telefone[3] == ')' &&
            telefone[4] == ' ' && telefone[10] == '-') {
            for (int i = 1; i <= 2; i++) if (telefone[i] < '0' || telefone[i] > '9') return 0;
            for (int i = 5; i <= 9; i++) if (telefone[i] < '0' || telefone[i] > '9') return 0;
            for (int i = 11; i <= 14; i++) if (telefone[i] < '0' || telefone[i] > '9') return 0;
            return 1;
        }
    }

    // Caso 2: (99)999999999  -> total 13 caracteres
    if (len == 13) {
        if (telefone[0] == '(' && telefone[3] == ')') {
            for (int i = 1; i <= 2; i++) if (telefone[i] < '0' || telefone[i] > '9') return 0;
            for (int i = 4; i <= 12; i++) if (telefone[i] < '0' || telefone[i] > '9') return 0;
            return 1;
        }
    }

    // Caso 3: 11999999999  -> total 11 caracteres (só números)
    if (len == 11) {
        for (int i = 0; i < 11; i++) {
            if (telefone[i] < '0' || telefone[i] > '9') return 0;
        }
        return 1;
    }

    return 0; // qualquer outro formato é inválido
}

// ---------- Cadastro & Login ----------

void cadastrar(Registro lista[], int *qtd, const char *arquivo) {
    Registro r;
    printf("RA: ");
    scanf(" %[^\n]", r.ra);

    if (existeRA(lista, *qtd, r.ra)) {
        printf("Erro: já existe um cadastro com esse RA!\n");
        return;
    }

    printf("Nome e Sobrenome: ");
    scanf(" %[^\n]", r.nome);
    printf("CPF: ");
    scanf(" %[^\n]", r.cpf);

    do {
        printf("Telefone: ");
        scanf(" %[^\n]", r.telefone);
        if (!validarTelefone(r.telefone)) {
            printf("Formato inválido! Tente novamente.\n");
        }
    } while (!validarTelefone(r.telefone));

    printf("E-mail: ");
    scanf(" %[^\n]", r.email);
    printf("Senha: ");
    scanf(" %[^\n]", r.senha);

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
    scanf(" %[^\n]", ra);
    printf("Senha: ");
    scanf(" %[^\n]", senha);

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
    scanf(" %[^\n]", ra);

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

void alterarCadastro(const char *arquivo, const char *usuarioRA) {
    Registro lista[MAX];
    int qtd = carregar(lista, arquivo);
    int encontrado = 0;

    for (int i = 0; i < qtd; i++) {
        if (strcmp(lista[i].ra, usuarioRA) == 0) {
            printf("Alterando cadastro de %s\n", lista[i].nome);
            printf("Novo nome: ");
            scanf(" %[^\n]", lista[i].nome);
            do {
                printf("Novo telefone (formato (99) 99999-9999): ");
                scanf(" %[^\n]", lista[i].telefone);
                if (!validarTelefone(lista[i].telefone)) {
                    printf("Formato inválido! Tente novamente.\n");
                }
            } while (!validarTelefone(lista[i].telefone));
            printf("Novo e-mail: ");
            scanf(" %[^\n]", lista[i].email);
            printf("Nova senha: ");
            scanf(" %[^\n]", lista[i].senha);
            salvar(lista, qtd, arquivo);
            printf("Cadastro atualizado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) printf("RA não encontrado.\n");
}

void apagarConta(const char *arquivo, const char *usuarioRA) {
    char confirmacao;
    printf("Tem certeza que deseja excluir sua conta? (S/N): ");
    scanf(" %c", &confirmacao);

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
    scanf(" %[^\n]", ra);

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
        printf("\n--- Menu %s ---\n", tipo);
        printf("1. Alterar Cadastro\n");
     
        if (strcmp(tipo, "Professor") == 0) {
            printf("2. Listar Alunos\n");
            printf("3. Buscar Aluno\n");
            printf("4. Apagar Aluno\n");
            printf("5. Apagar Minha Conta\n");
            printf("6. Voltar\n");
        } else {
            printf("3. Voltar\n");
        }

        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            alterarCadastro(arquivo, usuarioRA);
        } else if (opcao == 2) {
            apagarConta(arquivo, usuarioRA);
            break;
        } else if (strcmp(tipo, "Professor") == 0) {
            if (opcao == 3) listarAlunos();
            else if (opcao == 4) buscarAluno();
            else if (opcao == 5) apagarAluno();
        }
    } while ((strcmp(tipo, "Professor") == 0 && opcao != 6) ||
             (strcmp(tipo, "Aluno") == 0 && opcao != 3));
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

        switch (opcao) {
            case 1: cadastrar(lista, &qtd, arquivo); break;
            case 2: logado = login(arquivo, tipo, usuarioRA); break;
        }

        if (logado) {
            menuInterno(tipo, arquivo, usuarioRA);
            logado = 0;
        }

    } while (opcao != 3);
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    int opcao;
    do {
        printf("\n=== Sistema Escolar ===\n");
        printf("1. Aluno\n");
        printf("2. Professor\n");
        printf("3. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            menuCadastroLogin(ARQ_ALUNOS, "Aluno");
        }
        else if (opcao == 2) {
            menuCadastroLogin(ARQ_PROFESSORES, "Professor");
        }

    } while (opcao != 3);

    return 0;
}






