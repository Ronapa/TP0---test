// Template de arreglo dinamico

#ifndef ARRAYDIN_H
#define ARRAYDIN_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdio.h>

#define ARRAY_DEFAULT_SIZE 1
#define ARRAY_GROWTH_RATE 5

using namespace std;

template <typename T>
class Array
{
public:
	Array();
	Array(size_t n); 
	Array( const Array<T> & ); 
	~Array( ); 
	size_t 		size( ) const; 
	Array<T>&	operator=( const Array<T> & ); 
	bool 		operator==( const Array<T> & ) const; 
	bool 		operator!=( const Array<T> & ) const; 
	T &		operator[ ]( int );
	T const &	operator[ ]( int ) const;
	void		push_back(const T&);
	int 		linear_search(T&);
	void		clear();
	template <typename TT>
	friend std::ostream& operator<<(std::ostream&,const Array<TT>&);
	template <typename TT>
	friend std::istream& operator>>(std::istream&,Array<TT>&);

private:
	size_t alloc_size_; 
	size_t used_size_;
	T *ptr_; 

	void 		resize(size_t);
};

template <typename T> 
Array<T>::Array()
{
	// Desde afuera, para agregar elementos al arreglo habrá que hacer push_back

	alloc_size_ = ARRAY_DEFAULT_SIZE;
	used_size_ = 0;
	ptr_ = new T[alloc_size_];
}

template <typename T> 
Array<T>::Array(size_t n)
{
	// Si no se inicializan sus valores, están indeterminados (aunque
	// muchos compiladores suelen completar con ceros).
	// A su vez, es bueno notar que el usuario tiene a disposición todo el
	// arreglo (n elementos).

	alloc_size_ = n;
	used_size_ = n;
	ptr_ = new T[alloc_size_];
}

template <typename T> 
Array<T>::Array( const Array<T> &init )
{
	// Se asume que existe el operador = en la clase T, lo que permite
	// usar esta función para todo tipo de clases

	used_size_ = init.used_size_;
	alloc_size_ = init.alloc_size_;
	ptr_ = new T[ alloc_size_ ];
	for ( size_t i = 0; i < used_size_; i++ )
		ptr_[ i ] = init.ptr_[ i ]; 
}

template <typename T> 
Array<T>::~Array()
{
	if (ptr_)
		delete[] ptr_; 
}

template <typename T> 
size_t Array<T>::size() const { return used_size_; }

template <typename T> 
Array<T>& Array<T>::operator=( const Array<T> &rhs )
{
	T* aux;
	// Nomenclatura: rhs es right hand side (lado derecho), lhs (que acá no 
	// aparece) es left hand side (lado izquierdo)

	// Primero, comprobamos por la igualdad de la identidad, que
	// es una forma estilizada de decir que son la misma cosa, a diferencia
	// de la equivalencia matemática (igualdad de valor)

	if ( &rhs == this ) {
		return *this;
	}
	// Después, cambiamos el tamaño del arreglo si es necesario y procedemos a copiar

	if ( alloc_size_ != rhs.alloc_size_ ){
		aux = new T[ rhs.alloc_size_ ]; 
		delete [] ptr_; 
		ptr_ = aux;
		alloc_size_ = rhs.alloc_size_; 
		used_size_ = rhs.used_size_;
	}
	used_size_ = rhs.used_size_;
	for ( size_t i = 0; i < used_size_; i++ )
		ptr_[ i ] = rhs.ptr_[ i ]; 
	return *this;
}
template <typename T> 
bool Array<T>::operator==( const Array<T> &rhs ) const
{
	// Primero reviso si tienen igual tamaño. Si no, no pueden ser iguales.
	// Luego reviso miembro a miembro si estos son iguales.
	// Requiere que la clase tenga el operador !=

	if ( used_size_ != rhs.used_size_ )
		return false; 
	else{
		for ( size_t i = 0; i < used_size_; i++ ){
			if ( ptr_[ i ] != rhs.ptr_[ i ] )
       				return false; 
		}
	}
       	return true; 
}

template <typename T> 
bool Array<T>::operator!=( const Array<T> &rhs ) const
{
	// Si no son iguales son distintos
	if ( rhs == *this )
		return false;
	else
		return true;
}

template <typename T> 
T & Array<T>::operator [ ]( int pos )
{
	// Utilizamos assert para saber si se puede acceder al elemento.
	// Si no se puede, detiene el programa mandando la señal SIGABRT y dirá 
	// que fallo acá

	assert( (0 < pos) || (pos < used_size_) ) ; 
	return ptr_[ pos ]; 
}

template <typename T> 
const T & Array<T>::operator [ ]( int pos ) const
{
	// La diferencia acá es que tiene los operadores de const.
	// Es necesario si se llama desde dentro de una función que es const
	// El compilador se dará cuenta de cual usar

	assert( (0 < pos) || (pos < used_size_) ) ; 
	return ptr_[ pos ]; 
}

template <typename T> 
void Array<T>::clear()
{
	delete[] ptr_;
	alloc_size_ = ARRAY_DEFAULT_SIZE;
	used_size_ = 0;
	ptr_ = new T[alloc_size_];
}

template <typename T> 
void Array<T>::resize(size_t new_size)
{
	// Redimensiono el arreglo, y copio todo hasta donde puedo.
	// Desde afuera no se ve el redimensionamiento

	T *aux;

	aux = new T[new_size];
	if( new_size < used_size_ ){
		used_size_ = new_size;
	}
	for( size_t i=0;i<used_size_;++i){
		aux[i] = ptr_[i];
	}
	delete [] ptr_;
	ptr_ = aux;
	alloc_size_ = new_size;
}

template <typename T> 
void Array<T>::push_back(const T &new_thing)
{
	// Si es necesario agrandar el arreglo ya que no queda más espacio, lo
	// agrando por 2.
	// La decisión de cuando agrandar puede variar, ya que puede ser cuando 
	// el tamaño es la mitad del reservado, por ejemplo.
	// Al agrandar, copio todos los elementos del arreglo

	if(alloc_size_ == used_size_){
		this->resize(alloc_size_*ARRAY_GROWTH_RATE);
	}	
	ptr_[used_size_] = new_thing;
	used_size_++;
}

template <typename T> 
std::ostream & operator<< (std::ostream& os,const Array<T> & arr)
{
	// Escribe el arreglo en el formato (T1,T2,T3 ... Tn)
	// Se asume que existe el operador << para la clase T
	
	if( arr.size() == 0 ){
		os << "()";
		return os;
	}
	os << "(";

	for(size_t i=0; i<arr.size()-1; ++i){
		os << arr[i] << ",";
	}
	os << arr[arr.size()-1];
	os << ")";
	return os;
}

template <typename T>
std::istream & operator>> (std::istream& is,Array<T>& arr)
{
	// Limpio el arreglo y leo en formato (T1,T2,...,Tn) de is. Si no se hace conforme a lo
	// esperado, limpio el arreglo (devuelvo uno sin elementos)
	// Si llega a EOF, marcará en el istream

	T aux = 0;
	char ch = 0;

	arr.clear();
	if( (is >> ch) && (ch == '(') && (is >> aux) ){
		arr.push_back(aux);
		while( (is >> ch) && (ch == ',') && (is >> aux) ){
			arr.push_back(aux);
		}
	} 
	if ( ch != ')' ){
		arr.clear();
	}

	return is;
}

template <typename T>
int Array<T>::linear_search(T& objective)
{
	// Devuelve la posición del objeto encontrado o -1 si no lo encontró

	if(this->size()==0)
		return -1;
	for(size_t i=0;i<this->size();++i){
		if((*this)[i] == objective)
			return i;
	}
	return -1;
}

#endif
