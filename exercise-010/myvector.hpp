#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <algorithm>

namespace tfe24 {

/// \brief Ein einfacher dynamischer Vektor ähnlich std::vector.
/// \tparam T Elementtyp.
/// \invariant 0 <= size() <= capacity()
/// \exception std::out_of_range bei ungültigem Index für at().
template<typename T>
class myvector {
public:
    /// Standardkonstruktor - erstellt einen leeren Vektor
    myvector() noexcept;
    
    /// Konstruktor mit Größe - erstellt Vektor mit count Elementen
    /// \param count Anzahl der zu erstellenden Elemente
    explicit myvector(size_t count);
    
    /// Destruktor - gibt reservierten Speicher frei
    ~myvector();
    
    /// Copy-Konstruktor - erzeugt unabhängige Kopie (Rule of Three)
    myvector(const myvector& other);
    
    /// Copy-Zuweisungsoperator - erzeugt unabhängige Kopie (Rule of Three)
    myvector& operator=(const myvector& other);
    
    // === Minimal geforderte öffentliche Schnittstelle ===
    
    /// \brief Fügt Element am Ende hinzu
    /// \param value Zu kopierendes Element
    /// \complexity Amortisiert O(1)
    void push_back(const T& value);
    
    /// \brief Zugriff mit Grenzenprüfung (const-Version)
    /// \param index Elementindex
    /// \return Referenz auf das Element
    /// \throws std::out_of_range wenn index >= size()
    /// \complexity O(1)
    const T& at(size_t index) const;
    
    /// \brief Zugriff mit Grenzenprüfung (non-const-Version)
    /// \param index Elementindex
    /// \return Referenz auf das Element
    /// \throws std::out_of_range wenn index >= size()
    /// \complexity O(1)
    T& at(size_t index);
    
    /// \brief Unsicherer Zugriff ohne Grenzenprüfung
    /// \param index Elementindex
    /// \return Referenz auf das Element
    /// \complexity O(1)
    T& operator[](size_t index);
    
    /// \brief Unsicherer Zugriff ohne Grenzenprüfung (const-Version)
    /// \param index Elementindex
    /// \return Referenz auf das Element
    /// \complexity O(1)
    const T& operator[](size_t index) const;
    
    /// \brief Anzahl der Elemente
    /// \return Aktuelle Größe
    /// \complexity O(1)
    size_t size() const noexcept;
    
    /// \brief Verfügbare Kapazität (ohne Reallocation)
    /// \return Reservierter Speicherplatz
    /// \complexity O(1)
    size_t capacity() const noexcept;
    
    /// \brief Reserviert Speicher für mindestens new_cap Elemente
    /// \param new_cap Gewünschte Kapazität
    /// \complexity O(n) wenn Vergrößerung nötig, O(1) sonst
    void reserve(size_t new_cap);
    
    /// \brief Ändert die Größe des Vektors
    /// \param new_size Neue Größe
    /// \complexity O(new_size) bei Vergrößerung, O(1) bei Verkleinerung
    void resize(size_t new_size);
    
    /// \brief Entfernt alle Elemente (Speicher bleibt erhalten)
    /// \complexity O(1)
    void clear() noexcept;

private:
    T* m_data;          ///< Zeiger auf Speicher
    size_t m_size;      ///< Aktuelle Anzahl Elemente
    size_t m_capacity;  ///< Verfügbare Kapazität
    
    /// Interne Methode: Speicher neu allokieren
    void reallocate(size_t new_cap);
};

// === Template-Implementierung (im Header notwendig) ===

template<typename T>
myvector<T>::myvector() noexcept 
    : m_data(nullptr), m_size(0), m_capacity(0) {
}

template<typename T>
myvector<T>::myvector(size_t count) 
    : m_data(nullptr), m_size(count), m_capacity(count) {
    if (count > 0) {
        m_data = new T[count]();
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
        throw std::out_of_range("myvector::at() - index out of range");
    }
    return m_data[index];
}

template<typename T>
const T& myvector<T>::at(size_t index) const {
    if (index >= m_size) {
        throw std::out_of_range("myvector::at() const - index out of range");
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
    m_size = new_size;
}

template<typename T>
void myvector<T>::clear() noexcept {
    m_size = 0;
}

template<typename T>
void myvector<T>::reallocate(size_t new_cap) {
    T* new_data = new T[new_cap]();
    if (m_data != nullptr) {
        std::copy(m_data, m_data + m_size, new_data);
        delete[] m_data;
    }
    m_data = new_data;
    m_capacity = new_cap;
}

} // namespace tfe24

#endif // MYVECTOR_HPP
