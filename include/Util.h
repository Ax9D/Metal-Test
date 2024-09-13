#pragma once
#include <cstdint>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

#define ERROR(...) do { \
 spdlog::error(__VA_ARGS__);\
 std::exit(-1);\
} while(0)

#define INFO(...) do { \
 spdlog::info(__VA_ARGS__);\
} while(0)

#define DEBUG(...) do { \
 spdlog::debug(__VA_ARGS__);\
} while(0)

#define WARN(...) do { \
 spdlog::warn(__VA_ARGS__);\
} while(0)

std::string read_to_string(const std::string& path);

// From boost
template<typename T>
void hash_combine(uint64_t& existing, const T& val) {
    existing ^= std::hash<T>()(val) + 0x9e3779b9 + (existing << 6) + (existing >> 2);
}

template<typename T, const size_t N>
class ArrayVec {
    public:
        ArrayVec() {
            m_len = 0;
        }
        ArrayVec(std::initializer_list<T> list) {
            m_len = 0;

            for(auto& item: list) {
                push(item);
            }
        }
        ArrayVec(const ArrayVec<T, N>& other) {
            m_len = other.m_len;
            for(size_t i = 0; i < m_len; i++) {
                data()[i] = other[i];
            }
        }
        ArrayVec<T, N>& operator=(ArrayVec<T, N> other) {
            std::swap(m_len, other.m_len);
            std::swap(m_buf, other.m_buf);

            return *this;
        }
        ~ArrayVec() {
            for(size_t i = 0; i < m_len; i++) {
                data()[i].~T();
            }
        }
        T& operator[](size_t i) {
            return data()[i];
        }
        const T& operator[](size_t i) const {
            return data()[i];
        }
        bool operator==(const ArrayVec<T, N>& other) const {
            if(this->len() != other.len()) {
                return false;
            }

            for(size_t i = 0; i < this->len(); i++) {
                if(!((*this)[i] == other[i])) {
                    return false;
                }     
            }

            return true;
        }
        void push(const T& item) {
            assert(len() < capacity());
            data()[m_len++] = item; 
        }
        T& front() {
            return data()[0];
        }
        T& back() {
            return data()[m_len - 1];
        }
        void pop() {
            if(is_empty()) {
                return;
            }

            data()[--m_len].~T();
        }
        void clear() {
            for(size_t i = 0; i < len(); i++) {
                data()[i].~T();
            }
            m_len = 0;
        }
        T* data() {
            T* buf_t = (T*) m_buf;
            return buf_t;
        }
        const T* data() const {
            const T* buf_t = (const T*) m_buf;
            return buf_t; 
        }
        size_t len() const {
            return m_len;
        }
        size_t capacity() const {
            return N;
        }
        bool is_empty() const {
            return len() == 0;
        }
    private:
        size_t m_len;
        unsigned char m_buf[N * sizeof(T)];
};