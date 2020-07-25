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

type Line C.NEUIK_Line

func (line *Line) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(line))
	return elem
}

func NewHLine() (line *Line, e error) {
	var cVG *C.NEUIK_Line

	if C.NEUIK_NewHLine(&cVG) != 0 {
		e = errors.New("`NEUIK_NewHLine()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	line = (*Line)(cVG)

	return line, e
}

func NewVLine() (line *Line, e error) {
	var cVG *C.NEUIK_Line

	if C.NEUIK_NewVLine(&cVG) != 0 {
		e = errors.New("`NEUIK_NewVLine()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	line = (*Line)(cVG)

	return line, e
}

func (line *Line) SetThickness(px int) (e error) {
	var (
		cVG *C.NEUIK_Line
		cPX C.int
	)
	cVG = (*C.NEUIK_Line)(unsafe.Pointer(line))
	cPX = C.int(px)

	if C.NEUIK_Line_SetThickness(cVG, cPX) != 0 {
		e = errors.New("`NEUIK_Line_SetThickness()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}
