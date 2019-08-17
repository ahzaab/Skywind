#include "Delegates.h"


void UIDelegateBase::Dispose()
{
	delete this;
}


SetVisibleDelegate::SetVisibleDelegate(const RE::GPtr<RE::GFxMovieView>& a_view, bool a_visible) :
	_view(a_view),
	_visible(a_visible)
{
	assert(_view);
}


void SetVisibleDelegate::Run()
{
	_view->SetVisible(_visible);
}


AdjustScaleDelegate::AdjustScaleDelegate(const RE::GPtr<RE::GFxMovieView>& a_view, double a_scale) :
	_view(a_view),
	_scale(a_scale)
{
	assert(_view);
}


void AdjustScaleDelegate::Run()
{
	auto def = _view->GetMovieDef();
	auto width = def->GetWidth();
	auto height = def->GetHeight();

	RE::GFxValue num;
	bool success = false;

	num.SetNumber(width * _scale);
	success = _view->SetVariable("_root._width", num);
	assert(success);

	num.SetNumber(height * _scale);
	success = _view->SetVariable("_root._height", num);
	assert(success);

	num.SetNumber(width * (1.0 - _scale) / 2);
	success = _view->SetVariable("_root._x", num);
	assert(success);

	num.SetNumber(height * (1.0 - _scale) / 2);
	success = _view->SetVariable("_root._y", num);
	assert(success);
}


SetInputsDelegate::SetInputsDelegate(const RE::GPtr<RE::GFxMovieView>& a_view, const std::string& a_acceptText, UInt32 a_acceptIcon, const std::string& a_cancelText, UInt32 a_cancelIcon) :
	_view(a_view),
	_acceptText(a_acceptText),
	_acceptIcon(static_cast<double>(a_acceptIcon)),
	_cancelText(a_cancelText),
	_cancelIcon(static_cast<double>(a_cancelIcon))
{
	assert(_view);
}


void SetInputsDelegate::Run()
{
	RE::GFxValue args[kNumArgs];
	args[kAcceptText].SetString(_acceptText.c_str());
	args[kAcceptIcon].SetNumber(_acceptIcon);
	args[kCancelText].SetString(_cancelText.c_str());
	args[kCancelIcon].SetNumber(_cancelIcon);
	bool success = _view->Invoke("birthSignList.setInputs", 0, args, kNumArgs);
	assert(success);
}


GetCurrentSignDelegate::GetCurrentSignDelegate(const RE::GPtr<RE::GFxMovieView>& a_view, Callback a_callback) :
	_view(a_view),
	_callback(a_callback)
{
	assert(_view);
}


void GetCurrentSignDelegate::Run()
{
	RE::GFxValue result;
	bool success = _view->Invoke("birthSignList.getCurrentSign", &result, 0, 0);
	assert(success);
	_callback(result);
}
