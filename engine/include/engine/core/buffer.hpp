#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <utility>

namespace hellix::core {

    /**
     * @brief Estrutura de memória contígua para transporte de dados brutos (binários).
     * @details Implementa semântica de movimento (RAII) para evitar cópias desnecessárias.
     */
    struct Buffer {
        std::byte* data = nullptr;
        uint64_t size = 0;

        Buffer() = default;

        explicit Buffer(uint64_t inSize) {
            allocate(inSize);
        }

        ~Buffer() {
            release();
        }

        // Semântica de movimento
        Buffer(Buffer&& other) noexcept
            : data(other.data), size(other.size) {
            other.data = nullptr;
            other.size = 0;
        }

        Buffer& operator=(Buffer&& other) noexcept {
            if (this != &other) {
                release();
                data = other.data;
                size = other.size;
                other.data = nullptr;
                other.size = 0;
            }
            return *this;
        }

        // Proíbe cópia direta para evitar duplicação acidental de grandes buffers
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        void allocate(uint64_t inSize) {
            release();
            if (inSize > 0) {
                data = new std::byte[inSize];
                size = inSize;
            }
        }

        void release() {
            delete[] data;
            data = nullptr;
            size = 0;
        }

        [[nodiscard]] bool isValid() const {
            return data != nullptr && size > 0;
        }

        template<typename T>
        [[nodiscard]] T* as() {
            return reinterpret_cast<T*>(data);
        }

        template<typename T>
        [[nodiscard]] const T* as() const {
            return reinterpret_cast<const T*>(data);
        }
    };

}