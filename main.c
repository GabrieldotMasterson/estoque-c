#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_ITEM 100

// fgets x scanf
// fgets: lê a linha inteira, permitindo espaços e \n

// funções
void limparBuffer() { // otimizar input
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void adicionarItem(FILE *arquivo) {
    char item[MAX_ITEM];
    int qtd;

    printf("\nDigite o nome do item: ");
    limparBuffer();
    fgets(item, MAX_ITEM, stdin);
    item[strcspn(item, "\n")] = '\0'; // Remover o '\n'

    printf("Digite a quantidade: ");
    scanf("%d", &qtd);

    fprintf(arquivo, "%s\n%d\n", item, qtd);
    fflush(arquivo); // DEBUG: Garantir que grava no arquivo

    printf("Item adicionado com sucesso!\n");
}

void listarEstoque() {
    FILE *arquivo = fopen("estoque.txt", "r");
    char item[MAX_ITEM];
    int qtd;
    int estoqueVazio = 0;

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("\nEstoque Atual\n");

    while (fgets(item, MAX_ITEM, arquivo)) {
        item[strcspn(item, "\n")] = '\0';
        if (fscanf(arquivo, "%d\n", &qtd) == 1) {
            printf("Nome: %s | Quantidade: %d\n", item, qtd);
            estoqueVazio = 1;
        }
    }

    if (!estoqueVazio) {
        printf("O estoque está vazio.\n");
    }

    fclose(arquivo);
}

void removerItem() {
    char itemRemover[MAX_ITEM];
    char itemAtual[MAX_ITEM];
    int qtdAtual, qtdRemover;
    int encontrado = 0;

    printf("Digite o nome do item: ");
    limparBuffer();
    fgets(itemRemover, MAX_ITEM, stdin);
    itemRemover[strcspn(itemRemover, "\n")] = '\0';

    printf("Digite a quantidade a ser removida: ");
    scanf("%d", &qtdRemover);

    FILE *arquivo = fopen("estoque.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!arquivo || !temp) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    while (fgets(itemAtual, MAX_ITEM, arquivo)) {
        itemAtual[strcspn(itemAtual, "\n")] = '\0';
        fscanf(arquivo, "%d\n", &qtdAtual);

        if (strcmp(itemAtual, itemRemover) == 0) {
            encontrado = 1;

            if (qtdRemover < qtdAtual) {
                int novaQtd = qtdAtual - qtdRemover;
                fprintf(temp, "%s\n%d\n", itemAtual, novaQtd);
                printf("Quantidade atualizada com sucesso!\n");

            } else if (qtdRemover == qtdAtual) {
                printf("Item removido do estoque!\n");

            } else {
                fprintf(temp, "%s\n%d\n", itemAtual, qtdAtual); // mantêm o item
                printf("Estoque insuficiente. Quantidade disponível: %d\n", qtdAtual);

            }
        } else {
            fprintf(temp, "%s\n%d\n", itemAtual, qtdAtual); 
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove("estoque.txt");
    rename("temp.txt", "estoque.txt");

    if (!encontrado) {
        printf("Item não encontrado.\n");
    }
}


int main() {
    setlocale(LC_ALL, "pt_BR.utf8");

    int opcao;

    while (1) {
        printf("================================");
        printf("\n     Controle de Estoque");
        printf("\n================================");
        printf("\nSelecione uma opção:");
        printf("\n1. Adicionar Item");
        printf("\n2. Remover Item");
        printf("\n3. Listar Estoque");
        printf("\n4. Sair");
        printf("\nOpção: ");
        scanf("%d", &opcao);

        if (opcao == 4) {
            printf("\nObrigado por usar o Controle de Estoques! Até a próxima.\n");
            break;
        }

        switch (opcao) {
            case 1: {
                FILE *arquivo = fopen("estoque.txt", "a");
                if (!arquivo) {
                    printf("Erro ao abrir o arquivo.\n");
                    break;
                }
                adicionarItem(arquivo);
                fclose(arquivo);
                break;
            }

            case 2:
                removerItem();
                break;

            case 3:
                listarEstoque();
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

        printf("\nDeseja realizar outra operação? (s/n): ");
        char continuar;
        scanf(" %c", &continuar);
        if (continuar != 's' && continuar != 'S') {
            printf("\nObrigado por usar o Controle de Estoques! Até a próxima.\n");
            break;
        }
    }

    return 0;
}