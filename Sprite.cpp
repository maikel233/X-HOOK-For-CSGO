#include "Sprite.h"
cSprite::cSprite()
{
	pPos.x = 0;
	pPos.y = 0;
	pPos.z = 0;
	color = D3DCOLOR_RGBA(255, 255, 255, 255);
	result = false;
}

cSprite::cSprite(int x, int y)
{
	pPos.x = x;
	pPos.y = y;
	pPos.z = 0;
	color = D3DCOLOR_RGBA(255, 255, 255, 255);
	result = false;
}
cSprite::~cSprite()
{
	if (pSprite)
	{
		pSprite->Release();
		pSprite = nullptr;
	}
	if (pTex)
	{
		pTex->Release();
		pTex = nullptr;
	}
}
bool cSprite::IsInit() const
{
	return result;
}
bool cSprite::Init(IDirect3DDevice9* pDev, std::string file, int width, int height)
{
	if (!SUCCEEDED(D3DXCreateTextureFromFileEx(pDev, file.c_str(), width, height,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, NULL, NULL, &pTex)))
	{
		auto s = "Couldn't Create Sprite. Make sure the Image is in the correct place.  Requested image: " + file;
		MessageBox(nullptr, s.c_str(), nullptr, NULL);
		return false;
	}
	if (!SUCCEEDED(D3DXCreateSprite(pDev, &pSprite)))
	{
		MessageBox(nullptr, "Something Fucked up", nullptr, NULL);
		return false;
	}
	result = true;
	return true;
}
void cSprite::Draw()
{
	if (pSprite && pTex)
	{
		pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		pSprite->Draw(pTex, nullptr, nullptr, &pPos, color);
		pSprite->End();
	}
}

bool cSprite::Release()
{
	/*xhooksprite = new cSprite(0, 0);
	xhooksprite->Init(pDevice, XorStr("C://Karma//Pictures//Karma.png"), 250, 250);*/
	if (pSprite)
	{
		pSprite->Release();
		pSprite = nullptr;
	}
	if (pTex)
	{
		pTex->Release();
		pTex = nullptr;
	}
	return TRUE;
}

cSprite*				xhooksprite;