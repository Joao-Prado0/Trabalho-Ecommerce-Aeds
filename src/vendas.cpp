#include "vendas.h"
#include "produtos.h"
#include "compradores.h"
#include "vendedores.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <cmath>
using namespace std;
// -------------------------------------- Construtor ----------------------------------------
Vendas::Vendas(){}
// -------------------------------------- Uteis -----------------------------------------------
int Vendas::criar_codigoVenda() {
    static bool inicializado = false;
    if (!inicializado) {
        srand(time(nullptr));
        inicializado = true;
    }
    int codigo;
    do {
        codigo = (rand() % 900) + 100;
    } while (verificar_codigo(codigo));
    return codigo;
}
float Vendas::calcular_frete(float valorT) {
    if (valorT<=1000.00) {
        return 30.00;
    }
    if (valorT>1000.00 && valorT<=1500.00) {
        return 20.00;
    }
    return 0.00;
}
bool Vendas::verificar_codigo(int codigo) {
    ifstream arquivo("../data/vendas.txt");
    string linha;
    while (getline(arquivo, linha)) {
        if (linha.find("Codigo: " + to_string(codigo)) != string::npos) {
            return true;
        }
    }
    return false;
}
// ------------------------------------------ CRUD--------------------------------------------
void Vendas::consultar_venda(int codigoV) {
    ifstream arquivo("../data/vendas.txt");
    string linha;
    bool encontrou = false;
    bool imprimir = false;

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo" << endl;
        return;
    }

    while (getline(arquivo,linha)) {
        if (linha.find("Codigo: " + to_string(codigoV)) != string::npos) {
            encontrou = true;
            imprimir = true;
            cout << "\n=== Detalhes da Venda ===" << endl;
        }

        if (imprimir) {
            cout << linha << endl;
        }

        if (linha.find("---") != string::npos) {
            if (encontrou) break;
            imprimir = false;
        }
    }

    if (!encontrou) {
        cout << "Venda com codigo " << codigoV << " nao encontrada." << endl;
    }
    arquivo.close();
}
void Vendas::alterar_venda(int codigoV) {
    ifstream arquivoEntrada("../data/vendas.txt");
    if (!arquivoEntrada.is_open()) {
        cout << "Erro ao abrir o arquivo" << endl;
        return;
    }
    vector<string> linhas;
    string linha;
    while (getline(arquivoEntrada, linha)) {
        linhas.push_back(linha);
    }
    arquivoEntrada.close();

    bool encontrou = false;
    int inicio = -1, fim = -1;

    for (size_t i = 0; i < linhas.size(); ++i) {
        if (linhas[i] == "Codigo: " + to_string(codigoV)) {
            encontrou = true;
            inicio = i;
        }
        if (encontrou && linhas[i] == "---") {
            fim = i;
            break;
        }
    }
    if (!encontrou) {
        cout << "Venda com codigo " << codigoV << " nao encontrada." << endl;
        return;
    }
    int opcao;
    do {
        cout << "O que deseja alterar?"<<endl;
        cout << "1. Codigo da venda."<<endl;
        cout << "2. Nome do comprador." << endl;
        cout << "3. Valor total." << endl;
        cout << "4. Frete."<<endl;
        cout << "5. Itens da venda."<<endl;
        cout << "0. Sair e salvar."<<endl;
        cin >> opcao;
        cin.ignore();
        switch (opcao) {
            case 1: {
                int novoCodigo;
                cout << "Novo codigo: ";
                cin >> novoCodigo;
                linhas[inicio] = "Codigo: " + to_string(novoCodigo);
                break;
            }
            case 2: {
                string novoNome;
                cout << "Novo nome do comprador: ";
                getline(cin, novoNome);
                linhas[inicio + 1] = "Comprador: " + novoNome;
                break;
            }
            case 3: {
                float novoTotal;
                cout << "Novo valor total: ";
                cin >> novoTotal;
                linhas[fim - 1] = "Valor Total: " + to_string(novoTotal);
                break;
            }
            case 4: {
                float novoFrete;
                cout << "Novo valor do frete: ";
                cin >> novoFrete;
                linhas[fim - 2] = "Frete: " + to_string(novoFrete);
                break;
            }
            case 5:
                alterar_itens_venda(codigoV);
                break;
            case 0:
                cout << "Saindo da edicao." << endl;
                break;
            default:
                cout << "Opção invalida!" << endl;
        }
    } while (opcao != 0);

    ofstream arquivoSaida("../data/vendas.txt");
    if (!arquivoSaida.is_open()) {
        cout << "Erro ao regravar o arquivo." << endl;
        return;
    }
    for (size_t i = 0; i < linhas.size(); ++i) {
        arquivoSaida << linhas[i] << endl;
    }
    arquivoSaida.close();
    cout << "Venda atualizada com sucesso!" << endl;
}
void Vendas::alterar_itens_venda(int codigoV) {
    ifstream arquivoEntrada("../data/vendas.txt");
    if (!arquivoEntrada.is_open()) {
        cout << "Erro ao abrir o arquivo" << endl;
        return;
    }
    vector<string> linhas;
    string linha;
    while (getline(arquivoEntrada, linha)) {
        linhas.push_back(linha);
    }
    arquivoEntrada.close();
    int inicio = -1, fim = -1;
    for (size_t i = 0; i < linhas.size(); ++i) {
        if (linhas[i] == "Codigo: " + to_string(codigoV)) {
            inicio = i;
        }
        if (inicio != -1 && linhas[i] == "---") {
            fim = i;
            break;
        }
    }
    if (inicio == -1 || fim == -1) {
        cout << "Venda não encontrada." << endl;
        return;
    }

    int idxItens = -1;
    for (int i = inicio; i < fim; ++i) {
        if (linhas[i] == "Itens:") {
            idxItens = i;
            break;
        }
    }
    if (idxItens == -1) {
        cout << "Erro: seção de itens não encontrada." << endl;
        return;
    }

    int idxFrete = -1;
    for (int i = idxItens + 1; i < fim; ++i) {
        if (linhas[i].find("Frete:") != string::npos) {
            idxFrete = i;
            break;
        }
    }
    if (idxFrete == -1) {
        cout << "Erro: linha de frete não encontrada." << endl;
        return;
    }

    linhas.erase(linhas.begin() + idxItens + 1, linhas.begin() + idxFrete);

    int n;
    cout << "Quantos produtos deseja inserir? ";
    cin >> n;
    vector<string> novosItens;
    float novoTotal = 0.0f;

    for (int i = 0; i < n; ++i) {
        int cod, qtd;
        float preco;
        string nome;

        cout << "Produto " << (i+1) << endl;
        cout << "Código: "; cin >> cod;
        cout << "Nome: "; cin.ignore(); getline(cin, nome);
        cout << "Quantidade: "; cin >> qtd;
        cout << "Preço unitário: "; cin >> preco;

        preco = round(preco * 100) / 100;
        float total = round((preco * qtd) * 100) / 100;
        novoTotal += total;

        string itemLinha = " - Codigo: " + to_string(cod)
        + " | Nome: " + nome
        + " | Qtd: " + to_string(qtd)
        + " | Unitario: " + to_string(preco)
        + " | Total: " + to_string(total);
        novosItens.push_back(itemLinha);
    }
    linhas.insert(linhas.begin() + idxFrete, novosItens.begin(), novosItens.end());
    float novoFrete = calcular_frete(novoTotal);
    linhas[idxFrete] = "Frete: " + to_string(novoFrete);
    linhas[idxFrete + 1] = "Valor Total: " + to_string(novoTotal + novoFrete);

    ofstream arquivoaSaida("../data/vendas.txt");
    if (!arquivoaSaida.is_open()) {
        cout << "Erro ao reescrever o arquivo" << endl;
        return;
    }
    for (size_t i = 0; i < linhas.size(); ++i) {
        arquivoaSaida << linhas[i] << endl;
    }
    arquivoaSaida.close();
    cout << "Itens alterados com sucesso!" << endl;
}
void Vendas::deletar_venda(int codigoV) {
    ifstream entrada("../data/vendas.txt");
    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo" << endl;
        return;
    }

    vector<string> linhas;
    string linha;
    while (getline(entrada, linha)) {
        linhas.push_back(linha);
    }
    entrada.close();

    int inicio = -1, fim = -1;
    for (size_t i = 0; i < linhas.size(); ++i) {
        if (linhas[i] == "Codigo: " + to_string(codigoV)) {
            // "Venda:" está sempre uma linha antes
            if (i > 0 && linhas[i - 1] == "Venda:") {
                inicio = i - 1;
            } else {
                inicio = i;
            }
        }
        if (inicio != -1 && linhas[i] == "---") {
            fim = i;
            break;
        }
    }
    if (inicio == -1 || fim == -1) {
        cout << "Venda com código " << codigoV << " não encontrada." << endl;
        return;
    }

    linhas.erase(linhas.begin() + inicio, linhas.begin() + fim + 1);

    ofstream arquivosSaida("../data/vendas.txt");
    if (!arquivosSaida.is_open()) {
        cout << "Erro ao reescrever o arquivo" << endl;
        return;
    }
    for (const string& l : linhas) {
        arquivosSaida << l << endl;
    }
    arquivosSaida.close();
    cout << "Venda removida com sucesso!" << endl;
}
void Vendas::inserir_venda_manualmente() {
    Vendedores vendedores;
    Produtos produtoObj;
    int diferentesProdutos, codigoVR;
    float valorT = 0.0f;
    string nome;
    int codigoV = criar_codigoVenda();
    cin.clear();
    while (true) {
        cout << "Insira o numero de produtos comprados: " << endl;
        if (cin >> diferentesProdutos) {
            cin.ignore();
            if (diferentesProdutos > 0) {
                break;
            }
            cout << "Quantidade deve ser maior que zero!" << endl;
        } else {
            cout << "Entrada invalida! Digite um numero valido." << endl;
            cin.clear();
            cin.ignore();
        }
    }

    ItemVenda* produtos = new ItemVenda[diferentesProdutos];

    for (int i = 0; i < diferentesProdutos; i++) {
        Produto prodAtual;
        produtoObj.listar_produtos();
        cout << "\nProduto " << i+1 << ":\n";
        while (true) {
            cout << "Codigo: ";
            cin >> produtos[i].codigoProduto;
            prodAtual = produtoObj.consultar_produto(produtos[i].codigoProduto);
            if (prodAtual.encontrado){
                cin.ignore();
                break;
            }
            cout << "Codigo invalido! Digite um numero." << endl;
            cin.clear();
            cin.ignore();
        }
        prodAtual = produtoObj.consultar_produto(produtos[i].codigoProduto);
        produtos[i].nomeProduto = prodAtual.nome;

        while (true) {
            cout << "Quantidade: ";
            if (cin >> produtos[i].quantidadeVendida) {
                cin.ignore();
                if (produtos[i].quantidadeVendida > 0) {
                    break;
                }
                cout << "Quantidade deve ser maior que zero!" << endl;
            } else {
                cout << "Quantidade invalida! Digite um numero." << endl;
                cin.clear();
                cin.ignore();
            }
        }

        produtos[i].precoUnitario = prodAtual.precoVenda;
        produtoObj.confirmar_venda(produtos[i].codigoProduto,produtos[i].quantidadeVendida);
        produtos[i].precoTotal = produtos[i].quantidadeVendida * produtos[i].precoUnitario;
        valorT += produtos[i].precoTotal;
    }

    cout << "Insira o nome do comprador: " << endl;
    getline(cin, nome);

    while (true) {
        vendedores.listarTodosVendedores();
        cout << "Insira o codigo do vendedor: " << endl;
        cin >> codigoVR;
        cin.ignore();
        if (vendedores.verificarNumero(codigoVR)) {
            break;
        } else {
            cout << "Codigo invalido! Digite um numero." << endl;
            cin.clear();
            cin.ignore();
        }
    }
    vendedores.adicionarComissaoPorNumero(codigoVR,valorT);

    imprimir_no_documento(codigoV, codigoVR, nome, valorT, produtos, diferentesProdutos);
    delete[] produtos;
}
void Vendas::imprimir_no_documento(int codigoV,int codigoVR, string nome, float valorT, ItemVenda *itens, int quant) {
    ofstream arquivo("../data/vendas.txt", ios::app);
    if (!arquivo.is_open()) {
        cerr<<"Erro ao abrir o arquivo" << endl;
        return;
    }
    arquivo << "Venda:\n";
    arquivo << "Codigo: " << codigoV << "\n";
    arquivo << "Comprador: " << nome << "\n";
    arquivo << "Itens:\n";
    for (int i = 0;i<quant; i++) {
        arquivo << " - Codigo: " << itens[i].codigoProduto
               << " | Nome: " << itens[i].nomeProduto
               << " | Qtd: " << itens[i].quantidadeVendida
               << " | Unitario: " << fixed << setprecision(2) << itens[i].precoUnitario
               << " | Total: " << itens[i].precoTotal << "\n";
    }
    arquivo << "Frete: " << calcular_frete(valorT)<<"\n";
    arquivo << "Valor Total: " << valorT+calcular_frete(valorT)<<"\n";
    arquivo << "---\n";
    Vendedores vendedores;
    vendedores.adicionarComissaoPorNumero(codigoVR,valorT);
    arquivo.close();
    cout << "Venda registrada com sucesso! Codigo gerado: "<< codigoV<< endl;
}
//------------------------------------ Nota Fiscal ------------------------------
void Vendas::imprimir_nota_fiscal(Comprador compradorAtual,ItemVenda carrinho[],int contadorDeProdutos, float valorCompra) {
    string nomeArquivo = "NotaFiscal"+compradorAtual.getCpf()+".txt";
    ofstream arquivo(nomeArquivo);
    int codigoVendaAtual = criar_codigoVenda();
    if (!arquivo.is_open()) {
        cerr << "Erro ao criar a nota fiscal"<<endl;
        return;
    }

    arquivo << "==========================================="<< endl;
    arquivo << "              NOTA FISCAL" << endl;
    arquivo << "==========================================="<< endl;
    arquivo << "\n";
    arquivo << "Codigo da Venda: " << codigoVendaAtual << endl;
    arquivo << "\n";
    arquivo << "Comprador(a):"<<endl;
    arquivo << "Nome: " << compradorAtual.getNome() << endl;
    arquivo << "CPF: " << compradorAtual.getCpf() << endl;
    arquivo << "Endereco: " << compradorAtual.getEndereco() << endl;
    arquivo << "\n";
    arquivo << "-------------------------------------------" << endl;
    arquivo << "Itens da Venda:" << endl;
    arquivo << "-------------------------------------------" << endl;
    arquivo << "Código | Nome do Produto              | Qtde | Unitário (R$) | Total (R$)" << endl;
    arquivo << "-------|------------------------------|------|---------------|-----------" << endl;
    for (int i = 0; i<contadorDeProdutos; i++) {
        arquivo << setw(6) << carrinho[i].codigoProduto << " | "
            << setw(20) << left << carrinho[i].nomeProduto << " | "
            << setw(4) << carrinho[i].quantidadeVendida << " | "
            << setw(14) << fixed << setprecision(2) << carrinho[i].precoUnitario << " | "
            << setw(10) << carrinho[i].precoTotal << "\n";
    }
    arquivo << "-------------------------------------------"<<endl;
    arquivo << "Frete: R$ " << calcular_frete(valorCompra) << endl;
    arquivo << "Total da Compra: R$ " << calcular_frete(valorCompra)+valorCompra << endl;
    arquivo << "===========================================" << endl;
    salvar_venda_usuario(codigoVendaAtual,compradorAtual.getNome(),carrinho,contadorDeProdutos,valorCompra);
    arquivo.close();
    cout << "Nota fiscal gerada com sucesso: " << nomeArquivo << endl;
}
void Vendas::salvar_venda_usuario(int codigoV, string nomeUsuario,ItemVenda carrinho[], int contadorDeProdutos, float valorCompra) {
    ofstream arquivo("../data/vendas.txt", ios::app);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo" << endl;
        return;
    }
    arquivo << "Venda:\n";
    arquivo << "Codigo: " << codigoV << "\n";
    arquivo << "Comprador: " << nomeUsuario << "\n";
    arquivo << "Itens:\n";
    for (int i = 0;i<contadorDeProdutos; i++) {
        arquivo << " - Codigo: " << carrinho[i].codigoProduto
               << " | Nome: " << carrinho[i].nomeProduto
               << " | Qtd: " << carrinho[i].quantidadeVendida
               << " | Unitario: " << fixed << setprecision(2) << carrinho[i].precoUnitario
               << " | Total: " << carrinho[i].precoTotal << "\n";
    }
    arquivo << "Frete: " << calcular_frete(valorCompra)<<"\n";
    arquivo << "Valor Total: " << valorCompra+calcular_frete(valorCompra)<<"\n";
    arquivo << "---\n";
    arquivo.close();
    cout << "Venda registrada com sucesso! Codigo gerado: "<< codigoV<< endl;
}