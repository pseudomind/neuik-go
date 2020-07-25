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
)

type Container interface {
	Element
	isNEUIKTypeContainer()
}

func Container_SetElement(cont Container, elem Element) (e error) {
	var (
		cCont C.NEUIK_Element
		cElem C.NEUIK_Element
	)
	cCont = cont.getNEUIKTypeElement()
	cElem = elem.getNEUIKTypeElement()

	if C.NEUIK_Container_SetElement(cCont, cElem) != 0 {
		e = errors.New("`NEUIK_Container_SetElement()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}

func Container_AddElement(cont Container, elem Element) (e error) {
	var (
		cCont C.NEUIK_Element
		cElem C.NEUIK_Element
	)
	cCont = cont.getNEUIKTypeElement()
	cElem = elem.getNEUIKTypeElement()

	if C.NEUIK_Container_AddElement(cCont, cElem) != 0 {
		e = errors.New("`NEUIK_Container_AddElement()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	return e
}

func Container_AddElements(cont Container, elems ...Element) (e error) {
	var elem Element

	for _, elem = range elems {
		e = Container_AddElement(cont, elem)
		if e != nil {
			break
		}
	}

	return e
}
