package neuik

/*
#cgo pkg-config: sdl2
#cgo LDFLAGS: -lSDL2_ttf -lSDL2_image
#include "NEUIK.h"

int configWindowSingle(
	NEUIK_Window * win,
	const char   * set0)
{
	return NEUIK_Window_Configure(win, set0, NULL);
}
*/
import "C"

import (
	"errors"
	"unsafe"
)

type Window C.NEUIK_Window

func NewWindow() (win *Window, e error) {
	var cWin *C.NEUIK_Window

	if C.NEUIK_NewWindow(&cWin) != 0 {
		e = errors.New("`NEUIK_NewWindow()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	win = (*Window)(cWin)

	return win, e
}

func (win *Window) Create() (e error) {
	var cWin *C.NEUIK_Window

	cWin = (*C.NEUIK_Window)(unsafe.Pointer(win))

	if C.NEUIK_Window_Create(cWin) != 0 {
		e = errors.New("`NEUIK_Window_Create()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}

func (win *Window) Configure(cfgStrs ...string) (e error) {
	var (
		cWin    *C.NEUIK_Window
		cCfgStr *C.char
		cfgStr  string
	)
	cWin = (*C.NEUIK_Window)(unsafe.Pointer(win))

	for _, cfgStr = range cfgStrs {
		cCfgStr = C.CString(cfgStr)

		if C.configWindowSingle(cWin, cCfgStr) != 0 {
			e = errors.New("`NEUIK_Window_Configure()` Failed; call `neuik.BacktraceErrors()` for details.")
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

func (win *Window) SetSize(w, h int) (e error) {
	var (
		cWin *C.NEUIK_Window
		cW   C.int
		cH   C.int
	)

	cW = (C.int)(w)
	cH = (C.int)(h)

	cWin = (*C.NEUIK_Window)(unsafe.Pointer(win))

	if C.NEUIK_Window_SetSize(cWin, cW, cH) != 0 {
		e = errors.New("`NEUIK_Window_SetSize()` Failed; call `neuik.BacktraceErrors()` for details.")
	}
	return e
}

func (win *Window) SetTitle(title string) (e error) {
	var (
		cWin   *C.NEUIK_Window
		cTitle *C.char
	)
	cWin = (*C.NEUIK_Window)(unsafe.Pointer(win))
	cTitle = C.CString(title)

	if C.NEUIK_Window_SetTitle(cWin, cTitle) != 0 {
		e = errors.New("`NEUIK_Window_SetTitle()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	if cTitle != nil {
		C.free(unsafe.Pointer(cTitle))
	}
	return e
}

func (win *Window) SetElement(elem Element) (e error) {
	var (
		cWin  *C.NEUIK_Window
		cElem C.NEUIK_Element
	)
	cWin = (*C.NEUIK_Window)(unsafe.Pointer(win))
	cElem = elem.getNEUIKTypeElement()

	if C.NEUIK_Window_SetElement(cWin, cElem) != 0 {
		e = errors.New("`NEUIK_Window_SetElement()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}

func (win *Window) SetCallback(cbName string, cbFunc func([]interface{}), cbArgs ...interface{}) (e error) {
	var (
		argList []interface{}
		cWin    *C.NEUIK_Window
		ccbName *C.char
		bindID  C.uint
	)

	cWin = (*C.NEUIK_Window)(unsafe.Pointer(win))
	ccbName = C.CString(cbName)

	bindID = GetUniqueCallbackBindID()

	if C.NEUIK_Window_SetBindingCallback(cWin, ccbName, bindID) != 0 {
		e = errors.New("`NEUIK_Window_SetBindingCallback()` Failed; call `neuik.BacktraceErrors()` for details.")
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
