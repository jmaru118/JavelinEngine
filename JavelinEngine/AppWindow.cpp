#include "AppWindow.h"

struct vec4
{
	float x, y, z, w;
};

struct vertex
{
	vec4 position;
};

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] =
	{
		//X - Y - Z
		{-0.2f,-0.2f,0.0f, 1}, // POS1
		{-0.2f,0.2f,0.0f, 1}, // POS2
		{ 0.2f,-0.2f,0.0f, 1},// POS3
		{ 0.2f,0.2f,0.0f, 1}   // POS4
	};

	// create vertex buffer
	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);
	GraphicsEngine::get()->createShaders();
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	// CREATE VERTEX SHADER
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain",
											&shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	// Clear the render target
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(
			this->m_swap_chain, 0, 0.3f, 0.4f, 1);
	// set viewport size
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(
		rc.right - rc.left, rc.bottom - rc.top);
	// Set Default shader in graphics pipeline
	GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	// Set triangle vertices
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	// Draw the triangle
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(
		m_vb->getSizeVertexList(), 0);
	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	GraphicsEngine::get()->release();
}
