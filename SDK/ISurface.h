#pragma once

struct Vertex_t
{
	Vector2D m_Position;
	Vector2D m_TexCoord;

	Vertex_t() {}

	Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
};

typedef Vertex_t FontVertex_t;

class ISurface : public IAppSystem
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(void*, int r, int g, int b, int a);
		return getvfunc<OriginalFn>(this, 15)(this, r, g, b, a);
	}

	void DrawSetColor(Color color)
	{
		typedef void(__thiscall*oDrawSetColor)(void*, int, int, int, int);
		return getvfunc<oDrawSetColor>(this, 15)(this, color.r, color.g, color.b, color.a);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall*oDrawFilledRect)(void*, int, int, int, int);
		return getvfunc<oDrawFilledRect>(this, 16)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall*oDrawOutlinedRect)(void*, int, int, int, int);
		return getvfunc<oDrawOutlinedRect>(this, 18)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall*oDrawLine)(void*, int, int, int, int);
		return getvfunc<oDrawLine>(this, 19)(this, x0, y0, x1, y1);
	}

	void DrawPolyLine(int *x, int *y, int count)
	{
		typedef void(__thiscall*oDrawPolyLine)(void*, int*, int*, int);
		return getvfunc<oDrawPolyLine>(this, 20)(this, x, y, count);
	}

	void DrawSetTextFont(unsigned long index)
	{
		typedef void(__thiscall* DrawSetTextFontFn)(void*, unsigned long);
		return getvfunc<DrawSetTextFontFn>(this, 23)(this, index);
	}

	void DrawSetTextColor(int r, int g, int b, int a) {
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return getvfunc<OriginalFn>(this, 25)(this, r, g, b, a);
	}

	void DrawSetTextColor(Color color)
	{
		typedef void(__thiscall*oDrawSetTextColor)(void*, int, int, int, int);
		return getvfunc<oDrawSetTextColor>(this, 25)(this, color.r, color.g, color.b, color.a);
	}

	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall*oDrawSetTextPos)(void*, int, int);
		return getvfunc<oDrawSetTextPos>(this, 26)(this, x, y);
	}

	void DrawPrintText(const wchar_t *text, int textLen)
	{
		typedef void(__thiscall*oDrawPrintText)(void*, const wchar_t *, int, int);
		return getvfunc<oDrawPrintText>(this, 28)(this, text, textLen, 0);
	}

	void DrawSetTextureRGBA(int textureID, unsigned char const* colors, int w, int h)
	{
		typedef void(__thiscall*oDrawSetTextureRGBA)(void*, int, unsigned char const*, int, int);
		return getvfunc<oDrawSetTextureRGBA>(this, 37)(this, textureID, colors, w, h);
	}

	void DrawSetTexture(int textureID)
	{
		typedef void(__thiscall*oDrawSetTexture)(void*, int);
		return getvfunc<oDrawSetTexture>(this, 38)(this, textureID);
	}

	int CreateNewTextureID(bool procedural)
	{
		typedef int(__thiscall*oCreateNewTextureID)(void*, bool);
		return getvfunc<oCreateNewTextureID>(this, 43)(this, procedural);
	}

	HFont CreateFontA()
	{
		typedef HFont(__thiscall*oCreateFont)(void*);
		return getvfunc<oCreateFont>(this, 71)(this);
	}

	bool SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0)
	{
		typedef bool(__thiscall* SetFontGlyphSetFn)(void*, unsigned long, const char*, int, int, int, int, int, int, int);
		return getvfunc<SetFontGlyphSetFn>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
	}

	void GetTextSize(
		unsigned long font,
		const wchar_t* text,
		int32_t& wide,
		int32_t& tall) {
		typedef void(__thiscall* get_text_size_fn)(void*, unsigned long, const wchar_t*, int32_t&, int32_t&);
		return getvfunc<get_text_size_fn>(this, 79)(this, font, text, wide, tall);
	}
	void DrawOutlinedCircle(int x, int y, int radius, int segments)
	{
		typedef void(__thiscall* OriginalFn)(void*, int, int, int, int);
		return getvfunc<OriginalFn>(this, 103)(this, x, y, radius, segments);
	}

	void DrawTexturedPolygon(int vtxCount, FontVertex_t *vtx, bool bClipVertices = true)
	{
		typedef void(__thiscall*oDrawTexturedPolygon)(void*, int, FontVertex_t*, bool);
		return getvfunc<oDrawTexturedPolygon>(this, 106)(this, vtxCount, vtx, bClipVertices);
	}
};