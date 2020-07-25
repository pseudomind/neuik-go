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

type VGroup C.NEUIK_VGroup

func (vg *VGroup) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(vg))
	return elem
}

func (vg VGroup) isNEUIKTypeContainer() {}

func NewVGroup() (vg *VGroup, e error) {
	var cVG *C.NEUIK_VGroup

	if C.NEUIK_NewVGroup(&cVG) != 0 {
		e = errors.New("`NEUIK_NewVGroup()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	vg = (*VGroup)(cVG)

	return vg, e
}

func (vg *VGroup) SetVSpacing(spacing int) (e error) {
	var (
		cVG      *C.NEUIK_VGroup
		cSpacing C.int
	)
	cVG = (*C.NEUIK_VGroup)(unsafe.Pointer(vg))
	cSpacing = C.int(spacing)

	if C.NEUIK_VGroup_SetVSpacing(cVG, cSpacing) != 0 {
		e = errors.New("`NEUIK_VGroup_SetVSpacing()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}
