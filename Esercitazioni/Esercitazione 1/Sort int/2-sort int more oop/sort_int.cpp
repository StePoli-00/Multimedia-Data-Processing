#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <utility> 

int cmpfunc(const void* a, const void* b)
{
    int32_t x = *(int32_t*)a;
    int32_t y = *(int32_t*)b;
    if (x < y) {
        return -1;
    }
    else if (x > y) {
        return 1;
    }
    else {
        return 0;
    }
}

//viene definito un typedef in automatico
struct vector {
    int32_t* nums_; //si aggiunge _ per indicare che è una variabile della classe 
    int n_;
    int capacity_; // capacity of nums array

    /* this-> non serve se siamo all'interno di vector nome_classe
    i costruttori non hanno tipo di ritorno */
    vector() {
        nums_ = NULL;
        n_ = 0;
        capacity_ = 0;
    }

    /*in c++ non si passano mai gli oggetti nel modo seguente
    vector(vector other)
    {

    }
    */
    /*costruttore di copia=passiamo una reference e aggiungiamo const per
    indicare che non modifica il vettore passato*/
    vector(const vector& other)
    {
        n_ = other.n_;
        capacity_ = other.capacity_;
        nums_ = (int32_t*)malloc(capacity_ * sizeof(int32_t));

        for (size_t i = 0; i < n_; ++i)
        {
            nums_[i] = other.nums_[i];
        }

    }

    //vector &operator =(const vector& other) {
    //    //controllo il self assignment-> controllo se hanno lo stesso indirizzo
    //    // &other-> indirizzo di 
    //    if (this != &other) {
    //        if (capacity_ < other.n_) {
    //            free(nums_);
    //            capacity_ = other.capacity_;
    //            capacity_ = other.capacity_;
    //            nums_ = (int32_t*)malloc(capacity_ * sizeof(int32_t));
    //        }
    //        //distruggo il vecchio vettore: libero la memoria 
    //        n_ = other.n_;
    //        for (size_t i = 0; i < n_; ++i)
    //        {
    //            nums_[i] = other.nums_[i];
    //       }           
    //        //copio num di elementi e capacità               
    //    }
    //    /*Per risolvere il problema dell'assegnamento multiplo, devo ritornare l'oggetto qualcosa
    //    devo dereferenziare e ritornare this*/
    //    return *this;
    //}

    /*modo diverso di scrivere il metodo di prima
    copy and swap idiom*/
    vector& operator =(vector& other) {
        //viene eseguito il costruttore di copia
        swap(*this, other);
        return *this;

    }
    /*funzione amica della classe che può violare l'incapsulamento.
    Il modo corretto per scrivere il copy and swap idiom*/
    friend void swap(vector& left, vector& right)
    {
        using std::swap;
        std::swap(left.n_, right.n_);
        std::swap(left.capacity_, right.capacity_);
        std::swap(left.nums_, right.nums_);
    }


    vector(int initial_size) {
        nums_ = (int32_t*)calloc(initial_size, sizeof(int32_t));
        n_ = initial_size;
        capacity_ = initial_size;
    }

    /*distruttore ~nome_classe*/
    ~vector() {
        free(this->nums_);
    }
    void push_back(int32_t num) {
        if (this->n_ == this->capacity_) {
            this->capacity_ = (this->capacity_ == 0 ? 1 : this->capacity_ * 2);
            //se si vuole trasformare un puntatore void in altro bisogna metterlo in esplicito
            this->nums_ = (int32_t*)realloc(this->nums_, this->capacity_ * sizeof(int32_t));
            if (this->nums_ == NULL) {
                printf("Error: failed to allocate memory.\n");
                exit(EXIT_FAILURE);
            }
        }
        this->nums_[this->n_] = num;
        this->n_++;
    }
    void sort() {
        qsort(this->nums_, this->n_, sizeof(int32_t), cmpfunc);
    }
    //serve che il metodo non modifica i dati
    int size() const {
        return this->n_;
    }

    /*ridefinizione dell'operatore []
    in questo modo possiamo scrivere v[i], al posto di v.at(i)
    uso una reference per consentire l'assegnamento v[0]=5.
    Facendo ciò ho commesso un errore.
     int32_t &operator[](int i) const {
        return nums_[i];
    }
    l'operatore [] dovrebbe consentire di
    non modificare il valore, ma ritornando una reference concediamo di farlo.
    togliamo il const*/


    /*versione per modificare l'oggetto->overload
     ha un parametro nascosto al oggetto vector: this*/
    int32_t& operator[](int i) {
        return nums_[i];
    }

    /*versione per non modificare l'oggetto-> overload
    riferimento al vettore const*/
    const int32_t& operator[](int i) const {
        return nums_[i];
    }
    //stesso ragionamento per size
    int32_t at(int i) const {
        assert(i >= 0 && i < n_);
        return this->nums_[i];
    }


};

/*&val è una reference ad int, val è un puntatore,
ma all'interno della funzione viene usata come se fosse una variabile,
sostanzialmente è un alias */
void raddoppia(int& val) {

    val *= 2;
}

void scrivi_vettore(FILE* f, const vector& v)
{

    for (int i = 0; i < v.size(); i++) {
        fprintf(f, "%" PRId32 "\n", v[i]);
    }

}

struct numero {
    int val_;
    numero() {
        val_ = 7;
    }
};

int main(int argc, char* argv[])
{

    //numero* pn=(numero *) malloc(sizeof(numero)) la malloc alloca un pezzo di memoria, ma non chiama il costruttore
    numero* pn = new numero; //fa la malloc e chiama il costruttore dell'oggetto
    /*allocazione multipla*/
    int q = pn->val_;
    numero* pn2 = new numero[10];


    delete pn; //distrugge la memoria
    delete[] pn2;//delete per più oggetti

    if (argc != 3) {
        printf("Usage: sort_int <filein.txt> <fileout.txt>\n");
        return 1;
    }

    FILE* fin = fopen(argv[1], "r");
    if (fin == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    FILE* fout = fopen(argv[2], "w");
    if (fout == NULL) {
        printf("Error opening output file.\n");
        fclose(fin);
        return 1;
    }



    /*Notazioni del costruttore */
    /*v::costructor::definisce il namespace di dove è definito un metodo, non supportato dal linguaggio*/
    vector v;
    vector a = 10; //costruisce un vettore con initial capacity 10
    vector b(10); //
    vector c{ 10 }; //passo 10 al costruttore, uguale alle parentesi tonde
    vector d(); /* è la def di un puntatore and una function d() senza parametri,
    per definire un d senza parametri usiamo d senza()*/

    /*& (reference) != puntatori,
    non si può cambiare la cosa a cui fanno riferimento,
    sono puntatori fissati al momento
    della loro creazione, esempio dell'uso della reference*/

    int x = 2;
    raddoppia(x);



    while (1) {
        int32_t num; //dichiarazione
        if (fscanf(fin, "%" SCNd32, &num) == 1) {
            v.push_back(num); //expression statement
        }
        else if (feof(fin)) {
            break;
        }
        else {
            printf("Warning: incorrect data in input file.\n");
            break;
        }
    }



    /*Copia shallow di un vettore vector w = v;*/
    vector w = v; //dopo aver aggiunto il costruttore che prende un vector, questa istruzione funziona e copia il vettore 

    //vector a(10); //inizializzazione
    //a = w; //expression statement, assegnamento effettua una shallow copy
    int e = 5, f = 7, g = 34;
    (e = f) = 8; //e=f fa e a cui viene assegnato il valore 8 
    //problema da gestire
    e = f = g = 5;

    a = w = v;


    /*se scrivessimo questo assegnamento,
    v[0] = 5;
    operatore [] non si comporta come volevamo,v[0]
    non è lvalue(dev'essere una variabile e deve avere un indirizzo di memoria).
    l'operatore [] ritorna un valore e non un indirizzo*/


    v.sort();
    /*dopo la free, c'è un errore, c'è una copia di ogni elemento di vector
    la copia tra struct è shallow copy, quando si vuole fare la copia di oggetti
    si vuole che i due oggetti siano diversi */



    scrivi_vettore(fout, v);

    fclose(fin);
    fclose(fout);
    //il distruttore viene chiamato dopo la return 
    return 0;
}