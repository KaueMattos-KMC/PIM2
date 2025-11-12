#ifndef CRUD_CADASTRO_H
#define CRUD_CADASTRO_H

// Definições de constantes (ajuste MAX conforme o limite desejado)
#define MAX 50
#define ARQ_ALUNOS "alunos.txt"
#define ARQ_PROFESSORES "professores.txt"

// Estrutura de dados (Registro é usado tanto para Aluno quanto Professor)
typedef struct {
    char ra[20];
    char nome[50];
    char cpf[20];
    char telefone[20];
    char email[50];
    char senha[20];
} Registro;

// --- Protótipos das Funções ---

// Funções Auxiliares de Arquivo
void salvar(Registro lista[], int qtd, const char *arquivo);
int carregar(Registro lista[], const char *arquivo);

// Validações
int existeRA(Registro lista[], int qtd, const char *ra);
int validarTelefone(const char *telefone);

// Cadastro & Login
void cadastrar(Registro lista[], int *qtd, const char *arquivo);
int login(const char *arquivo, const char *tipo, char *usuarioRA);

// Funções CRUD (Acessíveis pelo Professor)
void listarAlunos();
void alterarCadastro(const char *arquivo, const char *usuarioRA);
void apagarConta(const char *arquivo, const char *usuarioRA);
void apagarAluno();

// Menus
int menuInterno(const char *tipo, const char *arquivo, const char *usuarioRA);
void menuCadastroLogin(const char *arquivo, const char *tipo);

#endif // CRUD_CADASTRO_H