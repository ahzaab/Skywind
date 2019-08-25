#pragma once

#include "CLIK/UIComponent.h"


namespace CLIK
{
	class Array;


	namespace GFx
	{
		namespace Controls
		{
			class CoreList : public Core::UIComponent
			{
			public:
				using super = Core::UIComponent;


				CoreList();
				CoreList(const CoreList& a_rhs);
				CoreList(CoreList&& a_rhs);
				explicit CoreList(const RE::GFxValue& a_val);
				explicit CoreList(RE::GFxValue&& a_val);
				~CoreList();

				CoreList& operator=(const CoreList& a_rhs);
				CoreList& operator=(CoreList&& a_rhs);
				CoreList& operator=(const RE::GFxValue& a_rhs);
				CoreList& operator=(RE::GFxValue&& a_rhs);

				std::string ItemRenderer() const;
				void ItemRenderer(const char* a_itemRenderer);

				Object DataProvider() const;
				void DataProvider(const Object& a_dataProvider);

				double SelectedIndex() const;
				void SelectedIndex(double a_selectedIndex);

				void ScrollToIndex(double a_index);

				std::string LabelField() const;
				void LabelField(const char* a_labelField);

				//Function& LabelFunction() const;
				//void LabelFunction(Function& a_labelFunction);

				std::string ItemToLabel(Object& a_item);

				void InvalidateData();

				double AvailableWidth() const;

				double AvailableHeight() const;

				void SetRendererList(Array& a_value);

				std::string RendererInstanceName() const;
				void RendererInstanceName(const char* a_rendererInstanceName);

				std::string ToString();
			};
		}
	}
}
