#pragma once
#include "OpenGlHelper.hpp"
#include  <UCodeRunTime\CoreRunTime.hpp>
#include <Imgui/imgui.h>
#include "../WindowData.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp"
RenderAPIStart


class OpenGlRender
{

public:
	OpenGlRender(GameRunTime* run);
	~OpenGlRender();
	static void RenderSetUp();
	void Init(WindowData window);


	

	static OpenGlRender* GetOpenGlRender(const GLFWwindow* window);

	

	void UpdateCamWindowSize();
	void UpdateScreen();
	void ProcessInput();

	void EndRender();
	bool IsRuning;
	
	GameRunTime* RunTime;

	inline bool HasWindow()
	{
		return window;
	}
	inline GLFWwindow* Get_Window()
	{
		return window;
	}
	void SetWindowIcon(const Texture& tex);
	void _DrawOpenGl(RenderRunTime2d::DrawData& Data,Camera2d* cam);
private:
	static void SetAppStyle_Editor(ImGuiStyle* dst = nullptr);
	static void glfwerror_callback(int error, const char* description);
	WindowData _windowdata;
	GLFWwindow* window;
	SInt32  oldwindowWidth, oldwindowHeight;
	inline static Vector<OpenGlRender*> Classes;

	//
	void Init();
	void Shutdown();
	void BeginBatch();
	void EndBatch();
	void Flush();
	void _DrawQuad2d(RenderRunTime2d::DrawQuad2dData& Data);
	void _DrawQuads2d(RenderRunTime2d::DrawData& Data);

	inline size_t GetMaxTextureSlots() const { return TextureSlots.size(); }
	Shader* UseingShader;

	Vector<Texture*> TextureSlots;
	UInt32 IndexCount;
	UInt32 NextTextureSlot;

	Unique_ptr<Texture> WhiteTexture;
	UInt32 WhiteTextureSlot;

	Unique_ptr<VertexArray> QuadVA;
	Unique_ptr<VertexBuffer> QuadVB;
	Unique_ptr<IndexBuffer> QuadIB;

	Unique_array<RenderRunTime2d::Vertex> QuadBuffer;
	RenderRunTime2d::Vertex* QuadBufferPtr;
};
RenderAPIEnd

