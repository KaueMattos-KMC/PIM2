#ifndef CRUD_CADASTRO_H
#define CRUD_CADASTRO_H

// Constantes de Limite e Arquivo
#define MAX 50
#define ARQ_ALUNOS "alunos.txt"
#define ARQ_PROFESSORES "professores.txt"

// Estrutura de Dados (Usada para Aluno e Professor)
typedef struct {
    char ra[20];
    char nome[50];
    char cpf[15];       // 14 caracteres é suficiente para CPF formatado (000.000.000-00\0)
    char telefone[20];  // 15 caracteres para (99) 99999-9999\0
    char email[50];
    char senha[20];
} Registro;

// Protótipo da função de utilidade (implementada no Main.c)
void limpar_buffer();

// Protótipos das Funções Auxiliares de Arquivo
void salvar(Registro lista[], int qtd, const char *arquivo);
int carregar(Registro lista[], const char *arquivo);

// Protótipos das Funções de Validação
int existeRA(Registro lista[], int qtd, const char *ra);
int validarTelefone(const char *telefone);

// Protótipos das Funções de Cadastro e Login
void cadastrar(Registro lista[], int *qtd, const char *arquivo);
int login(const char *arquivo, const char *tipo, char *usuarioRA);

// Protótipos das Funções CRUD
void listarAlunos();
void buscarAluno();
void alterarCadastro(const char *arquivo, const char *usuarioRA);
void apagarConta(const char *arquivo, const char *usuarioRA);
void apagarAluno();

// Protótipos das Funções de Menu (que fazem interface entre os módulos)
void menuInterno(const char *tipo, const char *arquivo, const char *usuarioRA);
void menuCadastroLogin(const char *arquivo, const char *tipo);

#endif // CRUD_CADASTRO_H
