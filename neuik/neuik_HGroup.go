package neuik

//
// #cgo pkg-config: sdl2
// #cgo LDFLAGS: -lSDL2_ttf -lSDL2_image
//
// #include "NEUIK.h"
//
import "C"

import (
	"errors"
	"unsafe"
)

type HGroup C.NEUIK_HGroup

func (hg *HGroup) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(hg))
	return elem
}

func (hg HGroup) isNEUIKTypeContainer() {}

func NewHGroup() (hg *HGroup, e error) {
	var cHG *C.NEUIK_HGroup

	if C.NEUIK_NewHGroup(&cHG) != 0 {
		e = errors.New("`NEUIK_NewHGroup()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	hg = (*HGroup)(cHG)

	return hg, e
}

func (hg *HGroup) SetHSpacing(spacing int) (e error) {
	var (
		cHG      *C.NEUIK_HGroup
		cSpacing C.int
	)
	cHG = (*C.NEUIK_HGroup)(unsafe.Pointer(hg))
	cSpacing = C.int(spacing)

	if C.NEUIK_HGroup_SetHSpacing(cHG, cSpacing) != 0 {
		e = errors.New("`NEUIK_HGroup_SetHSpacing()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}
