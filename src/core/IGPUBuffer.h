#pragma once

class IGPUBuffer
{
public:
	virtual ~IGPUBuffer() = default;
	virtual void Bind() const = 0;
};
