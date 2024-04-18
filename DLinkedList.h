DLinkedList:

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include <iostream>
#include <stdexcept>
#include "DLLNode.h"

template <class T>
class DLinkedList {
  private:
    DLLNode<T> *head;
    DLLNode<T> *tail;
    int numElements;
    // Contadores de swaps
    int quickSwaps;
    int mergeSwaps;

  public:
    DLinkedList();
    ~DLinkedList();
    int getNumElements();
    void printList();
    void printReverseList();
    void addFirst(T value);
    void addLast(T value);
    bool deleteData(T value);
    bool deleteAt(int position);
    T getData(int position);
    void updateData(T value, T newValue);
    // TO-DO act 2.2
    void updateAt(int position, T newValue);
    int findData(T value);
    void invert();
    int getProductSublist(int ini, int fin);
    // https://www.geeksforgeeks.org/quicksort-for-linked-list/

// Actividad Integradora 2
    int getNumSwapsMerge() {return mergeSwaps;}
    int getNumSwapsQuick() {return quickSwaps;}

    void quickSort() {
      // Reinicia el contador
      quickSwaps = 0; 
      // Llama a la función quicksort recursiva
      quick_sort(head, tail);
    }
    void mergeSort() {
      // Reinicia el contador
      mergeSwaps = 0; 
      merge_sort(&head);
    }

  private:
    // Quick sort 
    DLLNode<T>* partition(DLLNode<T>* low, DLLNode<T>* high);
    void quick_sort(DLLNode<T>* low, DLLNode<T>* high);
    void swap(T& a, T& b);

    // Merge sort
    void merge_sort(DLLNode<T>** headRef);
    DLLNode<T>* merge(DLLNode<T>* left, DLLNode<T>* right);
};

// Implementaición de quickSort
template <class T>
void DLinkedList<T>::quick_sort(DLLNode<T>* low, DLLNode<T>* high) {
  if (low != nullptr && high != nullptr && low != high && low != high->next) {
    DLLNode<T>* pivot = partition(low, high);
    quick_sort(low, pivot->prev);
    quick_sort(pivot->next, high);
  }
}

// Implementación de mergeSort
template <class T>
void DLinkedList<T>::merge_sort(DLLNode<T>** headRef) {
  DLLNode<T>* head = *headRef;
  DLLNode<T>* left;
  DLLNode<T>* right;

  // Lista vacía o con un nodo
  if (head == nullptr || head->next == nullptr) {
    return;
  }

  DLLNode<T>* slow = head;
  DLLNode<T>* fast = head->next;
  while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;
    fast = fast->next->next;
  }

  left = head; 
  right = slow->next;
  slow->next = nullptr;

  // Ordena cada mitad
  merge_sort(&left);
  merge_sort(&right);
  // Combina las mitades 
  *headRef = merge(left, right);
}

// Implementación de partition
template<class T>
DLLNode<T>* DLinkedList<T>::partition(DLLNode<T>* low, DLLNode<T>* high) {
  T pivotValue = high->data;
  DLLNode<T>* i = low->prev;

  for (DLLNode<T>* j = low; j != high; j = j->next) {
    if (j->data <= pivotValue) {
      i = (i == nullptr) ? low : i->next;
      swap(i->data, j->data);
      quickSwaps++;

    }
  }

  i = (i == nullptr) ? low : i->next;
  swap(i->data, high->data);

  return i;
}

// Implementación de merge 
template <class T>
DLLNode<T>* DLinkedList<T>::merge(DLLNode<T>* left, DLLNode<T>* rigth) {
  DLLNode<T>* result = nullptr;

  if (left == nullptr) {
    return rigth;
  }
  else if (rigth == nullptr) {
    return left;
  }

  if (left->data <= rigth->data) {
    result = left;
    result->next = merge(left->next, rigth);
    mergeSwaps++;
  }
  else {
    result = rigth;
    result->next = merge(left, rigth->next);
  }

  return result;
}

// Implementación de swap 
template <class T>
void DLinkedList<T>::swap(T& a, T& b) {
  T temp = a;
  a = b;
  b = temp;
}


// Complejidad O(1)
template <class T>
DLinkedList<T>::DLinkedList() {
  std::cout << "--> Creando una lista doblemente ligada vacia: " << this << std::endl; 
  head = nullptr;
  tail = nullptr;
  numElements = 0;
}

// Complejidad O(n)
template <class T>
DLinkedList<T>::~DLinkedList() {
  std::cout << "--> Liberando memoria de la lista doblemente ligada: " << this << std::endl;
  DLLNode<T> *p, *q;
  p = head;
  while (p != nullptr) {
    q = p->next;
    delete p;
    p = q;
  }
  head = nullptr;
  tail = nullptr;
  numElements = 0;
}

// Complejidad O(1)
template <class T>
int DLinkedList<T>::getNumElements() {
  return numElements;  
}

// Complejidad O(n)
template <class T>
void DLinkedList<T>::printList() {
  if (head == nullptr && tail == nullptr)
    std::cout << "La lista esta vacia" << std::endl;
  else {
    std::cout << "El contenido de la lista es: " << std::endl;
    DLLNode<T> *p = head;
    while(p != nullptr) {
      std::cout << p->data << " ";
      p = p->next;
    }
    std::cout << std::endl;
  }    
}

// Complejidad O(n)
template <class T>
void DLinkedList<T>::printReverseList() {
  if (head == nullptr && tail == nullptr)
    std::cout << "La lista esta vacia" << std::endl;
  else {
    std::cout << "El contenido inverso de la lista es: " << std::endl;
    DLLNode<T> *p = tail;
    while(p != nullptr) {
      std::cout << p->data << " ";
      p = p->prev;
    }
    std::cout << std::endl;
  }    
}

// Complejidad O(1)
template <class T>
void DLinkedList<T>::addFirst(T value) {
  // 1. crear nuevo nodo
  DLLNode<T> *newNode = new DLLNode<T>(value);
  // si la lista esta vacia
  if (head == nullptr && tail == nullptr) {
    head = newNode;
    tail = newNode;
  }
  else {
    // 2. apuntar newNode->next al nodo apuntado por head
    newNode->next = head;
    // 3. apuntar head->prev a newNode
    head->prev = newNode;
    // 4. actualizar head para apuntar a newNode
    head = newNode;
  }
  numElements++;
}

// Complejidad O(1)
template <class T>
void DLinkedList<T>::addLast(T value) {
  // La lista esta vacia
  if (head == nullptr && tail == nullptr)
    addFirst(value);
  else {
    // 1. crear nuevo nodo
    DLLNode<T> *newNode = new DLLNode<T>(value);
    // 2. apuntar tail->next a newNode
    tail->next = newNode;
    // 3. apuntar newNode->prev a tail
    newNode->prev = tail;
    // 4. actualizar tail para apuntar a newNode
    tail = newNode;
    numElements++;
  }
}

// Complejidad O(n)
template <class T>
bool DLinkedList<T>::deleteData(T value) {
  // La lista esta vacia
  if (head == nullptr && tail == nullptr) {
    std::cout << "La lista esta vacia" << std::endl;
    return false;
  }
  else {
    // Buscar value en la lista
    DLLNode<T> *p = head;
    DLLNode<T> *previous = nullptr;
    while(p != nullptr && p->data != value) {
      previous = p;
      p = p->next;
    }
    // si value no esta en la lista
    if (p == nullptr) {
      std::cout << "El valor no existe en la lista" << std::endl;
      return false;
    }
    // si debo borrar el primer nodo de la lista
    if (p != nullptr && p == head) {
      head = p->next;
      if (head == nullptr) // habia solo un nodo en la lista
        tail = nullptr;
      else
        head->prev = nullptr;
    }  
    else if (p->next == nullptr) { // borrar el ultimo nodo
      previous->next = nullptr;
      tail = previous;
    }
    else { // borrar cualquier otro nodo
      previous->next = p->next;
      p->next->prev = previous;
    }
    // borrar el nodo apuntado por p
    delete p;
    p = nullptr;
    numElements--;
    return true;
  }
}

// Complejidad O(n)
template <class T>
bool DLinkedList<T>::deleteAt(int position) {
  // La lista esta vacia
  if (position < 0 || position >= numElements) {
    throw std::out_of_range("Indice fuera de rango");
  } 
  else if (position == 0) { // primer nodo
    DLLNode<T> *p = head;
    // si la lista contiene un solo nodo
    if (p != nullptr && p->next == nullptr) {
      head = nullptr;
      tail = nullptr;
    }
    else {
      head = p->next;
      head->prev = nullptr;
    }
    delete p;
    p = nullptr;
    numElements--;
    return true;
  }
  else {
    // Buscar position en la lista
    DLLNode<T> *p = head;
    DLLNode<T> *previous = nullptr;
    int index = 0;
    while(index != position) {
      previous = p;
      p = p->next;
      index++;
    }
    // debo borrar el ultimo nodo
    if (p->next == nullptr) {
      previous->next = nullptr;
      tail = previous;
    }
    else { // borrar cualquier otro nodo
      previous->next = p->next;
      p->next->prev = previous;
    }
    delete p;
    p = nullptr;
    numElements--;
    return true;    
  }
}

// Complejidad O(n)
template <class T>
T DLinkedList<T>::getData(int position) {
  // Validar position
  if (position < 0 || position >= numElements) {
    throw std::out_of_range("Indice fuera de rango");
  } 
  else if (position == 0) { // primer nodo 
    return head->data;
  }
  else { // si es cualquier otra posicion
    // Buscar position en la lista
    DLLNode<T> *p = head;
    int index = 0;
    while(index != position) {
      p = p->next;
      index++;
    }
    if (p != nullptr)
      return p->data;
    else
      return {};
  }
}

// Complejidad O(n)
template <class T>
void DLinkedList<T>::updateData(T value, T newValue) {
  // Buscar value en la lista
  DLLNode<T> *p = head;
  while(p != nullptr && p->data != value) {
    p = p->next;
  }
  if (p != nullptr)
    p->data = newValue;
  else
    throw std::out_of_range("El elemento a actualizar no existe en la lista");
}

// Actividad 2.2
// Complejidad O(n)
template <class T>
void DLinkedList<T>::updateAt(int position , T newValue) {
  // Verificar si la posición está dentro de los límites de la lista
  if (position < 0 || position >= numElements) {
    throw std::out_of_range("Indice fuera de rango");
  }
  // Buscamos el nodo en la posición dada
  DLLNode<T> *p = head;
  int index = 0;
  while (index != position) {
    p = p->next;
    index++;
  }
  // Actualizamos el valor del nodo
  p->data =newValue;
}

// Complejidad O(n)
template <class T>
int DLinkedList<T>::findData(T value) {
  // Inicializamos el índice en -1 para indicar que el elemento no se encontró
  T index = -1;
  // Inicializamos el puntero al inicio de la lista
  DLLNode<T> *p = head;
  // Inicializamos el índice del elemento actual
  T actualIndex = 0;
  // Recorremos la lista mientras no lleguemos al final y no encontremos el elemento
  while (p != nullptr && index == -1) {
    // Si encontramos el elemento al actualizar el índice salimos del bucle
    if (p->data == value) {
      index = actualIndex;
    }
    // Avanza al siguiente nodo e incrementa el índice del elemento actual
    p = p->next;
    actualIndex++;
  }
  // Devuelve el índice del elemento encontrado o -1 si no se encontró
  return index;
}

// Complejidad O(n)
template <class T>
void DLinkedList<T>::invert() {
  // Verifica si la lista tiene un elemento o está vacia
  if (head == nullptr || head == tail) {
    return;
  }
  // Inicializa dos punteros para recorrer la lista ->, <-
  DLLNode<T> *actual = head;
  DLLNode<T> *temp = nullptr;

  // Recorremos la lista y cambiamos las referenicas de los nodos
  while (actual != nullptr) {
    // Intercambiamos los punteros prev y next
    temp = actual->prev;
    actual->prev = actual->next;
    actual->next = temp;
    // Avanza al siguiente nodo
    actual = actual->prev;
  }
  // Intercambiamos head y tail para ver el cambio en la lista
  temp = head;
  head = tail;
  tail = temp;
}

// Complejidad O(n)
template <class T>
int DLinkedList<T>::getProductSublist(int ini, int fin) {
  // Verificar que los índices estén dentro de los límites de la lista
  if (ini < 0 || fin >= numElements || ini > fin) {
    throw std::out_of_range("Indices de sublista fuera de rango");
  }
  // Busca el nodo inicial
  DLLNode<T> *p = head;
  int index = 0;
  while (index != ini) {
    p = p->next;
    index++;
  }
  // inicializa el producto 
  int product = 1;
  // Recorre la sublista y calcula el producto
  while (index <= fin) {
    product *= p->data;
    p = p->next;
    index++;
  }
  return product;
}

#endif  // DLINKEDLIST_H