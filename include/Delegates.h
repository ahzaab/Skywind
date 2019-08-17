#pragma once

#include <functional>
#include <string>

#include "RE/Skyrim.h"
#include "SKSE/API.h"


class UIDelegateBase : public UIDelegate_v1
{
public:
	UIDelegateBase() = default;
	virtual ~UIDelegateBase() = default;

	virtual void Dispose() override;
};


class SetVisibleDelegate : public UIDelegateBase
{
public:
	SetVisibleDelegate(const RE::GPtr<RE::GFxMovieView>& a_view, bool a_visible);

	virtual void Run() override;

private:
	RE::GPtr<RE::GFxMovieView> _view;
	bool _visible;
};


class AdjustScaleDelegate : public UIDelegateBase
{
public:
	AdjustScaleDelegate(const RE::GPtr<RE::GFxMovieView>& a_view, double a_scale = 0.9);

	virtual void Run() override;

private:
	RE::GPtr<RE::GFxMovieView> _view;
	double _scale;
};


class SetInputsDelegate : public UIDelegateBase
{
public:
	SetInputsDelegate(const RE::GPtr<RE::GFxMovieView>& a_view, const std::string& a_acceptText, UInt32 a_acceptIcon, const std::string& a_cancelText, UInt32 a_cancelIcon);

	virtual void Run() override;

private:
	enum
	{
		kAcceptText,
		kAcceptIcon,
		kCancelText,
		kCancelIcon,
		kNumArgs
	};


	RE::GPtr<RE::GFxMovieView> _view;
	std::string _acceptText;
	double _acceptIcon;
	std::string _cancelText;
	double _cancelIcon;
};


class GetCurrentSignDelegate : public UIDelegateBase
{
public:
	using Callback = std::function<void(RE::GFxValue& a_result)>;


	GetCurrentSignDelegate(const RE::GPtr<RE::GFxMovieView>& a_view, Callback a_callback);

	virtual void Run() override;

private:
	RE::GPtr<RE::GFxMovieView> _view;
	Callback _callback;
};
