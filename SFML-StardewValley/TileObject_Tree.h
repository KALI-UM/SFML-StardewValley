#pragma once

#include "Tile/TileObject.h"
class TileObject_Tree :
	public TileObject
{
public:

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;
	void Release()override;


};

