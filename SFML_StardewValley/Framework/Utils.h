#pragma once

class Utils
{
private:
	static std::mt19937 generator;
	static const float PI;

public:
	static void Initialize();

	//Random
	static int RandomRange(int min, int max);
	static float RandomRange(float min, float max);
	static float RandomValue();	// 0.0f ~ 1.0f
	static sf::Vector2f RandomOnUnitCircle();
	static sf::Vector2f RandomInUnitCircle();
	static sf::Color RandomColor(bool randomOpacity = false);

	template <typename T>
	static T Clamp(const T& v, const T& min, const T& max)
	{
		T value;
		value = v < min ? min : v;
		value = v > max ? max : v;
		return value;
	}

	template <typename T>
	static T Lerp(const T& left, const T& right, float ratio, bool clamp = true)
	{
		if (clamp)ratio = Utils::Clamp(ratio, 0.0f, 1.0f);
		float value = (float)left * (1.0f - ratio) + (float)right * ratio;
		return value;
	}

	template<>
	static double Lerp(const double& left, const double& right, float ratio, bool clamp)
	{
		if (clamp)ratio = Utils::Clamp(ratio, 0.0f, 1.0f);
		return left * (1.0 - (double)ratio) + right * (double)ratio;
	}

	static sf::Vector2f Lerp(const sf::Vector2f& left, const sf::Vector2f& right, float ratio, bool clamp = true);
	//static sf::Vector2f CLerp(const sf::Vector2f& left, const sf::Vector2f& right, float radius, float ratio, bool clamp = true);

	static float RadianToDegree(float radian);
	static float DegreeToRadian(float degree);
	static float AngleRadian(const sf::Vector2f& vec);
	static float Angle(const sf::Vector2f& vec);

	static float LineEquationX(float slope, float yIntercept, float y);
	static float LineEquationY(float slope, float yIntercept, float x);
	static float LineEquationX(const sf::Vector2f& p1, const sf::Vector2f& p2, float y);
	static float LineEquationY(const sf::Vector2f& p1, const sf::Vector2f& p2, float x);

	static bool CircleCollisionCheck(const sf::Vector2f& p1, float radius1, const sf::Vector2f& p2, float radius2);
	static bool CircleContainPoint(const sf::Vector2f& center, float radius, const sf::Vector2f& pos);

	// Math
	static float SqrMagnitude(const sf::Vector2f& vec);
	static float Magnitude(const sf::Vector2f& vec);

	static void Normailize(sf::Vector2f& vec);
	static sf::Vector2f GetNormal(const sf::Vector2f& vec);
	static float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

	static float Dot(const sf::Vector2f& a, const sf::Vector2f& b);

	// Ãæµ¹
	static bool CheckCollision(const sf::RectangleShape& shapeA, const sf::RectangleShape& shapeB);
	static bool CheckCollision(const sf::Sprite& shapeA, const sf::Sprite& shapeB);
	static bool PointInTransformBounds(const sf::Transformable& transformable,
		const sf::FloatRect& localBounds, const sf::Vector2f point);

	static std::vector<sf::Vector2f> GetShapePoints(const sf::RectangleShape& shape);
	static std::vector<sf::Vector2f> GetShapePoints(const sf::Sprite& shape);

	static std::vector<sf::Vector2f> GetRectanglePointsFromBounds(const sf::FloatRect& localBounds);
	static bool PolygonsIntersect(const std::vector<sf::Vector2f>& polygonA, const sf::Transform& transformA,
		const std::vector<sf::Vector2f>& polygonB, const sf::Transform& transformB);
};