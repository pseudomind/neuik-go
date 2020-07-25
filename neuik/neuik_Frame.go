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

type Frame C.NEUIK_Frame

func (fr *Frame) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(fr))
	return elem
}

func (fr Frame) isNEUIKTypeContainer() {}

func NewFrame() (fr *Frame, e error) {
	var cFR *C.NEUIK_Frame

	if C.NEUIK_NewFrame(&cFR) != 0 {
		e = errors.New("`NEUIK_NewFrame()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	fr = (*Frame)(cFR)

	return fr, e
}
