#pragma once

//共通言語クラス
//クラス宣言しないで！！！！

const int FIELD_WIDTH_X{ 5 };
const int FIELD_WIDTH_Z{ 10 };

const int Field_pos_row[FIELD_WIDTH_Z][FIELD_WIDTH_X]
=
{
 1,1,1,1,1,
 1,1,1,1,1,
 1,1,1,1,1,
 1,1,1,1,1,
 1,1,1,1,1,
 1,1,1,1,1,
 1,1,1,1,1,
 1,1,1,1,1,
 1,1,1,1,1,
 1,1,1,1,1
};

const int Field_pos_nor[FIELD_WIDTH_Z][FIELD_WIDTH_X]
=
{
 1,1,1,1,1,
 1,0,0,0,1,
 1,0,0,0,1,
 1,0,0,0,1,
 1,0,0,0,1,
 1,0,0,0,1,
 1,0,0,0,1,
 2,0,0,0,2,
 2,0,0,0,2,
 2,2,2,2,2
};

const int Field_pos_high[FIELD_WIDTH_Z][FIELD_WIDTH_X]
=
{
 0,0,1,0,0,
 0,0,0,0,0,
 0,0,0,0,0,
 0,0,0,0,0,
 2,0,0,0,2,
 2,0,0,0,2,
 2,0,0,0,2,
 0,0,0,0,0,
 0,0,0,0,0,
 0,0,0,0,0
};


enum SCENE
{
	SCENE_NONE = 0,		//何もなし
	SCENE_TITLE,		//タイトルシーン
	SCENE_GAME,			//ゲームシーン
	SCENE_RESULT,		//リザルトルシーン
};
