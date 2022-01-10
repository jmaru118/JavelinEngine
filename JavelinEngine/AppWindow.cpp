#include "AppWindow.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
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
		{-0.5f,-0.5f,0.0f},
		{0.0f,0.5f,0.0f},
		{0.5f,-0.5f,0.0f}
	};

	// create vertex buffer
	void* shader_byte_code = nullptr;
	UINT size_list = ARRAYSIZE(list);
	UINT size_shader = 0;
	m_vb = GraphicsEngine::get()->createVertexBuffer();
	GraphicsEngine::get()->createShaders();
	GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
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
	// Set triangle vertices
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	// Draw the triangle
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(
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
