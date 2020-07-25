package neuik

//
// #cgo pkg-config: sdl2
// #cgo CFLAGS: -I./include
// #cgo LDFLAGS: -lSDL2_ttf -lSDL2_image -lm
//
// #include <stdlib.h>
// #include "NEUIK_neuik.h"
// #include "NEUIK_error.h"
// #include "NEUIK_Event.h"
//
import "C"

import (
	"errors"
	"runtime"
	"unsafe"
)

// type Color C.NEUIK_Color

// type Color struct {
// 	R byte
// 	G byte
// 	B byte
// 	A byte
// }

func init() {
	runtime.LockOSThread()
}

func Init() (errOut bool) {
	// runtime.LockOSThread()

	if C.NEUIK_Init() != 0 {
		C.NEUIK_BacktraceErrors()
		errOut = true
	}
	go StartCallbackHandler()

	return errOut
}

func Quit() {
	C.NEUIK_Quit()
}

func SetAppName(appName string) (e error) {
	var (
		cAppName *C.char
	)

	cAppName = C.CString(appName)

	if C.NEUIK_SetAppName(cAppName) != 0 {
		e = errors.New("`NEUIK_SetAppName()` Failed.")
	}

	if cAppName != nil {
		C.free(unsafe.Pointer(cAppName))
	}

	return e
}

func EventLoop(killOnError bool) {
	var cKillOnError C.int
	if killOnError {
		cKillOnError = (C.int)(1)
	}

	C.NEUIK_EventLoop(cKillOnError)
}

func EventLoopNoErrHandling() {
	C.NEUIK_EventLoopNoErrHandling()
}
