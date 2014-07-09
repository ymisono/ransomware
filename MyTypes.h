/*
自作の数学系あれやこれや
*/
#pragma once
#include<Windows.h>
#include<d3d9.h>

namespace Inferno
{
//二次元座標
template<typename T> struct Vec2 //テンプレートはヘッダーに実装を書くのが正解
{
	T x;
	T y;
	Vec2() : x(0), y(0) {}
	Vec2(T x_, T y_) : x(x_), y(y_) {}

	const Vec2<T> operator+(const Vec2<T>& vec) const
	{
		Vec2<T> result = *this;
		result.x += vec.x;
		result.y += vec.y;
		return result;
	}

	const Vec2<T> operator+(Vec2<T>& vec) const
	{
		return this->operator+(vec);
	}

	void operator+=(const Vec2<T>& vec)
	{
		this->x += vec.x;
		this->y += vec.y;
	}

	void operator+=(Vec2<T>& vec)
	{
		this->operator+=(vec);
	}
};

//DirectXの標準行列（3DXのヘルパー関連はなるべく使わない）
typedef D3DMATRIX Matrix;
//以下行列関連ユーテリティ
Matrix* MatrixIdentity(Matrix* m);
Matrix* MatrixTransform2D(Matrix* m, const float x, const float y);

//Windows&DirectXのRECT拡張
//typedef RECT Rect;

class Rect
{
public:
	Rect();
	Rect(const int height, const int width);
	Rect(const Vec2<int>& p, const int height, const int width);

	//Win32のRECTを返す
	RECT GetMSRECT() const;

	int Top() const;
	int Right() const;
	int Left() const;
	int Bottom() const;
	Vec2<int> UpperLeft() const;
	Vec2<int> UpperRight() const;
	Vec2<int> BottomLeft() const;
	Vec2<int> BottomRight() const;

	void SetSize(const int Height, const int Width);
	void AMove(const Vec2<int>& v);
	void AMove(const int x, const int y);
	void RMove(const int x, const int y);
	void SetPosofULCorner(const int x, const int y);
	int Width() const;
	int Height() const;
	Vec2<int> Center() const;


	const Rect operator=(const Rect& r);

private:
	Vec2<int> m_center;
	int m_height;
	int m_width;
};


//以下Rect関連のユーテリティ
//幅と高さよりRectを作る（0を中心に作る）
Rect SetRectFromWH(const unsigned width, const unsigned height);
Vec2<int> GetCenterOfRect(const Rect& r);
long GetWidthOfRect(const Rect& r);
long GetHeightOfRect(const Rect& r);
Rect AddRect(const Rect& r1, const Rect& r2);

/*
//矩形のサイズを表す構造体（位置情報は持たない）
//＊Rectがあれば済むのではないか？
template<typename T> struct RectSize
{
	T width;
	T height;
};
*/
}