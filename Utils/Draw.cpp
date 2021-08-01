#include "Draw.h"
#include "Util.h"


void Draw::Circle(Vector2D position, float points, float radius, Color color)
{
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector2D start(radius * cosf(a) + position.x, radius * sinf(a) + position.y);
		Vector2D end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y);
		Line(start, end, color);
	}
}

/* Valve does the heavy lifting on this function */
void Draw::OutlinedCircle(int x0, int y0, int points, int radius, Color col) {
	pSurface->DrawSetColor(col);
	pSurface->DrawOutlinedCircle(x0, y0, points, radius);
}

void Draw::FilledCircle(Vector2D position, float points, float radius, Color color)
{
	std::vector<Vertex_t> vertices;
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + position.x, radius * sinf(a) + position.y)));

	Draw::TexturedPolygon((int)points, vertices.data(), color);
}

void Draw::Circle3D(Vector position, float points, float radius, Color color)
{
	float step = (float)M_PI * 2.0f / points;

	std::vector<Vector> points3d;
	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

		Vector start2d, end2d;
		if (pDebugOverlay->ScreenPosition(start, start2d) || pDebugOverlay->ScreenPosition(end, end2d))
			return;

		Draw::Line(Vector2D(start2d.x, start2d.y), Vector2D(end2d.x, end2d.y), color);
	}
}

void Draw::FilledRectangle(int x0, int y0, int x1, int y1, Color col)
{
	pSurface->DrawSetColor(col);
	pSurface->DrawFilledRect(x0, y0, x1, y1);
}

void Draw::FilledRectangle(Vector2D start_pos, Vector2D end_pos, Color col)
{
	FilledRectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
}

void Draw::Rectangle(int x0, int y0, int x1, int y1, Color col)
{
	pSurface->DrawSetColor(col);
	pSurface->DrawOutlinedRect(x0, y0, x1, y1);
}


void Draw::Rectangle(Vector2D start_pos, Vector2D end_pos, Color col)
{
	Rectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
}

void Draw::Line(int x0, int y0, int x1, int y1, Color col)
{
	pSurface->DrawSetColor(col);
	pSurface->DrawLine(x0, y0, x1, y1);
}
void Draw::Line(Vector2D start_pos, Vector2D end_pos, Color col)
{
	Line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
}

void Draw::PolyLine(int* px, int* py, int num_points, Color col)
{
	pSurface->DrawSetColor(col);
	pSurface->DrawPolyLine(px, py, num_points);
}


void Draw::PolyLine(Vertex_t* vertice, int num_points, Color col)
{
	static int* points_x = new int[num_points];
	static int* points_y = new int[num_points];
	for (int i = 0; i < num_points; i++)
	{
		points_x[i] = static_cast<int>(vertice[i].m_Position.x);
		points_y[i] = static_cast<int>(vertice[i].m_Position.y);
	}
	PolyLine(points_x, points_y, num_points, col);
}

void Draw::TexturedPolygon(int n, Vertex_t* vertice, Color col)
{
	static int texture_id = pSurface->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	pSurface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
	pSurface->DrawSetColor(col);
	pSurface->DrawSetTexture(texture_id);
	pSurface->DrawTexturedPolygon(n, vertice);
}

void Draw::TextW(int x, int y, const wchar_t* text, HFont font, Color col)
{
	pSurface->DrawSetTextPos(x, y);
	pSurface->DrawSetTextFont(font);
	pSurface->DrawSetTextColor(col);
	pSurface->DrawPrintText(text, wcslen(text));
}

void Draw::TextW(Vector2D pos, const wchar_t* text, HFont font, Color col)
{
	TextW(pos.x, pos.y, text, font, col);
}

void Draw::Text(int x, int y, const char* text, HFont font, Color col)
{
	std::wstring wc = Util::stringTowstring(text);

	pSurface->DrawSetTextPos(x, y);
	pSurface->DrawSetTextFont(font);
	pSurface->DrawSetTextColor(col);
	pSurface->DrawPrintText(wc.c_str(), wcslen(wc.c_str()));
}

void Draw::Text(Vector2D pos, const char* text, HFont font, Color col)
{
	Text(pos.x, pos.y, text, font, col);
}

void Draw::GetTextWSize(const wchar_t* text, HFont font, int& wide, int& tall) {
	pSurface->GetTextSize(font, text, wide, tall);
}

void Draw::GetTextSize(const char* text, HFont font, int& wide, int& tall) {
	std::wstring wc = Util::stringTowstring(text);

	pSurface->GetTextSize(font, wc.c_str(), wide, tall);
}

Vector2D Draw::GetTextWSize(const wchar_t* text, HFont font) {
	int x_res, y_res;
	pSurface->GetTextSize(font, text, x_res, y_res);
	return Vector2D(x_res, y_res);
}

Vector2D Draw::GetTextSize(const char* text, HFont font) {
	std::wstring wc = Util::stringTowstring(text);

	int x_res, y_res;
	pSurface->GetTextSize(font, wc.c_str(), x_res, y_res);
	return Vector2D(x_res, y_res);
}


HFont Draw::CreateFont(const char* fontName, int size, int flag)
{
	HFont newFont = pSurface->CreateFont();
	pSurface->SetFontGlyphSet(newFont, fontName, size, 0, 0, 0, flag);
	return newFont;
}

void Draw::ImStart()
{
	int width, height;
	pEngine->GetScreenSize(width, height);

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Always);
	ImGui::Begin("",
		(bool*)true,
		ImVec2(width, height),
		0.f,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
}



void Draw::ImText(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end, float wrap_width,
	const ImVec4* cpu_fine_clip_rect, ImFontFlags flags) {
	if (flags & ImFontFlags_Outline) {
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(pos.x - 1, pos.y - 1),
			ImColor(0, 0, 0, 255), text_begin, text_end, wrap_width,
			cpu_fine_clip_rect);
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(pos.x + 2, pos.y),
			ImColor(0, 0, 0, 255), text_begin, text_end, wrap_width,
			cpu_fine_clip_rect);
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(pos.x, pos.y + 2),
			ImColor(0, 0, 0, 255), text_begin, text_end, wrap_width,
			cpu_fine_clip_rect);
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(pos.x - 2, pos.y),
			ImColor(0, 0, 0, 255), text_begin, text_end, wrap_width,
			cpu_fine_clip_rect);
	}

	if (flags & ImFontFlags_Shadow)
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(pos.x + 1, pos.y + 1),
			ImColor(0, 0, 0, 255), text_begin, text_end, wrap_width,
			cpu_fine_clip_rect);

	ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, color, text_begin, text_end,
		wrap_width, cpu_fine_clip_rect);
}

void Draw::ImCircle(ImVec2 point, ImColor color, float radius, int num_segments, float thickness) {
	ImGui::GetWindowDrawList()->AddCircle(point, radius, color, num_segments, thickness);
}

void Draw::ImCircleFilled(ImVec2 point, ImColor color, float radius, int num_segments) {
	ImGui::GetWindowDrawList()->AddCircleFilled(point, radius, color, num_segments);
}

void Draw::ImRect(ImVec2 a, ImVec2 b, ImColor color, float rounding, int rounding_corners_flags, float thickness) {
	ImGui::GetWindowDrawList()->AddRect(a, b, color, rounding, rounding_corners_flags, thickness);
}

void Draw::ImLine(ImVec2 a, ImVec2 b, ImColor color, float thickness) {
	ImGui::GetWindowDrawList()->AddLine(a, b, color, thickness);
}
//#include <GL/gl.h>
void Draw::ImImage(const char* const imagePath, ImVec2 a, ImVec2 b) {
	//static std::map<const char* const, GLuint> images;
	//if (images.find(imagePath) == images.end()) {
	//	GLuint img = Image::LoadImageA(imagePath);
	//	
	//	if (img != EXIT_FAILURE)
	//		images[imagePath] = img;
	//}
	//
	////GLuint img = Image::LoadImage(imagePath);
	////if( img == NULL )
	////{
	////pCvar->ConsoleDPrintf("(Draw::ImImage): Warning, image \"%s\" failed to Load\n");
	////return;
	////}
	////
	
//	ImGui::GetWindowDrawList()->AddImage(reinterpret_cast<void*>(images[imagePath]), a, b);
}


bool Draw::ScreenTransform(const Vector& point, Vector& screen) // tots not pasted
{
	float w;
	const VMatrix& worldToScreen = pEngine->WorldToScreenMatrix();

	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f)
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool Draw::WorldToScreen(const Vector& origin, Vector& screen)
{
	if (!ScreenTransform(origin, screen))
	{
		int ScreenWidth, ScreenHeight;
		pEngine->GetScreenSize(ScreenWidth, ScreenHeight);
		float x = ScreenWidth / 2;
		float y = ScreenHeight / 2;
		x += 0.5 * screen.x * ScreenWidth + 0.5;
		y -= 0.5 * screen.y * ScreenHeight + 0.5;
		screen.x = x;
		screen.y = y;
		return true;
	}

	return false;
}


