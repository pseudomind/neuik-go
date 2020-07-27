/*******************************************************************************
 * Copyright (c) 2014-2020, Michael Leimon <leimon@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "NEUIK_error.h"
#include "NEUIK_render.h"
#include "NEUIK_structs_basic.h"
#include "NEUIK_colors.h"
#include "NEUIK_Window_internal.h"
#include "NEUIK_Button.h"
#include "NEUIK_Element_internal.h"
#include "neuik_internal.h"
#include "neuik_classes.h"

extern int neuik__isInitialized;
extern float neuik__HighDPI_Scaling;

/*----------------------------------------------------------------------------*/
/* Internal Function Prototypes                                               */
/*----------------------------------------------------------------------------*/
int neuik_Object_New__Button(void ** btnPtr);
int neuik_Object_Free__Button(void * btnPtr);
int neuik_Element_GetMinSize__Button(NEUIK_Element, RenderSize*);
neuik_EventState neuik_Element_CaptureEvent__Button(NEUIK_Element, SDL_Event*);
int neuik_Element_Render__Button(
    NEUIK_Element, RenderSize*, RenderLoc*, SDL_Renderer*, int);

/*----------------------------------------------------------------------------*/
/* neuik_Object    Function Table                                             */
/*----------------------------------------------------------------------------*/
neuik_Class_BaseFuncs  neuik_Button_BaseFuncs = {
    /* Init(): Class initialization (in most cases will not be needed) */
    NULL, /* (unused) */
    /* New(): Allocate and Initialize the object */
    neuik_Object_New__Button,
    /* Copy(): Copy the contents of one object into another */
    NULL,
    /* Free(): Free the allocated memory of an object */
    neuik_Object_Free__Button,
};

/*----------------------------------------------------------------------------*/
/* neuik_Element    Function Table                                            */
/*----------------------------------------------------------------------------*/
NEUIK_Element_FuncTable neuik_Button_FuncTable = {
    /* GetMinSize(): Get the minimum required size for the element  */
    neuik_Element_GetMinSize__Button,

    /* Render(): Redraw the element  element  */
    neuik_Element_Render__Button,

    /* CaptureEvent(): Determine if this element caputures a given event */
    neuik_Element_CaptureEvent__Button,

    /* Defocus(): This function will be called when an element looses focus */
    NULL,
};


/*******************************************************************************
 *
 *  Name:          neuik_RegisterClass_Button
 *
 *  Description:   Register this class with the NEUIK runtime.
 *
 *  Returns:       1 if there is an error; 0 otherwise.
 *
 ******************************************************************************/
int neuik_RegisterClass_Button()
{
    int            eNum       = 0; /* which error to report (if any) */
    static char    funcName[] = "neuik_RegisterClass_Button";
    static char  * errMsgs[]  = {"",                  // [0] no error
        "NEUIK library must be initialized first.",   // [1]
        "Failed to register `Button` object class .", // [2]
    };

    if (!neuik__isInitialized)
    {
        eNum = 1;
        goto out;
    }

    /*------------------------------------------------------------------------*/
    /* Otherwise, register the object                                         */
    /*------------------------------------------------------------------------*/
    if (neuik_RegisterClass(
        "Button",                               // className
        "A GUI button which may contain text.", // classDescription
        neuik__Set_NEUIK,                       // classSet
        neuik__Class_Element,                   // superClass
        &neuik_Button_BaseFuncs,                // baseFuncs
        NULL,                                   // classFuncs
        &neuik__Class_Button))                  // newClass
    {
        eNum = 2;
        goto out;
    }
out:
    if (eNum > 0)
    {
        NEUIK_RaiseError(funcName, errMsgs[eNum]);
        eNum = 1;
    }

    return eNum;
}


/*******************************************************************************
 *
 *  Name:          neuik_Object_New__Button
 *
 *  Description:   An implementation of the neuik_Object_New method.
 *
 *  Returns:       1 if there is an error; 0 otherwise.
 *
 ******************************************************************************/
int neuik_Object_New__Button(
    void ** btnPtr)
{
    int             eNum       = 0; /* which error to report (if any) */
    NEUIK_Button  * btn        = NULL;
    NEUIK_Element * sClassPtr  = NULL;
    static char     funcName[] = "neuik_Object_New__Button";
    static char   * errMsgs[]  = {"",                             // [0] no error
        "Failure to allocate memory.",                            // [1]
        "Failure in NEUIK_NewButtonConfig.",                      // [2]
        "Output Argument `btnPtr` is NULL.",                      // [3]
        "Failure in function `neuik_Object_New`.",                // [4]
        "Failure in function `neuik_Element_SetFuncTable`.",      // [5]
        "Failure in `neuik_GetObjectBaseOfClass`.",               // [6]
        "Failure in `NEUIK_Element_SetBackgroundColorGradient`.", // [7]
    };

    if (btnPtr == NULL)
    {
        eNum = 3;
        goto out;
    }

    (*btnPtr) = (NEUIK_Button*) malloc(sizeof(NEUIK_Button));
    btn = *btnPtr;
    if (btn == NULL)
    {
        eNum = 1;
        goto out;
    }

    /*------------------------------------------------------------------------*/
    /* Successful allocation of Memory -- Create Base Class Object            */
    /*------------------------------------------------------------------------*/
    if (neuik_GetObjectBaseOfClass(
            neuik__Set_NEUIK, 
            neuik__Class_Button, 
            NULL,
            &(btn->objBase)))
    {
        eNum = 6;
        goto out;
    }

    /*------------------------------------------------------------------------*/
    /* Create first level Base SuperClass Object                              */
    /*------------------------------------------------------------------------*/
    sClassPtr = (NEUIK_Element *) &(btn->objBase.superClassObj);
    if (neuik_Object_New(neuik__Class_Element, sClassPtr))
    {
        eNum = 4;
        goto out;
    }
    if (neuik_Element_SetFuncTable(*sClassPtr, &neuik_Button_FuncTable))
    {
        eNum = 5;
        goto out;
    }

    /* Allocation successful */
    btn->cfg          = NULL;
    btn->cfgPtr       = NULL;
    btn->text         = NULL;
    btn->selected     = 0;
    btn->wasSelected  = 0;
    btn->isActive     = 0;
    btn->clickOrigin  = 0;
    btn->needsRedraw  = 1;

    if (NEUIK_NewButtonConfig(&btn->cfg))
    {
        eNum = 2;
        goto out;
    }

    /*------------------------------------------------------------------------*/
    /* Set the default element background redraw styles.                      */
    /*------------------------------------------------------------------------*/
    if (NEUIK_Element_SetBackgroundColorGradient(btn, "normal", 'v',
        "220,220,220,255,0.0",
        "200,200,200,255,1.0",
        NULL))
    {
        eNum = 7;
        goto out;
    }
    if (NEUIK_Element_SetBackgroundColorGradient(btn, "selected", 'v',
        "116,153,230,255,0.0",
        "45,90,220,255,1.0",
        NULL))
    {
        eNum = 7;
        goto out;
    }
    if (NEUIK_Element_SetBackgroundColorGradient(btn, "hovered", 'v',
        "220,220,220,255,0.0",
        "200,200,200,255,1.0",
        NULL))
    {
        eNum = 7;
        goto out;
    }
out:
    if (eNum > 0)
    {
        NEUIK_RaiseError(funcName, errMsgs[eNum]);
        eNum = 1;
    }

    return eNum;
}


/*******************************************************************************
 *
 *  Name:          neuik_Object_Free__Button
 *
 *  Description:   An implementation of the neuik_Object_Free method.
 *
 *  Returns:       1 if there is an error; 0 otherwise.
 *
 ******************************************************************************/
int neuik_Object_Free__Button(
    void * btnPtr)  /* [out] the button to free */
{
    int            eNum       = 0; /* which error to report (if any) */
    NEUIK_Button * btn        = NULL;
    static char    funcName[] = "neuik_Object_Free__Button";
    static char  * errMsgs[]  = {"",                 // [0] no error
        "Argument `btnPtr` is not of Button class.", // [1]
        "Failure in function `neuik_Object_Free`.",  // [2]
        "Argument `btnPtr` is NULL.",                // [3]
    };

    if (btnPtr == NULL)
    {
        eNum = 3;
        goto out;
    }

    if (!neuik_Object_IsClass(btnPtr, neuik__Class_Button))
    {
        eNum = 1;
        goto out;
    }
    btn = (NEUIK_Button*)btnPtr;

    /*------------------------------------------------------------------------*/
    /* The object is what it says it is and it is still allocated.            */
    /*------------------------------------------------------------------------*/
    if(neuik_Object_Free(btn->objBase.superClassObj))
    {
        eNum = 2;
        goto out;
    }
    if(btn->text != NULL) free(btn->text);
    if(neuik_Object_Free((void**)btn->cfg))
    {
        eNum = 2;
        goto out;
    }

    free(btn);
out:
    if (eNum > 0)
    {
        NEUIK_RaiseError(funcName, errMsgs[eNum]);
        eNum = 1;
    }

    return eNum;
}


/*******************************************************************************
 *
 *  Name:          neuik_Element_GetMinSize__Button
 *
 *  Description:   Returns the rendered size of a given button.
 *
 *  Returns:       1 if there is an error; 0 otherwise.
 *
 ******************************************************************************/
int neuik_Element_GetMinSize__Button(
    NEUIK_Element    elem,
    RenderSize     * rSize)
{
    int                  tW;
    int                  tH;
    int                  eNum       = 0;    /* which error to report (if any) */
    TTF_Font           * font       = NULL;
    NEUIK_Button       * btn        = NULL;
    NEUIK_ButtonConfig * aCfg       = NULL; /* the active button config */
    static char          funcName[] = "neuik_Element_GetMinSize__Button";
    static char        * errMsgs[]  = {"",         // [0] no error
        "Argument `elem` is not of Button class.", // [1]
        "ButtonConfig* is NULL.",                  // [2]
        "ButtonConfig->FontSet is NULL.",          // [3]
        "FontSet_GetFont returned NULL.",          // [4]
    };

    /*------------------------------------------------------------------------*/
    /* Calculate the required size of the resultant texture                   */
    /*------------------------------------------------------------------------*/
    if (!neuik_Object_IsClass(elem, neuik__Class_Button))
    {
        eNum = 1;
        goto out;
    }
    btn = (NEUIK_Button*)elem;
    
    /* select the correct button config to use (pointer or internal) */
    if (btn->cfgPtr != NULL)
    {
        aCfg = btn->cfgPtr;
    }
    else 
    {
        aCfg = btn->cfg;
    }

    if (aCfg == NULL)
    {
        eNum = 2;
        goto out;
    } 

    if (aCfg->fontSet == NULL)
    {
        eNum = 3;
        goto out;
    }

    font = NEUIK_FontSet_GetFont(aCfg->fontSet, aCfg->fontSize, 
        aCfg->fontBold, aCfg->fontItalic);
    if (font == NULL) 
    {
        eNum = 4;
        goto out;
    }

    if (btn->text != NULL)
    {
        /* this button contains text */
        TTF_SizeText(font, btn->text, &tW, &tH);

    }
    else
    {
        /* this button does not contain text */
        TTF_SizeText(font, " ", &tW, &tH);
    }

    rSize->w = tW + aCfg->fontEmWidth;
    rSize->h = (int)(1.5 * (float)TTF_FontHeight(font));

    if (neuik__HighDPI_Scaling >= 2.0)
    {
        /*--------------------------------------------------------------------*/
        /* Add in additional pixels of width/height to accomodate for thicker */
        /* button borders.                                                    */
        /*--------------------------------------------------------------------*/
        rSize->w += 2*(int)(neuik__HighDPI_Scaling/2.0);
        rSize->h += 2*(int)(neuik__HighDPI_Scaling/2.0);
    }
out:
    if (eNum > 0)
    {
        NEUIK_RaiseError(funcName, errMsgs[eNum]);
        eNum = 1;
    }

    return eNum;
}

/*******************************************************************************
 *
 *  Name:          NEUIK_NewButton
 *
 *  Description:   Create a new NEUIK_Button without contained text.
 *
 *  Returns:       1 if there is an error; 0 otherwise.
 *
 ******************************************************************************/
int NEUIK_NewButton(
    NEUIK_Button ** btnPtr)  /* [out] The newly created NEUIK_Button.  */
{
    return neuik_Object_New__Button((void **)btnPtr);
}


/*******************************************************************************
 *
 *  Name:          NEUIK_MakeButton
 *
 *  Description:   Create a new NEUIK_Button with specified text.
 *
 *  Returns:       1 if there is an error; 0 otherwise.
 *
 ******************************************************************************/
int NEUIK_MakeButton(
    NEUIK_Button ** btnPtr,  /* [out] The newly created NEUIK_Button.  */
    const char    * text)    /* [in]  Initial button text. */
{
    size_t         sLen       = 1;
    int            eNum       = 0; /* which error to report (if any) */
    NEUIK_Button * btn        = NULL;
    static char    funcName[] = "NEUIK_MakeButton";
    static char  * errMsgs[]  = {"",                       // [0] no error
        "Failure in function `neuik_Object_New__Button`.", // [1]
        "Failure to allocate memory.",                     // [2]
    };

    if (neuik_Object_New__Button((void**)btnPtr))
    {
        eNum = 1;
        goto out;
    }
    btn = *btnPtr;

    /*------------------------------------------------------------------------*/
    /* Set the new Button text contents                                       */
    /*------------------------------------------------------------------------*/
    if (text == NULL){
        /* button will contain no text */
        btn->text = NULL;
    }
    else if (text[0] == '\0')
    {
        /* button will contain no text */
        btn->text = NULL;
    }
    else
    {
        sLen += strlen(text);
        btn->text = (char*)malloc(sLen*sizeof(char));
        if (btn->text == NULL) {
            eNum = 2;
            goto out;
        }
        /* Allocation successful */
        strcpy(btn->text, text);
    }
out:
    if (eNum > 0)
    {
        NEUIK_RaiseError(funcName, errMsgs[eNum]);
        eNum = 1;
    }

    return eNum;
}


/*******************************************************************************
 *
 *  Name:          NEUIK_Button_SetText
 *
 *  Description:   Update the text in a NEUIK_Button.
 *
 *  Returns:       1 if there is an error; 0 otherwise.
 *
 ******************************************************************************/
int NEUIK_Button_SetText(
    NEUIK_Button * btn,
    const char   * text)
{
    RenderSize     rSize;
    RenderLoc      rLoc;
    size_t         sLen = 1;
    int            eNum = 0; /* which error to report (if any) */
    static char    funcName[] = "NEUIK_Button_SetText";
    static char  * errMsgs[] = {"",                         // [0] no error
        "Argument `btn` is not of Button class.",           // [1]
        "Failure to allocate memory.",                      // [2]
        "Failure in `neuik_Element_GetSizeAndLocation()`.", // [3]
    };

    if (!neuik_Object_IsClass(btn, neuik__Class_Button))
    {
        eNum = 1;
        goto out;
    }

    /*------------------------------------------------------------------------*/
    /* Check first if the button already contained the desired text.          */
    /*------------------------------------------------------------------------*/
    if (btn->text != NULL)
    {
        /*--------------------------------------------------------------------*/
        /* Button currently contains text; check if new text is the same.     */
        /*--------------------------------------------------------------------*/
        if (text != NULL)
        {
            if (!strcmp(btn->text, text))
            {
                /* no change in button text; return */
                goto out;
            }
        }
    }
    else
    {
        /*--------------------------------------------------------------------*/
        /* Button currently contains no text; check if new text is also empty */
        /*--------------------------------------------------------------------*/
        if (text == NULL)
        {
            /* no change in button text; return */
            goto out;
        }
        else if (text[0] == '\0')
        {
            /* no change in button text; return */
            goto out;
        }
    }

    /*------------------------------------------------------------------------*/
    /* Conditionally free button text before setting the new contents         */
    /*------------------------------------------------------------------------*/
    if (btn->text != NULL) {
        free(btn->text);
    }

    /*------------------------------------------------------------------------*/
    /* Set the new Button text contents                                       */
    /*------------------------------------------------------------------------*/
    if (text == NULL){
        /* button will contain no text */
        btn->text = NULL;
    }
    else if (text[0] == '\0')
    {
        /* button will contain no text */
        btn->text = NULL;
    }
    else
    {
        sLen += strlen(text);
        btn->text = (char*)malloc(sLen*sizeof(char));
        if (btn->text == NULL) {
            eNum = 2;
            goto out;
        }
        /* Allocation successful */
        strcpy(btn->text, text);
    }

    if (neuik_Element_GetSizeAndLocation(btn, &rSize, &rLoc))
    {
        eNum = 3;
        goto out;
    }
    neuik_Element_RequestRedraw(btn, rLoc, rSize);
out:
    if (eNum > 0)
    {
        NEUIK_RaiseError(funcName, errMsgs[eNum]);
        eNum = 1;
    }

    return eNum;
}

/*******************************************************************************
 *
 *  Name:          NEUIK_Button_GetText
 *
 *  Description:   Get a pointer to the text in a NEUIK_Button.
 *
 *  Returns:       NULL if there is a problem; otherwise a valid string
 *
 ******************************************************************************/
const char * NEUIK_Button_GetText(
    NEUIK_Button * btn)
{
    int           eNum       = 0; /* which error to report (if any) */
    const char  * rvPtr      = NULL;
    static char   emptyStr[] = "";
    static char   funcName[] = "NEUIK_Button_GetText";
    static char * errMsgs[]  = {"",               // [0] no error
        "Argument `btn` is not of Button class.", // [1]
    };

    if (!neuik_Object_IsClass(btn, neuik__Class_Button))
    {
        eNum = 1;
        goto out;
    }

    /*------------------------------------------------------------------------*/
    /* Set the new Button text contents                                       */
    /*------------------------------------------------------------------------*/
    if (btn->text == NULL){
        /* button will contain no text */
        rvPtr = emptyStr;
    }
    else
    {
        rvPtr = btn->text;
    }
out:
    if (eNum > 0)
    {
        NEUIK_RaiseError(funcName, errMsgs[eNum]);
        rvPtr = NULL;
    }

    return rvPtr;
}


void neuik_Button_Configure_capture_segv(
    int sig_num)
{
    static char funcName[] = "NEUIK_Button_Configure";
    static char errMsg[] = 
        "SIGSEGV (segmentation fault) captured; is call `NULL` terminated?";

    NEUIK_RaiseError(funcName, errMsg);
    NEUIK_BacktraceErrors();
    exit(1);
}

/*******************************************************************************
 *
 *  Name:          NEUIK_Button_Configure
 *
 *  Description:   Allows the user to set a number of configurable parameters.
 *
 *                 NOTE: This list of named sets must be terminated by a NULL 
 *                 pointer
 *
 *  Returns:       Non-zero if an error occurs.
 *
 ******************************************************************************/
int NEUIK_Button_Configure(
    NEUIK_Button  * btn,
    const char    * set0,
    ...)
{
    int                   ns; /* number of items from sscanf */
    int                   ctr;
    int                   nCtr;
    int                   eNum      = 0; /* which error to report (if any) */
    int                   doRedraw  = 0;
    int                   isBool;
    int                   boolVal   = 0;
    int                   typeMixup;
    int                   fontSize;
    char                  buf[4096];
    va_list               args;
    RenderSize            rSize     = {0, 0};
    RenderLoc             rLoc      = {0, 0};;
    char                * strPtr    = NULL;
    char                * name      = NULL;
    char                * value     = NULL;
    const char          * set       = NULL;
    NEUIK_Color           clr;
    NEUIK_ButtonConfig  * cfg       = NULL;
    /*------------------------------------------------------------------------*/
    /* If a `name=value` string with an unsupported name is found, check to   */
    /* see if a boolName was mistakenly used instead.                         */
    /*------------------------------------------------------------------------*/
    static char          * boolNames[] = {
        "FontBold",
        "FontItalic",
        NULL,
    };
    /*------------------------------------------------------------------------*/
    /* If a boolName string with an unsupported name is found, check to see   */
    /* if a supported nameValue type was mistakenly used instead.             */
    /*------------------------------------------------------------------------*/
    static char          * valueNames[] = {
        "FontSize"
        "FontColor",
        "FontColorSelect",
        NULL,
    };
    static char           funcName[] = "NEUIK_Button_Configure";
    static char         * errMsgs[] = {"",                                // [ 0] no error
        "Argument `btn` does not implement Button class.",                // [ 1]
        "`name=value` string is too long.",                               // [ 2]
        "Invalid `name=value` string.",                                   // [ 3]
        "ValueType name used as BoolType, skipping.",                     // [ 4]
        "BoolType name unknown, skipping.",                               // [ 5]
        "NamedSet.name is NULL, skipping..",                              // [ 6]
        "NamedSet.name is blank, skipping..",                             // [ 7]
        "FontColor value invalid; should be comma separated RGBA.",       // [ 8]
        "FontColor value invalid; RGBA value range is 0-255.",            // [ 9]
        "FontColorSelect value invalid; should be comma separated RGBA.", // [10]
        "FontColorSelect value invalid; RGBA value range is 0-255.",      // [11]
        "FontSize value is invalid; must be int.",                        // [12]
        "BoolType name used as ValueType, skipping.",                     // [13]
        "NamedSet.name type unknown, skipping.",                          // [14]
        "Failure in `neuik_Element_GetSizeAndLocation()`.",               // [15]
    };

    if (!neuik_Object_IsClass(btn, neuik__Class_Button))
    {
        eNum = 1;
        goto out;
    }
    set = set0;

    /*------------------------------------------------------------------------*/
    /* select the correct button config to use (pointer or internal)          */
    /*------------------------------------------------------------------------*/
    cfg = btn->cfg;
    if (btn->cfgPtr != NULL)
    {
        cfg = btn->cfgPtr;
    }

    va_start(args, set0);

    for (ctr = 0;; ctr++)
    {
        isBool = 0;
        name   = NULL;
        value  = NULL;

        if (set == NULL) break;

        #ifndef NO_NEUIK_SIGNAL_TRAPPING
            signal(SIGSEGV, neuik_Button_Configure_capture_segv);
        #endif

        if (strlen(set) > 4095)
        {
            #ifndef NO_NEUIK_SIGNAL_TRAPPING
                signal(SIGSEGV, NULL);
            #endif
            NEUIK_RaiseError(funcName, errMsgs[2]);
            set = va_arg(args, const char *);
            continue;
        }
        else
        {
            #ifndef NO_NEUIK_SIGNAL_TRAPPING
                signal(SIGSEGV, NULL);
            #endif
            strcpy(buf, set);
            /* Find the equals and set it to '\0' */
            strPtr = strchr(buf, '=');
            if (strPtr == NULL)
            {
                /*------------------------------------------------------------*/
                /* Bool type configuration (or a mistake)                     */
                /*------------------------------------------------------------*/
                if (buf[0] == 0)
                {
                    NEUIK_RaiseError(funcName, errMsgs[3]);
                }

                isBool  = 1;
                boolVal = 1;
                name    = buf;
                if (buf[0] == '!')
                {
                    boolVal = 0;
                    name    = buf + 1;
                }
            }
            else
            {
                *strPtr = 0;
                strPtr++;
                if (*strPtr == 0)
                {
                    /* `name=value` string is missing a value */
                    NEUIK_RaiseError(funcName, errMsgs[3]);
                    set = va_arg(args, const char *);
                    continue;
                }
                name  = buf;
                value = strPtr;
            }
        }

        if (isBool)
        {
            /*----------------------------------------------------------------*/
            /* Check for boolean parameter setting.                           */
            /*----------------------------------------------------------------*/
            if (!strcmp("FontBold", name))
            {
                if (cfg->fontBold == boolVal) continue;

                /* else: The previous setting was changed */
                cfg->fontBold = boolVal;
                doRedraw = 1;
            }
            else if (!strcmp("FontItalic", name))
            {
                if (cfg->fontItalic == boolVal) continue;

                /* else: The previous setting was changed */
                cfg->fontItalic = boolVal;
                doRedraw = 1;
            }
            else 
            {
                /*------------------------------------------------------------*/
                /* Bool parameter not found; may be mixup or mistake .        */
                /*------------------------------------------------------------*/
                typeMixup = 0;
                for (nCtr = 0;; nCtr++)
                {
                    if (valueNames[nCtr] == NULL) break;

                    if (!strcmp(valueNames[nCtr], name))
                    {
                        typeMixup = 1;
                        break;
                    }
                }

                if (typeMixup)
                {
                    /* A value type was mistakenly used as a bool type */
                    NEUIK_RaiseError(funcName, errMsgs[4]);
                }
                else
                {
                    /* An unsupported name was used as a bool type */
                    NEUIK_RaiseError(funcName, errMsgs[5]);
                }
            }
        }
        else
        {
            if (name == NULL)
            {
                NEUIK_RaiseError(funcName, errMsgs[6]);
            }
            else if (name[0] == 0)
            {
                NEUIK_RaiseError(funcName, errMsgs[7]);
            }
            else if (!strcmp("FontColor", name))
            {
                /*------------------------------------------------------------*/
                /* Check for empty value errors.                              */
                /*------------------------------------------------------------*/
                if (value == NULL)
                {
                    NEUIK_RaiseError(funcName, errMsgs[8]);
                    continue;
                }
                if (value[0] == '\0')
                {
                    NEUIK_RaiseError(funcName, errMsgs[8]);
                    continue;
                }

                ns = sscanf(value, "%d,%d,%d,%d", &clr.r, &clr.g, &clr.b, &clr.a);
                /*------------------------------------------------------------*/
                /* Check for EOF, incorrect # of values, & out of range vals. */
                /*------------------------------------------------------------*/
            #ifndef WIN32
                if (ns == EOF || ns < 4)
            #else
                if (ns < 4)
            #endif /* WIN32 */
                {
                    NEUIK_RaiseError(funcName, errMsgs[8]);
                    continue;
                }

                if (clr.r < 0 || clr.r > 255 ||
                    clr.g < 0 || clr.g > 255 ||
                    clr.b < 0 || clr.b > 255 ||
                    clr.a < 0 || clr.a > 255)
                {
                    NEUIK_RaiseError(funcName, errMsgs[9]);
                    continue;
                }

                cfg->fgColor = clr;
                doRedraw = 1;
            }
            else if (!strcmp("FontColorSelect", name))
            {
                /*------------------------------------------------------------*/
                /* Check for empty value errors.                              */
                /*------------------------------------------------------------*/
                if (value == NULL)
                {
                    NEUIK_RaiseError(funcName, errMsgs[10]);
                    continue;
                }
                if (value[0] == '\0')
                {
                    NEUIK_RaiseError(funcName, errMsgs[10]);
                    continue;
                }

                ns = sscanf(value, "%d,%d,%d,%d", &clr.r, &clr.g, &clr.b, &clr.a);
                /*------------------------------------------------------------*/
                /* Check for EOF, incorrect # of values, & out of range vals. */
                /*------------------------------------------------------------*/
            #ifndef WIN32
                if (ns == EOF || ns < 4)
            #else
                if (ns < 4)
            #endif /* WIN32 */
                {
                    NEUIK_RaiseError(funcName, errMsgs[10]);
                    continue;
                }

                if (clr.r < 0 || clr.r > 255 ||
                    clr.g < 0 || clr.g > 255 ||
                    clr.b < 0 || clr.b > 255 ||
                    clr.a < 0 || clr.a > 255)
                {
                    NEUIK_RaiseError(funcName, errMsgs[11]);
                    continue;
                }

                cfg->fgColorSelect = clr;
                doRedraw = 1;
            }
            else if (!strcmp("FontSize", name))
            {
                /* Set autoResize parameters for both width and height */

                ns = sscanf(value, "%d", &fontSize);
                /*------------------------------------------------------------*/
                /* Check for EOF, incorrect # of values, & out of range vals. */
                /*------------------------------------------------------------*/
            #ifndef WIN32
                if (ns == EOF || ns < 1)
            #else
                if (ns < 1)
            #endif /* WIN32 */
                {
                    NEUIK_RaiseError(funcName, errMsgs[12]);
                    continue;
                }
                cfg->fontSize = fontSize;
                doRedraw = 1;
            }
            else
            {
                typeMixup = 0;
                for (nCtr = 0;; nCtr++)
                {
                    if (boolNames[nCtr] == NULL) break;

                    if (!strcmp(boolNames[nCtr], name))
                    {
                        typeMixup = 1;
                        break;
                    }
                }

                if (typeMixup)
                {
                    /* A bool type was mistakenly used as a value type */
                    NEUIK_RaiseError(funcName, errMsgs[13]);
                }
                else
                {
                    /* An unsupported name was used as a value type */
                    NEUIK_RaiseError(funcName, errMsgs[14]);
                }
            }
        }

        /* before starting */
        set = va_arg(args, const char *);
    }
    va_end(args);
out:
    if (eNum > 0)
    {
        NEUIK_RaiseError(funcName, errMsgs[eNum]);
        eNum = 1;
    }
    if (doRedraw) 
    {
        if (neuik_Element_GetSizeAndLocation(btn, &rSize, &rLoc))
        {
            eNum = 15;
            NEUIK_RaiseError(funcName, errMsgs[eNum]);
            eNum = 1;
        }
        else
        {
            neuik_Element_RequestRedraw(btn, rLoc, rSize);
        }
    }

    return eNum;
}

/*******************************************************************************
 *
 *  Name:          neuik_Element_Render__Button
 *
 *  Description:   Renders a single button as an SDL_Texture*.
 *
 *  Returns:       NULL if there is a problem, otherwise a valid SDL_Texture*.
 *
 ******************************************************************************/
int neuik_Element_Render__Button(
    NEUIK_Element   elem,
    RenderSize    * rSize, /* in/out the size the tex occupies when complete */
    RenderLoc     * rlMod, /* A relative location modifier (for rendering) */
    SDL_Renderer  * xRend, /* the external renderer to prepare the texture for */
    int             mock)  /* If true; calculate sizes/locations but don't draw */
{
    int                   ctr     = 0;
    int                   eNum    = 0; /* which error to report (if any) */
    int                   textW   = 0;
    int                   textH   = 0;
    int                   borderW = 1; /* width of button border line */
    SDL_Rect              rect;
    SDL_Renderer        * rend    = NULL;
    SDL_Texture         * tTex    = NULL; /* text texture */
    TTF_Font            * font    = NULL;
    const NEUIK_Color   * fgClr   = NULL;
    const NEUIK_Color   * bClr    = NULL; /* border color */
    NEUIK_ButtonConfig  * aCfg    = NULL; /* the active button config */
    NEUIK_Button        * btn     = NULL;
    NEUIK_ElementBase   * eBase   = NULL;
    neuik_MaskMap       * maskMap = NULL;
    RenderLoc             rl;
    RenderLoc             rlAdj;             /* loc. including adjustments */
    static char           funcName[] = "neuik_Element_Render__Button";
    static char         * errMsgs[] = {"", // [ 0] no error
        "Argument `elem` is not of Button class.",                       // [1]
        "Failure in `neuik_MakeMaskMap()`",                              // [2]
        "FontSet_GetFont returned NULL.",                                // [3]
        "", // [4]
        "RenderText returned NULL.",                                     // [5]
        "Invalid specified `rSize` (negative values).",                  // [6]
        "Argument `elem` caused `neuik_Object_GetClassObject` to fail.", // [7]
        "Failure in `neuik_Element_RedrawBackground()`.",                // [8]
    };

    if (!neuik_Object_IsClass(elem, neuik__Class_Button))
    {
        eNum = 1;
        goto out;
    }
    btn = (NEUIK_Button*)elem;

    if (neuik_Object_GetClassObject(btn, neuik__Class_Element, (void**)&eBase))
    {
        eNum = 7;
        goto out;
    }

    if (rSize->w < 0 || rSize->h < 0)
    {
        eNum = 6;
        goto out;
    }
    if (mock)
    {
        /*--------------------------------------------------------------------*/
        /* This is a mock render operation; don't draw anything...            */
        /*--------------------------------------------------------------------*/
        goto out;
    }

    eBase->eSt.rend = xRend;
    rend = eBase->eSt.rend;

    if (neuik__HighDPI_Scaling >= 2.0)
    {
        borderW = 2*(int)(neuik__HighDPI_Scaling/2.0);
    }

    /*------------------------------------------------------------------------*/
    /* select the correct button config to use (pointer or internal)          */
    /*------------------------------------------------------------------------*/
    aCfg = btn->cfg;
    if (btn->cfgPtr != NULL)
    {
        aCfg = btn->cfgPtr;
    }

    /*------------------------------------------------------------------------*/
    /* Select the correct foreground color                                    */
    /*------------------------------------------------------------------------*/
    fgClr = &(aCfg->fgColor); /* use the unselected colors */
    if (btn->selected)
    {
        /* use the selected colors */
        fgClr = &(aCfg->fgColorSelect);
    }

    rl = eBase->eSt.rLoc;
    rlAdj = rl;
    if (rlMod != NULL)
    {
        rlAdj.x += rlMod->x;
        rlAdj.y += rlMod->y;
    }
    rl = rlAdj;

    /*------------------------------------------------------------------------*/
    /* Create a MaskMap an mark off the trasnparent pixels.                   */
    /*------------------------------------------------------------------------*/
    if (neuik_MakeMaskMap(&maskMap, rSize->w, rSize->h))
    {
        eNum = 2;
        goto out;
    }

    /*------------------------------------------------------------------------*/
    /* Mark off the rounded sections of the button within the MaskMap.        */
    /*------------------------------------------------------------------------*/
    /* Apply transparent pixels to (round off) the upper-left corner */
    neuik_MaskMap_MaskPoint(maskMap, 0, 0);
    neuik_MaskMap_MaskPoint(maskMap, 0, 1);
    neuik_MaskMap_MaskPoint(maskMap, 1, 0);

    /* Apply transparent pixels to (round off) the lower-left corner */
    neuik_MaskMap_MaskPoint(maskMap, 0, rSize->h - 1);
    neuik_MaskMap_MaskPoint(maskMap, 0, rSize->h - 2);
    neuik_MaskMap_MaskPoint(maskMap, 1, rSize->h - 1);

    /* Apply transparent pixels to (round off) the upper-right corner */
    neuik_MaskMap_MaskPoint(maskMap, rSize->w - 1, 0);
    neuik_MaskMap_MaskPoint(maskMap, rSize->w - 1, 1);
    neuik_MaskMap_MaskPoint(maskMap, rSize->w - 2, 0);

    /* Apply transparent pixels to (round off) the lower-right corner */
    neuik_MaskMap_MaskPoint(maskMap, rSize->w - 1, rSize->h - 1);
    neuik_MaskMap_MaskPoint(maskMap, rSize->w - 1, rSize->h - 2);
    neuik_MaskMap_MaskPoint(maskMap, rSize->w - 2, rSize->h - 1);

    /*------------------------------------------------------------------------*/
    /* Redraw the background surface before continuing.                       */
    /*------------------------------------------------------------------------*/
    if (neuik_Element_RedrawBackground(elem, rlMod, maskMap))
    {
        eNum = 8;
        goto out;
    }

    /*------------------------------------------------------------------------*/
    /* Draw the border around the button.                                     */
    /*------------------------------------------------------------------------*/
    bClr = &(aCfg->borderColor);
    SDL_SetRenderDrawColor(rend, bClr->r, bClr->g, bClr->b, 255);

    /* Draw upper-left corner border pixels */
    for (ctr = 0; ctr < borderW; ctr++)
    {
        SDL_RenderDrawPoint(rend, rl.x + 1 + ctr, rl.y + 1 + ctr);
        SDL_RenderDrawPoint(rend, rl.x + 1 + ctr, rl.y + 2 + ctr);
        SDL_RenderDrawPoint(rend, rl.x + 2 + ctr, rl.y + 1 + ctr);
    }

    /* Draw lower-left corner border pixels */
    for (ctr = 0; ctr < borderW; ctr++)
    {
        SDL_RenderDrawPoint(rend, rl.x + 1 + ctr, rl.y + (rSize->h - 2) - ctr);
        SDL_RenderDrawPoint(rend, rl.x + 1 + ctr, rl.y + (rSize->h - 3) - ctr);
        SDL_RenderDrawPoint(rend, rl.x + 2 + ctr, rl.y + (rSize->h - 2) - ctr);
    }

    /* Draw upper-right corner border pixels */
    for (ctr = 0; ctr < borderW; ctr++)
    {
        SDL_RenderDrawPoint(rend, rl.x + (rSize->w - 2) - ctr, rl.y + 1 + ctr);
        SDL_RenderDrawPoint(rend, rl.x + (rSize->w - 2) - ctr, rl.y + 2 + ctr);
        SDL_RenderDrawPoint(rend, rl.x + (rSize->w - 3) - ctr, rl.y + 1 + ctr);
    }

    /* Draw lower-right corner border pixels */
    for (ctr = 0; ctr < borderW; ctr++)
    {
        SDL_RenderDrawPoint(rend, 
            rl.x + (rSize->w - 2) - ctr, 
            rl.y + (rSize->h - 2) - ctr);
        SDL_RenderDrawPoint(rend, 
            rl.x + (rSize->w - 2) - ctr, 
            rl.y + (rSize->h - 3) - ctr);
        SDL_RenderDrawPoint(rend, 
            rl.x + (rSize->w - 3) - ctr, 
            rl.y + (rSize->h - 2) - ctr);
    }

    /* upper border line */
    for (ctr = 0; ctr < borderW; ctr++)
    {
        SDL_RenderDrawLine(rend, 
            rl.x + 2,              rl.y + ctr, 
            rl.x + (rSize->w - 3), rl.y + ctr); 
    }
    /* left border line */
    for (ctr = 0; ctr < borderW; ctr++)
    {
        SDL_RenderDrawLine(rend, 
            rl.x + ctr, rl.y + 2, 
            rl.x + ctr, rl.y + (rSize->h - 3));
    }
    /* right border line */
    for (ctr = 0; ctr < borderW; ctr++)
    {
        SDL_RenderDrawLine(rend, 
            rl.x + (rSize->w - 1) - ctr, rl.y + 2, 
            rl.x + (rSize->w - 1) - ctr, rl.y + (rSize->h - 3));
    }

    /* lower border line */
    bClr = &(aCfg->borderColorDark);
    SDL_SetRenderDrawColor(rend, bClr->r, bClr->g, bClr->b, 255);
    for (ctr = 0; ctr < borderW; ctr++)
    {
        SDL_RenderDrawLine(rend, 
            rl.x + 2 + ctr,              rl.y + (rSize->h - 1) - ctr, 
            rl.x + (rSize->w - 3) - ctr, rl.y + (rSize->h - 1) - ctr);
    }

    /*------------------------------------------------------------------------*/
    /* Render the button text                                                 */
    /*------------------------------------------------------------------------*/
    if (btn->text != NULL)
    {
        font = NEUIK_FontSet_GetFont(aCfg->fontSet, aCfg->fontSize, 
            aCfg->fontBold, aCfg->fontItalic);
        if (font == NULL) 
        {
            eNum = 3;
            goto out;

        }

        tTex = NEUIK_RenderText(btn->text, font, *fgClr, rend, &textW, &textH);
        if (tTex == NULL)
        {
            eNum = 5;
            goto out;
        }

        rect.x = rl.x;
        rect.y = rl.y;
        rect.w = textW;
        rect.h = textH;

        switch (eBase->eCfg.HJustify)
        {
            case NEUIK_HJUSTIFY_LEFT:
                rect.x += 6;
                rect.y += (int) ((float)(rSize->h - textH)/2.0);
                break;

            case NEUIK_HJUSTIFY_CENTER:
            case NEUIK_HJUSTIFY_DEFAULT:
                rect.x += (int) ((float)(rSize->w - textW)/2.0);
                rect.y += (int) ((float)(rSize->h - textH)/2.0);
                break;

            case NEUIK_HJUSTIFY_RIGHT:
                rect.x += (int) (rSize->w - textW - 6);
                rect.y += (int) ((float)(rSize->h - textH)/2.0);
                break;
        }

        SDL_RenderCopy(rend, tTex, NULL, &rect);
    }
out:
    if (eBase != NULL)
    {
        if (!mock) eBase->eSt.doRedraw = 0;
    }

    ConditionallyDestroyTexture(&tTex);
    if (maskMap != NULL) neuik_Object_Free(maskMap);

    if (eNum > 0)
    {
        NEUIK_RaiseError(funcName, errMsgs[eNum]);
        eNum = 1;
    }

    return eNum;
}


/*******************************************************************************
 *
 *  Name:          neuik_Element_CaptureEvent__Button
 *
 *  Description:   Check to see if this event is captured by a NEUIK_Button.
 *
 *  Returns:       1 if event is captured; 0 otherwise
 *
 ******************************************************************************/
neuik_EventState neuik_Element_CaptureEvent__Button(
    NEUIK_Element   elem,
    SDL_Event     * ev)
{
    neuik_EventState       evCaputred   = NEUIK_EVENTSTATE_NOT_CAPTURED;
    RenderSize             rSize;
    RenderLoc              rLoc;
    SDL_Event            * e;
    NEUIK_Button         * btn          = NULL;
    NEUIK_ElementBase    * eBase        = NULL;
    SDL_MouseMotionEvent * mouseMotEv;
    SDL_MouseButtonEvent * mouseButEv;

    if (neuik_Object_GetClassObject(elem, neuik__Class_Element, (void**)&eBase))
    {
        /* not the right type of object */
        goto out;
    }
    btn = (NEUIK_Button*)elem;
    /*------------------------------------------------------------------------*/
    /* Check if the event is captured by the menu (mouseclick/mousemotion).   */
    /*------------------------------------------------------------------------*/
    e = (SDL_Event*)ev;
    switch (e->type)
    {
    case SDL_MOUSEBUTTONDOWN:
        mouseButEv = (SDL_MouseButtonEvent*)(e);
        
        if (mouseButEv->y >= eBase->eSt.rLoc.y &&
            mouseButEv->y <= eBase->eSt.rLoc.y + eBase->eSt.rSize.h)
        {
            if (mouseButEv->x >= eBase->eSt.rLoc.x &&
                mouseButEv->x <= eBase->eSt.rLoc.x + eBase->eSt.rSize.w)
            {
                /* This mouse click originated within this button */
                btn->clickOrigin      = 1;
                eBase->eSt.focusstate = NEUIK_FOCUSSTATE_SELECTED;
                btn->selected         = 1;
                btn->wasSelected      = 1;
                evCaputred            = NEUIK_EVENTSTATE_CAPTURED;
                neuik_Window_TakeFocus(eBase->eSt.window, (NEUIK_Element)btn);
                neuik_Element_TriggerCallback(btn, NEUIK_CALLBACK_ON_CLICK);
                if (!neuik_Object_IsNEUIKObject_NoError(btn))
                {
                    /* The object was freed/corrupted by the callback */
                    evCaputred = NEUIK_EVENTSTATE_OBJECT_FREED;
                    goto out;
                }
                rSize = eBase->eSt.rSize;
                rLoc  = eBase->eSt.rLoc;
                neuik_Element_RequestRedraw(btn, rLoc, rSize);
                goto out;
            }
        }
        break;
    case SDL_MOUSEBUTTONUP:
        mouseButEv = (SDL_MouseButtonEvent*)(e);
        if (btn->clickOrigin)
        {
            if (mouseButEv->y >= eBase->eSt.rLoc.y &&
                mouseButEv->y <= eBase->eSt.rLoc.y + eBase->eSt.rSize.h)
            {
                if (mouseButEv->x >= eBase->eSt.rLoc.x &&
                    mouseButEv->x <= eBase->eSt.rLoc.x + eBase->eSt.rSize.w)
                {
                    /* cursor is still within the button, activate cbFunc */
                    neuik_Element_TriggerCallback(btn, NEUIK_CALLBACK_ON_CLICKED);
                    if (!neuik_Object_IsNEUIKObject_NoError(btn))
                    {
                        /* The object was freed/corrupted by the callback */
                        evCaputred = NEUIK_EVENTSTATE_OBJECT_FREED;
                        goto out;
                    }
                }
            }
            eBase->eSt.focusstate = NEUIK_FOCUSSTATE_NORMAL;
            btn->selected         = 0;
            btn->wasSelected      = 0;
            btn->clickOrigin      = 0;
            evCaputred            = NEUIK_EVENTSTATE_CAPTURED;

            rSize = eBase->eSt.rSize;
            rLoc  = eBase->eSt.rLoc;
            neuik_Element_RequestRedraw(btn, rLoc, rSize);
            goto out;
        }
        break;

    case SDL_MOUSEMOTION:
        mouseMotEv = (SDL_MouseMotionEvent*)(e);

        if (btn->clickOrigin)
        {
            /*----------------------------------------------------------------*/
            /* The mouse was initially clicked within the button. If the user */
            /* moves the cursor out of the button area, deselect it.          */
            /*----------------------------------------------------------------*/
            eBase->eSt.focusstate = NEUIK_FOCUSSTATE_NORMAL;
            btn->selected         = 0;
            if (mouseMotEv->y >= eBase->eSt.rLoc.y &&
                mouseMotEv->y <= eBase->eSt.rLoc.y + eBase->eSt.rSize.h)
            {
                if (mouseMotEv->x >= eBase->eSt.rLoc.x &&
                    mouseMotEv->x <= eBase->eSt.rLoc.x + eBase->eSt.rSize.w)
                {
                    eBase->eSt.focusstate = NEUIK_FOCUSSTATE_SELECTED;
                    btn->selected         = 1;
                }
            }

            if (btn->wasSelected != btn->selected)
            {
                rSize = eBase->eSt.rSize;
                rLoc  = eBase->eSt.rLoc;
                neuik_Element_RequestRedraw(btn, rLoc, rSize);
            }
            btn->wasSelected = btn->selected;
            evCaputred = NEUIK_EVENTSTATE_CAPTURED;
            goto out;
        }

        break;
    }
out:
    return evCaputred;
}
