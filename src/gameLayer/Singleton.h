

template <class T>
class Singleton
{
public:
	static T* Instance()
	{
		if (instance == nullptr)
		{
			instance = new T;
		}
		return instance;
	}
protected:

private:
	static T* instance;
};
template<class T> T* Singleton<T>::instance = nullptr;