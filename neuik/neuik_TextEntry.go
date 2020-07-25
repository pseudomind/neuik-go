package neuik

//
// #cgo pkg-config: sdl2
// #cgo LDFLAGS: -lSDL2_ttf -lSDL2_image
//
// #include "NEUIK.h"
//
/*

int configTextEntrySingle(
	NEUIK_TextEntry * te,
	const char      * set0)
{
	return NEUIK_TextEntry_Configure(te, set0, NULL);
}
*/
import "C"

import (
	"errors"
	"unsafe"
)

type TextEntry C.NEUIK_TextEntry

func (te *TextEntry) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(te))
	return elem
}

func NewTextEntry() (te *TextEntry, e error) {
	var cTE *C.NEUIK_TextEntry

	if C.NEUIK_NewTextEntry(&cTE) != 0 {
		e = errors.New("`NEUIK_NewTextEntry()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	te = (*TextEntry)(cTE)

	return te, e
}

func MakeTextEntry(text string) (te *TextEntry, e error) {
	var (
		cTE   *C.NEUIK_TextEntry
		cText *C.char
	)
	cText = C.CString(text)

	if C.NEUIK_MakeTextEntry(&cTE, cText) != 0 {
		e = errors.New("`NEUIK_MakeTextEntry()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	te = (*TextEntry)(cTE)
	if cText != nil {
		C.free(unsafe.Pointer(cText))
	}

	return te, e
}

// const char *
// 	NEUIK_TextEntry_GetText(
// 			NEUIK_TextEntry  * label);

func (te *TextEntry) SetText(text string) (e error) {
	var (
		cTE   *C.NEUIK_TextEntry
		cText *C.char
	)
	cTE = (*C.NEUIK_TextEntry)(unsafe.Pointer(te))
	cText = C.CString(text)

	if C.NEUIK_TextEntry_SetText(cTE, cText) != 0 {
		e = errors.New("`NEUIK_TextEntry_SetText()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	if cText != nil {
		C.free(unsafe.Pointer(cText))
	}
	return e
}

func (te *TextEntry) Configure(cfgStrs ...string) (e error) {
	var (
		cTE     *C.NEUIK_TextEntry
		cCfgStr *C.char
		cfgStr  string
	)
	cTE = (*C.NEUIK_TextEntry)(unsafe.Pointer(te))

	for _, cfgStr = range cfgStrs {
		cCfgStr = C.CString(cfgStr)

		if C.configTextEntrySingle(cTE, cCfgStr) != 0 {
			e = errors.New("`NEUIK_TextEntry_Configure()` Failed; call `neuik.BacktraceErrors()` for details.")
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
