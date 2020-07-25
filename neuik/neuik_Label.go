package neuik

/*
#cgo pkg-config: sdl2
#cgo LDFLAGS: -lSDL2_ttf -lSDL2_image
#include "NEUIK.h"

int configLabelSingle(
	NEUIK_Label * lbl,
	const char  * set0)
{
	return NEUIK_Label_Configure(lbl, set0, NULL);
}
*/
import "C"

import (
	"errors"
	"unsafe"
)

type Label C.NEUIK_Label

func (lbl *Label) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(lbl))
	return elem
}

func NewLabel() (lbl *Label, e error) {
	var cLbl *C.NEUIK_Label

	if C.NEUIK_NewLabel(&cLbl) != 0 {
		e = errors.New("`NEUIK_NewLabel()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	lbl = (*Label)(cLbl)

	return lbl, e
}

func MakeLabel(text string) (lbl *Label, e error) {
	var (
		cLbl  *C.NEUIK_Label
		cText *C.char
	)
	cText = C.CString(text)

	if C.NEUIK_MakeLabel(&cLbl, cText) != 0 {
		e = errors.New("`NEUIK_MakeLabel()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	lbl = (*Label)(cLbl)
	if cText != nil {
		C.free(unsafe.Pointer(cText))
	}

	return lbl, e
}

// const char *
// 	NEUIK_Label_GetText(
// 			NEUIK_Label  * label);
func (lbl *Label) GetText() (text string, e error) {
	var (
		cLbl  *C.NEUIK_Label
		cText *C.char
	)
	cLbl = (*C.NEUIK_Label)(unsafe.Pointer(lbl))

	cText = C.NEUIK_Label_GetText(cLbl);
	if cText == nil {
		e = errors.New("`NEUIK_Label_GetText()` Failed; call `neuik.BacktraceErrors()` for details.")
		goto out
	}

	text = C.GoString(cText)
out:
	return text, e
}


func (lbl *Label) SetText(text string) (e error) {
	var (
		cLbl  *C.NEUIK_Label
		cText *C.char
	)
	cLbl = (*C.NEUIK_Label)(unsafe.Pointer(lbl))
	cText = C.CString(text)

	if C.NEUIK_Label_SetText(cLbl, cText) != 0 {
		e = errors.New("`NEUIK_Label_SetText()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	if cText != nil {
		C.free(unsafe.Pointer(cText))
	}
	return e
}

func (lbl *Label) Configure(cfgStrs ...string) (e error) {
	var (
		cLbl    *C.NEUIK_Label
		cCfgStr *C.char
		cfgStr  string
	)
	cLbl = (*C.NEUIK_Label)(unsafe.Pointer(lbl))

	for _, cfgStr = range cfgStrs {
		cCfgStr = C.CString(cfgStr)

		if C.configLabelSingle(cLbl, cCfgStr) != 0 {
			e = errors.New("`NEUIK_Label_Configure()` Failed; call `neuik.BacktraceErrors()` for details.")
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
