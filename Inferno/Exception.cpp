#include"MyException.h"

namespace Inferno{
MyExceptionBase_RuntimeError::MyExceptionBase_RuntimeError(const std::wstring& what_arg)
	: runtime_error(""), m_message_w(what_arg){}

MyExceptionBase_RuntimeError::MyExceptionBase_RuntimeError(const wchar_t* what_arg)
	: runtime_error(""), m_message_w(what_arg){}

const char* MyExceptionBase_RuntimeError::what() const
{
	return m_message.c_str();
}

const wchar_t* MyExceptionBase_RuntimeError::what_w() const
{
	return m_message_w.c_str();
}

MyExceptionBase_RuntimeError::~MyExceptionBase_RuntimeError(){}
}