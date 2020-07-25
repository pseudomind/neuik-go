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

type ListRow C.NEUIK_ListRow

func (row *ListRow) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(row))
	return elem
}

func (row ListRow) isNEUIKTypeContainer() {}

func NewListRow() (row *ListRow, e error) {
	var cHG *C.NEUIK_ListRow

	if C.NEUIK_NewListRow(&cHG) != 0 {
		e = errors.New("`NEUIK_NewListRow()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	row = (*ListRow)(cHG)

	return row, e
}

func (row *ListRow) SetHSpacing(spacing int) (e error) {
	var (
		cHG      *C.NEUIK_ListRow
		cSpacing C.int
	)
	cHG = (*C.NEUIK_ListRow)(unsafe.Pointer(row))
	cSpacing = C.int(spacing)

	if C.NEUIK_ListRow_SetHSpacing(cHG, cSpacing) != 0 {
		e = errors.New("`NEUIK_ListRow_SetHSpacing()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}
