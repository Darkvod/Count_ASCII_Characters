#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

class DataRec {
public:

    char symbol;
    unsigned int runLen;
    unsigned long freq;

    DataRec() = default;

    DataRec(char symbol, char freq) {
        this->symbol = symbol;
        this->freq = freq;
        this->runLen = 1;
    }


};

//funkcja zlicza czestotliwosc i powtarzalnosc znakow
void zliczanie(char ch,char poprzedni, vector<DataRec> &data) {
    for (auto &x : data) {
        if (ch == x.symbol) {
            if (poprzedni == ch) {
                x.runLen++;
            } else {
                x.freq++;
            }
        }
    }
}


//wypisuje wyniki w konsoli i zapisuje do pliku
void wypisz_zapisz(string nazwaPliku_wyjsciowego, vector<DataRec> &data, int size){

    fstream plik_wy;
    plik_wy.open(nazwaPliku_wyjsciowego,ios::out);
    string s;
    for (auto x : data) {
        s = x.symbol;
        if (x.freq == 0) continue;
        if (x.symbol == 0) continue;
        if (x.symbol == 0xA) s = "LF";
        if (x.symbol == 9) s = "TB";
        if (x.symbol == 0xD) s = "CR";
        if (x.symbol == 0x20) s = "SP";

        cout << "0x" << hex << uppercase << (int)x.symbol <<
             dec << "," << s << "," << x.runLen << ":" << x.freq << "\n";


        plik_wy << "0x" << hex << uppercase << (int) x.symbol <<
                dec << "," << s << "," << x.runLen << ":" << x.freq << "\n";
    }

    //entropia
    double pi = 0.0, ent=0.0;
    for(auto x : data){
        if (x.freq == 0) continue;
        pi = (double)x.freq/size;
        ent -= pi * log2(pi);
    }
    cout << endl <<"Entropia: " << ent;

    plik_wy << endl <<"Entropia: " << ent;

    plik_wy.close();
}


const string nazwaPliku = "sample_input_file.txt";
const string nazwaPliku_wyjsciowego = "output.dat";
int main() {

    vector<DataRec> data;

    data.reserve(126);

    // wypelnianie wektora znakami ascii
    for (int i = 0; i < 126; i++) {
        data.emplace_back(i, 0);
    }


    // otwarcie pliku
    ifstream plik_we;
    plik_we.open(nazwaPliku, ios::binary);
    plik_we.seekg(0, ios::end);
    int size = plik_we.tellg();

    vector<char> dane;              // wektor przechowuje tablice znakow zczytaną z pliku

    dane.resize(size);
    plik_we.seekg(0, std::ios::beg);
    plik_we.read(&dane[0], size);

    // zliczanie
    char ch;
    char poprzedni;
    int i=0;
    while (i < size) {
        ch = dane[i];
        zliczanie(ch,poprzedni,data);
        poprzedni = ch;
        i++;
    }

    plik_we.close(); // zamkniecie pliku


    // wypisanie w konsoli
    wypisz_zapisz(nazwaPliku_wyjsciowego,data,size);



    return 0;
}
