#pragma once
#include <memory>
#include "../Utils/minhook/Minhook.h"

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



namespace hookHelper
{
	template<typename T>
	inline constexpr void** ORIGINAL(T& arg)
	{
		return reinterpret_cast<void**>(&arg);
	}

	inline void tryHook(void* target, void* detour, void* original, const std::string_view name)
	{
		const MH_STATUS hk = MH_CreateHook(static_cast<LPVOID>(target), static_cast<LPVOID>(detour), static_cast<LPVOID*>(original));
		if (hk != MH_OK)
			throw std::runtime_error("{} hook error"), name;
		//console.log(TypeLogs::LOG_INFO, XOR("{} -> {} hooked at addr {:#0x}"), name, MH_StatusToString(hk), reinterpret_cast<uintptr_t>(target));
	}

	inline void checkAllHooks()
	{
		const auto status = MH_EnableHook(MH_ALL_HOOKS);
		if (status != MH_OK)
			throw std::runtime_error("MH_EnableHook error");
	}

	inline void initMinhook()
	{
		if (MH_Initialize() != MH_OK)
			throw std::runtime_error("MH_Initialize error");
	}

	inline void shutdownAllHooks()
	{
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);
		MH_Uninitialize();
	}
}