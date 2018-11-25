#pragma once
#include <memory>
class VMTHook //credits @ https://github.com/aixxe/vmthook
{
private:
	std::uintptr_t** baseclass = nullptr;
	std::unique_ptr<std::uintptr_t[]> current_vft = nullptr;
	std::uintptr_t* original_vft = nullptr;
	std::size_t total_functions = 0;
public:
	VMTHook(void) = default;

	VMTHook(void* baseclass)
	{
		this->baseclass = static_cast<std::uintptr_t**>(baseclass);

		while (static_cast<std::uintptr_t*>(*this->baseclass)[this->total_functions])
			++this->total_functions;

		const std::size_t table_size = this->total_functions * sizeof(std::uintptr_t);

		this->original_vft = *this->baseclass;
		this->current_vft = std::make_unique<std::uintptr_t[]>(this->total_functions);

		std::memcpy(this->current_vft.get(), this->original_vft, table_size);

		*this->baseclass = this->current_vft.get();
	};

	~VMTHook()
	{
		*this->baseclass = this->original_vft;
	};

	template <typename Fn = void*> inline const Fn GetOriginalFunction(std::size_t function_index) {
		return reinterpret_cast<Fn>(this->original_vft[function_index]);
	}

	inline bool HookFunction(void* new_function, const std::size_t function_index)
	{
		if (function_index > this->total_functions)
			return false;

		this->current_vft[function_index] = reinterpret_cast<std::uintptr_t>(new_function);

		return true;
	}

	inline bool UnhookFunction(const std::size_t function_index)
	{
		if (function_index > this->total_functions)
			return false;

		this->current_vft[function_index] = this->original_vft[function_index];

		return true;
	}

	inline std::size_t GetTotalFunctions()
	{
		return this->total_functions;
	}
};

//class VMHook
//{
//private:
//	uintptr_t* vmt;
//public:
//	uintptr_t** p_interface = nullptr;
//
//	uintptr_t* original_vmt = nullptr;
//
//	uint32_t methodCount = 0;
//	VMHook()
//	{
//
//	}
//
//	VMHook(void* p_interface)
//	{
//		this->p_interface = reinterpret_cast<uintptr_t**>(p_interface);
//
//		size_t method_count = 0;
//
//		while (reinterpret_cast<uintptr_t*>(*this->p_interface)[method_count])
//			method_count++;
//
//		original_vmt = *this->p_interface;
//
//		//vmt = new uintptr_t[(sizeof(uintptr_t) * method_count)+ sizeof(uintptr_t)];
//		vmt = new uintptr_t[method_count + 1];
//
//		memcpy(vmt, &original_vmt[-1], (sizeof(uintptr_t) * method_count) + sizeof(uintptr_t));
//	}
//	void HookVM(void* method, size_t methodIndex) {
//		vmt[methodIndex + 1] = reinterpret_cast<uintptr_t>(method);
//	}
//
//	template<typename Fn>
//	Fn GetOriginalMethod(size_t methodIndex) {
//		return reinterpret_cast<Fn>(original_vmt[methodIndex]);
//	}
//
//	void ApplyVMT() {
//		*this->p_interface = &vmt[1];
//	}
//
//	void ReleaseVMT() {
//		*this->p_interface = original_vmt;
//		delete[] vmt;
//	}
//};
