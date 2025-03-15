#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "list/XArrayList.h"
#include "list/DLinkedList.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

// -------------------- List1D --------------------
template <typename T>
class List1D
{
private:
    IList<T> *pList;

public:
    List1D();
    List1D(int num_elements);
    List1D(const T *array, int num_elements);
    List1D(const List1D<T> &other);
    virtual ~List1D();

    int size() const;
    T get(int index) const;
    void set(int index, T value);
    void add(const T &value);
    string toString() const;

    friend ostream &operator<<(ostream &os, const List1D<T> &list);
};

// -------------------- List2D --------------------
template <typename T>
class List2D
{
private:
    IList<IList<T> *> *pMatrix;

public:
    List2D();
    List2D(List1D<T> *array, int num_rows);
    List2D(const List2D<T> &other);
    virtual ~List2D();

    int rows() const;
    void setRow(int rowIndex, const List1D<T> &row);
    T get(int rowIndex, int colIndex) const;
    List1D<T> getRow(int rowIndex) const;
    string toString() const;

    friend ostream &operator<<(ostream &os, const List2D<T> &matrix);
};

struct InventoryAttribute
{
    string name;
    double value;
    InventoryAttribute(const string &name, double value) : name(name), value(value) {}
    string toString() const { return name + ": " + to_string(value); }
};

// -------------------- InventoryManager --------------------
class InventoryManager
{
private:
    List2D<InventoryAttribute> attributesMatrix;
    List1D<string> productNames;
    List1D<int> quantities;

public:
    InventoryManager();
    InventoryManager(const List2D<InventoryAttribute> &matrix,
                     const List1D<string> &names,
                     const List1D<int> &quantities);
    InventoryManager(const InventoryManager &other);

    int size() const;
    List1D<InventoryAttribute> getProductAttributes(int index) const;
    string getProductName(int index) const;
    int getProductQuantity(int index) const;
    void updateQuantity(int index, int newQuantity);
    void addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity);
    void removeProduct(int index);

    List1D<string> query(int attributeName, const double &minValue,
                         const double &maxValue, int minQuantity, bool ascending) const;

    void removeDuplicates();

    static InventoryManager merge(const InventoryManager &inv1,
                                  const InventoryManager &inv2);

    void split(InventoryManager &section1,
               InventoryManager &section2,
               double ratio) const;

    List2D<InventoryAttribute> getAttributesMatrix() const;
    List1D<string> getProductNames() const;
    List1D<int> getQuantities() const;
    string toString() const;
};

// -------------------- List1D Method Definitions --------------------
template <typename T>
List1D<T>::List1D()
{
    // TODO
    pList = new XArrayList<T>();
}

template <typename T>
List1D<T>::List1D(int num_elements)
{
    // TODO
    pList = new XArrayList<T>(num_elements);
}

template <typename T>
List1D<T>::List1D(const T *array, int num_elements)
{
    // TODO
    pList = new XArrayList<T>(num_elements);
    for (int i = 0; i < num_elements; i++)
        pList->add(array[i]);
}

template <typename T>
List1D<T>::List1D(const List1D<T> &other)
{
    // TODO
    pList = new XArrayList<T>(other.pList);
}

template <typename T>
List1D<T>::~List1D()
{
    // TODO
    pList->clear();
    delete pList;
}

template <typename T>
int List1D<T>::size() const
{
    // TODO
    return pList->size();
}

template <typename T>
T List1D<T>::get(int index) const
{
    // TODO
    return pList->get(index);
}

template <typename T>
void List1D<T>::set(int index, T value)
{
    // TODO
    typename IList<T>::Iterator it = pList->begin();
    for (int i = 0; i < index; i++)
        it++;
    *it = value;
}

template <typename T>
void List1D<T>::add(const T &value)
{
    // TODO
    pList->add(value);
}

template <typename T>
string List1D<T>::toString() const
{
    // TODO
    return pList->toString();
}

template <typename T>
ostream &operator<<(ostream &os, const List1D<T> &list)
{
    // TODO
    os << list.toString();
    return os;
}

// -------------------- List2D Method Definitions --------------------
template <typename T>
List2D<T>::List2D()
{
    // TODO
    pMatrix = new DLinkedList<IList<T> *>();
}

template <typename T>
List2D<T>::List2D(List1D<T> *array, int num_rows)
{
    // TODO
    pMatrix = new DLinkedList<IList<T> *>();
    for (int i = 0; i < num_rows; i++)
        pMatrix->add(array[i]);
}

template <typename T>
List2D<T>::List2D(const List2D<T> &other)
{
    // TODO
    pMatrix = new DLinkedList<IList<T> *>(other.pMatrix);
}

template <typename T>
List2D<T>::~List2D()
{
    // TODO
    typename DLinkedList<IList<T> *>::Iterator it = pMatrix->begin();
    while (it != pMatrix->end())
    {
        delete *it;
        it++;
    }
    delete pMatrix;
}

template <typename T>
int List2D<T>::rows() const
{
    // TODO
    return pMatrix->size();
}

template <typename T>
void List2D<T>::setRow(int rowIndex, const List1D<T> &row)
{
    // TODO
    if (rowIndex < 0 || rowIndex >= pMatrix->size())
        throw out_of_range("Index of row in the matrix");
    if (rowIndex == pMatrix->size())
        pMatrix->add(*row);
    else
    {
        typename DLinkedList<IList<T> *>::Iterator it = pMatrix->begin();
        for (int i = 0; i < rowIndex; i++)
            it++;
        delete *it;
        *it = new XArrayList<T>(*row);
    }
}

template <typename T>
T List2D<T>::get(int rowIndex, int colIndex) const
{
    // TODO
    return pMatrix->get(rowIndex)->get(colIndex);
}

template <typename T>
List1D<T> List2D<T>::getRow(int rowIndex) const
{
    // TODO
    if (rowIndex < 0 || rowIndex >= pMatrix->size())
        throw out_of_range("Index of row in the matrix");
    return *(pMatrix->get(rowIndex));
}

template <typename T>
string List2D<T>::toString() const
{
    // TODO
    string result = "[";
    typename DLinkedList<IList<T> *>::Iterator it = pMatrix->begin();
    while (it++ != pMatrix->end())
    {
        if (it != pMatrix->begin())
            result += ", ";
        result += (*it)->toString();
    }
    result += "]";
    return result;
}

template <typename T>
ostream &operator<<(ostream &os, const List2D<T> &matrix)
{
    // TODO
    os << matrix.toString();
    return os;
}

// -------------------- InventoryManager Method Definitions --------------------
InventoryManager::InventoryManager()
{
    // TODO
    attributesMatrix = List2D<InventoryAttribute>();
    productNames = List1D<string>();
    quantities = List1D<int>();
}

InventoryManager::InventoryManager(const List2D<InventoryAttribute> &matrix,
                                   const List1D<string> &names,
                                   const List1D<int> &quantities)
{
    // TODO
    this->attributesMatrix = matrix;
    this->productNames = names;
    this->quantities = quantities;
}

InventoryManager::InventoryManager(const InventoryManager &other)
{
    // TODO
    this->attributesMatrix = other.attributesMatrix;
    this->productNames = other.productNames;
    this->quantities = other.quantities;
}

int InventoryManager::size() const
{
    // TODO
    return productNames.size();
}

List1D<InventoryAttribute> InventoryManager::getProductAttributes(int index) const
{
    // TODO
    return attributesMatrix.getRow(index);
}

string InventoryManager::getProductName(int index) const
{
    // TODO
    return productNames.get(index);
}

int InventoryManager::getProductQuantity(int index) const
{
    // TODO
    return quantities.get(index);
}

void InventoryManager::updateQuantity(int index, int newQuantity)
{
    // TODO
    return quantities.set(index, newQuantity);
}

void InventoryManager::addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity)
{
    // TODO
    productNames.add(name);
    quantities.add(quantity);
    attributesMatrix.setRow(attributesMatrix.rows(), attributes);
}

void InventoryManager::removeProduct(int index)
{
    // TODO
}

List1D<string> InventoryManager::query(int attributeName, const double &minValue,
                                       const double &maxValue, int minQuantity, bool ascending) const
{
    // TODO
}

void InventoryManager::removeDuplicates()
{
    // TODO
}

InventoryManager InventoryManager::merge(const InventoryManager &inv1,
                                         const InventoryManager &inv2)
{
    // TODO
}

void InventoryManager::split(InventoryManager &section1,
                             InventoryManager &section2,
                             double ratio) const
{
    // TODO
}

List2D<InventoryAttribute> InventoryManager::getAttributesMatrix() const
{
    // TODO
}

List1D<string> InventoryManager::getProductNames() const
{
    // TODO
}

List1D<int> InventoryManager::getQuantities() const
{
    // TODO
}

string InventoryManager::toString() const
{
    // TODO
}

#endif /* INVENTORY_MANAGER_H */
