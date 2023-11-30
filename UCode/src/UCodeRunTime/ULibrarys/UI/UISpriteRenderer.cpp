#include "UISpriteRenderer.hpp"
#include "../Rendering/Camera2d.hpp"
UIStart

UComponentData UISpriteRenderer::type_Data = { "UISpriteRenderer",nullptr};
UISpriteRenderer::UISpriteRenderer(Entity* e) :Renderer2d(e,&type_Data),
shader(Shader::Default_Shader(e->NativeGameRunTime()->Get_Library_Edit())),
texture(nullptr),
flipX(false),
flipY(false),
color(Color::White()),
DrawLayer(UIHelper::UI_DrawLayer),
DrawOrder(0)
{
}
UISpriteRenderer::~UISpriteRenderer()
{
}

void UISpriteRenderer::Start()
{
}

void UISpriteRenderer::OnDraw()
{
	auto Cam = Camera2d::FindMainCam(GetGameRunTime());
	if (Cam == nullptr) { return; }

	auto Render = GetRenderRunTime();

	/*

	auto Scale = Get_Scale();
	if (flipX) { Scale.X = -Scale.X; }
	if (flipX) { Scale.Y = -Scale.Y; }

		auto campos = Cam->Get_Position();
	auto windowsize = Cam->Get_windowSize();


	Vec2 SP = { UIData.Pos.Value.X / windowsize.X,UIData.Pos.Value.Y / windowsize.Y };
	Vec2 Ss = { UIData.Size.Value.X * windowsize.X,UIData.Size.Value.Y * windowsize.Y };


	Vec2 pos = campos;
	Vec2 size = Scale;
	RenderRunTime::DrawQuad2dData Data = RenderRunTime::DrawQuad2dData(pos - Vec2(size.X / 2, size.Y / 2), size, Get_Rotation());
	Data.Spr = texture;
	Data.color = color;
	Data.draworder = DrawOrder;


#if UCodeGameEngineDEBUG
	Data.madeby = this;
#endif
	Render->DrawQuad2d(Data);
	*/
}
UIEnd
