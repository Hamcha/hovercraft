/*! \file font.h
 *  \brief Bitmap font management and drawing
 */

#ifndef _FONT_H
#define _FONT_H

#include <ogcsys.h>
#include "gxutils.h"

typedef struct {
	f32 uvTop[2], uvBottom[2];
} charuv_t;

typedef struct {
	GXTexObj* texture;
	u32       charHeight, charWidth;
	u32       columns;
	u8        charIndex[128];
	charuv_t* charUV;
} font_t;

/*! \brief Load a font from texture and data
 *  \param texture    Font texture
 *  \param chars      Character order (required for UV generation)
 *  \param charWidth  Width of each character
 *  \param charHeight Height of each character
 *  \param columns    Number of characters per row
 *  \param texSize    Texture size
 */
font_t* FONT_load(GXTexObj* texture, const char* chars, const u32 charWidth, const u32 charHeight, const u32 columns, const TexSize texSize);

/*! \brief Frees font data 
 *  \param font Font structure to destroy
 */
void FONT_free(font_t* font);

#endif