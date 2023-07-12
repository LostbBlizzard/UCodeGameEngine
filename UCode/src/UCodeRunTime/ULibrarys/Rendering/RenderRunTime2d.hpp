#pragma once
#include <UCodeRunTime/includeULibrayCompoent.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/Vector.hpp>
#include "RenderingNamespace.hpp"

#include "UCodeRunTime/RunTimeBasicTypes/Bounds.hpp"

#include <UCodeRunTime/ULibrarys/Rendering/Camera2d.hpp>
#include "GPUTypes.hpp"
RenderingStart

class RenderRunTime2d;

class Renderer2d :public Compoent
{
protected:
	Renderer2d(Entity* entity, UComponentData* typeData);
public:
	virtual ~Renderer2d();
	RenderRunTime2d* GetRenderRunTime();
	virtual void OnDraw() = 0;
};


class RenderRunTime2d final:private Compoent
{
	friend Renderer2d;
private:
	RenderRunTime2d(Entity* entity);
	~RenderRunTime2d();
public:
	
	typedef unsigned char DrawLayer_t;
	static constexpr DrawLayer_t DrawLayer_Min = 0;
	static constexpr DrawLayer_t DrawLayer_Max = UCHAR_MAX;

	typedef unsigned char DrawOrder_t;
	static constexpr DrawOrder_t DrawOrderType_Min =0;
	static constexpr  DrawOrder_t DrawOrderType_Max = UCHAR_MAX;

	struct Stats
	{
		SInt32 DrawCount = 0;
		SInt32 QuadCount = 0;
	};
	struct Vertex
	{
		Vec3 _Position;
		Color _Color;
		Vec2 _TexCoords;
		float32 _TexIndex = 0;
		inline void SetData(Vec3 p, Color c, Vec2 TC, UInt32 TI)
		{
			_Position = p;
			_Color = c;
			_TexCoords = TC;
			_TexIndex = (float32)TI;
		}

	};
	struct DrawQuad2dData
	{
	public:
		Vec2 pos;
		Vec2 size;
		Vec2 rotation;
		Sprite* Spr;
		Color color;
		DrawLayer_t drawLayer;
		DrawOrder_t draworder;
#ifdef DEBUG
		Renderer2d* madeby = nullptr;
#endif // DEBUG
		DrawQuad2dData(Vec2 p, Vec2 s, Vec2 rot)
			:pos(p), size(s), rotation(rot), Spr(nullptr), color(Color())
			, drawLayer(DrawLayer_Min)
			, draworder(DrawOrderType_Min)
		{

		};
		inline void SetTexture(Sprite* v) { Spr = v; }
		inline void SetColor(Color v) { color = v; }
		inline void SetDrawLayer(DrawLayer_t v) { drawLayer = v; }
		inline void SetDraworder(DrawOrder_t v) { draworder = v; }

	};
	struct DrawData
	{
		Vector<DrawQuad2dData> Quad2d;
	}; 

	

	static RenderRunTime2d* FindRenderRunTime(const GameRunTime* e);
	static RenderRunTime2d* GetRenderRunTime(GameRunTime* e);
	
	
	//
	void UpdateDrawData();

	void ReSetStats();

	inline DrawData& Get_DrawData() { return _DrawData; }

	inline void DrawQuad2d(const DrawQuad2dData& Data)
	{
		_DrawData.Quad2d.push_back(Data);
	}
	inline const Stats& Get_Stats() const { return DataStats; }

	inline Gamelibrary* GetGameLib() { return GetGameRunTime()->Get_Library_Edit(); }
private:	
	Vector<Renderer2d*> All_Renderers;
	DrawData _DrawData;
	

	Stats DataStats;
	static UComponentData type_Data;
};
struct draworder_Sort
{

	inline bool operator()(const RenderRunTime2d::DrawQuad2dData& a, const RenderRunTime2d::DrawQuad2dData& b)
	{

		if (a.drawLayer == b.drawLayer)
		{
			return a.draworder < b.draworder;
		}
		else
		{
			return a.drawLayer < b.drawLayer;
		}
	}
};
RenderingEnd
