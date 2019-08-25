#include "CLIK/TextField.h"


namespace CLIK
{
	TextField::TextField() :
		super()
	{}


	TextField::TextField(const TextField& a_rhs) :
		super(a_rhs._instance)
	{}


	TextField::TextField(TextField&& a_rhs) :
		super(std::move(a_rhs._instance))
	{}


	TextField::TextField(const RE::GFxValue& a_val) :
		super(a_val)
	{}


	TextField::TextField(RE::GFxValue&& a_val) :
		super(std::move(a_val))
	{}


	TextField::~TextField()
	{}


	TextField& TextField::operator=(const TextField& a_rhs)
	{
		_instance = a_rhs._instance;
		return *this;
	}


	TextField& TextField::operator=(TextField&& a_rhs)
	{
		_instance = std::move(a_rhs._instance);
		return *this;
	}


	TextField& TextField::operator=(const RE::GFxValue& a_rhs)
	{
		_instance = a_rhs;
		return *this;
	}


	TextField& TextField::operator=(RE::GFxValue&& a_rhs)
	{
		_instance = std::move(a_rhs);
		return *this;
	}


	double TextField::Alpha() const
	{
		return GetNumber("_alpha");
	}


	void TextField::Alpha(double a_alpha)
	{
		SetNumber("_alpha", a_alpha);
	}


	std::string TextField::AntiAliasType() const
	{
		return GetString("antiAliasType");
	}


	void TextField::AntiAliasType(const char* a_antiAliasType)
	{
		SetString("antiAliasType", a_antiAliasType);
	}


	Object TextField::AutoSize() const
	{
		return GetObject("autoSize");
	}


	void TextField::AutoSize(const Object& a_autoSize)
	{
		SetObject("autoSize", a_autoSize);
	}


	bool TextField::Background() const
	{
		return GetBoolean("background");
	}


	void TextField::Background(bool a_background)
	{
		SetBoolean("background", a_background);
	}


	double TextField::BackgroundColor() const
	{
		return GetNumber("backgroundColor");
	}


	void TextField::BackgroundColor(double a_backgroundColor)
	{
		SetNumber("backgroundColor", a_backgroundColor);
	}


	bool TextField::Border() const
	{
		return GetBoolean("border");
	}


	void TextField::Border(bool a_border)
	{
		SetBoolean("border", a_border);
	}


	double TextField::BorderColor() const
	{
		return GetNumber("borderColor");
	}


	void TextField::BorderColor(double a_borderColor)
	{
		SetNumber("borderColor", a_borderColor);
	}


	double TextField::BottomScroll() const
	{
		return GetNumber("bottomScroll");
	}


	bool TextField::CondenseWhite() const
	{
		return GetBoolean("condenseWhite");
	}


	void TextField::CondenseWhite(bool a_condenseWhite)
	{
		SetBoolean("condenseWhite", a_condenseWhite);
	}


	bool TextField::EmbedFonts() const
	{
		return GetBoolean("embedFonts");
	}


	void TextField::EmbedFonts(bool a_embedFonts)
	{
		SetBoolean("embedFonts", a_embedFonts);
	}


	std::string TextField::GridFitType() const
	{
		return GetString("gridFitType");
	}


	void TextField::GridFitType(const char* a_gridFitType)
	{
		SetString("gridFitType", a_gridFitType);
	}


	double TextField::Height() const
	{
		return GetNumber("_height");
	}


	void TextField::Height(double a_height)
	{
		SetNumber("_height", a_height);
	}


	double TextField::HighQuality() const
	{
		return GetNumber("_highquality");
	}


	void TextField::HighQuality(double a_highQuality)
	{
		SetNumber("_highquality", a_highQuality);
	}


	double TextField::HScroll() const
	{
		return GetNumber("hscroll");
	}


	void TextField::HScroll(double a_hscroll)
	{
		SetNumber("hscroll", a_hscroll);
	}


	bool TextField::HTML() const
	{
		return GetBoolean("html");
	}


	void TextField::HTML(bool a_html)
	{
		SetBoolean("html", a_html);
	}


	std::string TextField::HTMLText() const
	{
		return GetString("htmlText");
	}


	void TextField::HTMLText(const char* a_htmlText)
	{
		SetString("htmlText", a_htmlText);
	}


	double TextField::Length() const
	{
		return GetNumber("length");
	}


	double TextField::MaxChars() const
	{
		return GetNumber("maxChars");
	}


	void TextField::MaxChars(double a_maxChars)
	{
		SetNumber("maxChars", a_maxChars);
	}


	double TextField::MaxHScroll() const
	{
		return GetNumber("maxhscroll");
	}


	double TextField::MaxScroll() const
	{
		return GetNumber("maxscroll");
	}


	bool TextField::MouseWheelEnabled() const
	{
		return GetBoolean("mouseWheelEnabled");
	}


	void TextField::MouseWheelEnabled(bool a_mouseWheelEnabled)
	{
		SetBoolean("mouseWheelEnabled", a_mouseWheelEnabled);
	}


	bool TextField::Multiline() const
	{
		return GetBoolean("multiline");
	}


	void TextField::Multiline(bool a_multiline)
	{
		SetBoolean("multiline", a_multiline);
	}


	std::string TextField::Name() const
	{
		return GetString("_name");
	}


	void TextField::Name(const char* a_name)
	{
		SetString("_name", a_name);
	}


	bool TextField::Password() const
	{
		return GetBoolean("password");
	}


	void TextField::Password(bool a_password)
	{
		SetBoolean("password", a_password);
	}


	std::string TextField::Quality() const
	{
		return GetString("_quality");
	}


	void TextField::Quality(const char* a_quality)
	{
		SetString("_quality", a_quality);
	}


	std::string TextField::Restrict() const
	{
		return GetString("restrict");
	}


	void TextField::Restrict(const char* a_restrict)
	{
		SetString("restrict", a_restrict);
	}


	double TextField::Rotation() const
	{
		return GetNumber("_rotation");
	}


	void TextField::Rotation(double a_rotation)
	{
		SetNumber("_rotation", a_rotation);
	}


	double TextField::Scroll() const
	{
		return GetNumber("scroll");
	}


	void TextField::Scroll(double a_scroll)
	{
		SetNumber("scroll", a_scroll);
	}


	bool TextField::Selectable() const
	{
		return GetBoolean("selectable");
	}


	void TextField::Selectable(bool a_selectable)
	{
		SetBoolean("selectable", a_selectable);
	}


	double TextField::Sharpness() const
	{
		return GetNumber("sharpness");
	}


	void TextField::Sharpness(double a_sharpness)
	{
		SetNumber("sharpness", a_sharpness);
	}


	double TextField::SoundBufTime() const
	{
		return GetNumber("_soundbuftime");
	}


	void TextField::SoundBufTime(double a_soundBufTime)
	{
		SetNumber("_soundbuftime", a_soundBufTime);
	}


	bool TextField::TabEnabled() const
	{
		return GetBoolean("tabEnabled");
	}


	void TextField::TabEnabled(bool a_tabEnabled)
	{
		SetBoolean("tabEnabled", a_tabEnabled);
	}


	double TextField::TabIndex() const
	{
		return GetNumber("tabIndex");
	}


	void TextField::TabIndex(double a_tabIndex)
	{
		SetNumber("tabIndex", a_tabIndex);
	}


	std::string TextField::Target() const
	{
		return GetString("_target");
	}


	std::string TextField::Text() const
	{
		return GetString("text");
	}


	void TextField::Text(const char* a_text)
	{
		SetString("text", a_text);
	}


	double TextField::TextColor() const
	{
		return GetNumber("textColor");
	}


	void TextField::TextColor(double a_textColor)
	{
		SetNumber("textColor", a_textColor);
	}


	double TextField::TextHeight() const
	{
		return GetNumber("textHeight");
	}


	void TextField::TextHeight(double a_textHeight)
	{
		SetNumber("textHeight", a_textHeight);
	}


	double TextField::TextWidth() const
	{
		return GetNumber("textWidth");
	}


	void TextField::TextWidth(double a_textWidth)
	{
		SetNumber("textWidth", a_textWidth);
	}


	double TextField::Thickness() const
	{
		return GetNumber("thickness");
	}


	void TextField::Thickness(double a_thickness)
	{
		SetNumber("thickness", a_thickness);
	}


	std::string TextField::Type() const
	{
		return GetString("type");
	}


	void TextField::Type(const char* a_type)
	{
		SetString("type", a_type);
	}


	std::string TextField::URL() const
	{
		return GetString("_url");
	}


	std::string TextField::Variable() const
	{
		return GetString("variable");
	}


	void TextField::Variable(const char* a_variable)
	{
		SetString("variable", a_variable);
	}


	bool TextField::Visible() const
	{
		return GetBoolean("_visible");
	}


	void TextField::Visible(bool a_visible)
	{
		SetBoolean("_visible", a_visible);
	}


	double TextField::Width() const
	{
		return GetNumber("_width");
	}


	void TextField::Width(double a_width)
	{
		SetNumber("_width", a_width);
	}


	bool TextField::WordWrap() const
	{
		return GetBoolean("wordWrap");
	}


	void TextField::WordWrap(bool a_wordWrap)
	{
		SetBoolean("wordWrap", a_wordWrap);
	}


	double TextField::X() const
	{
		return GetNumber("_x");
	}


	void TextField::X(double a_x)
	{
		SetNumber("_x", a_x);
	}


	double TextField::XMouse() const
	{
		return GetNumber("_xmouse");
	}


	double TextField::XScale() const
	{
		return GetNumber("_xscale");
	}


	void TextField::XScale(double a_xScale)
	{
		SetNumber("_xscale", a_xScale);
	}


	double TextField::Y() const
	{
		return GetNumber("_y");
	}


	void TextField::Y(double a_y)
	{
		SetNumber("_y", a_y);
	}


	double TextField::YMouse() const
	{
		return GetNumber("_ymouse");
	}


	double TextField::YScale() const
	{
		return GetNumber("_yscale");
	}


	void TextField::YScale(double a_yScale)
	{
		SetNumber("_yscale", a_yScale);
	}
}
