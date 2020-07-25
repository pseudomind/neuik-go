package neuik

//
// #cgo pkg-config: sdl2
// #cgo LDFLAGS: -lSDL2_ttf -lSDL2_image
//
// #include "NEUIK_Callback.h"
//
import "C"

import (
	"time"
)

type callback struct {
	isUsed bool
	cbFunc func([]interface{})
	cbArgs []interface{}
}

func (cb *callback) Trigger() {
	cb.cbFunc(cb.cbArgs)
}

var (
	bindID           C.uint
	next             C.uint = 0
	nBindIDsInChan   int
	chanNextBindID   chan C.uint
	chanBindIDRecv   chan bool
	bindingCallbacks map[C.uint]callback
)

func StartCallbackHandler() {
	var (
		bindCtr  C.uint
		thisCB   callback
		stackPop chan C.uint
	)

	bindingCallbacks = make(map[C.uint]callback)
	chanNextBindID = make(chan C.uint, 20)
	chanBindIDRecv = make(chan bool, 20)
	stackPop = make(chan C.uint, 20)

	//------------------------------------------------------------------------//
	// Place five valid bindIDs onto the nextBindID channel                   //
	//------------------------------------------------------------------------//
	for bindCtr = 0; bindCtr < 5; bindCtr += 1 {
		nBindIDsInChan += 1
		chanNextBindID <- bindCtr
	}

	//------------------------------------------------------------------------//
	// Start popping Binding Callbacks from the C API as soon as they are     //
	// available.                                                             //
	//------------------------------------------------------------------------//
	go func(outChan chan C.uint) {
		for {
			outChan <- C.NEUIK_WaitForBindingCallback(3)
		}
	}(stackPop)

	for {
		select {
		case <-chanBindIDRecv:
			chanNextBindID <- bindCtr
			nBindIDsInChan += 1
			bindCtr += 1
		case bindID = <-stackPop:
			thisCB = bindingCallbacks[bindID]
			thisCB.Trigger()
		}
	}
}

func RegisterBindingCallback(bindID C.uint, cbFunc func([]interface{}), cbArgs []interface{}) {
	bindingCallbacks[bindID] = callback{true, cbFunc, cbArgs}
}

func GetUniqueCallbackBindID() (bindID C.uint) {
	//------------------------------------------------------------------------//
	// Make sure that there are availible bindIDs in the channel before       //
	// attempting to block on receive.                                        //
	//------------------------------------------------------------------------//
	for {
		if nBindIDsInChan > 0 {
			break
		}
		time.Sleep(2)
	}

	bindID = <-chanNextBindID
	chanBindIDRecv <- true
	nBindIDsInChan -= 1

	return bindID
}
