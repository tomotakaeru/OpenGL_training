#pragma once
#include <algorithm>
#include <cmath>
#include <GL/glew.h> 

// 変換行列
class Matrix {
	// 変換行列の要素
	GLfloat matrix[16];

public:

	// コンストラクタ
	Matrix() {} 

	// 配列の内容で初期化するコンストラクタ
	//   a: GLfloat 型の 16 要素の配列
	Matrix(const GLfloat *a)
	{
		std::copy(a, a + 16, matrix);
	} 

	// 変換行列の配列を返す
	const GLfloat *data() const
	{
		return matrix;
	}

	// 単位行列を設定する
	void loadIdentity()
	{
		std::fill(matrix, matrix + 16, 0.0f);
		matrix[ 0] = matrix[ 5] = matrix[10] = matrix[15] = 1.0f;
	} 

	// 単位行列を作成する
	static Matrix identity()
	{
		Matrix t;
		t.loadIdentity();
		return t;
	} 

	// (x, y, z) だけ平行移動する変換行列を作成する
	static Matrix translate(GLfloat x, GLfloat y, GLfloat z)
	{
		Matrix t; 
		t.loadIdentity();
		t.matrix[12] = x;
		t.matrix[13] = y;
		t.matrix[14] = z;
		return t;
	}
	// (x, y, z)  倍に拡大縮小する変換行列を作成する
	static Matrix scale(GLfloat x, GLfloat y, GLfloat z)
	{
		Matrix t;
		t.loadIdentity();
		t.matrix[ 0] = x;
		t.matrix[ 5] = y;
		t.matrix[10] = z;
		return t;
	}

	// a 座標値に，b 座標値をs 倍したものを加える変換行列を作成する
	// a, b: x座標=0, y座標=1, z座標=2
	static Matrix shear(GLfloat s, GLint a, GLint b)
	{
		Matrix t;
		t.loadIdentity();

		switch (a)
		{
		case 0:
			switch (b)
			{
			case 1:
				t.matrix[4] = s;
			case 2:
				t.matrix[8] = s;
			default:
				printf("Error: a,b must be defferent num from 0,1,2\n");
				break;
			}
		case 1:
			switch (b)
			{
			case 0:
				t.matrix[1] = s;
			case 2:
				t.matrix[9] = s;
			default:
				printf("Error: a,b must be defferent num from 0,1,2\n");
				break;
			}
		case 2:
			switch (b)
			{
			case 0:
				t.matrix[2] = s;
			case 1:
				t.matrix[6] = s;
			default:
				printf("Error: a,b must be defferent num from 0,1,2\n");
				break;
			}
		default:
			printf("Error: a,b must be defferent num from 0,1,2\n");
			break;
		}

		return t;
	}

	// (x, y, z) を軸に a 回転する変換行列を作成する
	static Matrix rotate(GLfloat a, GLfloat x, GLfloat y, GLfloat z)
	{
		Matrix t;
		const GLfloat d(sqrt(x * x + y * y + z * z)); 

		if (d > 0.0f) {
			const GLfloat l(x / d), m(y / d), n(z / d);
			const GLfloat l2(l * l), m2(m * m), n2(n * n);
			const GLfloat lm(l * m), mn(m * n), nl(n * l);
			const GLfloat c(cos(a)), c1(1.0f - c), s(sin(a));

			t.loadIdentity();
			t.matrix[0] = (1.0f - l2) * c + l2;
			t.matrix[1] = lm * c1 + n * s;
			t.matrix[2] = nl * c1 - m * s;
			t.matrix[4] = lm * c1 - n * s;
			t.matrix[5] = (1.0f - m2) * c + m2;
			t.matrix[6] = mn * c1 + l * s;
			t.matrix[8] = nl * c1 + m * s;
			t.matrix[9] = mn * c1 - l * s;
			t.matrix[10] = (1.0f - n2) * c + n2;
		}
		else
		{
			printf("Error: unset rotate axis\n");
		}

		return t;
	}

	// 乗算
	Matrix operator*(const Matrix &m) const
	{
		Matrix t; 

		for (int i = 0; i < 16; ++i) {
			const int j(i & 3), k(i & ~3);

			t.matrix[i] = 
				matrix[0 + j] * m.matrix[k + 0] + 
				matrix[4 + j] * m.matrix[k + 1] + 
				matrix[8 + j] * m.matrix[k + 2] + 
				matrix[12 + j] * m.matrix[k + 3];
		}

		return t;
	}

	// ビュー変換行列を作成する
	static Matrix lookat(
		GLfloat ex, GLfloat ey, GLfloat ez,   // 視点の位置
		GLfloat gx, GLfloat gy, GLfloat gz,   // 目標点の位置
		GLfloat ux, GLfloat uy, GLfloat uz)   // 上方向のベクトル（上下がどっちかを決めるだけ）
	{
		// 平行移動の変換行列
		const Matrix tv(translate(-ex, -ey, -ez));

		// t 軸 = e - g
		const GLfloat tx(ex - gx);
		const GLfloat ty(ey - gy);
		const GLfloat tz(ez - gz); 

		// r 軸 = u x t 軸
		const GLfloat rx(uy * tz - uz * ty);
		const GLfloat ry(uz * tx - ux * tz);
		const GLfloat rz(ux * ty - uy * tx); 

		// s 軸 = t 軸 x r 軸
		const GLfloat sx(ty * rz - tz * ry);
		const GLfloat sy(tz * rx - tx * rz);
		const GLfloat sz(tx * ry - ty * rx);

		// s 軸の長さのチェック
		const GLfloat s2(sx * sx + sy * sy + sz * sz);
		if (s2 == 0.0f) return tv; 

		// 回転の変換行列
		Matrix rv;
		rv.loadIdentity(); 

		// r 軸を正規化して配列変数に格納
		const GLfloat r(sqrt(rx * rx + ry * ry + rz * rz));
		rv.matrix[ 0] = rx / r;
		rv.matrix[ 4] = ry / r;
		rv.matrix[ 8] = rz / r; 

		// s 軸を正規化して配列変数に格納
		const GLfloat s(sqrt(s2));
		rv.matrix[ 1] = sx / s;
		rv.matrix[ 5] = sy / s;
		rv.matrix[ 9] = sz / s; 

		// t 軸を正規化して配列変数に格納
		const GLfloat t(sqrt(tx * tx + ty * ty + tz * tz));
		rv.matrix[ 2] = tx / t;
		rv.matrix[ 6] = ty / t;
		rv.matrix[10] = tz / t; 
	
		// 視点の平行移動の変換行列に視線の回転の変換行列を乗じる
		return rv * tv;
	} 
};