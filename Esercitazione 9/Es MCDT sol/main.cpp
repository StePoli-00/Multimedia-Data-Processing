//#include <unordered_map>
//#include <algorithm>
//#include <ranges>
//#include <functional>
//
//
///*
//    is.tellg ritorna la posizione all'interno di un file ritornando un fpos oggetto che funziona come un int o long
//	v.reserve allora semplicemente la memoria, ma la dimensione è ancora zero
//	v.resize:alloca la memoria e mette la size alla dimensione del vettore e costruisce tutti gli elementi
//	v(4)*/
//
//template<typename iT>
//double entropy(iT first, iT last)
//{
//	
//	/*first è un iteratore, *first=deferenziazione, punta al primo elemento dell'iteratore 
//	decltype=type of, resituisce il tipo
//	Se abbiamo un tipo di dato che è una reference a qualcosa, mediante remove_reference, otteniamo il tipo 
//	*first ritorna una reference, per togliere la reference usiamo remove_reference
//	type sarà uguale a short se passiamo un iteratore */
//	using type = std::remove_reference<decltype(*first)>::type;
//	/*restituisce la dimensione del contenitore, utilizzando distance che calcola la distanza
//	tra due iteratori*/
//	double tot = static_cast<double>(distance(fist, last));
//	/*costruiamo una mappa */
//	std::undordered_map<type, size_t> count;
//	/*classico algoritmo degli iteratori*/
//	while (first != last)
//	{
//		/*per ogni elemento puntato da first incrementa count e dopo inclementa first*/
//		++count[*first++];
//	}
//	dobule H = 0.0;
//	/*range based for,for sugli intervalli
//	auto &x:count x è una pair
//	auto [x, c] : count=spacchettamento della pair
//	può andare bene anche auto &[x, c] : count */
//	for (auto [x, c] : count)
//	{	
//		H += c * log2(c);/*è un modo analogo per ottenere l'entropia*/
//	}
//	return log2(tot) - H / tot;/*fa parte sempre  della analoga formula*/
//}
//
//int main() {
//
//	/*se abbiamo un oggetto che si comporta come una funzione
//	possiamo legare uno dei parametri ad un certo valore,
//	prendiamo la divides e facciamo il bindings dei suoi due parametri
//	_1=std::placeholder accettiamo un solo parametro
//	Q= dividiamo per Q*/
//	using namespace std::placeholders;
//	std::vector<int16_t> samples;
//	std::vector<int16_t> qsamples;
//	int Q = 2600;
//	//vedere quella dopo perchè è un pelo difficile
//	//std::transform(begin(samples), end(samples), begin(qsamples), std::bind(std::divides<int>{}, _1, Q)); 
//
//	/*								LAMDA FUNCTION
//	* <name_function>[](){}
//	[]=serve per memorizzare le variabili locali che servono dentro alla lambda function 
//	se si vuole far accedere a qualunque variabile locale definte fino ad ora, = 
//	()=parametri della funzione
//	{}=corpo della funzione 
//	es:[&Q] reference a Q
//	es [Q] copia di Q
//	le funzioni lambda sono in grado di capire il tipo di ritorno se ritornano sempre lo stesso tipo di dato.
//	Per specificare il tipo di ritorno <lamba function> -> <tipo di ritorno>
//	es: [Q](int val) -> double {return val / Q; };
//	*/
//	auto div_fn = [Q](int val) {return val / Q; };
//	/*transform old style*/
//	std::transform(begin(samples), end(samples), begin(qsamples), div_fn);
//	
//	/*transform new style
//	dentro questa libreria ci sono tutte delle funzione basate sui range=qualcosa su cui si può applicare begin e end,
//	*/
//
//	/*primo prametro è un range*/
//	std::ranges::transform(samples, begin(qsamples), [Q](int val) {return val / Q; });
//	/*ESEMPI DELLE FUNZIONI CON I RANGES*/
//	/*copiare il contenuto di un container all'interno di un altro*/
//	std::ranges::copy(samples, begin(qsamples));
//	/*ordinare elementi di un container*/
//	std::ranges::sort(samples);
//	/*views::transfofrm= dato in input un range,in output ritorna un range con la funzione applicata 
//	alla funzione specificata utilizzando la ridefinizione dell'operatore pipe '|'*/
//	std::ranges::copy(samples | std::views::transform([Q](int val) {return val / Q; }));
//
//}