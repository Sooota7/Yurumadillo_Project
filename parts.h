#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"


class PARTS
{
private:
	XMFLOAT3 m_Position;//ç¿ïW
	XMFLOAT3 m_Velocity;//à⁄ìÆíl
	XMFLOAT3 m_Rotation;//âÒì]ó¶
	XMFLOAT3 m_Scaling;	//ägëÂó¶

public:
	void	PartsSet(XMFLOAT3 position,XMFLOAT3 rotation);
	void	PartsInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const char* model);
	void	PartsFinalize();
	void	PartsUpdate();
	void	PartsDraw();
	
	MODEL*	m_Model;		//ÉÇÉfÉã

	void SetPartsPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3 GetPartsPosition() { return m_Position; };

	void SetPartsRotation(XMFLOAT3 rotate) { m_Rotation = rotate; };
	XMFLOAT3 GetPartsRotation() { return m_Rotation; };

	void SetPartsScaling(XMFLOAT3 sca) { m_Scaling = sca; };
	XMFLOAT3 GetPartsScaling() { return m_Scaling; };

	void SetPartsVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3 GetPartsVelocity() { return m_Velocity; };


};