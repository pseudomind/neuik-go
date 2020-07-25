package neuik

/*
#cgo pkg-config: sdl2
#cgo LDFLAGS: -lSDL2_ttf -lSDL2_image
#include "NEUIK.h"

int configToggleButtonSingle(
	NEUIK_ToggleButton * btn,
	const char         * set0)
{
	return NEUIK_ToggleButton_Configure(btn, set0, NULL);
}
*/
import "C"

import (
	"errors"
	"unsafe"
)

type ToggleButton C.NEUIK_ToggleButton

func (btn *ToggleButton) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(btn))
	return elem
}

func NewToggleButton() (btn *ToggleButton, e error) {
	var cBtn *C.NEUIK_ToggleButton

	if C.NEUIK_NewToggleButton(&cBtn) != 0 {
		e = errors.New("`NEUIK_NewToggleButton()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	btn = (*ToggleButton)(cBtn)

	return btn, e
}

func MakeToggleButton(text string) (btn *ToggleButton, e error) {
	var (
		cBtn  *C.NEUIK_ToggleButton
		cText *C.char
	)
	cText = C.CString(text)

	if C.NEUIK_MakeToggleButton(&cBtn, cText) != 0 {
		e = errors.New("`NEUIK_MakeToggleButton()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	btn = (*ToggleButton)(cBtn)
	if cText != nil {
		C.free(unsafe.Pointer(cText))
	}

	return btn, e
}

func (btn *ToggleButton) GetText() (text string, e error) {
	var (
		cBtn  *C.NEUIK_ToggleButton
		cText *C.char // Don't free; it's a pointer to the value, not a copy.
	)
	cBtn = (*C.NEUIK_ToggleButton)(unsafe.Pointer(btn))

	cText = C.NEUIK_ToggleButton_GetText(cBtn)
	if cText == nil {
		e = errors.New("`NEUIK_ToggleButton_GetText()` Failed; call `neuik.BacktraceErrors()` for details.")
	} else {
		text = C.GoString(cText)
	}

	return text, e
}

func (btn *ToggleButton) SetText(text string) (e error) {
	var (
		cBtn  *C.NEUIK_ToggleButton
		cText *C.char
	)
	cBtn = (*C.NEUIK_ToggleButton)(unsafe.Pointer(btn))
	cText = C.CString(text)

	if C.NEUIK_ToggleButton_SetText(cBtn, cText) != 0 {
		e = errors.New("`NEUIK_ToggleButton_SetText()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	if cText != nil {
		C.free(unsafe.Pointer(cText))
	}
	return e
}

func (btn *ToggleButton) Configure(cfgStrs ...string) (e error) {
	var (
		cBtn    *C.NEUIK_ToggleButton
		cCfgStr *C.char
		cfgStr  string
	)
	cBtn = (*C.NEUIK_ToggleButton)(unsafe.Pointer(btn))

	for _, cfgStr = range cfgStrs {
		cCfgStr = C.CString(cfgStr)

		if C.configToggleButtonSingle(cBtn, cCfgStr) != 0 {
			e = errors.New("`NEUIK_ToggleButton_Configure()` Failed; call `neuik.BacktraceErrors()` for details.")
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

func (btn *ToggleButton) Activate() (e error) {
	var cBtn *C.NEUIK_ToggleButton

	cBtn = (*C.NEUIK_ToggleButton)(unsafe.Pointer(btn))

	if C.NEUIK_ToggleButton_Activate(cBtn) != 0 {
		e = errors.New("`NEUIK_ToggleButton_Activate()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}

func (btn *ToggleButton) Deactivate() (e error) {
	var cBtn *C.NEUIK_ToggleButton

	cBtn = (*C.NEUIK_ToggleButton)(unsafe.Pointer(btn))

	if C.NEUIK_ToggleButton_Deactivate(cBtn) != 0 {
		e = errors.New("`NEUIK_ToggleButton_Deactivate()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}

// int
// 	NEUIK_ToggleButton_IsActivated(
// 			NEUIK_ToggleButton  * btn);
func (btn *ToggleButton) IsActivated() (isActivated bool) {
	var cBtn *C.NEUIK_ToggleButton

	cBtn = (*C.NEUIK_ToggleButton)(unsafe.Pointer(btn))

	if C.NEUIK_ToggleButton_IsActivated(cBtn) == 0 {
		isActivated = false
	} else {
		isActivated = true
	}

	return isActivated
}
