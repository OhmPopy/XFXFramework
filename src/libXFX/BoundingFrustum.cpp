// Copyright (C) XFX Team
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright 
//       notice, list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright 
//       notice, list of conditions and the following disclaimer in the 
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holder nor the names of any 
//       contributors may be used to endorse or promote products derived from 
//       software without specific prior written permission.
// 
// SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#include <System/Array.h>
#include <System/Math.h>
#include <System/Single.h>
#include <System/Type.h>
#include <BoundingBox.h>
#include <BoundingSphere.h>
#include <BoundingFrustum.h>
#include <Ray.h>
#include <Vector3.h>

#include <sassert.h>

using namespace System;

namespace XFX
{
	const Type BoundingFrustumTypeInfo("BoundingFrustum", "XFX::BoundingFrustum", TypeCode::Object);

	BoundingFrustum::BoundingFrustum()
	{
		planes[0] = Plane();
		planes[1] = Plane();
		planes[2] = Plane();
		planes[3] = Plane();
		planes[4] = Plane();
		planes[5] = Plane();
		cornerArray[0] = Vector3();
		cornerArray[1] = Vector3();
		cornerArray[2] = Vector3();
		cornerArray[3] = Vector3();
		cornerArray[4] = Vector3();
		cornerArray[5] = Vector3();
		cornerArray[6] = Vector3();
		cornerArray[7] = Vector3();
	}

	BoundingFrustum::BoundingFrustum(Matrix value)
	{
		planes[0] = Plane();
		planes[1] = Plane();
		planes[2] = Plane();
		planes[3] = Plane();
		planes[4] = Plane();
		planes[5] = Plane();
		cornerArray[0] = Vector3();
		cornerArray[1] = Vector3();
		cornerArray[2] = Vector3();
		cornerArray[3] = Vector3();
		cornerArray[4] = Vector3();
		cornerArray[5] = Vector3();
		cornerArray[6] = Vector3();
		cornerArray[7] = Vector3();

		setMatrix(value);
	}

	BoundingFrustum::BoundingFrustum(const BoundingFrustum &obj)
	{
		planes[0] = obj.planes[0];
		planes[1] = obj.planes[1];
		planes[2] = obj.planes[2];
		planes[3] = obj.planes[3];
		planes[4] = obj.planes[4];
		planes[5] = obj.planes[5];
		cornerArray[0] = obj.cornerArray[0];
		cornerArray[1] = obj.cornerArray[1];
		cornerArray[2] = obj.cornerArray[2];
		cornerArray[3] = obj.cornerArray[3];
		cornerArray[4] = obj.cornerArray[4];
		cornerArray[5] = obj.cornerArray[5];
		cornerArray[6] = obj.cornerArray[6];
		cornerArray[7] = obj.cornerArray[7];

		setMatrix(obj.matrix);
	}

	Plane BoundingFrustum::Bottom()
	{
		return planes[5];
	}

	Plane BoundingFrustum::Far()
	{
		return planes[1];
	}

	Plane BoundingFrustum::Left()
	{
		return planes[2];
	}

	Matrix BoundingFrustum::getMatrix()
	{
		return matrix;
	}

	void BoundingFrustum::setMatrix(Matrix value)
	{
		matrix = value;
		planes[2].Normal.X = -value.M14 - value.M11;
		planes[2].Normal.Y = -value.M24 - value.M21;
		planes[2].Normal.Z = -value.M34 - value.M31;
		planes[2].D = -value.M44 - value.M41;
		planes[3].Normal.X = -value.M14 + value.M11;
		planes[3].Normal.Y = -value.M24 + value.M21;
		planes[3].Normal.Z = -value.M34 + value.M31;
		planes[3].D = -value.M44 + value.M41;
		planes[4].Normal.X = -value.M14 + value.M12;
		planes[4].Normal.Y = -value.M24 + value.M22;
		planes[4].Normal.Z = -value.M34 + value.M32;
		planes[4].D = -value.M44 + value.M42;
		planes[5].Normal.X = -value.M14 - value.M12;
		planes[5].Normal.Y = -value.M24 - value.M22;
		planes[5].Normal.Z = -value.M34 - value.M32;
		planes[5].D = -value.M44 - value.M42;
		planes[0].Normal.X = -value.M13;
		planes[0].Normal.Y = -value.M23;
		planes[0].Normal.Z = -value.M33;
		planes[0].D = -value.M43;
		planes[1].Normal.X = -value.M14 + value.M13;
		planes[1].Normal.Y = -value.M24 + value.M23;
		planes[1].Normal.Z = -value.M34 + value.M33;
		planes[1].D = -value.M44 + value.M43;

		for (int i = 0; i < 6; i++)
		{
			float num2 = planes[i].Normal.Length();
			planes[i].Normal = (planes[i].Normal / num2);
			planes[i].D /= num2;
		}

		Ray ray = ComputeIntersectionLine(planes[0], planes[2]);
		cornerArray[0] = ComputeIntersection(planes[4], ray);
		cornerArray[3] = ComputeIntersection(planes[5], ray);
		ray = ComputeIntersectionLine(planes[3], planes[0]);
		cornerArray[1] = ComputeIntersection(planes[4], ray);
		cornerArray[2] = ComputeIntersection(planes[5], ray);
		ray = ComputeIntersectionLine(planes[2], planes[1]);
		cornerArray[4] = ComputeIntersection(planes[4], ray);
		cornerArray[7] = ComputeIntersection(planes[5], ray);
		ray = ComputeIntersectionLine(planes[1], planes[3]);
		cornerArray[5] = ComputeIntersection(planes[4], ray);
		cornerArray[6] = ComputeIntersection(planes[5], ray);
	}

	Plane BoundingFrustum::Near()
	{
		return planes[0];
	}

	Plane BoundingFrustum::Right()
	{
		return planes[3];
	}

	Plane BoundingFrustum::Top()
	{
		return planes[4];
	}

	Vector3 BoundingFrustum::ComputeIntersection(Plane plane, Ray ray)
	{
		float num = (-plane.D - Vector3::Dot(plane.Normal, ray.Position)) / Vector3::Dot(plane.Normal, ray.Direction);
		return (ray.Position + (ray.Direction * num));
	}

	Ray BoundingFrustum::ComputeIntersectionLine(Plane p1, Plane p2)
	{
		Ray ray = Ray();
		ray.Direction = Vector3::Cross(p1.Normal, p2.Normal);
		float num = ray.Direction.LengthSquared();
		ray.Position = (Vector3::Cross(((p2.Normal * -p1.D) + (p1.Normal * p2.D)), ray.Direction) / num);
		return ray;
	}

	ContainmentType_t BoundingFrustum::Contains(BoundingBox box)
	{
		bool flag = false;
		for(int i = 0; i < 6; i++)
		{
			switch (box.Intersects(planes[i]))
			{
			case PlaneIntersectionType::Front:
				return ContainmentType::Disjoint;

			case PlaneIntersectionType::Intersecting:
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			return ContainmentType::Contains;
		}

		return ContainmentType::Intersects;
	}

	ContainmentType_t BoundingFrustum::Contains(BoundingFrustum frustrum)
	{
		ContainmentType_t disjoint = ContainmentType::Disjoint;

		if (Intersects(frustrum))
		{
			disjoint = ContainmentType::Contains;

			for (int i = 0; i < 8; i++)
			{
				if (Contains(frustrum.cornerArray[i]) == ContainmentType::Disjoint)
				{
					return ContainmentType::Intersects;
				}
			}
		}

		return disjoint;
	}

	ContainmentType_t BoundingFrustum::Contains(BoundingSphere sphere)
	{
		Vector3 center = sphere.Center;
		float radius = sphere.Radius;
		int num2 = 0;

		for (int i = 0; i < 6; i++)
		{
			float num5 = ((planes[i].Normal.X * center.X) + (planes[i].Normal.Y * center.Y)) + (planes[i].Normal.Z * center.Z);
			float num3 = num5 + planes[i].D;

			if (num3 > radius)
			{
				return ContainmentType::Disjoint;
			}

			if (num3 < -radius)
			{
				num2++;
			}
		}

		if (num2 != 6)
		{
			return ContainmentType::Intersects;
		}

		return ContainmentType::Contains;
	}

	ContainmentType_t BoundingFrustum::Contains(Vector3 point)
	{
		for (int i = 0; i < 6; i++)
		{
			float num2 = (((planes[i].Normal.X * point.X) + (planes[i].Normal.Y * point.Y)) + (planes[i].Normal.Z * point.Z)) + planes[i].D;

			if (num2 > 1E-05f)
			{
				return ContainmentType::Disjoint;
			}
		}

		return ContainmentType::Contains;
	}

	void BoundingFrustum::Contains(BoundingBox box, out ContainmentType_t& result)
	{
		bool flag = false;

		for (int i = 0; i < 6; i++)
		{
			switch (box.Intersects(planes[i]))
			{
			case PlaneIntersectionType::Front:
				result = ContainmentType::Disjoint;
				return;

			case PlaneIntersectionType::Intersecting:
				flag = true;
				break;
			}
		}

		result = flag ? ContainmentType::Intersects : ContainmentType::Contains;
	}

	void BoundingFrustum::Contains(BoundingSphere sphere, out ContainmentType_t& result)
	{
		Vector3 center = sphere.Center;
		float radius = sphere.Radius;
		int num2 = 0;

		for (int i = 0; i < 6; i++)
		{
			float num5 = ((planes[i].Normal.X * center.X) + (planes[i].Normal.Y * center.Y)) + (planes[i].Normal.Z * center.Z);
			float num3 = num5 + planes[i].D;

			if (num3 > radius)
			{
				result = ContainmentType::Disjoint;
				return;
			}

			if (num3 < -radius)
			{
				num2++;
			}
		}

		result = (num2 == 6) ? ContainmentType::Contains : ContainmentType::Intersects;
	}

	void BoundingFrustum::Contains(Vector3 point, out ContainmentType_t& result)
	{
		for (int i = 0; i < 6; i++)
		{
			float num2 = (((planes[i].Normal.X * point.X) + (planes[i].Normal.Y * point.Y)) + (planes[i].Normal.Z * point.Z)) + planes[i].D;

			if (num2 > 1E-05f)
			{
				result = ContainmentType::Disjoint;
				return;
			}
		}

		result = ContainmentType::Contains;
	}

	bool BoundingFrustum::Equals(Object const * const obj) const
	{
		return (obj != null && is(this, obj)) ? this->Equals(*(BoundingFrustum *)obj) : false;
	}

	bool BoundingFrustum::Equals(const BoundingFrustum other) const
	{
		return (matrix == other.matrix);
	}

	Vector3* BoundingFrustum::GetCorners()
	{
		return cornerArray;
	}

	void BoundingFrustum::GetCorners(Vector3 corners[])
	{
		sassert(corners != null, "corners cannot be null.");

		for (int i = 0; i < 8; i++)
		{
			corners[i] = cornerArray[i];
		}
	}

	int BoundingFrustum::GetHashCode() const
	{
		return matrix.GetHashCode();
	}

	const Type& BoundingFrustum::GetType()
	{
		return BoundingFrustumTypeInfo;
	}

	bool BoundingFrustum::Intersects(BoundingBox box)
	{
		bool flag = false;
		Intersects(box, flag);
		return flag;
	}

	bool BoundingFrustum::Intersects(BoundingFrustum frustrum)
	{
		sassert(false, "function not implemented.");

		return false;
	}

	bool BoundingFrustum::Intersects(BoundingSphere sphere)
	{
		bool flag = false;
		Intersects(sphere, flag);
		return flag;
	}

	PlaneIntersectionType_t BoundingFrustum::Intersects(Plane plane)
	{
		int num = 0;

		for (int i = 0; i < 8; i++)
		{
			float num3 = 0;
			Vector3::Dot(cornerArray[i], plane.Normal, num3);

			if ((num3 + plane.D) > 0)
			{
				num |= 1;
			}
			else
			{
				num |= 2;
			}

			if (num == 3)
			{
				return PlaneIntersectionType::Intersecting;
			}
		}

		if (num != 1)
		{
			return PlaneIntersectionType::Back;
		}

		return PlaneIntersectionType::Front;
	}

	float BoundingFrustum::Intersects(Ray ray)
	{
		float result = 0;
		Intersects(ray, result);
		return result;
	}

	void BoundingFrustum::Intersects(BoundingBox box, out bool& result)
	{
		sassert(false, "function not implemented.");
	}

	void BoundingFrustum::Intersects(BoundingSphere sphere, out bool& result)
	{
		sassert(false, "function not implemented.");
	}

	void BoundingFrustum::Intersects(Plane plane, out PlaneIntersectionType_t& result)
	{
		int num = 0;

		for (int i = 0; i < 8; i++)
		{
			float num3 = 0;
			Vector3::Dot(cornerArray[i], plane.Normal, num3);

			if ((num3 + plane.D) > 0)
			{
				num |= 1;
			}
			else
			{
				num |= 2;
			}

			if (num == 3)
			{
				result = PlaneIntersectionType::Intersecting;
				return;
			}
		}

		result = (num == 1) ? PlaneIntersectionType::Front : PlaneIntersectionType::Back;
	}

	void BoundingFrustum::Intersects(Ray ray, out float& result)
	{
		ContainmentType_t type = Contains(ray.Position);

		if (type == ContainmentType::Contains)
		{
			result = 0.0f;
		}
		else
		{
			float minValue = Single::MinValue;
			float maxValue = Single::MaxValue;
			result = 0;

			for (int i = 0; i < 6; i++)
			{
				float num3 = 0;
				float num6 = 0;
				Vector3 normal = planes[i].Normal;
				Vector3::Dot(ray.Direction, normal, num6);
				Vector3::Dot(ray.Position, normal, num3);
				num3 += planes[i].D;

				if (Math::Abs(num6) < 1E-05f)
				{
					if (num3 > 0.0f)
					{
						return;
					}
				}
				else
				{
					float num = -num3 / num6;

					if (num6 < 0.0f)
					{
						if (num > maxValue)
						{
							return;
						}

						if (num > minValue)
						{
							minValue = num;
						}
					}
					else
					{
						if (num < minValue)
						{
							return;
						}

						if (num < maxValue)
						{
							maxValue = num;
						}
					}
				}
			}

			float num7 = (minValue >= 0) ? minValue : maxValue;

			if (num7 >= 0)
			{
				result = float(num7);
			}
		}
	}

	bool BoundingFrustum::operator !=(const BoundingFrustum& right) const
	{
		return !Equals(right);
	}

	bool BoundingFrustum::operator ==(const BoundingFrustum& right) const
	{
		return Equals(right);
	}
}
