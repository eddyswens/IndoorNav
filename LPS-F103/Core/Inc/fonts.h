#ifndef FONTS_H
#define FONTS_H 120


/**
 *
 * Default fonts library. It is used in all LCD based libraries.
 *
 * \par Supported fonts
 *
 * Currently, these fonts are supported:
 *  - 7 x 10 pixels
 *  - 11 x 18 pixels
 *  - 16 x 26 pixels

 */
#include "string.h"
#include "stdint.h"

typedef struct {
    wchar_t character;
    int dataIndex;
} CharIndex;

typedef struct {
    uint32_t bitmap_index;          /**< Start index of the bitmap. A font can be max 4 GB.*/
    uint32_t adv_w;                 /**< Draw the next glyph after this width. 28.4 format (real_value * 16 is stored).*/
    uint16_t box_w;                 /**< Width of the glyph's bounding box*/
    uint16_t box_h;                 /**< Height of the glyph's bounding box*/
    int16_t ofs_x;                  /**< x offset of the bounding box*/
    int16_t ofs_y;                  /**< y offset of the bounding box. Measured from the top of the line*/
} font_info;

typedef struct {
    const uint8_t *glyph_bitmap; // Указатель на растровые данные шрифта
    const font_info *glyph_dsc;  // Указатель на массив описаний глифов
    uint8_t size;
} font_t;


extern CharIndex charIndexArray[];
/**
 *
 *
 * @defgroup FONTS_FontVariables
 * @brief    Library font variables
 * @{
 */
extern const uint8_t Font_bitmap[];
extern const font_info Font_dsc[];
extern const font_t Font;

/**
 * @brief  Calculates string length and height in units of pixels depending on string and font used
 * @param  *str: String to be checked for length and height
 * @param  *SizeStruct: Pointer to empty @ref FONTS_SIZE_t structure where informations will be saved
 * @param  *Font: Pointer to @ref FontDef_t font used for calculations
 * @retval Pointer to string used for length and height
 */
uint8_t getFontDataIndex(wchar_t ch, const CharIndex* charIndexArray);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/* C++ detection */


#endif
