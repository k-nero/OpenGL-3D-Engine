#include "ShadowMap.h"

#include <iostream>

using namespace std;

ShadowMap::ShadowMap()
{
	FBO = 0;
	shadowMap = 0;
	shadowHeight = 0;
	shadowWidth = 0;
}

bool ShadowMap::Init(const int width, const int height)
{
	shadowWidth = width;
	shadowHeight = height;
	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTextureParameteri(shadowMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(shadowMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTextureParameteri(shadowMap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(shadowMap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	constexpr float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (const unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER); status != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Frame-buffer error: " << status << endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::Read(const unsigned textureUnit)
{
	glBindTextureUnit(textureUnit, shadowMap);
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

ShadowMap::~ShadowMap()
{
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}
	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap);
	}
}