package neuik

/*
#cgo pkg-config: sdl2
#cgo LDFLAGS: -lSDL2_ttf -lSDL2_image
#include "NEUIK.h"

// int configProgressBarSingle(
// 	NEUIK_ProgressBar * pb,
// 	const char   * set0)
// {
// 	return NEUIK_ProgressBar_Configure(pb, set0, NULL);
// }
*/
import "C"

import (
	"errors"
	"unsafe"
)

type ProgressBar C.NEUIK_ProgressBar

func (pb *ProgressBar) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(pb))
	return elem
}

func NewProgressBar() (pb *ProgressBar, e error) {
	var cPB *C.NEUIK_ProgressBar

	if C.NEUIK_NewProgressBar(&cPB) != 0 {
		e = errors.New("`NEUIK_NewProgressBar()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	pb = (*ProgressBar)(cPB)

	return pb, e
}

func (pb *ProgressBar) GetFraction() (frac float64, e error) {
	var (
		cPB   *C.NEUIK_ProgressBar
		cFrac C.double
	)
	cPB = (*C.NEUIK_ProgressBar)(unsafe.Pointer(pb))

	if C.NEUIK_ProgressBar_GetFraction(cPB, &cFrac) != 0 {
		e = errors.New("`NEUIK_ProgressBar_GetFrac()` Failed; call `neuik.BacktraceErrors()` for details.")
	} else {
		frac = (float64)(cFrac)
	}

	return frac, e
}

func (pb *ProgressBar) SetFraction(frac float64) (e error) {
	var (
		cPB   *C.NEUIK_ProgressBar
		cFrac C.double
	)
	cPB = (*C.NEUIK_ProgressBar)(unsafe.Pointer(pb))
	cFrac = C.double(frac)

	if C.NEUIK_ProgressBar_SetFraction(cPB, cFrac) != 0 {
		e = errors.New("`NEUIK_ProgressBar_SetFrac()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}

// func (pb *ProgressBar) Configure(cfgStrs ...string) (e error) {
// 	var (
// 		cPB     *C.NEUIK_ProgressBar
// 		cCfgStr *C.char
// 		cfgStr  string
// 	)
// 	cPB = (*C.NEUIK_ProgressBar)(unsafe.Pointer(pb))

// 	for _, cfgStr = range cfgStrs {
// 		cCfgStr = C.CString(cfgStr)

// 		if C.configProgressBarSingle(cPB, cCfgStr) != 0 {
// 			e = errors.New("`NEUIK_ProgressBar_Configure()` Failed; call `neuik.BacktraceErrors()` for details.")
// 			break
// 		}

// 		if cCfgStr != nil {
// 			C.free(unsafe.Pointer(cCfgStr))
// 			cCfgStr = nil
// 		}
// 	}

// 	if cCfgStr != nil {
// 		C.free(unsafe.Pointer(cCfgStr))
// 		cCfgStr = nil
// 	}

// 	return e
// }
