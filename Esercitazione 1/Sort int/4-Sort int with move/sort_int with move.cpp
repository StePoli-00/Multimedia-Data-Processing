//sono i generics in Java
#include <stdio.h>
/*ector della liberia standard, tutto funziona come prima
perchè tutte le cose nella libreria standard hanno un namespace
bisogna scrivere std::vector per utilizzare la classe vector della libreria standard*/
#include <vector>
#include <cassert>

/*scrivere un namespace personalizzato*/
namespace mdp {
    template <typename T >
    struct vector {
    private:
        T *nums_;
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
                other.nums_= nullptr;
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
                T *tmp = new  T[capacity_];
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
}
struct esempio {
    int val_;
    /*esempio() : val_(0) {};
    esempio(int val ) : val_(val) {};*/
    /* costruttore con valore di default, ce ne deve essere solo uno di default */
    esempio(int val = 0) : val_(val) {
        printf("esempio(%d)\n", val);
    };

};


/*possiamo scrivere class alla nostra struct, di default class fa si che tutto sia privato
* e specificare cosa è pubblico, invece nella struct di default è tutto pubblico
*/
using mdp::vector;
class vector<esempio> crea_elementi() {
    vector<esempio> out;
    out.push_back(6);
    out.push_back(3);
    out.push_back(2);
    return out;

}

auto crea_elementi(int x) {


    vector<esempio> a;
    a.push_back(6);
    a.push_back(3);
    a.push_back(2);

    vector<esempio> b;
    b.push_back(1);
    b.push_back(4);
    b.push_back(-2);


    if (x > 5)
    {
        /*non può creare l'oggetto di uscita a priori,
        ma non c'è bisogno di copiare a, possiamo rubare i suoi dati
        eseguendo la move
        */
        return a;
    }
    else {
        return b;
    }

}


int main(int argc, char** argv) {


    //using namespace mdp;riporto tutte le variabili nello scope globale
    vector <esempio> es;
    es = crea_elementi();
    //vector <esempio> es= crea_elementi() in questo modo verrebbe solo chiamato il costruttore;
    return 0;

    /*
    * costruttore di default di vector
    * costruttore di
    * assegnamento
    * distruttore out
    * distruttore es
    * se si ritorna un oggetto deve essere memorizzato da qualche parte.
    * tutte le volte che si passano parametri ad una funzione vengono copiati ed anche
    * quando vengono ritornate.
    */

    vector<esempio> es2 = es;
    vector<esempio> es3;
    es3 = es;


    /* es.capacity_; vorremo evitare di violare l'incapsulamento, vogliamo
       fare information hiding, aggiungendo private e public */



}
