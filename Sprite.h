#include "Shared.h"
#pragma once
class cSprite
{
public:
	//Constructors
	cSprite();
	explicit cSprite(int x, int y);
	//Destructor
	virtual ~cSprite();
	//Functions
public:
	bool Init(IDirect3DDevice9*, std::string, int, int);
	bool IsInit() const;
	virtual void Draw();
	//Variable
	D3DXVECTOR3 pPos;
	bool Release();
private:
	LPDIRECT3DTEXTURE9 pTex;
	LPD3DXSPRITE pSprite;
	D3DCOLOR color;
	bool result;
};

extern cSprite*					xhooksprite;
