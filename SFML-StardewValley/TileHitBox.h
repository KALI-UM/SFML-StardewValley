#pragma once
class TileHitBox :
	public GameObject
{
public:

	bool Initialize();
	void Reset();
	void Update(float dt);


	sf::FloatRect m_HitBox = { 0,0,16,16 };
	DRectangle* m_HitBoxRect;
};

