package neuik

/*
#cgo pkg-config: sdl2
#cgo LDFLAGS: -lSDL2_ttf -lSDL2_image
#include "NEUIK.h"

int configElementSingle(
	NEUIK_Element    elem,
	const char     * set0)
{
	return NEUIK_Element_Configure(elem, set0, NULL);
}
*/
import "C"

import (
	"errors"
	"unsafe"
)

type Element interface {
	getNEUIKTypeElement() C.NEUIK_Element
}

type voidPtr interface{}

func Element_SetCallback(elem Element, cbName string, cbFunc func([]interface{}), cbArgs ...interface{}) (e error) {
	var (
		argList []interface{}
		cElem   C.NEUIK_Element
		ccbName *C.char
		bindID  C.uint
	)
	cElem = elem.getNEUIKTypeElement()
	ccbName = C.CString(cbName)

	bindID = GetUniqueCallbackBindID()

	if C.NEUIK_Element_SetBindingCallback(cElem, ccbName, bindID) != 0 {
		e = errors.New("`NEUIK_Element_SetBindingCallback()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	for _, arg := range cbArgs {
		argList = append(argList, arg)
	}
	RegisterBindingCallback(bindID, cbFunc, argList)

	if ccbName != nil {
		C.free(unsafe.Pointer(ccbName))
	}
	return e
}

func Element_Configure(elem Element, cfgStrs ...string) (e error) {
	var (
		cElem   C.NEUIK_Element
		cCfgStr *C.char
		cfgStr  string
	)
	cElem = elem.getNEUIKTypeElement()

	for _, cfgStr = range cfgStrs {
		cCfgStr = C.CString(cfgStr)

		if C.configElementSingle(cElem, cCfgStr) != 0 {
			e = errors.New("`NEUIK_Element_Configure()` Failed; call `neuik.BacktraceErrors()` for details.")
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

// int
// 	NEUIK_Element_SetBackgroundColorGradient(
// 			NEUIK_Element   elem,
// 			const char    * styleName,
// 			char            direction,
// 			const char    * colorStop0,
// 			...);

// int
// 	NEUIK_Element_SetBackgroundColorSolid(
// 			NEUIK_Element   elem,
// 			const char    * styleName,
// 			unsigned char   r,
// 			unsigned char   g,
// 			unsigned char   b,
// 			unsigned char   a);

func Element_SetBackgroundColorSolid(elem Element, styleName string, r uint8, g uint8, b uint8, a uint8) (e error) {
	var (
		cElem      C.NEUIK_Element
		cstyleName *C.char
		cR         C.uchar
		cG         C.uchar
		cB         C.uchar
		cA         C.uchar
	)
	cElem = elem.getNEUIKTypeElement()
	cstyleName = C.CString(styleName)

	cR = C.uchar(r)
	cG = C.uchar(g)
	cB = C.uchar(b)
	cA = C.uchar(a)

	if C.NEUIK_Element_SetBackgroundColorSolid(cElem, cstyleName, cR, cG, cB, cA) != 0 {
		e = errors.New("`NEUIK_Element_SetBackgroundColorSolid()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	if cstyleName != nil {
		C.free(unsafe.Pointer(cstyleName))
	}
	return e
}

// int
// 	NEUIK_Element_SetBackgroundColorTransparent(
// 			NEUIK_Element   elem,
// 			const char    * styleName);
