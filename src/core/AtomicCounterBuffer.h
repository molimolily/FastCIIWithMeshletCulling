#pragma once

#include "IGPUBuffer.h"
#include <glad/gl.h>
#include <stdexcept>

class AtomicCounterBuffer : public IGPUBuffer {
public:
    AtomicCounterBuffer(GLuint initialValue = 0, GLuint bindingPoint = 0)
        : bindingPoint(bindingPoint) {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buffer);
        glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), &initialValue, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
    }

    ~AtomicCounterBuffer() override {
        glDeleteBuffers(1, &buffer);
    }

    void Bind() const override {
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, bindingPoint, buffer);
    }

    void Clear(GLuint clearValue = 0) {
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buffer);
        glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &clearValue);
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
    }

    GLuint GetValue() const
    {
        GLuint value;
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, buffer);
        glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &value);
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
        return value;
    }

private:
    GLuint buffer;
    GLuint bindingPoint;
};
