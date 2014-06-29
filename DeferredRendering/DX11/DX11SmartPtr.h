
template <typename T>
class DX11SmartPtr
{
public:
	DX11SmartPtr() :
		mValue(NULL)
	{

	}

	DX11SmartPtr(const T* value) :
		mValue(const_cast<T*>(value))
	{
		AddRef();
	}

	DX11SmartPtr(const DX11SmartPtr& other) :
		mValue(const_cast<T*>(other.mValue))
	{
		AddRef();
	}

	~DX11SmartPtr()
	{
		Release();
	}

	DX11SmartPtr& operator=(const DX11SmartPtr& other)
	{
		Release();
		mValue = const_cast<T*>(other.mValue);
		AddRef();

		return *this;
	}
	

	DX11SmartPtr& operator=(const T* newValue)
	{
		Release();
		mValue = const_cast<T*>(newValue);
		AddRef();
		
		return *this;
	}

	T& operator*()
	{
		return *mValue;
	}

	const T& operator*() const
	{
		return *mValue;
	}

	T* Get()
	{
		return mValue;
	}

	const T* Get() const
	{
		return mValue;
	}

private:
	void AddRef()
	{
		if (mValue != NULL)
		{
			mValue->AddRef();
		}
	}

	void Release()
	{
		if (mValue != NULL)
		{
			mValue->Release();
			mValue = NULL;
		}
	}

	T *mValue;
};