package neuik

/*
#cgo pkg-config: sdl2
#cgo LDFLAGS: -lSDL2_ttf -lSDL2_image
#include "NEUIK.h"

int configButtonSingle(
	NEUIK_Button * btn,
	const char   * set0)
{
	return NEUIK_Button_Configure(btn, set0, NULL);
}
*/
import "C"

import (
	"errors"
	"unsafe"
)

type Button C.NEUIK_Button

func (btn *Button) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(btn))
	return elem
}

func NewButton() (btn *Button, e error) {
	var cBtn *C.NEUIK_Button

	if C.NEUIK_NewButton(&cBtn) != 0 {
		e = errors.New("`NEUIK_NewButton()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	btn = (*Button)(cBtn)

	return btn, e
}

func MakeButton(text string) (btn *Button, e error) {
	var (
		cBtn  *C.NEUIK_Button
		cText *C.char
	)
	cText = C.CString(text)

	if C.NEUIK_MakeButton(&cBtn, cText) != 0 {
		e = errors.New("`NEUIK_MakeButton()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	btn = (*Button)(cBtn)
	if cText != nil {
		C.free(unsafe.Pointer(cText))
	}

	return btn, e
}

func (btn *Button) GetText() (text string, e error) {
	var (
		cBtn  *C.NEUIK_Button
		cText *C.char // Don't free; it's a pointer to the value, not a copy.
	)
	cBtn = (*C.NEUIK_Button)(unsafe.Pointer(btn))

	cText = C.NEUIK_Button_GetText(cBtn)
	if cText == nil {
		e = errors.New("`NEUIK_Button_GetText()` Failed; call `neuik.BacktraceErrors()` for details.")
	} else {
		text = C.GoString(cText)
	}

	return text, e
}

func (btn *Button) SetText(text string) (e error) {
	var (
		cBtn  *C.NEUIK_Button
		cText *C.char
	)
	cBtn = (*C.NEUIK_Button)(unsafe.Pointer(btn))
	cText = C.CString(text)

	if C.NEUIK_Button_SetText(cBtn, cText) != 0 {
		e = errors.New("`NEUIK_Button_SetText()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	if cText != nil {
		C.free(unsafe.Pointer(cText))
	}
	return e
}

func (btn *Button) Configure(cfgStrs ...string) (e error) {
	var (
		cBtn    *C.NEUIK_Button
		cCfgStr *C.char
		cfgStr  string
	)
	cBtn = (*C.NEUIK_Button)(unsafe.Pointer(btn))

	for _, cfgStr = range cfgStrs {
		cCfgStr = C.CString(cfgStr)

		if C.configButtonSingle(cBtn, cCfgStr) != 0 {
			e = errors.New("`NEUIK_Button_Configure()` Failed; call `neuik.BacktraceErrors()` for details.")
			break
		}

		if cCfgStr != nil {
			C.free(unsafe.Pointer(cCfgStr))
			cCfgStr = nil
		}
	}

	if cCfgStr != nil {
		C.free(unsafe.Pointer(cCfgStr))
		cCfgStr = nil
	}

	return e
}
