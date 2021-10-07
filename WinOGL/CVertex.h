#pragma once
class CVertex
{
public:
	CVertex();
	CVertex(double new_x, double new_y, CVertex* new_next);
	~CVertex();
private:
	//　頂点のX座標
	double x;
	//　頂点のY座標
	double y;
	//　次の頂点リストを指すポインタ
	CVertex* next_vertex;
public:
	//　頂点のX座標を書き込む
	void SetX(double new_x);
	//　頂点のY座標を書き込む
	void SetY(double new_y);
	//　頂点のX・Y座標の両方を書き込む
	void SetXY(int new_x, int new_y);
	//　頂点のX座標を読み込む込む
	double GetX();
	//　頂点のY座標を読み込む
	double GetY();
	//　頂点のX・Y座標の両方を読み込む
	void GetXY(double* get_x, double* get_y);
	//　次の頂点リストを指すポインタを書き込む
	void SetNext(CVertex* new_next);
	//　次の頂点リストを指すポインタを読み込む
	CVertex* GetNext();
};

