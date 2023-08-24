#ifndef RotationBuffer_h
#define RotationBuffer_h

template <typename T> class RotationBuffer
{
public:
	T* values;
	T sum, mean;
	int size, count, index;

	RotationBuffer()
	{
		this->values = 0;

		this->size = 0;
		this->count = 0;
		this->index = 0;

		this->sum = 0;
		this->mean = 0;
	};

	~RotationBuffer()
	{
		this->SetSize(0);
	};

	void SetSize(int size)
	{
		if (this->values != 0)
			delete[] this->values;

		if (size > 0)
			this->values = new T[size];
		else
			this->values = 0;

		this->size = size;
		this->count = 0;
		this->index = 0;
	};

	void Add(const T& value)
	{
		if (this->size == 0)
			return;

		this->values[this->index] = value;
		this->sum = this->sum + value;
		this->index = (this->index + 1) % this->size;
		if (this->count == this->size)
		{
			this->sum = this->sum - this->values[this->index];
		}
		else
			this->count++;

		this->mean = this->sum / this->count;
	};
};

#endif
// RotationBuffer_h

