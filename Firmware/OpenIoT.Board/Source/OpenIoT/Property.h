#ifndef Property_h
#define Property_h

enum PropertyType
{
	PropertyType_None = 0,
	PropertyType_Action = 1,
	PropertyType_Integer = 2,
	PropertyType_Float = 3,
	PropertyType_Bool = 4,
	PropertyType_Data = 5
};

enum PropertyFlag
{
	PropertyFlag_Read = 1 << 0,
	PropertyFlag_Write = 1 << 1,
	PropertyFlag_EEPRom = 1 << 6,
	PropertyFlag_Subscribed = 1 << 7,
};

class Property
{
protected:
	void Init(unsigned char semantic, PropertyType type, unsigned char flags);
	void InitWithValue(void** data, PropertyType type, unsigned char flags);

public:
	union
	{
		float fValue;
		int iValue;
		bool bValue;
		void* data;
	};

	static const unsigned char propertyFlag_Default = PropertyFlag_Read | PropertyFlag_Write;

	bool isChanged;

	unsigned char semantic;
	PropertyType type;
	unsigned char flags;

	static unsigned char typeSizes[5];

	Property();
	Property(unsigned char semantic, int value, unsigned char flags = propertyFlag_Default);
	Property(unsigned char semantic, float value, unsigned char flags = propertyFlag_Default);
	// The PropertyType is there, just to distinguish from the int overload, because the compiler interprets this constructor(bool) as the int one
	Property(unsigned char semantic, bool value, PropertyType type, unsigned char flags = propertyFlag_Default);
	Property(Property* param, unsigned char semantic, unsigned char flags = propertyFlag_Default);
	Property(void** data, PropertyType type, unsigned char flags = propertyFlag_Default);
	~Property();

	void SetBool(bool value);
	void SetInt(int value);
	void SetFloat(float value);
	bool SetDataSize(int size);
	void SetDataValue(void* value, int size);

	void SetStringData(const char* str);
	char* GetStringData();

	bool IsReadable();
	bool IsWritable();
	bool IsEEPRom();
	bool IsSubscribed();

	int Size();
	int SizeInMemory();

	void Set(void* value);
	void SetValue(void* value);
	void GetValue(void* value);

	int WriteValueToStream(void* value);
	int ReadValueFromStream(void* value);

	bool IsChanged();
	void ResetChanged();

	virtual void Update();

	int ToString(char* dest, bool nullTerminate = true);
};

#endif
// Property_h
