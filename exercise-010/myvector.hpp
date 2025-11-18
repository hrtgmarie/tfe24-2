#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <algorithm>

namespace tfe24 {

/// Generic dynamic vector similar to std::vector
/// Implements Rule of Three for safe memory management
template<typename T>
class myvector {
public:
    // Constructors and destructors
    myvector() noexcept;
    explicit myvector(std::size_t count);
    ~myvector();
    
    // Copy semantics (Rule of Three)
    myvector(const myvector& other);
    myvector& operator=(const myvector& other);

    // Core functionality
    void push_back(const T& value);
    T& at(std::size_t index);
    const T& at(std::size_t index) const;
    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    // Capacity management
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    void reserve(std::size_t new_cap);
    void resize(std::size_t new_size);
    void clear() noexcept;

private:
    T* m_data;
    std::size_t m_size;
    std::size_t m_capacity;
    
    void reallocate(std::size_t new_cap);
};

// === Template implementation ===

template<typename T>
myvector<T>::myvector() noexcept 
    : m_data(nullptr), m_size(0), m_capacity(0) {}

template<typename T>
myvector<T>::myvector(std::size_t count) 
    : m_data(count ? new T[count]() : nullptr), 
      m_size(count), 
      m_capacity(count) {}

template<typename T>
myvector<T>::~myvector() {
    delete[] m_data;
}

template<typename T>
myvector<T>::myvector(const myvector& other)
    : m_data(other.m_capacity ? new T[other.m_capacity] : nullptr),
      m_size(other.m_size),
      m_capacity(other.m_capacity) {
    if (m_data) {
        std::copy(other.m_data, other.m_data + other.m_size, m_data);
    }
}

template<typename T>
myvector<T>& myvector<T>::operator=(const myvector& other) {
    if (this != &other) {
        T* new_data = other.m_capacity ? new T[other.m_capacity] : nullptr;
        try {
            if (new_data) {
                std::copy(other.m_data, other.m_data + other.m_size, new_data);
            }
        } catch (...) {
            delete[] new_data;
            throw;
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
        reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    }
    m_data[m_size++] = value;
}

template<typename T>
T& myvector<T>::at(std::size_t index) {
    if (index >= m_size) {
        throw std::out_of_range("Index out of range");
    }
    return m_data[index];
}

template<typename T>
const T& myvector<T>::at(std::size_t index) const {
    if (index >= m_size) {
        throw std::out_of_range("Index out of range");
    }
    return m_data[index];
}

template<typename T>
T& myvector<T>::operator[](std::size_t index) {
    return m_data[index];
}

template<typename T>
const T& myvector<T>::operator[](std::size_t index) const {
    return m_data[index];
}

template<typename T>
std::size_t myvector<T>::size() const noexcept {
    return m_size;
}

template<typename T>
std::size_t myvector<T>::capacity() const noexcept {
    return m_capacity;
}

template<typename T>
void myvector<T>::reserve(std::size_t new_cap) {
    if (new_cap > m_capacity) {
        reallocate(new_cap);
    }
}

template<typename T>
void myvector<T>::resize(std::size_t new_size) {
    if (new_size > m_capacity) {
        reserve(new_size);
    }
    if (new_size > m_size) {
        std::fill(m_data + m_size, m_data + new_size, T());
    }
    m_size = new_size;
}

template<typename T>
void myvector<T>::clear() noexcept {
    m_size = 0;
}

template<typename T>
void myvector<T>::reallocate(std::size_t new_cap) {
    T* new_data = new T[new_cap]();
    if (m_data) {
        std::copy(m_data, m_data + m_size, new_data);
        delete[] m_data;
    }
    m_data = new_data;
    m_capacity = new_cap;
}

} // namespace tfe24

#endif // MYVECTOR_HPP
