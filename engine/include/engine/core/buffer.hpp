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
        /** @brief Ponteiro para a memória alocada. */
        std::byte* data = nullptr;
        /** @brief Quantidade de bytes alocados. */
        uint64_t size = 0;

        Buffer() = default;

        explicit Buffer(uint64_t inSize) {
            allocate(inSize);
        }

        ~Buffer() {
            release();
        }

        // Semântica de movimento.
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

        /**
         * @brief Aloca uma quantidade de bytes, liberando o conteúdo anterior.
         * @param inSize Quantidade de bytes a alocar.
         */
        void allocate(uint64_t inSize) {
            release();
            if (inSize > 0) {
                data = new std::byte[inSize];
                size = inSize;
            }
        }

        /** @brief Libera a memória e zera o estado do buffer. */
        void release() {
            delete[] data;
            data = nullptr;
            size = 0;
        }

        /** @brief Informa se o buffer possui memória e tamanho válidos. */
        [[nodiscard]] bool isValid() const {
            return data != nullptr && size > 0;
        }

        template<typename T>
        /** @brief Interpreta o conteúdo como um ponteiro para o tipo informado. */
        [[nodiscard]] T* as() {
            return reinterpret_cast<T*>(data);
        }

        template<typename T>
        [[nodiscard]] const T* as() const {
            return reinterpret_cast<const T*>(data);
        }
    };

}