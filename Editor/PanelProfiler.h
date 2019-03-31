#pragma once

#include <Editor/Panel.h>

namespace Columbus
{

	class EditorPanelProfiler : public EditorPanel
	{
	private:
		static constexpr int GraphCount = 600;

		float RedrawTime = 0.0f;
		float T = 0.0f;
		float RedrawTimeCurve[GraphCount] = { 0.0f };
		int Index = 0;
		int ZeroIndex = 0;
	public:
		EditorPanelProfiler() : EditorPanel("Profiler") {}

		void SetRedrawTime(float Time);

		virtual void Draw() final override;
		virtual ~EditorPanelProfiler() final override;
	};

}


