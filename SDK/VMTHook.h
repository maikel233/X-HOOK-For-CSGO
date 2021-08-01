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