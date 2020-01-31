// Fill out your copyright notice in the Description page of Project Settings.

#include "XRProject.h"
#include "Modules/ModuleManager.h"
#include <locale>

DEFINE_LOG_CATEGORY(XRProject);
IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, XRProject, "XRProject" );

std::string wcs_to_mbs(std::wstring const& str, std::locale const& loc)
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	std::codecvt<wchar_t, char, std::mbstate_t> const& codecvt = std::use_facet<codecvt_t>(loc);
	std::mbstate_t state = std::mbstate_t();
	std::vector<char> buff((str.size() + 1) * codecvt.max_length());
	wchar_t const* in_next = str.c_str();
	char* out_next = &buff[0];
	codecvt_t::result r = codecvt.out(state, str.c_str(), str.c_str() + str.size(), in_next, &buff[0], &buff[0] + buff.size(), out_next);
	return std::string(&buff[0]);
}

std::wstring mbs_to_wcs(std::string const& str, std::locale const& loc)
{
	typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
	std::mbstate_t state = std::mbstate_t();
	std::vector<wchar_t> buf(str.size() + 1);
	char const* in_next = str.c_str();
	wchar_t* out_next = &buf[0];
	std::codecvt_base::result r = codecvt.in(state,
		str.c_str(), str.c_str() + str.size(), in_next,
		&buf[0], &buf[0] + buf.size(), out_next);
	return std::wstring(&buf[0]);
}