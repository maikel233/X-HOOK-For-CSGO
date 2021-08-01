#pragma once
enum class Colors
{
	WHITE = 1,
	DARK_RED,
	LIGHT_PURPLE,
	DARK_GREEN,
	LIGHT_GREEN,
	GREEN,
	LIGHT_RED,
	GRAY,
	YELLOW,
	LIGHT_BLUE,
	BLUE,
	DARK_BLUE,
	DARK_GRAY,
	PURPLE,
	RED,
	ORANGE,
};

enum class NC_Type
{
	NC_NORMAL,
	NC_TROLL,
	NC_RAINBOW,
	NC_SOLID,

};
namespace NameChanger
{
	extern int changes;
	extern NC_Type type;
	extern std::string origName;
	extern Colors color;
	extern std::vector<std::pair<Colors, const char*>> colors;

	std::string GetName();
	void BeginFrame(float frameTime);
	void SetName(const char* name);
	void InitColorChange(NC_Type type, Colors color = Colors::LIGHT_RED);
	bool changeName(bool, const char*, float) noexcept;

	void fakeBan(bool = false) noexcept;
};