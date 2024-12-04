#pragma once

class D9SliceSprite;
class ImguiWindowTab :
    public UIObject
{
public:

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;

protected:
	D9SliceSprite* m_Bar;
};

