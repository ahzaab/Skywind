#include "CLIK/Button.h"


namespace CLIK
{
	namespace GFx
	{
		namespace Controls
		{
			Button::Button() :
				super()
			{}


			Button::Button(const Button& a_rhs) :
				super(a_rhs._instance)
			{}


			Button::Button(Button&& a_rhs) :
				super(std::move(a_rhs._instance))
			{}


			Button::Button(const RE::GFxValue& a_val) :
				super(a_val)
			{}


			Button::Button(RE::GFxValue&& a_val) :
				super(std::move(a_val))
			{}


			Button::~Button()
			{}


			Button& Button::operator=(const Button& a_rhs)
			{
				_instance = a_rhs._instance;
				return *this;
			}


			Button& Button::operator=(Button&& a_rhs)
			{
				_instance = std::move(a_rhs._instance);
				return *this;
			}


			Button& Button::operator=(const RE::GFxValue& a_rhs)
			{
				_instance = a_rhs;
				return *this;
			}


			Button& Button::operator=(RE::GFxValue&& a_rhs)
			{
				_instance = std::move(a_rhs);
				return *this;
			}


			std::string Button::LabelID() const
			{
				return GetString("labelID");
			}


			void Button::LabelID(const char* a_labelID)
			{
				SetString("labelID", a_labelID);
			}


			std::string Button::Label() const
			{
				return GetString("label");
			}


			void Button::Label(const char* a_label)
			{
				SetString("label", a_label);
			}


			bool Button::Disabled() const
			{
				return GetBoolean("disabled");
			}


			void Button::Disabled(bool a_disabled)
			{
				SetBoolean("disabled", a_disabled);
			}


			bool Button::Selected() const
			{
				return GetBoolean("selected");
			}


			void Button::Selected(bool a_selected)
			{
				SetBoolean("selected", a_selected);
			}


			std::string Button::GroupName() const
			{
				return GetString("groupName");
			}


			void Button::GroupName(const char* a_groupName)
			{
				SetString("groupName", a_groupName);
			}


			Object Button::Group() const
			{
				return GetObject("group");
			}


			void Button::Group(const Object& a_group)
			{
				SetObject("group", a_group);
			}


			bool Button::DisableFocus() const
			{
				return GetBoolean("disableFocus");
			}


			void Button::DisableFocus(bool a_disableFocus)
			{
				SetBoolean("disableFocus", a_disableFocus);
			}


			bool Button::DisableConstraints() const
			{
				return GetBoolean("disableConstraints");
			}


			void Button::DisableConstraints(bool a_disableConstraints)
			{
				SetBoolean("disableConstraints", a_disableConstraints);
			}


			std::string Button::AutoSize() const
			{
				return GetString("autoSize");
			}


			void Button::AutoSize(const char* a_autoSize)
			{
				SetString("autoSize", a_autoSize);
			}


			void Button::SetSize(double a_width, double a_height)
			{
				enum
				{
					kWidth,
					kHeight,
					kNumArgs
				};

				RE::GFxValue args[kNumArgs];

				args[kWidth] = a_width;
				assert(args[kWidth].IsNumber());

				args[kHeight] = a_height;
				assert(args[kHeight].IsNumber());

				auto success = _instance.Invoke("setSize", 0, args, kNumArgs);
				assert(success);
			}


			std::string Button::ToString()
			{
				RE::GFxValue str;
				auto success = _instance.Invoke("toString", &str);
				assert(success);
				return str.GetString();
			}
		}
	}
}
