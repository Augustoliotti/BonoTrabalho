#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

// =========================================================
// VARIAVEIS GLOBAIS DE RASTREAMENTO
// =========================================================
int totalDivisoes = 0;
int totalMerges   = 0;
int passoMerge    = 0;

// =========================================================
// A LOGICA DA CONQUISTA (FUNCAO MERGE)
// Responsavel por unir dois pedacos que ja estao ordenados.
// =========================================================
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) { arr[k] = L[i]; i++; }
        else               { arr[k] = R[j]; j++; }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }

    // RASTREAMENTO DO MERGE (SUBIDA)
    totalMerges++;
    passoMerge++;
    int bufferSize = n1 + n2;

    cout << "  [Merge #" << passoMerge << "] Resultado: [";
    for (int x = l; x <= r; x++) {
        cout << arr[x];
        if (x < r) cout << ", ";
    }
    cout << "]  | Buffer de memoria extra usado: " << bufferSize << " elemento(s)" << endl;
}

// =========================================================
// A LOGICA DA DIVISAO (FUNCAO RECURSIVA)
// Responsavel por quebrar o problema em partes menores.
// =========================================================
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // RASTREAMENTO DA DIVISAO (DESCIDA)
        totalDivisoes++;
        cout << "  [Divisao #" << totalDivisoes << "]"
             << "  Esquerda: [";
        for (int x = l; x <= m; x++) {
            cout << arr[x];
            if (x < m) cout << ", ";
        }
        cout << "]  |  Direita: [";
        for (int x = m + 1; x <= r; x++) {
            cout << arr[x];
            if (x < r) cout << ", ";
        }
        cout << "]" << endl;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// =========================================================
// FUNCAO PRINCIPAL
// =========================================================
int main() {
    // ---- Dados do aluno (preencha com seus dados) ----
    string nomeAluno   = "Augusto Liotti";
    string codigoAluno = "2267893";

    // ---- Geracao do array com 20 numeros aleatorios sem repeticao ----
    srand((unsigned int)time(nullptr));

    const int TAM = 20;
    vector<int> pool;
    for (int i = 1; i <= 100; i++) pool.push_back(i);
    for (int i = (int)pool.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(pool[i], pool[j]);
    }

    int data[TAM];
    for (int i = 0; i < TAM; i++) data[i] = pool[i];

    // Salva array original antes de ordenar
    int original[TAM];
    for (int i = 0; i < TAM; i++) original[i] = data[i];

    // Exibe array sorteado
    cout << "Array sorteado: [";
    for (int i = 0; i < TAM; i++) {
        cout << data[i];
        if (i < TAM - 1) cout << ", ";
    }
    cout << "]" << endl << endl;

    cout << "=== RASTREAMENTO DA EXECUCAO ===" << endl;
    mergeSort(data, 0, TAM - 1);

    // Relatorio final
    cout << endl;
    cout << "=======================================" << endl;
    cout << "Aluno: "              << nomeAluno       << endl;
    cout << "Codigo Aluno: "       << codigoAluno     << endl;
    cout << "Array sorteado: [";
    for (int i = 0; i < TAM; i++) {
        cout << original[i];
        if (i < TAM - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "Numero de divisoes: " << totalDivisoes   << endl;
    cout << "Numero de merges: "   << totalMerges     << endl;
    cout << "Array ordenado: [";
    for (int i = 0; i < TAM; i++) {
        cout << data[i];
        if (i < TAM - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "=======================================" << endl;

    return 0;
}