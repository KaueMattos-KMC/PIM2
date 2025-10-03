#ifndef CRUD_CADASTRO_H
#define CRUD_CADASTRO_H
void limpar_buffer();

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// ---------- Constantes ----------
#define MAX 100
#define ARQ_ALUNOS "alunos.txt"
#define ARQ_PROFESSORES "professores.txt"

// ---------- Struct ----------
typedef struct {
    char ra[20];
    char nome[50];
    char cpf[20];
    char telefone[20];
    char email[50];
    char senha[20];
} Registro;

// ---------- Funções Auxiliares ----------
void salvar(Registro lista[], int qtd, const char *arquivo);
int carregar(Registro lista[], const char *arquivo);

// ---------- Validações ----------
int existeRA(Registro lista[], int qtd, const char *ra);
int validarTelefone(const char *telefone);

// ---------- Cadastro & Login ----------
void cadastrar(Registro lista[], int *qtd, const char *arquivo);
int login(const char *arquivo, const char *tipo, char *usuarioRA);

// ---------- CRUD ----------
void listarAlunos();
void buscarAluno();
void alterarCadastro(const char *arquivo, const char *usuarioRA);
void apagarConta(const char *arquivo, const char *usuarioRA);
void apagarAluno();

// ---------- Menus ----------
void menuInterno(const char *tipo, const char *arquivo, const char *usuarioRA);
void menuCadastroLogin(const char *arquivo, const char *tipo);

#endif // CRUD_CADASTRO_H
