#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <thread>
#include <string>
#include <clocale>

using namespace std;
using namespace std::chrono;

// ================= CORES ANSI =================
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD    "\033[1m"

// ================= METRICS =================
struct Metrics{
    long long comparisons=0;
    long long swaps=0;
    long long time_us=0;
};

// ================= UI =================
void printHeader(string text){
    cout << BOLD << CYAN;
    cout << "\n====================================================\n";
    cout << " " << text << endl;
    cout << "====================================================\n";
    cout << RESET;
}

void loading(string msg){
    cout << YELLOW << msg;
    for(int i=0;i<3;i++){
        cout << ".";
        cout.flush();
        this_thread::sleep_for(milliseconds(250));
    }
    cout << " " << GREEN << "[OK]\n" << RESET;
}

// ================= INSERTION =================
Metrics insertionSort(vector<int>& arr){

    Metrics m;
    auto start=high_resolution_clock::now();

    for(int i=1;i<arr.size();i++){

        int key=arr[i];
        int j=i-1;

        while(j>=0){
            m.comparisons++;
            if(arr[j]>key){
                arr[j+1]=arr[j];
                m.swaps++;
                j--;
            }else break;
        }
        arr[j+1]=key;
        m.swaps++;
    }

    auto end=high_resolution_clock::now();
    m.time_us=duration_cast<microseconds>(end-start).count();

    return m;
}

// ================= QUICK =================
void quickHelper(vector<int>& arr,int low,int high,Metrics& m){

    if(low>=high) return;

    int pivot=arr[high];
    int i=low-1;

    for(int j=low;j<high;j++){
        m.comparisons++;
        if(arr[j]<pivot){
            i++;
            swap(arr[i],arr[j]);
            m.swaps++;
        }
    }

    swap(arr[i+1],arr[high]);
    m.swaps++;

    int pi=i+1;

    quickHelper(arr,low,pi-1,m);
    quickHelper(arr,pi+1,high,m);
}

Metrics quickSort(vector<int>& arr){
    Metrics m;

    auto start=high_resolution_clock::now();
    quickHelper(arr,0,arr.size()-1,m);
    auto end=high_resolution_clock::now();

    m.time_us=duration_cast<microseconds>(end-start).count();

    return m;
}

// ================= STD SORT =================
Metrics stdSortWrapper(vector<int> arr){

    Metrics m;

    auto start=high_resolution_clock::now();
    sort(arr.begin(),arr.end());
    auto end=high_resolution_clock::now();

    m.time_us=duration_cast<microseconds>(end-start).count();
    m.comparisons=-1;
    m.swaps=-1;

    return m;
}

// ================= PRINT =================
void printMetrics(string name,Metrics m){

    cout << GREEN << "► " << name << RESET
         << " | Tempo: " << CYAN << m.time_us << " us" << RESET
         << " | Comp: " << MAGENTA << m.comparisons << RESET
         << " | Swaps: " << YELLOW << m.swaps << RESET
         << endl;
}

// ================= MAIN =================
int main(){

    setlocale(LC_ALL,"");

    mt19937 rng(42);

    vector<Metrics> ins(3),qui(3),stdm(3);

    // ================= ETAPA 1 =================
    printHeader("ETAPA 1 - AQUECIMENTO (50)");
    loading("Gerando numeros aleatorios");

    vector<int> c1(50);
    uniform_int_distribution<int> d1(1,1000);

    for(auto& x:c1) x=d1(rng);

    auto a1=c1,b1=c1,c1std=c1;

    ins[0]=insertionSort(a1);
    qui[0]=quickSort(b1);
    stdm[0]=stdSortWrapper(c1std);

    printMetrics("Insertion Sort",ins[0]);
    printMetrics("Quick Sort",qui[0]);
    printMetrics("std::sort",stdm[0]);

    // ================= ETAPA 2 =================
    printHeader("ETAPA 2 - AGULHA NO PALHEIRO (10k)");

    vector<int> c2(10000);
    for(int i=0;i<10000;i++) c2[i]=i+1;

    loading("Aplicando bagunca");

    uniform_int_distribution<int> d2(0,9999);

    cout << BLUE << "Indices sorteados:\n" << RESET;

    for(int i=0;i<10;i++){
        int a=d2(rng);
        int b=d2(rng);
        cout << "("<<a<<","<<b<<") ";
        swap(c2[a],c2[b]);
    }
    cout << endl;

    auto a2=c2,b2=c2,c2std=c2;

    ins[1]=insertionSort(a2);
    qui[1]=quickSort(b2);
    stdm[1]=stdSortWrapper(c2std);

    printMetrics("Insertion Sort",ins[1]);
    printMetrics("Quick Sort",qui[1]);
    printMetrics("std::sort",stdm[1]);

    // ================= ETAPA 3 =================
    printHeader("ETAPA 3 - PIOR CASO (20k)");
    loading("Gerando vetor invertido");

    vector<int> c3(20000);
    for(int i=0;i<20000;i++)
        c3[i]=20000-i;

    auto a3=c3,b3=c3,c3std=c3;

    ins[2]=insertionSort(a3);
    qui[2]=quickSort(b3);
    stdm[2]=stdSortWrapper(c3std);

    printMetrics("Insertion Sort",ins[2]);
    printMetrics("Quick Sort",qui[2]);
    printMetrics("std::sort",stdm[2]);

    // ================= ETAPA 4 =================
    printHeader("TABELA FINAL DE PERFORMANCE");

    string cenarios[3]={"50","10k","20k"};

    cout<<BOLD<<left<<setw(12)<<"Algoritmo"
        <<setw(10)<<"Caso"
        <<setw(12)<<"Tempo(us)"
        <<setw(15)<<"Comparacoes"
        <<setw(10)<<"Trocas"<<RESET<<endl;

    cout<<CYAN<<"----------------------------------------------------"<<RESET<<endl;

    for(int i=0;i<3;i++){

        cout<<GREEN<<setw(12)<<"Insertion"<<RESET
            <<setw(10)<<cenarios[i]
            <<setw(12)<<ins[i].time_us
            <<setw(15)<<ins[i].comparisons
            <<setw(10)<<ins[i].swaps<<endl;

        cout<<MAGENTA<<setw(12)<<"Quick"<<RESET
            <<setw(10)<<cenarios[i]
            <<setw(12)<<qui[i].time_us
            <<setw(15)<<qui[i].comparisons
            <<setw(10)<<qui[i].swaps<<endl;

        cout<<BLUE<<setw(12)<<"std::sort"<<RESET
            <<setw(10)<<cenarios[i]
            <<setw(12)<<stdm[i].time_us
            <<setw(15)<<"N/A"
            <<setw(10)<<"N/A"<<endl;

        cout<<CYAN<<"----------------------------------------------------"<<RESET<<endl;
    }

    printHeader("ANALISE CONCLUIDA");

    return 0;
}