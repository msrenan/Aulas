#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

void show_files(vector<string> prods) {
    int i = 0;
    cout << "------------------------------" << endl;
    for (string p : prods) {
        cout << "[" << i << "] "
        << p << endl;
        i++;
    }
    cout << "------------------------------" << endl;
}

int main() {
    ifstream rfile("produtos.txt");
    ofstream wfile;
    vector<string> produtos;

    if (rfile.is_open()) {
        string product;
        while (getline(rfile, product)) {
            if (product[0] == '#') {
                continue;
            }

            produtos.push_back(product);
            cout << "{" << product << "}" << "was loaded!" << endl;
        }
        cout << "All products was loaded." << endl;
        rfile.close();
    } else {
        rfile.close();
        wfile.open("produtos.txt");
        if (wfile.is_open()) {
            wfile << "# File created" << endl;
            wfile.close();
            cout << "Arquivo básico criado!" << endl;
        } else {
            wfile.close();
            cout << "Erro." << endl;
            return 1;
        }
    }

    
    bool showFile;
    string new_prod;
    while (1) {
        if (showFile) {
            show_files(produtos);
        }

        showFile = false;
        string option;
        
        cout << "[1] Ver produtos" << "\t"
        << "[2] Adicionar um Produto" << "\n"
        << "[3] Deletar Produto" << "\t"
        << "[4] Sair" << endl;

        cout << ">_ ";

        cin >> option;
        int o = 0;
        try {
            o = stoi(option);
        } catch (const invalid_argument& e) {
            cout << "Digite uma opção válida!";
        }

        new_prod = "";

        switch (o) {
            case 1:
                //Ver
                showFile = true;
                break;
            case 2:
                wfile.open("produtos.txt");
                cout << "Digite o nome do produto: ";
                cin >> new_prod;
                produtos.push_back(new_prod);
                for (string prod : produtos) {
                    wfile << prod << endl;
                }
                wfile.close();
                break;
            case 3:
                while (1) {
                    string idStr = "";
                    cout << "Digite o id do produto a ser deletado (ou -1 para sair): ";
                    cin >> idStr;
                    int id;
                    try {
                        id = stoi(idStr);
                    } catch (const invalid_argument& e) {
                        cout << "Digite um id válido!" << endl;
                    }

                    if (id == -1) {
                        break;
                    }

                    if (id < produtos.size() && id >= 0) {
                        
                    } else {
                        cout << "Digite um id válido!" << endl;
                    }
                }
                
                break;
            case 4:
                return 0;
                break;
            default:
                cout << "Digite uma opção válida!" << endl;
            
        }
         system("clear");
    } 
}