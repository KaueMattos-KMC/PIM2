#include <stdio.h>
#include <locale.h>
#include <stdlib.h> 
#include "CRUD_Cadastro.h"
#include "CRUD_Turmas.h"
#include "CRUD_Atividades.h"
#include "CRUD_Diario.h" // MÓDULO DIÁRIO ELETRÔNICO

// Função auxiliar para limpar o buffer de entrada (implementada aqui)
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// Função ajustada do menu de cadastro/login que retorna se houve login e preenche RA
int menuCadastroLoginComRA(const char *arquivo, const char *tipo, char *usuarioRA) {
    Registro lista[MAX];
    int qtd = carregar(lista, arquivo); 
    int opcao, logado = 0;

    do {
        printf("\n--- %s ---\n", tipo);
        printf("1. Cadastrar\n");
        printf("2. Login\n");
        printf("3. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: 
                cadastrar(lista, &qtd, arquivo); 
                break;
            case 2: 
                logado = login(arquivo, tipo, usuarioRA);
                if (logado) return 1;
                break;
            default: // Opção inválida
                if (opcao != 3) {
                    printf("Opção inválida. Tente novamente.\n");
                }
                break;
        }
    } while (opcao != 3);

    return 0;
}

int main() {
    // Define o locale para Português
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
        limpar_buffer();

        if (opcao == 1) { // MENU ALUNO
            int alunoLogado = menuCadastroLoginComRA(ARQ_ALUNOS, "Aluno", usuarioRA);
            if (alunoLogado) {
                int escolha_aluno;
                do {
                    printf("\n--- Menu Aluno ---\n");
                    printf("1. Alterar Cadastro\n");
                    printf("2. Ver Minhas Turmas\n"); 
                    printf("3. Ver Atividades\n"); 
                    printf("4. Consultar Desempenho (Notas/Faltas)\n"); 
                    printf("5. Apagar Minha Conta\n"); 
                    printf("6. Voltar\n"); 
                    printf("Escolha: ");
                    scanf("%d", &escolha_aluno);
                    limpar_buffer();

                    switch (escolha_aluno) {
                        case 1: menuInterno("Aluno", ARQ_ALUNOS, usuarioRA); break;
                        case 2: listarTurmasAluno(usuarioRA); break; 
                        // case 3: listarAulasAluno(usuarioRA); break; // REMOVIDO
                        case 3: listarAtividadesAluno(usuarioRA); break; 
                        case 4: consultarDesempenho(usuarioRA); break;
                        case 5: apagarConta(ARQ_ALUNOS, usuarioRA); break; 
                        default:
                            if (escolha_aluno != 6) {
                                printf("Opção inválida. Tente novamente.\n");
                            }
                            break;
                    }
                    if (escolha_aluno == 5) break; // Sai se a conta foi apagada
                } while (escolha_aluno != 6);
            }
        } 
        else if(opcao == 2) { // MENU PROFESSOR
            int professorLogado = menuCadastroLoginComRA(ARQ_PROFESSORES, "Professor", usuarioRA);
            if(professorLogado) {
                int escolha;
                do { 
                    printf("\n--- Menu Professor ---\n");
                    printf("1. Gerenciar Cadastro\n");
                    printf("2. Gerenciar Turmas\n");
                    printf("3. Gerenciar Atividades\n");
                    printf("4. Diário Eletrônico (Notas/Faltas)\n"); 
                    printf("5. Voltar\n");
                    printf("Escolha: ");
                    scanf("%d", &escolha);
                    limpar_buffer(); 
                    
                    if(escolha == 1) {
                     
                        int status = menuInterno("Professor", ARQ_PROFESSORES, usuarioRA);
                        if (status == 0) {
                            break; 
                        }
                    }
                    else if(escolha == 2) {
                        menuTurmas(usuarioRA); 
                    }
                    else if(escolha == 3) { 
                        menuAtividades(usuarioRA); 
                    }
                    else if(escolha == 4) { 
                        menuDiario(usuarioRA); 
                    }
                    else if (escolha != 5) { // Validação
                         printf("Opção inválida. Tente novamente.\n");
                    }
                } while(escolha != 5);
            }
        } else if (opcao != 3) {
             printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 3);

    printf("Saindo do sistema. Adeus!!!\n");
    return 0;
}