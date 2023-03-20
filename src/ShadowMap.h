#pragma once
#include "GL/glew.h"

class ShadowMap
{
public:
	ShadowMap();
	virtual bool Init( int width,  int height);
	virtual void Write();
	virtual void Read(unsigned int textureUnit);
	[[nodiscard]] unsigned int GetShadowWidth() const { return shadowWidth; }
	[[nodiscard]] unsigned int GetShadowHeight() const { return shadowHeight; }
	[[nodiscard]] unsigned int GetFBO() const { return FBO; }
	[[nodiscard]] unsigned int GetShadowMap() const { return shadowMap; }
	~ShadowMap();
protected:
	unsigned int FBO;
	unsigned int shadowMap;
	int shadowWidth;
	int shadowHeight;
private:
};

