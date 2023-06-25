#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <assert.h>
#include <algorithm>

template <typename T >
struct vector {
private:
        T* nums_;
    int n_;
    int capacity_; // capacity of nums array
public:
        vector() {
        printf("default constructor\n");
        nums_ = NULL;
        n_ = 0;
        capacity_ = 0;
    }
    vector(int initial_size) {
        //rimpiazzo della calloc, non li mette più a zero
        nums_ = new T[initial_size];
        n_ = initial_size;
        capacity_ = initial_size;
    }
    vector(const vector& other) {

        printf("copy constructor\n");
        n_ = other.n_;
        capacity_ = other.capacity_;
        nums_ = new T[capacity_];
        for (size_t i = 0; i < n_; ++i) {
            nums_[i] = other.nums_[i];
        }
    }
    vector& operator=(vector other) { // copy-and-swap idiom
        printf("assignment\n");
        swap(*this, other);
        return *this;
    }
    /*&& other è una r-value reference: other è un oggetto che sta per essere distrutto
    e sappiamo che non avrà più bisogno dei suoi dati, perciò non c'è bisogno
    di conservarlo.*/
    // ci andrebb noexept
    vector& operator=(const vector&& other) {
        printf("move assignment\n");
        if (this != &other) {
            delete[] nums_;
            n_ = other.n_;
            capacity_ = other.capacity_;
            nums_ = other.nums_;
            /*facciamo puntare ad other a qualcos'altro per evitare
            di eliminare i dati a cui ora this punta
            */
            other.nums_ = nullptr;
        }

        return *this;
    }

    friend void swap(vector& left, vector& right) {
        /*esiste una funzione std::swap, enable argumenta dependent lookup ADL,
        se esiste una funzione che è più specifica per il tipo, userà la
        nostra implementazione della swap*/
        using std::swap;
        swap(left.n_, right.n_);
        swap(left.capacity_, right.capacity_);
        swap(left.nums_, right.nums_);
    }
    ~vector() {
        delete[] nums_;
    }
    void push_back(const T num) {
        printf("push back\n");
        if (n_ == capacity_) {
            //alloco nuova memoria
            capacity_ = (capacity_ == 0 ? 1 : capacity_ * 2);
            /*Consente di evitare di scrivere il tipo di ritorno della funzione
            viene sostituito a tempo di compilazione*/
            //chiama il costruttore
            auto T* tmp = new  T[capacity_];
            if (tmp == NULL) {
                exit(EXIT_FAILURE);
            }
            //copio i dati vecchi
            for (int i = 0; i < n_; ++i)
            {
                tmp[i] = nums_[i];
            }
            delete[] nums_;
            nums_ = tmp;

        }
        nums_[n_] = num;
        n_++;
    }

    int size() const {
        return n_;
    }
    T at(int i) const {
        assert(i >= 0 && i < n_);
        return nums_[i];
    }

    T& operator[](int i) {
        return nums_[i];
    }
    const  T& operator[](int i) const {
        return nums_[i];
    }

};

struct mio_int {
    int val_;
    /*explicit serve per evitare conversioni di tipo implicito
        * es: mio_int = int oppure int = mio_int
        * */
    explicit mio_int(int val) : val_(val) {};
    /*ridefinizione di type casting, tipo()*/

    explicit operator int() {
        return val_;
    }
};

int func2(int argc, char** argv) {
    /*OPERATORI DI TYPECAST */
    int x = 4;
    //mio_int a = 5; dopo explicit non si potrà scrivere
    mio_int a(5);
    //il c++ converte impliciamente int in un mio_int per l'operatore di assegnamento
    a = mio_int(x);
    x = int(a);
    // x = a; questo non è posssibile farlo,possiamo definire l'operatore di casting
}



int main(int argc, char** argv)
{

    if (argc != 3)
    {
        //printf("Errore\n");
        //<< operatore shift a sinistra->si chiama inserter in c++
        /*scrive su stdout un const char zero terminato, chiama la printf
        se si da un un numero chiama la print con %d*/

        std::cout << "Errore numero di parametri\n";
        return 1;
    }


    //FILE* f = fopen(argv[1], "r");
    //input file stream,crea un input stream con una stringa
    std::ifstream is(argv[1]);
    //verifica apertura del file 
    if (!is) // is.fail())ritorna good or fail
    {
        std::cout << "Errore apertura file di lettura\n";
        return 1;
    }
    std::ofstream os(argv[2]);

    if (!os)
    {
        std::cout << "Errore apertura file di scrittura\n";
        return 1;
    }
    //non dobbiamo più ricordarci di chiudere i file


    std::vector<double> v;
    int i;
    double d;
    char c;
    //saltano TUTTI i whitespace
    is >> i >> d >> c;

    /*Altra versione del while leggi-controlla usa
    while(1){

    double num;
    is>>num;
    if(!is){
        break;
    }
    v.push_back(num);

    }*/
    //NON USARE !is.eof()

    /*versione compatta*/
    double num;
    while (is >> num)
    {
        v.push_back(num);
    }
    /*versione corretta ma prolissa
    while (1)
    {
        double num;
        //(fscanf(fin,"%lf",&num)==1) fa l'fscanf,senza dover specificare il tipo
        is >> num;
        /*se is >> num fallisce non ritorna un valore,
        ma lo stream stesso, perciò dobbiamo controllare lo stato
        dello stream */
        /*
        if (is.good())
        {
            v.push_back(num);
        }
        //controlliamo se siamo alla fine dei file
        else if (is.eof())
        {
            break;
        }
        else
        {
            std::cout << "Warning:incorrect data\n";
            break;
        }
    } */

    //prendo il container(vettore,lista) e gli chiedo dove comincia e dove finisce
    std::sort(v.begin(), v.end());
    //sort prende due elementi usando l'operatore minore

    /*Versione classica*/
    //for (size_t i = 0; i < v.size(); ++i)
    //{
    //    //l'operatore << ritorna lo stesso stream per riferimento, si può fare il chainig dell'operatore
    //    os <<v[i]<<'\n';
    //}

    /*For per iteratori vecchia versione */
    /*it è un iteratore definito in vector double*/
    //std::vector<double>::iterator start = v.begin();
    //std::vector<double>::iterator stop = v.end();
    //std::vector<double>::iterator it;
    //for (it = start; it != stop; ++it)
    //{
    //    //it si comporta come un puntatore
    //    os << *it << '\n';
    //}

    /*For per iteratori versione moderna*/
    auto start = v.begin();
    auto stop = v.end();
    std::vector<double>::iterator it;
    //se si sostituisse vector con una lista,funzionerebbe comunque
    for (auto it = start; it != stop; ++it)
    {
        double& x = *it;
        os << x << '\n';
    }

    /*For per iteratori versione moderna*/
    auto start = v.begin();
    auto stop = v.end();
    std::vector<double>::iterator it;
    //se si sostituisse vector con una lista,funzionerebbe comunque
    for (auto it = start; it != stop; ++it)
    {
        //double x=*it  fa la copia
        //& serve per non copiare la cosa puntata da it; 
        const auto& x = *it;
        os << x << '\n';
    }

    //For con range-based for
    auto start = v.begin();
    auto stop = v.end();
    std::vector<double>::iterator it;


    for (const auto x : v)
    {
        os << x << '\n';
    }

    /*copio il vettore dentro l'iteratore per l'output,
    l'iteratore ad output è a template, bisogna specificare il tipo di dato
    */
    std::copy(v.begin(), v.end(), std::ostream_iterator<double>(os, "\n"));

    //iterator per inputstream
    std::istream_iterator<double>start(is);
    std::istream_iterator<double>stop; //chiama il costruttore di default
    for (auto it = start; it != stop; ++it)
    {
        v.push_back(*it);
    }
    //iteratori adapter=si comportano come iteratori ma fanno altro
    //esistono iteratori che fanno automaticamente push_back
    std::copy(start, stop, std::back_inserter(v));


    //costruisco un vettore tramite inputstream
    std::vector<double> v(start, stop);



    return 0;








}