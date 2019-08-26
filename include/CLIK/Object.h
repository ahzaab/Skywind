#pragma once

#include <cassert>
#include <optional>
#include <string_view>

#include "RE/Skyrim.h"


namespace CLIK
{
	class Object
	{
	public:
		Object();
		Object(const Object& a_rhs);
		Object(Object&& a_rhs);
		explicit Object(double a_val);
		explicit Object(bool a_val);
		explicit Object(std::string_view a_val);
		explicit Object(std::wstring_view a_val);
		explicit Object(const RE::GFxValue& a_val);
		explicit Object(RE::GFxValue&& a_val);
		~Object();

		Object& operator=(const Object& a_rhs);
		Object& operator=(Object&& a_rhs);
		Object& operator=(double a_val);
		Object& operator=(bool a_val);
		Object& operator=(std::string_view a_val);
		Object& operator=(std::wstring_view a_val);
		Object& operator=(const RE::GFxValue& a_rhs);
		Object& operator=(RE::GFxValue&& a_rhs);

		RE::GFxValue& GetInstance();

		// properties
		Object Constructor() const;

		Object Prototype() const;

		Object Resolve() const;
		void Resolve(Object& a_resolve);

		// methods
		//bool AddProperty(std::string_view a_name, Function& a_getter, Function& a_setter);
		bool HasOwnProperty(std::string_view a_name);
		bool IsPropertyEnumerable(std::string_view a_name);

		bool IsPrototypeOf(Object& a_theClass);

		//bool Watch(std::string_view a_name, Function& a_callback, Object& a_userData);
		bool Unwatch(std::string_view a_name);

		std::string ToString();
		Object ValueOf();

	protected:
		bool GetBoolean(const char* a_path) const;
		double GetNumber(const char* a_path) const;
		Object GetObject(const char* a_path) const;
		std::string_view GetString(const char* a_path) const;

		void SetBoolean(const char* a_path, bool a_boolean);
		void SetNumber(const char* a_path, double a_number);
		void SetObject(const char* a_path, const Object& a_object);
		void SetString(const char* a_path, std::string_view a_string);


		RE::GFxValue _instance;
	};
}
