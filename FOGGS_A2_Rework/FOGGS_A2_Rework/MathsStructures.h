#pragma once
#include <vector>
#include <string>

// Structure: Vector2
	//
	// Description: A 2D Vector that Holds Positional Data
struct Vector2
{
	// Default Constructor
	Vector2()
	{
		X = 0.0f;
		Y = 0.0f;
	}
	// Variable Set Constructor
	Vector2(float X_, float Y_)
	{
		X = X_;
		Y = Y_;
	}
	// Bool Equals Operator Overload
	bool operator==(const Vector2& other) const
	{
		return (this->X == other.X && this->Y == other.Y);
	}
	// Bool Not Equals Operator Overload
	bool operator!=(const Vector2& other) const
	{
		return !(this->X == other.X && this->Y == other.Y);
	}
	// Addition Operator Overload
	Vector2 operator+(const Vector2& right) const
	{
		return Vector2(this->X + right.X, this->Y + right.Y);
	}
	// Subtraction Operator Overload
	Vector2 operator-(const Vector2& right) const
	{
		return Vector2(this->X - right.X, this->Y - right.Y);
	}
	// Float Multiplication Operator Overload
	Vector2 operator*(const float& other) const
	{
		return Vector2(this->X * other, this->Y * other);
	}

	// Positional Variables
	float X;
	float Y;
};

// Structure: Vector3
//
// Description: A 3D Vector that Holds Positional Data
struct Vector3
{
	// Default Constructor
	Vector3()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}
	// Variable Set Constructor
	Vector3(float X_, float Y_, float Z_)
	{
		X = X_;
		Y = Y_;
		Z = Z_;
	}
	// Bool Equals Operator Overload
	bool operator==(const Vector3& other) const
	{
		return (this->X == other.X && this->Y == other.Y && this->Z == other.Z);
	}
	// Bool Not Equals Operator Overload
	bool operator!=(const Vector3& other) const
	{
		return !(this->X == other.X && this->Y == other.Y && this->Z == other.Z);
	}
	// Addition Operator Overload
	Vector3 operator+(const Vector3& right) const
	{
		return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
	}
	// Subtraction Operator Overload
	Vector3 operator-(const Vector3& right) const
	{
		return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
	}
	// Float Multiplication Operator Overload
	Vector3 operator*(const float& other) const
	{
		return Vector3(this->X * other, this->Y * other, this->Z * other);
	}
	// Float Division Operator Overload
	Vector3 operator/(const float& other) const
	{
		return Vector3(this->X / other, this->Y / other, this->Z / other);
	}

	// Positional Variables
	float X;
	float Y;
	float Z;
};

// Structure: Vertex
//
// Description: Model Vertex object that holds
//	a Position, Normal, and Texture Coordinate



// Namespace: Math
//
// Description: This namespace  holds all of the math
//	functions need for loading OBJ files
namespace math
{
	// Vector3 Cross Product
	Vector3 CrossV3(const Vector3 a, const Vector3 b)
	{
		return Vector3(a.Y * b.Z - a.Z * b.Y,
			a.Z * b.X - a.X * b.Z,
			a.X * b.Y - a.Y * b.X);
	}

	// Vector3 Magnitude Calculation
	float MagnitudeV3(const Vector3 in)
	{
		return (sqrtf(powf(in.X, 2) + powf(in.Y, 2) + powf(in.Z, 2)));
	}

	// Vector3 DotProduct
	float DotV3(const Vector3 a, const Vector3 b)
	{
		return (a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z);
	}

	// Angle between 2 Vector3 Objects
	float AngleBetweenV3(const Vector3 a, const Vector3 b)
	{
		float angle = DotV3(a, b);
		angle /= (MagnitudeV3(a) * MagnitudeV3(b));
		return angle = acosf(angle);
	}

	// Projection Calculation of a onto b
	Vector3 ProjV3(const Vector3 a, const Vector3 b)
	{
		Vector3 bn = b / MagnitudeV3(b);
		return bn * DotV3(a, bn);
	}
}

// Namespace: Algorithm
//
// Description: The namespace that holds all of the
// Algorithms needed for OBJL
namespace algorithm
{
	// Vector3 Multiplication Opertor Overload
	Vector3 operator*(const float& left, const Vector3& right)
	{
		return Vector3(right.X * left, right.Y * left, right.Z * left);
	}

	// A test to see if P1 is on the same side as P2 of a line segment ab
	bool SameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b)
	{
		Vector3 cp1 = math::CrossV3(b - a, p1 - a);
		Vector3 cp2 = math::CrossV3(b - a, p2 - a);

		if (math::DotV3(cp1, cp2) >= 0)
			return true;
		else
			return false;
	}

	// Generate a cross produect normal for a triangle
	Vector3 GenTriNormal(Vector3 t1, Vector3 t2, Vector3 t3)
	{
		Vector3 u = t2 - t1;
		Vector3 v = t3 - t1;

		Vector3 normal = math::CrossV3(u, v);

		return normal;
	}

	// Check to see if a Vector3 Point is within a 3 Vector3 Triangle
	bool inTriangle(Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3)
	{
		// Test to see if it is within an infinite prism that the triangle outlines.
		bool within_tri_prisim = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3)
			&& SameSide(point, tri3, tri1, tri2);

		// If it isn't it will never be on the triangle
		if (!within_tri_prisim)
			return false;

		// Calulate Triangle's Normal
		Vector3 n = GenTriNormal(tri1, tri2, tri3);

		// Project the point onto this normal
		Vector3 proj = math::ProjV3(point, n);

		// If the distance from the triangle to the point is 0
		//	it lies on the triangle
		if (math::MagnitudeV3(proj) == 0)
			return true;
		else
			return false;
	}

	// Split a String into a string array at a given token
	inline void split(const std::string& in,
		std::vector<std::string>& out,
		std::string token)
	{
		out.clear();

		std::string temp;

		for (int i = 0; i < int(in.size()); i++)
		{
			std::string test = in.substr(i, token.size());

			if (test == token)
			{
				if (!temp.empty())
				{
					out.push_back(temp);
					temp.clear();
					i += (int)token.size() - 1;
				}
				else
				{
					out.push_back("");
				}
			}
			else if (i + token.size() >= in.size())
			{
				temp += in.substr(i, token.size());
				out.push_back(temp);
				break;
			}
			else
			{
				temp += in[i];
			}
		}
	}

	// Get tail of string after first token and possibly following spaces
	inline std::string tail(const std::string& in)
	{
		size_t token_start = in.find_first_not_of(" \t");
		size_t space_start = in.find_first_of(" \t", token_start);
		size_t tail_start = in.find_first_not_of(" \t", space_start);
		size_t tail_end = in.find_last_not_of(" \t");
		if (tail_start != std::string::npos && tail_end != std::string::npos)
		{
			return in.substr(tail_start, tail_end - tail_start + 1);
		}
		else if (tail_start != std::string::npos)
		{
			return in.substr(tail_start);
		}
		return "";
	}

	// Get first token of string
	inline std::string firstToken(const std::string& in)
	{
		if (!in.empty())
		{
			size_t token_start = in.find_first_not_of(" \t");
			size_t token_end = in.find_first_of(" \t", token_start);
			if (token_start != std::string::npos && token_end != std::string::npos)
			{
				return in.substr(token_start, token_end - token_start);
			}
			else if (token_start != std::string::npos)
			{
				return in.substr(token_start);
			}
		}
		return "";
	}

	// Get element at given index position
	template <class T>
	inline const T& getElement(const std::vector<T>& elements, std::string& index)
	{
		int idx = std::stoi(index);
		if (idx < 0)
			idx = int(elements.size()) + idx;
		else
			idx--;
		return elements[idx];
	}
}