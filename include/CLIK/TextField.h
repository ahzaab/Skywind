#pragma once

#include "CLIK/Object.h"


namespace CLIK
{
	class TextField : public Object
	{
	public:
		using super = Object;


		TextField();
		TextField(const TextField& a_rhs);
		TextField(TextField&& a_rhs);
		explicit TextField(const RE::GFxValue& a_val);
		explicit TextField(RE::GFxValue&& a_val);
		~TextField();

		TextField& operator=(const TextField& a_rhs);
		TextField& operator=(TextField&& a_rhs);
		TextField& operator=(const RE::GFxValue& a_rhs);
		TextField& operator=(RE::GFxValue&& a_rhs);

		// properties
		double Alpha() const;
		void Alpha(double a_alpha);

		std::string AntiAliasType() const;
		void AntiAliasType(const char* a_antiAliasType);

		Object AutoSize() const;
		void AutoSize(const Object& a_autoSize);

		bool Background() const;
		void Background(bool a_background);

		double BackgroundColor() const;
		void BackgroundColor(double a_backgroundColor);

		bool Border() const;
		void Border(bool a_border);

		double BorderColor() const;
		void BorderColor(double a_borderColor);

		double BottomScroll() const;

		bool CondenseWhite() const;
		void CondenseWhite(bool a_condenseWhite);

		bool EmbedFonts() const;
		void EmbedFonts(bool a_embedFonts);

		//Array Filters() const;
		//void Filters(Array& a_filters);

		std::string GridFitType() const;
		void GridFitType(const char* a_gridFitType);

		double Height() const;
		void Height(double a_height);

		double HighQuality() const;
		void HighQuality(double a_highQuality);

		double HScroll() const;
		void HScroll(double a_hscroll);

		bool HTML() const;
		void HTML(bool a_html);

		std::string HTMLText() const;
		void HTMLText(const char* a_htmlText);

		double Length() const;

		double MaxChars() const;
		void MaxChars(double a_maxChars);

		double MaxHScroll() const;

		double MaxScroll() const;

		//ContextMenu Menu() const;

		bool MouseWheelEnabled() const;
		void MouseWheelEnabled(bool a_mouseWheelEnabled);

		bool Multiline() const;
		void Multiline(bool a_multiline);

		std::string Name() const;
		void Name(const char* a_name);

		//MovieClip Parent() const;
		//void Parent(const MovieClip& a_parent);

		bool Password() const;
		void Password(bool a_password);

		std::string Quality() const;
		void Quality(const char* a_quality);

		std::string Restrict() const;
		void Restrict(const char* a_restrict);

		double Rotation() const;
		void Rotation(double a_rotation);

		double Scroll() const;
		void Scroll(double a_scroll);

		bool Selectable() const;
		void Selectable(bool a_selectable);

		double Sharpness() const;
		void Sharpness(double a_sharpness);

		double SoundBufTime() const;
		void SoundBufTime(double a_soundBufTime);

		//StyleSheet StyleSheet() const;
		//void StyleSheet(const StyleSheet& a_styleSheet);

		bool TabEnabled() const;
		void TabEnabled(bool a_tabEnabled);

		double TabIndex() const;
		void TabIndex(double a_tabIndex);

		std::string Target() const;

		std::string Text() const;
		void Text(const char* a_text);

		double TextColor() const;
		void TextColor(double a_textColor);

		double TextHeight() const;
		void TextHeight(double a_textHeight);

		double TextWidth() const;
		void TextWidth(double a_textWidth);

		double Thickness() const;
		void Thickness(double a_thickness);

		std::string Type() const;
		void Type(const char* a_type);

		std::string URL() const;

		std::string Variable() const;
		void Variable(const char* a_variable);

		bool Visible() const;
		void Visible(bool a_visible);

		double Width() const;
		void Width(double a_width);

		bool WordWrap() const;
		void WordWrap(bool a_wordWrap);

		double X() const;
		void X(double a_x);

		double XMouse() const;

		double XScale() const;
		void XScale(double a_xScale);

		double Y() const;
		void Y(double a_y);

		double YMouse() const;

		double YScale() const;
		void YScale(double a_yScale);
	};
}
