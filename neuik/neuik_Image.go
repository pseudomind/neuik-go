package neuik

/*
#cgo pkg-config: sdl2
#cgo LDFLAGS: -lSDL2_ttf -lSDL2_image
#include "NEUIK.h"

// int configLabelSingle(
// 	NEUIK_Label * lbl,
// 	const char  * set0)
// {
// 	return NEUIK_Label_Configure(lbl, set0, NULL);
// }
*/
import "C"

import (
	"errors"
	"unsafe"
)

type Image C.NEUIK_Image

func (img *Image) getNEUIKTypeElement() (elem C.NEUIK_Element) {
	elem = (C.NEUIK_Element)(unsafe.Pointer(img))
	return elem
}

func NewImage() (img *Image, e error) {
	var cImg *C.NEUIK_Image

	if C.NEUIK_NewImage(&cImg) != 0 {
		e = errors.New("`NEUIK_NewImage()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	img = (*Image)(cImg)

	return img, e
}

func MakeImage(text string) (img *Image, e error) {
	var (
		cImg  *C.NEUIK_Image
		cText *C.char
	)
	cText = C.CString(text)

	if C.NEUIK_MakeImage(&cImg, cText) != 0 {
		e = errors.New("`NEUIK_MakeImage()` Failed; call `neuik.BacktraceErrors()` for details.")
	}

	img = (*Image)(cImg)
	if cText != nil {
		C.free(unsafe.Pointer(cText))
	}

	return img, e
}

// const char *
// 	NEUIK_Label_GetText(
// 			NEUIK_Label  * label);
// func (lbl *Label) GetText() (text string, e error) {
// 	var (
// 		cLbl  *C.NEUIK_Label
// 		cText *C.char
// 	)
// 	cLbl = (*C.NEUIK_Label)(unsafe.Pointer(lbl))

// 	cText = C.NEUIK_Label_GetText(cLbl)
// 	if cText == nil {
// 		e = errors.New("`NEUIK_Label_GetText()` Failed; call `neuik.BacktraceErrors()` for details.")
// 		goto out
// 	}

// 	text = C.GoString(cText)
// out:
// 	return text, e
// }

// func (lbl *Label) SetText(text string) (e error) {
// 	var (
// 		cLbl  *C.NEUIK_Label
// 		cText *C.char
// 	)
// 	cLbl = (*C.NEUIK_Label)(unsafe.Pointer(lbl))
// 	cText = C.CString(text)

// 	if C.NEUIK_Label_SetText(cLbl, cText) != 0 {
// 		e = errors.New("`NEUIK_Label_SetText()` Failed; call `neuik.BacktraceErrors()` for details.")
// 	}

// 	if cText != nil {
// 		C.free(unsafe.Pointer(cText))
// 	}
// 	return e
// }
