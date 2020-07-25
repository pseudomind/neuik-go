/*******************************************************************************
 * Copyright (c) 2014-2017, Michael Leimon <leimon@gmail.com>
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
#ifndef NEUIK_HGROUP_H
#define NEUIK_HGROUP_H

// #include "NEUIK_Event.h"
#include "NEUIK_Element.h"


typedef struct {
		neuik_Object objBase;   /* this structure is requied to be an neuik object */
		int          HSpacing;
		int          selected;
		int          isActive;
} NEUIK_HGroup;

int 
	NEUIK_NewHGroup(
			NEUIK_HGroup **);

int 
	NEUIK_HGroup_SetHSpacing(
			NEUIK_HGroup * hg,
			int            spacing);

void 
	NEUIK_HGroup_Deselect(
			NEUIK_HGroup  * hg);


#endif /* NEUIK_HGROUP_H */
