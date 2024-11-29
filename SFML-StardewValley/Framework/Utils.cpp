#include "pch.h"
#include "Utils.h"

std::mt19937 Utils::generator;
const float Utils::PI = acosf(-1.f);

void Utils::Initialize()
{
	std::random_device rd;
	generator.seed(rd());
}

int Utils::RandomRange(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(generator);
}

float Utils::RandomRange(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(generator);
}

float Utils::RandomValue()
{
	return Utils::RandomRange(0.f, 1.f);
}

sf::Vector2f Utils::RandomOnUnitCircle()
{
	float angle = Utils::RandomRange(0.f, 2.f * PI);
	return sf::Vector2f(std::cosf(angle), std::sinf(angle));
}

sf::Vector2f Utils::RandomInUnitCircle()
{
	return RandomOnUnitCircle() * RandomValue();
}

sf::Color Utils::RandomColor(bool randomOpacity)
{
	return sf::Color(RandomRange(0, 255), RandomRange(0, 255), RandomRange(0, 255), randomOpacity ? RandomRange(0, 255) : 255);
}

sf::Vector2f Utils::Lerp(const sf::Vector2f& left, const sf::Vector2f& right, float ratio, bool clamp)
{
	if (clamp)
		ratio = Utils::Clamp(ratio, 0.f, 1.f);
	return left * (1.f - ratio) + right * ratio;
}

float Utils::RadianToDegree(float radian)
{
	return radian * (180.f / PI);
}

float Utils::DegreeToRadian(float degree)
{
	return degree * (PI / 180.f);
}

float Utils::AngleRadian(const sf::Vector2f& vec)
{
	return std::atan2(vec.y, vec.x);
}

float Utils::Angle(const sf::Vector2f& vec)
{
	return RadianToDegree(AngleRadian(vec));
}

float Utils::LineEquationX(float slope, float yIntercept, float y)
{
	return (y - yIntercept) / slope;
}

float Utils::LineEquationY(float slope, float yIntercept, float x)
{
	return slope * x + yIntercept;
}

float Utils::LineEquationX(const sf::Vector2f& p1, const sf::Vector2f& p2, float y)
{
	sf::Vector2f vec = p1 - p2;
	//평범한 기울기값이 나온다고 가정함
	float slope = vec.y / vec.x;
	return LineEquationX(slope, p1.y - slope * p1.x, y);
}

float Utils::LineEquationY(const sf::Vector2f& p1, const sf::Vector2f& p2, float x)
{
	sf::Vector2f vec = p1 - p2;
	//평범한 기울기값이 나온다고 가정함
	float slope = vec.y / vec.x;
	return LineEquationY(slope, p1.y - slope * p1.x, x);
}

bool Utils::CircleCollisionCheck(const sf::Vector2f& p1, float radius1, const sf::Vector2f& p2, float radius2)
{
	return SqrtMagnitude(p1 - p2) <= (radius1 + radius2) * (radius1 + radius2);
}

bool Utils::CircleContainPoint(const sf::Vector2f& center, float radius, const sf::Vector2f& pos)
{
	return SqrtMagnitude(center - pos) <= radius * radius;
}

float Utils::SqrMagnitude(const sf::Vector2f& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

float Utils::Magnitude(const sf::Vector2f& vec)
{
	return std::sqrtf(SqrMagnitude(vec));
}

void Utils::Normailize(sf::Vector2f& vec)
{
	float mag = Magnitude(vec);
	if (mag != 0.f)
		vec /= mag;
}

sf::Vector2f Utils::GetNormal(const sf::Vector2f& vec)
{
	float mag = Magnitude(vec);
	if (mag == 0)
		return vec;
	return vec / mag;
}

float Utils::Distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return Magnitude(p2 - p1);
}

float Utils::Dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return a.x * b.x + a.y * b.y;
}


bool Utils::CheckCollision(const sf::RectangleShape& shapeA, const sf::RectangleShape& shapeB)
{
    auto pointsA = GetShapePoints(shapeA);
    auto pointsB = GetShapePoints(shapeB);
    return PolygonsIntersect(pointsA, shapeA.getTransform(), pointsB, shapeB.getTransform());
}

bool Utils::CheckCollision(const sf::Sprite& shapeA, const sf::Sprite& shapeB)
{
    auto pointsA = GetShapePoints(shapeA);
    auto pointsB = GetShapePoints(shapeB);
    return PolygonsIntersect(pointsA, shapeA.getTransform(), pointsB, shapeB.getTransform());
}

bool Utils::PointInTransformBounds(const sf::Transformable& transformable, const sf::FloatRect& localBounds, const sf::Vector2f point)
{
    sf::Transform inverse = transformable.getInverseTransform();
    sf::Vector2f localPoint = inverse.transformPoint(point);
    return localBounds.contains(localPoint);
}

std::vector<sf::Vector2f> Utils::GetShapePoints(const sf::RectangleShape& shape)
{
    sf::FloatRect localBounds = shape.getLocalBounds();
    return GetRectanglePointsFromBounds(localBounds);
}

std::vector<sf::Vector2f> Utils::GetShapePoints(const sf::Sprite& shape)
{
    sf::FloatRect localBounds = shape.getLocalBounds();
    return GetRectanglePointsFromBounds(localBounds);
}

std::vector<sf::Vector2f> Utils::GetRectanglePointsFromBounds(const sf::FloatRect& localBounds)
{
    std::vector<sf::Vector2f> points(4);
    points[0] = sf::Vector2f(localBounds.left, localBounds.top);
    points[1] = sf::Vector2f(localBounds.left + localBounds.width, localBounds.top);
    points[2] = sf::Vector2f(localBounds.left + localBounds.width, localBounds.top + localBounds.height);
    points[3] = sf::Vector2f(localBounds.left, localBounds.top + localBounds.height);
    return points;
}

bool Utils::PolygonsIntersect(const std::vector<sf::Vector2f>& polygonA, const sf::Transform& transformA, const std::vector<sf::Vector2f>& polygonB, const sf::Transform& transformB)
{
    std::vector<sf::Vector2f> axes;
    int countA = polygonA.size();
    for (int i = 0; i < countA; ++i)
    {
        sf::Vector2f p1 = transformA.transformPoint(polygonA[i]);
        sf::Vector2f p2 = transformA.transformPoint(polygonA[(i + 1) % countA]);
        sf::Vector2f edge = p2 - p1;
        sf::Vector2f normal(-edge.y, edge.x);
        axes.push_back(Utils::GetNormal(normal));
    }

    int countB = polygonB.size();
    for (int i = 0; i < countB; ++i)
    {
        sf::Vector2f p1 = transformB.transformPoint(polygonB[i]);
        sf::Vector2f p2 = transformB.transformPoint(polygonB[(i + 1) % countB]);
        sf::Vector2f edge = p2 - p1;
        sf::Vector2f normal(-edge.y, edge.x);
        axes.push_back(Utils::GetNormal(normal));
    }

    for (const auto& axis : axes)
    {
        float minA = std::numeric_limits<float>::max();
        float maxA = std::numeric_limits<float>::lowest();
        for (const auto& point : polygonA)
        {
            sf::Vector2f transformedPoint = transformA.transformPoint(point);
            float projection = Dot(axis, transformedPoint);
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }

        float minB = std::numeric_limits<float>::max();
        float maxB = std::numeric_limits<float>::lowest();
        for (const auto& point : polygonB)
        {
            sf::Vector2f transformedPoint = transformB.transformPoint(point);
            float projection = Dot(axis, transformedPoint);
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        if (maxA < minB || maxB < minA)
        {
            return false;
        }
    }
    return true;
}


//sf::Vector2f Utils::CLerp(const sf::Vector2f& left, const sf::Vector2f& right, float radius, float ratio, bool clamp)
//{
//	if (clamp)
//		ratio = Utils::Clamp(ratio, 0.f, 1.f);
//
//	//두 점이 원관계인지... 두점이 가질수 있는 센터 두개중 누구를 기준으로 돌릴지,...후
//
//	float theta = atan2f(right.y, right.x) - atan2f(left.y, left.x);
//	//검증필
//	if (theta > PI)
//	{
//		theta -= 2 * PI;
//	}
//	else if (theta < -PI)
//	{
//		theta += 2 * PI;
//	}
//	return sf::Vector2f(radius * cosf(theta * ratio), radius * sinf(theta * ratio));
//}
