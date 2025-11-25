
#include "Block.h"
#include "camera.h"
#include "model.h"

MODEL* Model[FIELD_MAX] = { NULL };//�f�o�b�O


//�O���[�o���ϐ�
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;

#define		BOX_NUM_VERTEX (24)


//BOX���_�f�[�^
static	Vertex3D Box_vdata[BOX_NUM_VERTEX] =
{
	//-Z
	{//���_�O�@LEFT-TOP
		XMFLOAT3(-0.5f,0.5f,-0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(0.0f,0.0f)				//�e�N�X�`�����W
	},
	{//���_�P�@RIGHT-TOP
		XMFLOAT3(0.5f,0.5f,-0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(1.0f,0.0f)				//�e�N�X�`�����W
	},
	{//���_�Q�@LEFT-BOTTOM
		XMFLOAT3(-0.5f,-0.5f,-0.5f),	//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(0.0f,1.0f)				//�e�N�X�`�����W
	},
	{//���_3�@RIGHT-BOTTOM
		XMFLOAT3(0.5f,-0.5f,-0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(1.0f,1.0f)				//�e�N�X�`�����W
	},

	//+X��
	{//���_4�@LEFT-TOP
		XMFLOAT3(0.5f,0.5f,-0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(0.0f,0.0f)				//�e�N�X�`�����W
	},
	{//���_5�@RIGHT-TOP
		XMFLOAT3(0.5f,0.5f,0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(1.0f,0.0f)				//�e�N�X�`�����W
	},
	{//���_6�@LEFT-BOTTOM
		XMFLOAT3(0.5f,-0.5f,-0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(0.0f,1.0f)				//�e�N�X�`�����W
	},
	{//���_7�@RIGHT-BOTTOM
		XMFLOAT3(0.5f,-0.5f,0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(1.0f,1.0f)				//�e�N�X�`�����W
	},

	//+Z

	{//���_8�@RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),//�F
		XMFLOAT2(0.0f,0.0f)//�e�N�X�`�����W
	},
	{//���_9�@LEFT-TOP
		XMFLOAT3(-0.5f,0.5f,0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(1.0f,0.0f)				//�e�N�X�`�����W
	},
	{//���_10�@LEFT-BOTTOM
		XMFLOAT3(0.5f,-0.5f,0.5f),	//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(0.0f,1.0f)				//�e�N�X�`�����W
	},
	{//���_11�@RIGHT-TOP
		XMFLOAT3(-0.5f,-0.5f,0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(1.0f,1.0f)				//�e�N�X�`�����W
	},
	//-X

	{//���_12�@LEFT-TOP
		XMFLOAT3(-0.5f,0.5f,0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(0.0f,0.0f)				//�e�N�X�`�����W
	},
	{//���_13�@LEFT-BOTTOM
		XMFLOAT3(-0.5f,0.5f,-0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(1.0f,0.0f)				//�e�N�X�`�����W
	},
	{//���_14�@RIGHT-TOP
		XMFLOAT3(-0.5f,-0.5f,0.5f),		//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(0.0f,1.0f)				//�e�N�X�`�����W
	},
	{//���_15�@RIGHT-TOP
		XMFLOAT3(-0.5f,-0.5f,-0.5f),	//���W
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�F
		XMFLOAT2(1.0f,1.0f)				//�e�N�X�`�����W
	},

	//+Y

	{//���_16 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.0f,1.0f,0.0f),		//�@��
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(0.0f,0.0f)
	},
	{//���_17 RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.0f,1.0f,0.0f),		//�@��
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f,0.0f)
	},
	{//���_18 LEFT-BOTTOM
		XMFLOAT3(-0.5f, 0.5f, -0.5f),
		XMFLOAT3(0.0f,1.0f,0.0f),		//�@��
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(0.0f,0.25f)
	},
	{//���_19 RIGHT-BOTTOM
		XMFLOAT3(0.5f, 0.5f, -0.5f),
		XMFLOAT3(0.0f,1.0f,0.0f),		//�@��
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f,0.25f)
	},
	//-Y��
	{//���_20 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, -0.5f),
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(0.0f,0.75f)
	},
	{//���_21 RIGHT-TOP
		XMFLOAT3(0.5f, -0.5f, -0.5f),
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f,0.75f)
	},
	{//���_22 LEFT-BOTTOM
		XMFLOAT3(-0.5f, -0.5f, 0.5f),
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(0.0f,1.0f)
	},
	{//���_23 RIGHT-BOTTOM
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(0.5f,0.5f,0.5f),		//�@��
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f,1.0f)
	},

	//��ő����܂�

};

//�C���f�b�N�X�z��
static UINT Box_idxdata[6 * 6] =
{
	 0, 1, 2, 2, 1, 3,	//-Z��
	 4, 5, 6, 6, 5, 7,	//+X��
	 8, 9,10,10, 9,11,	//+Z��
	12,13,14,14,13,15,	//-X��
	16,17,18,18,17,19,	//+Y��
	20,21,22,22,21,23,	//-Y��
};

void MAP::MapData_Initialize(XMFLOAT3 pPos,FIELD pNo)
{
	position = pPos;
	no = pNo;
}

void  MAP::MapData_Finalize(void)
{

}


void  MAP::MapData_Update(void)
{
}


////BOX�f�[�^���쐬����
//void CreateBox()
//{
//
//}

MAP* MAP::GetFieldMap()
{
	//return Map�Ƃ������邪�z��ƕ����肸�炢����
	return this;
}
