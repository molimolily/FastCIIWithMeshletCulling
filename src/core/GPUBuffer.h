#pragma once

#include "IGPUBuffer.h"
#include <glad/glad.h>
#include <vector>
#include <stdexcept>

enum class BufferType
{
    SSBO,
    UBO
};

template<typename T>
class GPUBuffer : public IGPUBuffer
{
public:

    GPUBuffer() : size(0), bindingPoint(-1) {}
    /*
    * @param size: The number of elements in the buffer.
    * @param bindingPoint: The binding point of the buffer.
    * @param data: The data to be stored in the buffer.
    * @param bufferType: The type of the buffer. SSBO or UBO.
    */
    GPUBuffer(size_t size, GLuint bindingPoint, const std::vector<T>& data, BufferType bufferType = BufferType::SSBO, GLenum usage = GL_STATIC_DRAW)
        : size(size), bindingPoint(bindingPoint), bufferType(bufferType), target(BufferTypeToGLenum(bufferType))
    {
        SetupBuffer(data,usage);
    }

    ~GPUBuffer()
    {
        glDeleteBuffers(1, &buffer);
    }

    void Update(const std::vector<T>& data)
    {
        if (data.size() != size)
        {
            throw std::runtime_error("Instance count mismatch");
        }

        glBindBuffer(target, buffer);
        glBufferSubData(target, 0, size * sizeof(T), data.data());
        glBindBuffer(target, 0);
    }

    void Update(size_t index, const T& data)
    {
        if (index >= size)
        {
            throw std::out_of_range("Index out of range.");
        }

        glBindBuffer(target, buffer);
        glBufferSubData(target, index * sizeof(T), sizeof(T), &data);
        glBindBuffer(target, 0);
    }

    void Bind() const override
    {
        glBindBuffer(target, buffer);
        // glBindBufferBase(target, bindingPoint, buffer);
    }

    void Clear(GLenum internalFormat, GLenum format, GLenum type) const
    {
		glBindBuffer(target, buffer);
		glClearBufferData(target, internalFormat, format, type, nullptr);
		glBindBuffer(target, 0);
	}

    void Clear() const
    {
        Clear(GL_R8, GL_RED_INTEGER, GL_UNSIGNED_BYTE);
    }

    void Debug() const
	{
		std::cout << "size: " << size << ", bindingPoint: " << bindingPoint << ", bufferType: " << (bufferType == BufferType::SSBO ? "SSBO" : "UBO") << std::endl;
	}

    std::vector<T> GetData() const
	{
		std::vector<T> data(size);
		glBindBuffer(target, buffer);
		glGetBufferSubData(target, 0, size * sizeof(T), data.data());
		glBindBuffer(target, 0);
		return data;
	}

    T GetData(size_t index) const
    {
        T data;
        glBindBuffer(target, buffer);
        glGetBufferSubData(target, index * sizeof(T), sizeof(T), &data);
        glBindBuffer(target, 0);
        return data;
    }

private:
    size_t size;
    GLuint buffer;
    GLuint bindingPoint;
    BufferType bufferType;
    GLenum target;

    void SetupBuffer(const std::vector<T>& data, const GLenum usage)
    {
        glGenBuffers(1, &buffer);
        glBindBuffer(target, buffer);
        glBufferData(target, size * sizeof(T), data.data(), usage);
        glBindBufferBase(target, bindingPoint, buffer);
        glBindBuffer(target, 0);
    }

    GLenum BufferTypeToGLenum(BufferType bufferType) const
    {
        switch (bufferType)
        {
        case BufferType::SSBO:
            return GL_SHADER_STORAGE_BUFFER;
        case BufferType::UBO:
            return GL_UNIFORM_BUFFER;
        default:
            throw std::runtime_error("Invalid buffer type.");
        }
    }
};
