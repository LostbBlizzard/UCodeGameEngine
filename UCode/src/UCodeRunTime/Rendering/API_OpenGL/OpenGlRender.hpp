#pragma once
#include "OpenGlHelper.hpp"
#include  <UCodeRunTime/CoreRunTime.hpp>
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
	void SetWaitForEvents(bool Value)
	{
		_windowdata.UpdateAppIfInputUpdate = Value;
	}

	void _DrawOpenGl(RenderRunTime2d::DrawData& Data,Camera2d* cam);
	
	static void SetStyle_WoodLandDay(ImGuiStyle* dst = nullptr);
	static void SetStyle_WoodLandNight(ImGuiStyle* dst = nullptr);
	static void SetStyle_Gray(ImGuiStyle* dst = nullptr);
	static void SetStyle_Dark(ImGuiStyle* dst = nullptr);
private:
	
	static void glfwerror_callback(int error, const char* description);
	static void glfwfiledrop(GLFWwindow* window, int count, const char** paths);
	WindowData _windowdata;
	GLFWwindow* window;
	i32  oldwindowWidth, oldwindowHeight;
	inline static Vector<OpenGlRender*> Classes;

	//
	struct DrawInstructions
	{

		enum class DrawType
		{
			Ouad2d,
			Line,
		};
		struct IndexSpan
		{
			size_t StartIndex = 0;
			size_t Count = 0;
		};
		struct Instruction
		{
			IndexSpan span;
			DrawType type = DrawType::Ouad2d;
		};
		Vector<Instruction> Instructions;
	};
	DrawInstructions MakeDrawInstruction(RenderRunTime2d::DrawData& Data);
	void RunInstructions(DrawInstructions& Instructions,RenderRunTime2d::DrawData& Data);
	void Init();
	void Shutdown();
	void BeginBatchQuad();
	void EndBatchQuad();
	void FlushQuad();
	void _DrawQuad2d(RenderRunTime2d::DrawQuad2dData& Data);
	void _DrawLine2d(RenderRunTime2d::Draw2DLineData& Data);
	
	void BeginBatchLine();
	void EndBatchLine();
	void FlushLine();

	inline size_t GetMaxTextureSlots() const { return TextureSlots.size(); }
	Shader* UseingShader;

	Vector<Texture*> TextureSlots;
	u32 IndexCount;
	u32 NextTextureSlot;

	Unique_ptr<Texture> WhiteTexture;
	u32 WhiteTextureSlot;

	Unique_ptr<VertexArray> QuadVA;
	Unique_ptr<VertexBuffer> QuadVB;
	Unique_ptr<IndexBuffer> QuadIB;

	Unique_array<RenderRunTime2d::Vertex> QuadBuffer;
	RenderRunTime2d::Vertex* QuadBufferPtr;
	
	size_t LineCount = 0;
    float linewidth = 0;
	struct TextureCod
	{
		Texture* Tex;
		Array<Vec2, 4> Cords;
	};
	TextureCod GetTextureCords(Sprite* spr);
	i32 GetTextureIndex(Texture* tex );

	void BindTextures();
};
RenderAPIEnd

