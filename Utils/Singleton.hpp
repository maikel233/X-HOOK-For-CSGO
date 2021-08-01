#pragma once

// Implement Singletons
#define Singletons(TYPE) \
	public: \
	static std::shared_ptr<TYPE> Get() { return Single; } \
	private: \
	static std::shared_ptr<TYPE> Single;

#define Singleton_CPP(TYPE) \
	std::shared_ptr<TYPE> TYPE::Single = std::make_shared<TYPE>();


template<typename T>
class Singleton
{
protected:
	Singleton() {}
	~Singleton() {}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

public:
	static T& Get()
	{
		static T inst{};
		return inst;
	}
};

