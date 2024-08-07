
#include "fonts.h"

const size_t CHAR_INDEX_ARRAY_SIZE = 161;

const font_t Font = {
    .glyph_bitmap = Font_bitmap,
    .glyph_dsc = Font_dsc,
	.size = 14
};


//Используемый алфавит: !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя

CharIndex charIndexArray[] = { //1 - для букв сверху, 0 - стандартная высота
		{32, 0}, // Space
		    {33, 1}, // !
		    {34, 2}, // "
		    {35, 3}, // #
		    {36, 4}, // $
		    {37, 5}, // %
		    {38, 6}, // &
		    {39, 7}, // '
		    {40, 8}, // (
		    {41, 9}, // )
		    {42, 10}, // *
		    {43, 11}, // +
		    {44, 12}, // ,
		    {45, 13}, // -
		    {46, 14}, // .
		    {47, 15}, // /
		    {48, 16}, // 0
		    {49, 17}, // 1
		    {50, 18}, // 2
		    {51, 19}, // 3
		    {52, 20}, // 4
		    {53, 21}, // 5
		    {54, 22}, // 6
		    {55, 23}, // 7
		    {56, 24}, // 8
		    {57, 25}, // 9
		    {58, 26}, // :
		    {59, 27}, // ;
		    {60, 28}, // <
		    {61, 29}, // =
		    {62, 30}, // >
		    {63, 31}, // ?
		    {64, 32}, // @
		    {65, 33}, // A
		    {66, 34}, // B
		    {67, 35}, // C
		    {68, 36}, // D
		    {69, 37}, // E
		    {70, 38}, // F
		    {71, 39}, // G
		    {72, 40}, // H
		    {73, 41}, // I
		    {74, 42}, // J
		    {75, 43}, // K
		    {76, 44}, // L
		    {77, 45}, // M
		    {78, 46}, // N
		    {79, 47}, // O
		    {80, 48}, // P
		    {81, 49}, // Q
		    {82, 50}, // R
		    {83, 51}, // S
		    {84, 52}, // T
		    {85, 53}, // U
		    {86, 54}, // V
		    {87, 55}, // W
		    {88, 56}, // X
		    {89, 57}, // Y
		    {90, 58}, // Z
		    {91, 59}, // [
		    {92, 60}, // backslash
		    {93, 61}, // ]
		    {94, 62}, // ^
		    {95, 63}, // _ underscore
		    {96, 64}, // ` backtick
		    {97, 65}, // a
		    {98, 66}, // b
		    {99, 67}, // c
		    {100, 68}, // d
		    {101, 69}, // e
		    {102, 70}, // f
		    {103, 71}, // g
		    {104, 72}, // h
		    {105, 73}, // i
		    {106, 74}, // j
		    {107, 75}, // k
		    {108, 76}, // l
		    {109, 77}, // m
		    {110, 78}, // n
		    {111, 79}, // o
		    {112, 80}, // p
		    {113, 81}, // q
		    {114, 82}, // r
		    {115, 83}, // s
		    {116, 84}, // t
		    {117, 85}, // u
		    {118, 86}, // v
		    {119, 87}, // w
		    {120, 88}, // x
		    {121, 89}, // y
		    {122, 90}, // z
		    {123, 91}, // {
		    {124, 92}, // |
		    {125, 93}, // }
		    {126, 94}, // ~ Tilde
			{1025, 95},  // Ё
			{1040, 96},  // А
			{1041, 97},  // Б
			{1042, 98},  // В
			{1043, 99},  // Г
			{1044, 100}, // Д
			{1045, 101}, // Е
			{1046, 102}, // Ж
			{1047, 103}, // З
			{1048, 104}, // И
			{1049, 105}, // Й
			{1050, 106}, // К
			{1051, 107}, // Л
			{1052, 108}, // М
			{1053, 109}, // Н
			{1054, 110}, // О
			{1055, 111}, // П
			{1056, 112}, // Р
			{1057, 113}, // С
			{1058, 114}, // Т
			{1059, 115}, // У
			{1060, 116}, // Ф
			{1061, 117}, // Х
			{1062, 118}, // Ц
			{1063, 119}, // Ч
			{1064, 120}, // Ш
			{1065, 121}, // Щ
			{1066, 122}, // Ъ
			{1067, 123}, // Ы
			{1068, 124}, // Ь
			{1069, 125}, // Э
			{1070, 126}, // Ю
			{1071, 127}, // Я
			{1072, 128}, // а
			{1073, 129}, // б
			{1074, 130}, // в
			{1075, 131}, // г
			{1076, 132}, // д
			{1077, 133}, // е
			{1078, 134}, // ж
			{1079, 135}, // з
			{1080, 136}, // и
			{1081, 137}, // й
			{1082, 138}, // к
			{1083, 139}, // л
			{1084, 140}, // м
			{1085, 141}, // н
			{1086, 142}, // о
			{1087, 143}, // п
			{1088, 144}, // р
			{1089, 145}, // с
			{1090, 146}, // т
			{1091, 147}, // у
			{1092, 148}, // ф
			{1093, 149}, // х
			{1094, 150}, // ц
			{1095, 151}, // ч
			{1096, 152}, // ш
			{1097, 153}, // щ
			{1098, 154}, // ъ
			{1099, 155}, // ы
			{1100, 156}, // ь
			{1101, 157}, // э
			{1102, 158}, // ю
			{1103, 159}, // я
			{1105, 160}, // ё

};


inline uint8_t getFontDataIndex(wchar_t ch, const CharIndex* charIndexArray) {
    int16_t left = 0;
    int16_t right = CHAR_INDEX_ARRAY_SIZE - 1;

    while (left <= right) {
        int middle = (left + right) / 2;
        if (charIndexArray[middle].character == ch) {

           return charIndexArray[middle].dataIndex;

        } else if (charIndexArray[middle].character < ch) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }
    return 0; //возвращаем пробел, если символ не найден
}

const uint8_t Font_bitmap[] = {
  /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xf0, 0xc0,

    /* U+0022 "\"" */
    0xde, 0xf7, 0x20,

    /* U+0023 "#" */
    0xd, 0x86, 0xc6, 0xcf, 0xf3, 0x67, 0xf9, 0xb0,
    0x90, 0xd8, 0x0,

    /* U+0024 "$" */
    0x18, 0xc, 0x1f, 0xfb, 0xd, 0x86, 0xc1, 0xfc,
    0x33, 0x19, 0x8c, 0xff, 0xc3, 0x0,

    /* U+0025 "%" */
    0xf1, 0xa4, 0x4f, 0x20, 0x18, 0xc, 0x6, 0xf1,
    0x24, 0x89, 0x63, 0xc0,

    /* U+0026 "&" */
    0x7f, 0xb0, 0xc, 0x3, 0x0, 0x3f, 0xf0, 0x6c,
    0x1b, 0x6, 0x7f, 0x0,

    /* U+0027 "'" */
    0xfe,

    /* U+0028 "(" */
    0x7c, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x70,

    /* U+0029 ")" */
    0xe3, 0x33, 0x33, 0x33, 0x33, 0x33, 0xe0,

    /* U+002A "*" */
    0x0, 0x3c, 0x5a, 0x3c, 0x3c, 0x24,

    /* U+002B "+" */
    0x20, 0x8f, 0xc8, 0x20,

    /* U+002C "," */
    0xfe,

    /* U+002D "-" */
    0xf8,

    /* U+002E "." */
    0xc0,

    /* U+002F "/" */
    0x1, 0x81, 0x81, 0x80, 0x80, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0x0,

    /* U+0030 "0" */
    0x7f, 0xb0, 0xc, 0xf, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0x7f, 0x80,

    /* U+0031 "1" */
    0x31, 0xe2, 0xc1, 0x83, 0x6, 0xc, 0x18, 0xfe,

    /* U+0032 "2" */
    0x7f, 0xe0, 0xf0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xe0, 0xff, 0x80,

    /* U+0033 "3" */
    0xff, 0x80, 0xc0, 0x60, 0x60, 0xf0, 0xf, 0x7,
    0x83, 0x7f, 0x0,

    /* U+0034 "4" */
    0x6, 0x3, 0x1, 0x80, 0xc6, 0x61, 0x9f, 0xf0,
    0x18, 0x6, 0x1, 0x80,

    /* U+0035 "5" */
    0xff, 0xe0, 0x30, 0x18, 0xf, 0xf0, 0xc, 0x6,
    0x3, 0xff, 0x0,

    /* U+0036 "6" */
    0x7f, 0xe0, 0x30, 0x18, 0xf, 0xf6, 0xf, 0x7,
    0x83, 0x7f, 0x0,

    /* U+0037 "7" */
    0xff, 0x0, 0xc0, 0x40, 0x60, 0x60, 0x30, 0x30,
    0x30, 0x18, 0x0,

    /* U+0038 "8" */
    0x7f, 0xb0, 0x3c, 0xf, 0x3, 0x3f, 0x30, 0x3c,
    0xf, 0x3, 0x7f, 0x80,

    /* U+0039 "9" */
    0x7f, 0x60, 0xf0, 0x78, 0x37, 0xf8, 0xc, 0x6,
    0x3, 0xff, 0x0,

    /* U+003A ":" */
    0xc0, 0xc0,

    /* U+003B ";" */
    0xc3, 0xf8,

    /* U+003C "<" */
    0x1c, 0x63, 0x18, 0xc1, 0x83, 0x6, 0x1c,

    /* U+003D "=" */
    0xff, 0x0, 0x0, 0xff,

    /* U+003E ">" */
    0x61, 0x83, 0x6, 0xc, 0x63, 0x18, 0x60,

    /* U+003F "?" */
    0x7f, 0xb0, 0x7c, 0x18, 0xc, 0x6, 0x3, 0x80,
    0xc0, 0x0, 0xc, 0x0,

    /* U+0040 "@" */
    0xff, 0xc0, 0x6f, 0xb7, 0xda, 0x2d, 0xde, 0x1,
    0xfc,

    /* U+0041 "A" */
    0x1e, 0x1, 0xe0, 0x6c, 0xc, 0xc3, 0x18, 0x61,
    0x9f, 0xf3, 0x3, 0xc0, 0x60,

    /* U+0042 "B" */
    0xff, 0x98, 0x36, 0xd, 0x83, 0x7f, 0x18, 0x36,
    0xd, 0x83, 0x7f, 0x80,

    /* U+0043 "C" */
    0x7f, 0x60, 0xf0, 0x78, 0xc, 0x6, 0x3, 0x1,
    0x80, 0x7f, 0x80,

    /* U+0044 "D" */
    0xff, 0x0, 0xd8, 0x6c, 0x36, 0x1b, 0xd, 0x86,
    0xc3, 0x7f, 0x0,

    /* U+0045 "E" */
    0xff, 0xb0, 0x18, 0xc, 0x7, 0xf3, 0x1, 0x80,
    0xc0, 0x7f, 0x80,

    /* U+0046 "F" */
    0xff, 0xb0, 0x18, 0xc, 0x7, 0xf3, 0x1, 0x80,
    0xc0, 0x60, 0x0,

    /* U+0047 "G" */
    0x7f, 0x60, 0x30, 0x18, 0xc, 0x7e, 0xf, 0x7,
    0x83, 0x7f, 0x0,

    /* U+0048 "H" */
    0xe0, 0xd8, 0x36, 0xd, 0x83, 0x7f, 0xd8, 0x36,
    0xd, 0x83, 0x60, 0xc0,

    /* U+0049 "I" */
    0xff, 0xff, 0xc0,

    /* U+004A "J" */
    0x1f, 0x80, 0xc0, 0x60, 0x30, 0x18, 0xf, 0x7,
    0x83, 0x7f, 0x0,

    /* U+004B "K" */
    0xe0, 0xd8, 0x26, 0x19, 0x84, 0x7f, 0x18, 0x46,
    0x19, 0x82, 0x60, 0xc0,

    /* U+004C "L" */
    0xe0, 0x30, 0x18, 0xc, 0x6, 0x3, 0x1, 0x80,
    0xc0, 0x7f, 0x80,

    /* U+004D "M" */
    0xe0, 0x6e, 0x1d, 0xe7, 0xb7, 0xb6, 0x66, 0xcc,
    0xd8, 0x1b, 0x3, 0x60, 0x60,

    /* U+004E "N" */
    0xe1, 0xb8, 0xde, 0x6d, 0x36, 0xdb, 0x3d, 0x8e,
    0xc7, 0x61, 0x80,

    /* U+004F "O" */
    0x7f, 0xb0, 0x3c, 0xf, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0x7f, 0x80,

    /* U+0050 "P" */
    0xff, 0x0, 0xd8, 0x6c, 0x36, 0x1b, 0xf9, 0x80,
    0xc0, 0x60, 0x0,

    /* U+0051 "Q" */
    0x7f, 0x98, 0x1b, 0x3, 0x60, 0x6c, 0xd, 0x81,
    0xb0, 0x36, 0x6, 0x7f, 0xc0, 0x30, 0x3, 0x0,

    /* U+0052 "R" */
    0xff, 0x0, 0x66, 0x19, 0x86, 0x61, 0x9f, 0xc6,
    0x61, 0x8c, 0x61, 0x80,

    /* U+0053 "S" */
    0x7f, 0xe0, 0x30, 0x18, 0x7, 0xf0, 0xc, 0x6,
    0x3, 0xff, 0x0,

    /* U+0054 "T" */
    0xff, 0x86, 0x3, 0x1, 0x80, 0xc0, 0x60, 0x30,
    0x18, 0xc, 0x0,

    /* U+0055 "U" */
    0xe0, 0xd8, 0x36, 0xd, 0x83, 0x60, 0xd8, 0x36,
    0xd, 0x83, 0x3f, 0x80,

    /* U+0056 "V" */
    0xc0, 0x6c, 0xc, 0x83, 0x18, 0x61, 0x18, 0x33,
    0x2, 0xc0, 0x78, 0x6, 0x0,

    /* U+0057 "W" */
    0xe0, 0x6c, 0xd, 0x99, 0xb3, 0x36, 0xf6, 0xf3,
    0xde, 0x7b, 0x87, 0x60, 0x60,

    /* U+0058 "X" */
    0x61, 0x98, 0xc3, 0x30, 0x78, 0xc, 0x7, 0x83,
    0x31, 0x8c, 0x61, 0x80,

    /* U+0059 "Y" */
    0xc1, 0xe0, 0xf0, 0x78, 0x37, 0xf8, 0xc, 0x6,
    0x3, 0x7f, 0x0,

    /* U+005A "Z" */
    0xff, 0x0, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
    0x80, 0x7f, 0x80,

    /* U+005B "[" */
    0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xf0,

    /* U+005C "\\" */
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x60, 0x18,
    0x6, 0x1, 0x80,

    /* U+005D "]" */
    0xf3, 0x33, 0x33, 0x33, 0x33, 0x33, 0xf0,

    /* U+005E "^" */
    0x27, 0xe2,

    /* U+005F "_" */
    0xff, 0xe0,

    /* U+0060 "`" */
    0x70,

    /* U+0061 "a" */
    0xfe, 0x3, 0x3, 0x3, 0xff, 0xc3, 0xc3, 0x7d,

    /* U+0062 "b" */
    0xff, 0x30, 0xd8, 0x6c, 0x37, 0xf3, 0xd, 0x86,
    0xfe,

    /* U+0063 "c" */
    0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x7f,

    /* U+0064 "d" */
    0xfe, 0x3, 0x63, 0x63, 0x63, 0x63, 0x63, 0x7e,

    /* U+0065 "e" */
    0x7e, 0xc3, 0xc3, 0xc3, 0xff, 0xc0, 0xc0, 0x7f,

    /* U+0066 "f" */
    0xff, 0x60, 0x60, 0x60, 0x7e, 0x60, 0x60, 0x60,

    /* U+0067 "g" */
    0x7e, 0xc0, 0xc0, 0xc0, 0xcf, 0xc3, 0xc3, 0x7e,

    /* U+0068 "h" */
    0xe1, 0xb0, 0xd8, 0x6c, 0x37, 0xfb, 0xd, 0x86,
    0xc3,

    /* U+0069 "i" */
    0xff, 0xff,

    /* U+006A "j" */
    0x1f, 0x3, 0x3, 0x3, 0x3, 0x3, 0xc3, 0x7e,

    /* U+006B "k" */
    0xe1, 0xb0, 0x98, 0xcc, 0x47, 0xe3, 0x19, 0x8c,
    0xc3,

    /* U+006C "l" */
    0xe0, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x3f,

    /* U+006D "m" */
    0xe0, 0xdc, 0x77, 0xbd, 0xbb, 0x6e, 0xd9, 0x36,
    0xd, 0x83,

    /* U+006E "n" */
    0xe3, 0x73, 0x73, 0x6b, 0x6b, 0x67, 0x67, 0x63,

    /* U+006F "o" */
    0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f,

    /* U+0070 "p" */
    0xfe, 0x3, 0x63, 0x63, 0x63, 0x7f, 0x60, 0x60,

    /* U+0071 "q" */
    0x7e, 0x61, 0xb0, 0xd8, 0x6c, 0x36, 0x1b, 0xc,
    0xfe, 0x7, 0x1, 0xc0,

    /* U+0072 "r" */
    0xfe, 0x1, 0x98, 0xcc, 0x66, 0x33, 0xf9, 0x98,
    0xc6,

    /* U+0073 "s" */
    0x7f, 0xc0, 0xc0, 0xc0, 0x7f, 0x3, 0x3, 0xfe,

    /* U+0074 "t" */
    0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,

    /* U+0075 "u" */
    0xe1, 0xb0, 0xd8, 0x6c, 0x36, 0x1b, 0xd, 0x86,
    0x7e,

    /* U+0076 "v" */
    0xc0, 0x98, 0x62, 0x18, 0xcc, 0x33, 0x5, 0x81,
    0xe0, 0x30,

    /* U+0077 "w" */
    0xe0, 0xd9, 0x36, 0x4d, 0xbb, 0x6a, 0xde, 0xf7,
    0x1d, 0x83,

    /* U+0078 "x" */
    0xc3, 0x66, 0x3c, 0x18, 0x18, 0x3c, 0x66, 0xc3,

    /* U+0079 "y" */
    0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x3, 0x3, 0x7e,

    /* U+007A "z" */
    0xff, 0x3, 0xe, 0x1c, 0x38, 0x70, 0xc0, 0xff,

    /* U+007B "{" */
    0x1c, 0xc3, 0xc, 0x71, 0x8c, 0x18, 0x70, 0xc3,
    0xc, 0x1c,

    /* U+007C "|" */
    0xff, 0xff, 0xff,

    /* U+007D "}" */
    0xe0, 0xc3, 0xc, 0x38, 0x60, 0xc6, 0x38, 0xc3,
    0xc, 0xe0,

    /* U+007E "~" */
    0x37, 0x9e, 0x80,

    /* U+0401 "Ё" */
    0x1e, 0x0, 0x3f, 0xec, 0x6, 0x3, 0x1, 0xfc,
    0xc0, 0x60, 0x30, 0x1f, 0xe0,

    /* U+0410 "А" */
    0x1e, 0x1, 0xe0, 0x6c, 0xc, 0xc3, 0x18, 0x61,
    0x9f, 0xf3, 0x3, 0xc0, 0x60,

    /* U+0411 "Б" */
    0xff, 0xcc, 0x1, 0x80, 0x30, 0x7, 0xfc, 0xc0,
    0xd8, 0x1b, 0x3, 0x7f, 0xc0,

    /* U+0412 "В" */
    0xff, 0x98, 0x36, 0xd, 0x83, 0x7f, 0x18, 0x36,
    0xd, 0x83, 0x7f, 0x80,

    /* U+0413 "Г" */
    0xff, 0xb0, 0x58, 0xc, 0x6, 0x3, 0x1, 0x80,
    0xc0, 0x60, 0x0,

    /* U+0414 "Д" */
    0x3f, 0xc2, 0x18, 0xc3, 0x18, 0x63, 0xc, 0x41,
    0x98, 0x33, 0x6, 0xff, 0xe0, 0xc, 0x0, 0x80,

    /* U+0415 "Е" */
    0xff, 0xb0, 0x18, 0xc, 0x7, 0xf3, 0x1, 0x80,
    0xc0, 0x7f, 0x80,

    /* U+0416 "Ж" */
    0xc3, 0xd, 0x8c, 0x62, 0x31, 0xc, 0xcc, 0x1f,
    0xe0, 0xcc, 0xc2, 0x31, 0x18, 0xc6, 0xc3, 0xc,

    /* U+0417 "З" */
    0x7f, 0x60, 0xf0, 0x60, 0x31, 0xe0, 0xc, 0x6,
    0x3, 0xff, 0x0,

    /* U+0418 "И" */
    0xe1, 0xb1, 0xd9, 0xec, 0xb6, 0xdb, 0xcd, 0xc6,
    0xe3, 0x61, 0x80,

    /* U+0419 "Й" */
    0x12, 0xf, 0x0, 0x1c, 0x36, 0x3b, 0x3d, 0x96,
    0xdb, 0x79, 0xbc, 0xdc, 0x6c, 0x30,

    /* U+041A "К" */
    0xe0, 0xd8, 0x26, 0x19, 0x84, 0x7f, 0x18, 0x46,
    0x19, 0x82, 0x60, 0xc0,

    /* U+041B "Л" */
    0x3f, 0xe3, 0xc, 0x61, 0x8c, 0x31, 0x6, 0x60,
    0xcc, 0x19, 0x3, 0xe0, 0x60,

    /* U+041C "М" */
    0xe0, 0x6e, 0x1d, 0xe7, 0xb7, 0xb6, 0x66, 0xcc,
    0xd8, 0x1b, 0x3, 0x60, 0x60,

    /* U+041D "Н" */
    0xe0, 0xd8, 0x36, 0xd, 0x83, 0x7f, 0xd8, 0x36,
    0xd, 0x83, 0x60, 0xc0,

    /* U+041E "О" */
    0x7f, 0xb0, 0x3c, 0xf, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0x7f, 0x80,

    /* U+041F "П" */
    0xff, 0xd8, 0x36, 0xd, 0x83, 0x60, 0xd8, 0x36,
    0xd, 0x83, 0x60, 0xc0,

    /* U+0420 "Р" */
    0xff, 0x0, 0xd8, 0x6c, 0x36, 0x1b, 0xf9, 0x80,
    0xc0, 0x60, 0x0,

    /* U+0421 "С" */
    0x7f, 0x60, 0xf0, 0x78, 0xc, 0x6, 0x3, 0x1,
    0x80, 0x7f, 0x80,

    /* U+0422 "Т" */
    0xff, 0x86, 0x3, 0x1, 0x80, 0xc0, 0x60, 0x30,
    0x18, 0xc, 0x0,

    /* U+0423 "У" */
    0xc1, 0xe0, 0xf0, 0x78, 0x37, 0xf8, 0xc, 0x6,
    0x3, 0x7f, 0x0,

    /* U+0424 "Ф" */
    0x7f, 0xec, 0x3, 0xc6, 0x3c, 0x63, 0xc6, 0x3c,
    0x63, 0x7f, 0xe0, 0x60, 0x6, 0x0,

    /* U+0425 "Х" */
    0x61, 0x98, 0xc3, 0x30, 0x78, 0xc, 0x7, 0x83,
    0x31, 0x8c, 0x61, 0x80,

    /* U+0426 "Ц" */
    0xe0, 0xcc, 0x19, 0x83, 0x30, 0x66, 0xc, 0xc1,
    0x98, 0x33, 0x6, 0x7f, 0xe0, 0xc, 0x0, 0x80,

    /* U+0427 "Ч" */
    0xe0, 0xd8, 0x36, 0xd, 0x83, 0x60, 0xcf, 0xf0,
    0xc, 0x3, 0x0, 0xc0,

    /* U+0428 "Ш" */
    0xe3, 0x1b, 0x18, 0xd8, 0xc6, 0xc6, 0x36, 0x31,
    0xb1, 0x8d, 0x8c, 0x6c, 0x63, 0x7f, 0xf8,

    /* U+0429 "Щ" */
    0xe3, 0x19, 0x8c, 0x66, 0x31, 0x98, 0xc6, 0x63,
    0x19, 0x8c, 0x66, 0x31, 0x98, 0xc6, 0x7f, 0xfc,
    0x0, 0x30, 0x0, 0x40,

    /* U+042A "Ъ" */
    0xf8, 0x1, 0x80, 0x18, 0x1, 0xfe, 0x18, 0x31,
    0x83, 0x18, 0x31, 0x83, 0x1f, 0xe0,

    /* U+042B "Ы" */
    0xc0, 0x3c, 0x3, 0xc0, 0x3f, 0xf3, 0xc1, 0xbc,
    0x1b, 0xc1, 0xbc, 0x1b, 0xff, 0x30,

    /* U+042C "Ь" */
    0xc0, 0x60, 0x30, 0x1f, 0xec, 0x1e, 0xf, 0x7,
    0x83, 0xff, 0x0,

    /* U+042D "Э" */
    0x7f, 0x60, 0xf0, 0x60, 0x31, 0xf8, 0xc, 0x6,
    0x3, 0xff, 0x0,

    /* U+042E "Ю" */
    0xe7, 0xf9, 0xb0, 0x36, 0xc0, 0xdb, 0x3, 0x7c,
    0xd, 0xb0, 0x36, 0xc0, 0xdb, 0x3, 0x67, 0xf8,

    /* U+042F "Я" */
    0x3f, 0xec, 0x1, 0x86, 0x30, 0xc6, 0x18, 0x7f,
    0x6, 0x61, 0x8c, 0x61, 0x80,

    /* U+0430 "а" */
    0xfe, 0x3, 0x3, 0x3, 0xff, 0xc3, 0xc3, 0x7d,

    /* U+0431 "б" */
    0xff, 0xb0, 0x18, 0xf, 0xf6, 0x1b, 0xd, 0x86,
    0x7f,

    /* U+0432 "в" */
    0xff, 0x30, 0xd8, 0x6f, 0xe6, 0x1b, 0xd, 0x86,
    0x7e,

    /* U+0433 "г" */
    0xff, 0x61, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,

    /* U+0434 "д" */
    0x3f, 0x98, 0xcc, 0x66, 0x32, 0x1b, 0xd, 0x87,
    0xff, 0x0, 0x80, 0x0,

    /* U+0435 "е" */
    0x7e, 0xc3, 0xc3, 0xc3, 0xff, 0xc0, 0xc0, 0x7f,

    /* U+0436 "ж" */
    0xc6, 0x36, 0x62, 0x66, 0x62, 0x64, 0x3f, 0xc2,
    0x64, 0x66, 0x6c, 0x63,

    /* U+0437 "з" */
    0x7e, 0xc3, 0x3, 0x2, 0x1e, 0x3, 0x3, 0xfe,

    /* U+0438 "и" */
    0xe3, 0x67, 0x67, 0x6b, 0x7b, 0x73, 0x73, 0x63,

    /* U+0439 "й" */
    0x1c, 0x1c, 0xe3, 0x67, 0x67, 0x6b, 0x7b, 0x73,
    0x73, 0x63,

    /* U+043A "к" */
    0xe1, 0xb0, 0x98, 0xcc, 0x47, 0xe3, 0x19, 0x8c,
    0xc3,

    /* U+043B "л" */
    0x7f, 0x98, 0xcc, 0x64, 0x36, 0x1b, 0xd, 0x7,
    0x83,

    /* U+043C "м" */
    0xe0, 0xdc, 0x77, 0xbd, 0xbb, 0x6e, 0xd9, 0x36,
    0xd, 0x83,

    /* U+043D "н" */
    0xe1, 0xb0, 0xd8, 0x6c, 0x37, 0xfb, 0xd, 0x86,
    0xc3,

    /* U+043E "о" */
    0x7e, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f,

    /* U+043F "п" */
    0xff, 0xb0, 0xd8, 0x6c, 0x36, 0x1b, 0xd, 0x86,
    0xc3,

    /* U+0440 "р" */
    0xfe, 0x3, 0x63, 0x63, 0x63, 0x7f, 0x60, 0x60,

    /* U+0441 "с" */
    0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x7f,

    /* U+0442 "т" */
    0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,

    /* U+0443 "у" */
    0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x3, 0x3, 0x7e,

    /* U+0444 "ф" */
    0x7f, 0xb0, 0x3c, 0xcf, 0x33, 0xcc, 0xff, 0xf0,
    0xc0, 0x30,

    /* U+0445 "х" */
    0xc3, 0x66, 0x3c, 0x18, 0x18, 0x3c, 0x66, 0xc3,

    /* U+0446 "ц" */
    0xe1, 0x98, 0x66, 0x19, 0x86, 0x61, 0x98, 0x66,
    0x19, 0xff, 0x0, 0xc0, 0x10,

    /* U+0447 "ч" */
    0xe1, 0xb0, 0xd8, 0x6c, 0x33, 0xf8, 0xc, 0x6,
    0x3,

    /* U+0448 "ш" */
    0xe6, 0x6c, 0xcd, 0x99, 0xb3, 0x36, 0x66, 0xcc,
    0xd9, 0x9b, 0xff,

    /* U+0449 "щ" */
    0xe6, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
    0x66, 0x66, 0x67, 0xff, 0x0, 0x30, 0x1,

    /* U+044A "ъ" */
    0xf0, 0xc, 0x3, 0x0, 0xfe, 0x30, 0xcc, 0x33,
    0xc, 0x7e,

    /* U+044B "ы" */
    0xc0, 0x78, 0xf, 0x1, 0xff, 0x3c, 0x37, 0x86,
    0xf0, 0xdb, 0xf3,

    /* U+044C "ь" */
    0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0x7e,

    /* U+044D "э" */
    0x7e, 0xc3, 0x3, 0x3, 0x1f, 0x3, 0x3, 0xfe,

    /* U+044E "ю" */
    0xe7, 0xe6, 0xc3, 0x6c, 0x36, 0xc3, 0x7c, 0x36,
    0xc3, 0x6c, 0x36, 0x7e,

    /* U+044F "я" */
    0x3f, 0xb0, 0x18, 0xcc, 0x66, 0x33, 0xf8, 0xcc,
    0xc6,

    /* U+0451 "ё" */
    0x3c, 0x0, 0x7e, 0xc3, 0xc3, 0xc3, 0xff, 0xc0,
    0xc0, 0x7f
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

const font_info Font_dsc[] = {
  {.bitmap_index = 0, .adv_w = 90, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 49, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 105, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 7, .adv_w = 148, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 18, .adv_w = 162, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 32, .adv_w = 185, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 174, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 56, .adv_w = 49, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 57, .adv_w = 69, .box_w = 4, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 64, .adv_w = 69, .box_w = 4, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 71, .adv_w = 140, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 77, .adv_w = 116, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 81, .adv_w = 49, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 82, .adv_w = 101, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 83, .adv_w = 56, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 149, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 177, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 107, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 115, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 126, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 177, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 149, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 150, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 177, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 194, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 205, .adv_w = 58, .box_w = 2, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 207, .adv_w = 58, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 209, .adv_w = 97, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 216, .adv_w = 146, .box_w = 8, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 220, .adv_w = 97, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 227, .adv_w = 171, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 239, .adv_w = 174, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 248, .adv_w = 168, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = 0},//A
    {.bitmap_index = 261, .adv_w = 170, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 273, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 284, .adv_w = 161, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 295, .adv_w = 157, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 148, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 317, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 328, .adv_w = 172, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 340, .adv_w = 49, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 343, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 166, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 366, .adv_w = 148, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 377, .adv_w = 199, .box_w = 11, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 390, .adv_w = 161, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 401, .adv_w = 177, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 413, .adv_w = 157, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 424, .adv_w = 177, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 440, .adv_w = 164, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 162, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 463, .adv_w = 153, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 474, .adv_w = 172, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 486, .adv_w = 187, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 499, .adv_w = 189, .box_w = 11, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 512, .adv_w = 157, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 524, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 535, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 546, .adv_w = 69, .box_w = 4, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 553, .adv_w = 149, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 564, .adv_w = 69, .box_w = 4, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 571, .adv_w = 101, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 573, .adv_w = 168, .box_w = 11, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 575, .adv_w = 93, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 10},
    {.bitmap_index = 576, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 584, .adv_w = 146, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 593, .adv_w = 153, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 601, .adv_w = 151, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 609, .adv_w = 151, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 617, .adv_w = 131, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 625, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 633, .adv_w = 161, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 0},//h
    {.bitmap_index = 642, .adv_w = 56, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 644, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 652, .adv_w = 149, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 661, .adv_w = 110, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 668, .adv_w = 165, .box_w = 10, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 678, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 686, .adv_w = 158, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 694, .adv_w = 134, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 702, .adv_w = 158, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 714, .adv_w = 140, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 723, .adv_w = 141, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 731, .adv_w = 137, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 739, .adv_w = 149, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 748, .adv_w = 159, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 758, .adv_w = 163, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 768, .adv_w = 135, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 776, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 784, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},//z
    {.bitmap_index = 792, .adv_w = 96, .box_w = 6, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 802, .adv_w = 45, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 805, .adv_w = 96, .box_w = 6, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 815, .adv_w = 185, .box_w = 10, .box_h = 2, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 818, .adv_w = 157, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 831, .adv_w = 182, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 844, .adv_w = 178, .box_w = 11, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 857, .adv_w = 170, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 869, .adv_w = 148, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 880, .adv_w = 179, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 896, .adv_w = 157, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 907, .adv_w = 220, .box_w = 14, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 923, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 934, .adv_w = 161, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 945, .adv_w = 161, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 959, .adv_w = 166, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 971, .adv_w = 172, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 984, .adv_w = 189, .box_w = 11, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 997, .adv_w = 172, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1009, .adv_w = 177, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1021, .adv_w = 172, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1033, .adv_w = 161, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1044, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1055, .adv_w = 153, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1066, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1077, .adv_w = 215, .box_w = 12, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1091, .adv_w = 157, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1103, .adv_w = 179, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1119, .adv_w = 178, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1131, .adv_w = 213, .box_w = 13, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1146, .adv_w = 220, .box_w = 14, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1166, .adv_w = 200, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1180, .adv_w = 218, .box_w = 12, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1194, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1205, .adv_w = 171, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1216, .adv_w = 235, .box_w = 14, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1232, .adv_w = 164, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1245, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1253, .adv_w = 155, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1262, .adv_w = 149, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1271, .adv_w = 133, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1279, .adv_w = 163, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1291, .adv_w = 153, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1299, .adv_w = 193, .box_w = 12, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1311, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1319, .adv_w = 141, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1327, .adv_w = 141, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1337, .adv_w = 149, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1346, .adv_w = 151, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1355, .adv_w = 165, .box_w = 10, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1365, .adv_w = 151, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1374, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1382, .adv_w = 151, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1391, .adv_w = 144, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1399, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1407, .adv_w = 137, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1415, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1423, .adv_w = 186, .box_w = 10, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1433, .adv_w = 135, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1441, .adv_w = 157, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1454, .adv_w = 155, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1463, .adv_w = 185, .box_w = 11, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1474, .adv_w = 195, .box_w = 12, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1489, .adv_w = 174, .box_w = 10, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1499, .adv_w = 189, .box_w = 11, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1510, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1518, .adv_w = 149, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1526, .adv_w = 204, .box_w = 12, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1538, .adv_w = 144, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1547, .adv_w = 153, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0}
};

