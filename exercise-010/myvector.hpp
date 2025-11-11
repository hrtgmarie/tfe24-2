#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <utility>

namespace mystd {

template<typename T>
class myvector {
public:
    // Konstruktoren
    myvector() noexcept;
    explicit myvector(size_t count);
    
    // Destruktor
    ~myvector();
    
    // Rule of Three
    myvector(const myvector& other);
    myvector& operator=(const myvector& other);
    
    // Zentrale Methoden
    void push_back(const T& value);
    T& at(size_t index);
    const T& at(size_t index) const;
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    
    void reserve(size_t new_cap);
    void resize(size_t new_size);
    void clear() noexcept;

private:
    T* m_data;
    size_t m_size;
    size_t m_capacity;
    
    void reallocate(size_t new_cap);
};

// Implementierung der Template-Methoden (muss im Header sein)

template<typename T>
myvector<T>::myvector() noexcept 
    : m_data(nullptr), m_size(0), m_capacity(0) {
}

template<typename T>
myvector<T>::myvector(size_t count) 
    : m_data(nullptr), m_size(count), m_capacity(count) {
    if (count > 0) {
        m_data = new T[count]();  // Wertinitialisierung
    }
}

template<typename T>
myvector<T>::~myvector() {
    delete[] m_data;
}

template<typename T>
myvector<T>::myvector(const myvector& other) 
    : m_data(nullptr), m_size(other.m_size), m_capacity(other.m_capacity) {
    if (m_capacity > 0) {
        m_data = new T[m_capacity];
        std::copy(other.m_data, other.m_data + m_size, m_data);
    }
}

template<typename T>
myvector<T>& myvector<T>::operator=(const myvector& other) {
    if (this != &other) {
        // Copy-and-swap Idiom für Exception Safety
        T* new_data = nullptr;
        if (other.m_capacity > 0) {
            new_data = new T[other.m_capacity];
            std::copy(other.m_data, other.m_data + other.m_size, new_data);
        }
        
        delete[] m_data;
        m_data = new_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
    }
    return *this;
}

template<typename T>
void myvector<T>::push_back(const T& value) {
    if (m_size >= m_capacity) {
        size_t new_cap = (m_capacity == 0) ? 1 : m_capacity * 2;
        reserve(new_cap);
    }
    m_data[m_size++] = value;
}

template<typename T>
T& myvector<T>::at(size_t index) {
    if (index >= m_size) {
        throw std::out_of_range("myvector::at: index out of range");
    }
    return m_data[index];
}

template<typename T>
const T& myvector<T>::at(size_t index) const {
    if (index >= m_size) {
        throw std::out_of_range("myvector::at: index out of range");
    }
    return m_data[index];
}

template<typename T>
T& myvector<T>::operator[](size_t index) {
    return m_data[index];
}

template<typename T>
const T& myvector<T>::operator[](size_t index) const {
    return m_data[index];
}

template<typename T>
size_t myvector<T>::size() const noexcept {
    return m_size;
}

template<typename T>
size_t myvector<T>::capacity() const noexcept {
    return m_capacity;
}

template<typename T>
void myvector<T>::reserve(size_t new_cap) {
    if (new_cap > m_capacity) {
        reallocate(new_cap);
    }
}

template<typename T>
void myvector<T>::resize(size_t new_size) {
    if (new_size > m_capacity) {
        reserve(new_size);
    }
    
    // Neue Elemente wertinitialisieren
    for (size_t i = m_size; i < new_size; ++i) {
        m_data[i] = T();
    }
    
    m_size = new_size;
}

template<typename T>
void myvector<T>::clear() noexcept {
    m_size = 0;
}

template<typename T>
void myvector<T>::reallocate(size_t new_cap) {
    T* new_data = new T[new_cap];
    
    // Kopiere alte Elemente
    size_t elements_to_copy = (m_size < new_cap) ? m_size : new_cap;
    std::copy(m_data, m_data + elements_to_copy, new_data);
    
    delete[] m_data;
    m_data = new_data;
    m_capacity = new_cap;
}

} // namespace mystd

#endif // MYVECTOR_HPP
