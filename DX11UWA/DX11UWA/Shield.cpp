#include "pch.h"
#include "Shield.h"
#include "DDSTextureLoader.h"
#include "..\Common\DirectXHelper.h"

#include "LoadMesh.h"

using namespace DX11UWA;

using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Shield::Shield(const std::shared_ptr<DX::DeviceResources>& deviceResources, int number, int type) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	ModelCount(number),
	TypeIs(type),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	memset(m_kbuttons, 0, sizeof(m_kbuttons));
	m_currMousePos = nullptr;
	m_prevMousePos = nullptr;
	memset(&m_camera, 0, sizeof(XMFLOAT4X4));

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void Shield::CreateWindowSizeDependentResources(void)
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / (outputSize.Height * .5);
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(&m_constantBufferData.projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, 0.7f, -1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_camera, XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up)));
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));

	static const XMVECTORF32 eye2 = { 0.0f, 25.0f, -1.5f, 0.0f };
	static const XMVECTORF32 at2 = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up2 = { 0.0f, 10.0f, 0.0f, 0.0f };
	XMStoreFloat4x4(&m_camera2, XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye2, at2, up2)));
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye2, at2, up2)));
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Shield::Update(DX::StepTimer const& timer, int modify)
{
	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		Rotate(radians, modify);
	}

	// Update or move camera here
	UpdateCamera(timer, 7.0f, 0.75f);

}

//Rotate the 3D cube model a set amount of radians.
void Shield::Rotate(float radians, int model)
{
	// Prepare to pass the updated model matrix to the shader
	//To adjust the trees
	if (TypeIs == 1)
	{
		if (model == 0)
		{
			XMStoreFloat4x4(&m_constantBufferData.model[0], XMMatrixTranspose(XMMatrixTranslation(15, -5, 20) * XMMatrixScaling(.1, .1, .1)));
		}
		else if (model == 1)
		{
			XMStoreFloat4x4(&m_constantBufferData.model[1], XMMatrixTranspose(XMMatrixTranslation(-15, -5, 20) * XMMatrixScaling(.1, .1, .1)));
		}
	}

	//To adjust the cubes
	if (TypeIs == 2)
	{
		XMStoreFloat4x4(&m_constantBufferData.model[0], XMMatrixTranspose(XMMatrixTranslation(0, -1, 3)));

		if (model == 0)
		{
			XMStoreFloat4x4(&m_constantBufferData.model[0], XMMatrixTranspose(XMMatrixTranslation(0, -1, 3)));
		}
		else if (model == 1)
		{
			XMStoreFloat4x4(&m_constantBufferData.model[1], XMMatrixTranspose(XMMatrixTranslation(0, -1, 6)));
		}
		else if (model == 2)
		{
			XMStoreFloat4x4(&m_constantBufferData.model[2], XMMatrixTranspose(XMMatrixTranslation(0, -1, 9)));
		}
		else if (model == 3)
		{
			XMStoreFloat4x4(&m_constantBufferData.model[3], XMMatrixTranspose(XMMatrixTranslation(20, 5, 3)));
		}
	}

	if (TypeIs == 3)
	{
		XMStoreFloat4x4(&m_constantBufferData.model[0], XMMatrixTranspose(XMMatrixTranslation(-3, -1, 3)));
	}

	if (TypeIs == 4)
	{
		XMStoreFloat4x4(&m_constantBufferData.model[0], XMMatrixTranspose(XMMatrixTranslation(3, -1, 3)));
	}
}

void Shield::UpdateCamera(DX::StepTimer const& timer, float const moveSpd, float const rotSpd)
{
	const float delta_time = (float)timer.GetElapsedSeconds();

	if (m_kbuttons['W'])
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, moveSpd * delta_time);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}
	if (m_kbuttons['S'])
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -moveSpd * delta_time);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}
	if (m_kbuttons['A'])
	{
		XMMATRIX translation = XMMatrixTranslation(-moveSpd * delta_time, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}
	if (m_kbuttons['D'])
	{
		XMMATRIX translation = XMMatrixTranslation(moveSpd * delta_time, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}
	if (m_kbuttons['X'])
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, -moveSpd * delta_time, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}
	if (m_kbuttons[VK_SPACE])
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, moveSpd * delta_time, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_camera, result);
	}

	if (m_currMousePos)
	{
		if (m_currMousePos->Properties->IsRightButtonPressed && m_prevMousePos)
		{
			float dx = m_currMousePos->Position.X - m_prevMousePos->Position.X;
			float dy = m_currMousePos->Position.Y - m_prevMousePos->Position.Y;

			XMFLOAT4 pos = XMFLOAT4(m_camera._41, m_camera._42, m_camera._43, m_camera._44);

			m_camera._41 = 0;
			m_camera._42 = 0;
			m_camera._43 = 0;

			XMMATRIX rotX = XMMatrixRotationX(dy * rotSpd * delta_time);
			XMMATRIX rotY = XMMatrixRotationY(dx * rotSpd * delta_time);

			XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera);
			temp_camera = XMMatrixMultiply(rotX, temp_camera);
			temp_camera = XMMatrixMultiply(temp_camera, rotY);

			XMStoreFloat4x4(&m_camera, temp_camera);

			m_camera._41 = pos.x;
			m_camera._42 = pos.y;
			m_camera._43 = pos.z;
		}
		m_prevMousePos = m_currMousePos;
	}


}

void Shield::SetKeyboardButtons(const char* list)
{
	memcpy_s(m_kbuttons, sizeof(m_kbuttons), list, sizeof(m_kbuttons));
}

void Shield::SetMousePosition(const Windows::UI::Input::PointerPoint^ pos)
{
	m_currMousePos = const_cast<Windows::UI::Input::PointerPoint^>(pos);
}

void Shield::SetInputDeviceData(const char* kb, const Windows::UI::Input::PointerPoint^ pos)
{
	SetKeyboardButtons(kb);
	SetMousePosition(pos);
}

void DX11UWA::Shield::StartTracking(void)
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Shield::TrackingUpdate(float positionX, int modify)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		Rotate(radians, modify);
	}
}

void Shield::StopTracking(void)
{
	m_tracking = false;
}

void Shield::ChangeRender(XMFLOAT4X4 m_camera, int input)
{
	for (size_t i = 0; i < input - 1; i++)
	{
		XMFLOAT3 Cube1 = { m_constantBufferData.model[i]._14, m_constantBufferData.model[i]._24 , m_constantBufferData.model[i]._34 };
		XMFLOAT3 Cube2 = { m_constantBufferData.model[i + 1]._14, m_constantBufferData.model[i + 1]._24 , m_constantBufferData.model[i + 1]._34 };
		float Dis1 = pow(m_camera._41 - Cube1.x, 2) + pow(m_camera._42 - Cube1.y, 2) + pow(m_camera._43 - Cube1.z, 2);
		float Dis2 = pow(m_camera._41 - Cube2.x, 2) + pow(m_camera._42 - Cube2.y, 2) + pow(m_camera._43 - Cube2.z, 2);

		if (Dis1 < Dis2)
		{
			XMFLOAT4X4 mTemp = m_constantBufferData.model[i];
			m_constantBufferData.model[i] = m_constantBufferData.model[i + 1];
			m_constantBufferData.model[i + 1] = mTemp;
			i = -1;
		}
	}
}

// Renders one frame using the vertex and pixel shaders.
void Shield::Render(void)
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	ChangeRender(m_camera, 3);

	auto context = m_deviceResources->GetD3DDeviceContext();

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_camera))));

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(m_constantBuffer.Get(), 0, NULL, &m_constantBufferData, 0, 0, 0);

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionUVNormal);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Each index is one 16-bit unsigned integer (short).
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers1(0, 1, m_constantBuffer.GetAddressOf(), nullptr, nullptr);

	// Attach our pixel shader.
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//For Texture
	ID3D11ShaderResourceView* textViews[] = { m_SRV.Get(), m_SRV2.Get() };
	context->PSSetShaderResources(0, 2, textViews);

	// Draw the objects.
	context->OMSetBlendState(m_blend.Get(), 0, 0xFFFFFFFF);
	context->DrawIndexedInstanced(m_indexCount, ModelCount, 0, 0, 0);

	//Creating my second view
	auto viewport2 = m_deviceResources->GetScreenViewport2();
	context->RSSetViewports(1, &viewport2);
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_camera2))));
	context->UpdateSubresource1(m_constantBuffer.Get(), 0, NULL, &m_constantBufferData, 0, 0, 0);
	context->OMSetBlendState(m_blend.Get(), 0, 0xFFFFFFFF);
	context->DrawIndexedInstanced(m_indexCount, ModelCount, 0, 0, 0);
}

void Shield::CreateDeviceDependentResources(void)
{
	// Load shaders asynchronously.
	if (TypeIs == 1)
	{
		Vertex = L"SampleVertexShader.cso";
		Pixel = L"SamplePixelShader.cso";
	}
	if (TypeIs == 2)
	{
		Vertex = L"SampleVertexShader.cso";
		Pixel = L"CubePixelShader.cso";
	}
	if (TypeIs == 3)
	{
		Vertex = L"SampleVertexShader.cso";
		Pixel = L"2Texture.cso";
	}
	if (TypeIs == 4)
	{
		Vertex = L"SampleVertexShader.cso";
		Pixel = L"CubePixelShader.cso";
	}

	auto loadVSTask = DX::ReadDataAsync(Vertex);
	auto loadPSTask = DX::ReadDataAsync(Pixel);

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateVertexShader(&fileData[0], fileData.size(), nullptr, &m_vertexShader));

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &fileData[0], fileData.size(), &m_inputLayout));
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreatePixelShader(&fileData[0], fileData.size(), nullptr, &m_pixelShader));
		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, &m_constantBuffer));
	});

	// Once both shaders are loaded, create the mesh.
	auto createCubeTask = (createPSTask && createVSTask).then([this]()
	{
		if (TypeIs == 1)
		{
			Mesh m;
			m.LoadThatMesh("Assets/mapleShape.mesh");

			D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
			vertexBufferData.pSysMem = m.VPUVN.data();
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC vertexBufferDesc(m.VPUVN.size() * sizeof(VertexPositionUVNormal), D3D11_BIND_VERTEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer));

			m_indexCount = m.VertexIndices.size();

			D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
			indexBufferData.pSysMem = m.VertexIndices.data();
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC indexBufferDesc(m.VertexIndices.size() * sizeof(unsigned short), D3D11_BIND_INDEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer));
		}


		if (TypeIs == 2 || TypeIs == 3 || TypeIs == 4)
		{
			Mesh n;
			n.LoadThatMesh("Assets/pCube1Shape.mesh");

			D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
			vertexBufferData.pSysMem = n.VPUVN.data();
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC vertexBufferDesc(n.VPUVN.size() * sizeof(VertexPositionUVNormal), D3D11_BIND_VERTEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer));

			m_indexCount = n.VertexIndices.size();

			D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
			indexBufferData.pSysMem = n.VertexIndices.data();
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC indexBufferDesc(n.VertexIndices.size() * sizeof(unsigned short), D3D11_BIND_INDEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer));

			if (TypeIs == 2)
			{
				D3D11_BLEND_DESC blendDesc = { 0 };
				blendDesc.AlphaToCoverageEnable = false;
				blendDesc.IndependentBlendEnable = false;
				blendDesc.RenderTarget[0].BlendEnable = TRUE;
				blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				m_deviceResources->GetD3DDevice()->CreateBlendState(&blendDesc, &m_blend);
			}
		}
	});

	Concurrency::task<void> task([this](void)
	{
		if (TypeIs == 1)
		{
			CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/mapletree.dds", (ID3D11Resource **)m_texture2D.Get(), m_SRV.GetAddressOf());
		}
	});

	Concurrency::task<void> task2([this](void)
	{
		if (TypeIs == 2 || TypeIs == 3 || TypeIs == 4)
		{
			CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/cube.dds", (ID3D11Resource **)m_texture2D.Get(), m_SRV.GetAddressOf());
		}
		if (TypeIs == 3)
		{
			CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/energy_seamless.dds", (ID3D11Resource **)m_texture2D.Get(), m_SRV.GetAddressOf());
			CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/platform_seamless.dds", (ID3D11Resource **)m_texture2D2.Get(), m_SRV2.GetAddressOf());
		}
	});

	auto createTask = (task && task2 && createCubeTask).then([this]()
	{
		m_loadingComplete = true;
	});
}

void Shield::ReleaseDeviceDependentResources(void)
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}