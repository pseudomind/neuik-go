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
#ifndef NEUIK_TEXTEDIT_CONFIG_H
#define NEUIK_TEXTEDIT_CONFIG_H

#include "NEUIK_structs_basic.h"
#include "NEUIK_FontSet.h"
#include "neuik_internal.h"


typedef struct {
		neuik_Object    objBase;           /* this structure is requied to be an neuik object */
		NEUIK_FontSet * fontSet;           /* NEUIK_FontSet */
		NEUIK_FontSet * fontSetMS;         /* NEUIK_FontSet (monospaced) */
		int             fontSize;          /* point size to use for the TTF_Font */
		int             fontBold;          /* (bool) use bold style */
		int             fontItalic;        /* (bool) use italic style */
		int             fontMono;          /* (bool) use monospaced FontSet */
		char          * fontName;          /* font name for the TTF_Font */
		char          * fontNameMS;        /* font name for the TTF_Font (monospaced) */
		NEUIK_Color     bgColor;           /* background color used when not selected */
		NEUIK_Color     fgColor;           /* foreground color used when not selected */
		NEUIK_Color     bgColorHl;         /* background color used when highlighted */
		NEUIK_Color     fgColorHl;         /* foreground color used when highlighted */
		NEUIK_Color     bgColorSelect;     /* border color used when selected */
		NEUIK_Color     borderColor;       /* color used for the textEdit border */
		NEUIK_Color     borderColorDark;   /* color used for the textEdit border */
		NEUIK_Color     bgScrollColor;     /* background color for the textEdit scrollbar */
		NEUIK_Color     scrollSliderColor; /* color used for the textEdit scrollbar slider */
		int             textVJustify;      /* vertical justification of text */
		int             textHJustify;      /* horizontal justification of text */
		int             fontEmWidth;       /* nuber of pixels required for an `M` of this font */
		int             restriction;       /* limit what can be put into the text */
		char          * restrict_str;      /* used for custom text restrictions */
		int             emptySpaces;       /* used to size the empty textEdit */
} NEUIK_TextEditConfig;

NEUIK_TextEditConfig * NEUIK_GetDefaultTextEditConfig();

int 
	NEUIK_NewTextEditConfig(
			NEUIK_TextEditConfig ** cfgPtr);
int 
	NEUIK_TextEditConfig_Copy(
			NEUIK_TextEditConfig       * dst,
			const NEUIK_TextEditConfig * src);


#endif /* NEUIK_TEXTEDIT_CONFIG_H */
