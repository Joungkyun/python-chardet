/*
 * Copyfight 2021. JoungKyun.Kim all rights reserevd.
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 */
#include <chardet.h>
#include <string.h>

int legacy_bom (char **text) {
	char * buf = (char *) *text;

	if ( strcmp ("001000006", detect_uversion ()) < 1 )
		return 0;

	switch (*buf) {
		case '\xEF' :
			// EF BB BF  UTF-8 encoded BOM
			if ( *(buf + 1) == '\xBB' && *(buf + 2) == '\xBF' )
				return 1;
			break;
		case '\xFE' :
			// FE FF 00 00  UCS-4, unusual octet order BOM (3412)
			// FE FF        UTF-16, big endian BOM
			if ( *(buf + 1) == '\xFF' )
				return 1;
			break;
		case '\x00' :
			if ( *(buf + 1) != '\x00' )
				return 0;

			// 00 00 FE FF  UTF-32, big-endian BOM
			if ( *(buf + 2) == '\xFE' && *(buf + 3) == '\xFF' )
				return 1;
			// 00 00 FF FE  UCS-4, unusual octet order BOM (2143)
			else if ( *(buf + 2) == '\xFF' && *(buf + 3) == '\xFE' )
				return 1;

			break;
		case '\xFF' :
			// FF FE 00 00  UTF-32, little-endian BOM
			// FF FE        UTF-16, little endian BOM
			if ( *(buf + 1) == '\xFE' )
				return 1;
			break;
		case '\x2B' :
			// https://en.wikipedia.org/wiki/Byte_order_mark#Byte_order_marks_by_encoding
			// 2B 2F 76 38  UTF-7
			// 2B 2F 76 39  UTF-7
			// 2B 2F 76 2B  UTF-7
			// 2B 2F 76 2F  UTF-7
			if ( *(buf + 1) == '\x2F' && *(buf + 2) == '\x76' ) {
				switch (*(buf + 3)) {
					case '\x38' :
					case '\x39' :
					case '\x2B' :
					case '\x2F' :
						return 1;
						break;
				}
			}
			break;
		case '\xE7' :
			// E7 64 4c  UTF-1 encoded BOM
			if ( *(buf + 1) == '\x64' && *(buf + 2) == '\x4C' )
				return 1;
			break;
		case '\xDD' :
			// DD 73 66 73  UTF-EBCDIC encoded BOM
			if ( *(buf + 1) == '\x73' && *(buf + 2) == '\x66' && *(buf + 3) == '\x73' )
				return 1;
			break;
		case '\x0E' :
			// 0E FE FF  SCSU encoded BOM
			if ( *(buf + 1) == '\xFE' && *(buf + 2) == '\xFF' )
				return 1;
			break;
		case '\xFB' :
			// FB EE 28  BOCU-1 encoded BOM
			if ( *(buf + 1) == '\xEE' && *(buf + 2) == '\x28' )
				return 1;
			break;
		case '\x84' :
			// 84 31 95 33  GB18030 encoded BOM
			if ( *(buf + 1) == '\x31' && *(buf + 2) == '\x95' && *(buf + 3) == '\x33' )
				return 1;
			break;
	}

	return 0;
}
