#include <EditorWindows/EditorWindow.hpp>

EditorStart
class ProfilerWindow :public EditorWindow
{
public:
	ProfilerWindow(const NewEditorWindowData& project);
	~ProfilerWindow() override;

	void UpdateWindow() override;
	void OnSaveWindow(USerializer& JsonToSaveIn) override;
	void OnLoadWindow(UDeserializer& JsonToOutof) override;
	static EditorWindowData GetEditorData();
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& project);

};

EditorEnd

