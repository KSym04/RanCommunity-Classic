#pragma once

#include "../DxOctree/DxLandMan.h"

class GLActiveMap
{
public:
	virtual DxLandMan* getactivemap() = 0;
};