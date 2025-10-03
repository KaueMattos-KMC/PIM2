#include <stdio.h>
#include <locale.h>
#include "CRUD_Cadastro.h"
#include "CRUD_Turmas.h"

// Função auxiliar para limpar o buffer de entrada (incluída no main para garantir uso)
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}


// Função ajustada do menu de cadastro/login que retorna se houve login e preenche RA
int menuCadastroLoginComRA(const char *arquivo, const char *tipo, char *usuarioRA) {
    Registro lista[MAX];
    // NOTA: 'carregar' (do CRUD_Cadastro.c) deve ser o carregar robusto (fgets/strtok).
    int qtd = carregar(lista, arquivo); 
    int opcao, logado = 0;

    do {
        printf("\n--- %s ---\n", tipo);
        printf("1. Cadastrar\n");
        printf("2. Login\n");
        printf("3. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer(); // <-- Limpeza de buffer após ler a opção

        switch (opcao) {
            case 1: 
                cadastrar(lista, &qtd, arquivo); 
                break;
            case 2: 
                // NOTA: 'login' (do CRUD_Cadastro.c) deve ter os scanf limitados.
                logado = login(arquivo, tipo, usuarioRA);
                if (logado) return 1; // login bem-sucedido
                break;
        }
    } while (opcao != 3);

    return 0; // voltou sem logar
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    int opcao;
    char usuarioRA[20];

    do {
        printf("\n=== Sistema Escolar ===\n");
        printf("1. Menu Aluno\n");
        printf("2. Menu Professor\n");
        printf("3. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer(); // <-- Limpeza de buffer principal

        if (opcao == 1) {
            int alunoLogado = menuCadastroLoginComRA(ARQ_ALUNOS, "Aluno", usuarioRA);
            if (alunoLogado) {
    int escolha_aluno;
    do {
        printf("\n--- Menu Aluno ---\n");
        printf("1. Alterar Cadastro\n");
        printf("2. Ver Minhas Turmas\n"); // NOVO: Opção para o aluno ver suas turmas
        printf("3. Apagar Minha Conta\n");
        printf("4. Voltar\n"); // Novo 'Voltar' para o aluno
        printf("Escolha: ");
        scanf("%d", &escolha_aluno);
        limpar_buffer();

        if (escolha_aluno == 1) {
            menuInterno("Aluno", ARQ_ALUNOS, usuarioRA);
        } else if (escolha_aluno == 2) {
            // CHAMA A NOVA FUNÇÃO:
            listarTurmasAluno(usuarioRA); 
        } else if (escolha_aluno == 3) {
            // Chama a lógica de apagar conta (que deve estar em CRUD_Cadastro.c)
            apagarConta(ARQ_ALUNOS, usuarioRA); 
            break; // Sai do menu interno após apagar a conta
        }
        // Repetição se escolha_aluno != 4
    } while (escolha_aluno != 4);
} 
        } 
        else if(opcao == 2) {
            int professorLogado = menuCadastroLoginComRA(ARQ_PROFESSORES, "Professor", usuarioRA);
            if(professorLogado) {
                int escolha;
                do { 
                    printf("\n--- Menu Professor ---\n");
                    printf("1. Gerenciar Cadastro\n2. Gerenciar Turmas\n3. Voltar\nEscolha: ");
                    scanf("%d", &escolha);
                    limpar_buffer(); // <-- Limpeza de buffer do menu interno do Professor
                    
                    if(escolha == 1) {
                        // 'menuInterno' tratará Alterar Cadastro, CRUD Alunos e Apagar Conta Professor
                        menuInterno("Professor", ARQ_PROFESSORES, usuarioRA);
                    }
                    else if(escolha == 2) {
                        // 'menuTurmas' é o código que acabamos de corrigir (CRUD_Turmas.c)
                        menuTurmas(usuarioRA); 
                    }
                } while(escolha != 3);
            }
        }

    } while (opcao != 3);

    printf("Saindo do sistema. Até mais!\n");
    return 0;
}