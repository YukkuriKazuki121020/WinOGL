#pragma once
class CVertex
{
public:
	CVertex();
	CVertex(double new_x, double new_y);
	CVertex(double new_x, double new_y, CVertex* new_next);
	~CVertex();
private:
	
	double x;// 頂点のX座標
	double y;// 頂点のY座標
	bool selected_flag;// 選択されたか
	CVertex* next_vertex;// 次の頂点リストを指すポインタ

public:
	void SetX(double new_x);// 頂点のX座標を書き込む
	void SetY(double new_y);// 頂点のY座標を書き込む
	void SetXY(double new_x, double new_y);// 頂点のX・Y座標の両方を書き込む
	double GetX();// 頂点のX座標を読み込む込む
	double GetY();// 頂点のY座標を読み込む
	void GetXY(double* get_x, double* get_y);// 頂点のX・Y座標の両方を読み込む
	void SetNext(CVertex* new_next);// 次の頂点リストを指すポインタを書き込む
	CVertex* GetNext();// 次の頂点リストを指すポインタを読み込む
	void SetSelectedFlag(bool selected_flag);// selected_flagを設定する関数
	bool GetSelectedFlag();// 頂点が選択されたかを返す関数
	void Insert(CVertex* insertVer);// 頂点を追加する関数
};

