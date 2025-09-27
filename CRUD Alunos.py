import json
import os

# Nome do arquivo onde vamos salvar os alunos
ARQUIVO_alunos = 'alunos.json'

# Função para carregar os alunos do arquivo JSON
def carregar_alunos():
    if os.path.exists(ARQUIVO_alunos):
        with open(ARQUIVO_alunos, 'r') as arquivo:
            return json.load(arquivo)
    return {}

# Função para salvar os alunos no arquivo JSON
def salvar_alunos(alunos):
    with open(ARQUIVO_alunos, 'w') as arquivo:
        json.dump(alunos, arquivo, indent=4)

# Função para cadastrar um novo aluno
def cadastrar_aluno(alunos):
    cpf = input("Digite o CPF do aluno: ")
    if cpf in alunos:
        print("Já existe um aluno com esse CPF.")
        return

    nome = input("Nome: ")
    telefone = input("Telefone: ")
    email = input("E-mail: ")
    endereco = input("Endereço: ")

    alunos[cpf] = {
        'nome': nome,
        'telefone': telefone,
        'email': email,
        'endereco': endereco
    }
    salvar_alunos(alunos)
    print("aluno cadastrado com sucesso!")

# Função para buscar um aluno
def buscar_aluno(alunos):
    cpf = input("Digite o CPF do aluno para buscar: ")
    aluno = alunos.get(cpf)
    if aluno:
        print(f"Nome: {aluno['nome']}")
        print(f"Telefone: {aluno['telefone']}")
        print(f"E-mail: {aluno['email']}")
        print(f"Endereço: {aluno['endereco']}")
    else:
        print("aluno não encontrado.")

# Função para alterar um aluno
def alterar_aluno(alunos):
    cpf = input("Digite o CPF do aluno que deseja alterar: ")
    if cpf in alunos:
        print("Deixe em branco caso não queira alterar o campo.")
        nome = input(f"Nome atual ({alunos[cpf]['nome']}): ") or alunos[cpf]['nome']
        telefone = input(f"Telefone atual ({alunos[cpf]['telefone']}): ") or alunos[cpf]['telefone']
        email = input(f"E-mail atual ({alunos[cpf]['email']}): ") or alunos[cpf]['email']
        endereco = input(f"Endereço atual ({alunos[cpf]['endereco']}): ") or alunos[cpf]['endereco']

        alunos[cpf] = {
            'nome': nome,
            'telefone': telefone,
            'email': email,
            'endereco': endereco
        }
        salvar_alunos(alunos)
        print("aluno atualizado com sucesso!")
    else:
        print("aluno não encontrado.")

# Função para apagar um aluno
def apagar_aluno(alunos):
    cpf = input("Digite o CPF do aluno que deseja apagar: ")
    if cpf in alunos:
        del alunos[cpf]
        salvar_alunos(alunos)
        print("aluno apagado com sucesso!")
    else:
        print("aluno não encontrado.")

# Função para mostrar todos os alunos
def mostrar_todos(alunos):
    if alunos:
        for cpf, dados in alunos.items():
            print(f"\nCPF: {cpf}")
            print(f"Nome: {dados['nome']}")
            print(f"Telefone: {dados['telefone']}")
            print(f"E-mail: {dados['email']}")
            print(f"Endereço: {dados['endereco']}")
    else:
        print("Nenhum aluno cadastrado.")

# Menu principal
def menu():
    alunos = carregar_alunos()

    while True:
        print("\n--- Menu ---")
        print("1. Cadastrar aluno")
        print("2. Buscar aluno")
        print("3. Alterar aluno")
        print("4. Apagar aluno")
        print("5. Mostrar Todos os alunos")
        print("6. Sair")

        opcao = input("Escolha uma opção: ")

        if opcao == '1':
            cadastrar_aluno(alunos)
        elif opcao == '2':
            buscar_aluno(alunos)
        elif opcao == '3':
            alterar_aluno(alunos)
        elif opcao == '4':
            apagar_aluno(alunos)
        elif opcao == '5':
            mostrar_todos(alunos)
        elif opcao == '6':
            break
        else:
            print("Opção inválida! Tente novamente.")

# Executar o programa
menu()