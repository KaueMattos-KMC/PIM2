#include <stdio.h>
#include <locale.h>
#include "CRUD_Cadastro.h"
#include "CRUD_Turmas.h"
#include "CRUD_Aulas.h"

// Função auxiliar para limpar o buffer de entrada (mantida aqui para garantir uso)
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
    
    // LINHA system("chcp 65001"); FOI REMOVIDA
    
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
                    printf("2. Ver Minhas Turmas\n"); 
                    printf("3. Apagar Minha Conta\n");
                    printf("4. Voltar\n"); 
                    printf("Escolha: ");
                    scanf("%d", &escolha_aluno);
                    limpar_buffer();

                    if (escolha_aluno == 1) {
                        menuInterno("Aluno", ARQ_ALUNOS, usuarioRA);
                    } else if (escolha_aluno == 2) {
                        // CHAMA A FUNÇÃO DE TURMAS DO ALUNO:
                        listarTurmasAluno(usuarioRA); 
                    } else if (escolha_aluno == 3) {
                        // Chama a lógica de apagar conta
                        apagarConta(ARQ_ALUNOS, usuarioRA); 
                        break; // Sai do menu interno após apagar a conta
                    }
                } while (escolha_aluno != 4);
            } 
        } 
        else if(opcao == 2) {
            int professorLogado = menuCadastroLoginComRA(ARQ_PROFESSORES, "Professor", usuarioRA);
            if(professorLogado) {
                int escolha;
                do { 
                    printf("\n--- Menu Professor ---\n");
                    printf("1. Gerenciar Cadastro\n2. Gerenciar Turmas\n3. Gerenciar Aulas\n4. Voltar\nEscolha: ");
                    scanf("%d", &escolha);
                    limpar_buffer(); 
                    
                    if(escolha == 1) {
                        // Gerenciar Cadastro (Alterar/CRUD Alunos/Apagar Conta Prof)
                        menuInterno("Professor", ARQ_PROFESSORES, usuarioRA);
                    }
                    else if(escolha == 2) {
                        // Gerenciar Turmas (Criar/Listar/Add Aluno)
                        menuTurmas(usuarioRA); 
                    }
                    else if(escolha == 3) {
                        // Gerenciar Aulas (Criar/Listar Aulas)
                        menuAulas(usuarioRA);
                    }
                } while(escolha != 4); // Ajustado para 4 opções no menu Professor
            }
        }

    } while (opcao != 3);

    printf("Saindo do sistema.  Adeus!!!\n");
    return 0;
}