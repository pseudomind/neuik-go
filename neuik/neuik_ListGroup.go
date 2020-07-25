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

type ListGroup C.NEUIK_ListGroup

func (lg *ListGroup) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(lg))
	return elem
}

func (lg ListGroup) isNEUIKTypeContainer() {}

func NewListGroup() (lg *ListGroup, e error) {
	var cVG *C.NEUIK_ListGroup

	if C.NEUIK_NewListGroup(&cVG) != 0 {
		e = errors.New("`NEUIK_NewListGroup()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	lg = (*ListGroup)(cVG)

	return lg, e
}

func (lg *ListGroup) AddRow(row *ListRow) (e error) {
	var (
		cLG  *C.NEUIK_ListGroup
		cRow *C.NEUIK_ListRow
	)
	cLG  = (*C.NEUIK_ListGroup)(lg.getNEUIKTypeElement())
	cRow = (*C.NEUIK_ListRow)(row.getNEUIKTypeElement())

	if C.NEUIK_ListGroup_AddRow(cLG, cRow) != 0 {
		// (*C.NEUIK_ListGroup)(cLG), (*C.NEUIK_ListRow)(cRow) != 0 {
		e = errors.New("`NEUIK_ListGroup_AddRow()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}

func (lg *ListGroup) AddRows(rows ...*ListRow) (e error) {
	var row *ListRow

	for _, row = range rows {
		e = lg.AddRow(row)
		if e != nil {
			break
		}
	}

	return e
}
