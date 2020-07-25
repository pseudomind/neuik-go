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

type FlowGroup C.NEUIK_FlowGroup

func (fg *FlowGroup) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(fg))
	return elem
}

func (fg FlowGroup) isNEUIKTypeContainer() {}

func NewFlowGroup() (fg *FlowGroup, e error) {
	var cVG *C.NEUIK_FlowGroup

	if C.NEUIK_NewFlowGroup(&cVG) != 0 {
		e = errors.New("`NEUIK_NewFlowGroup()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	fg = (*FlowGroup)(cVG)

	return fg, e
}

// func (fg *FlowGroup) SetHSpacing(spacing int) (e error) {
// 	var (
// 		cVG      *C.NEUIK_FlowGroup
// 		cSpacing C.int
// 	)
// 	cVG = (*C.NEUIK_FlowGroup)(unsafe.Pointer(fg))
// 	cSpacing = C.int(spacing)

// 	if C.NEUIK_FlowGroup_SetHSpacing(cVG, cSpacing) != 0 {
// 		e = errors.New("`NEUIK_FlowGroup_SetHSpacing()` Failed; call `neuik.BacktraceErrors()` for details.")
// 	}

// 	return e
// }
