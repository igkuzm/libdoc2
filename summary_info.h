/**
 * File              : summary_info.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 04.11.2022
 * Last Modified Date: 04.11.2022
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */

#include "ole2.h"

typedef struct {
	uint32_t		format[4];
	uint32_t		offset;
} sectionList;

typedef struct {
	uint16_t		sig;
	uint16_t		_empty;
	uint32_t		os;
	uint32_t		format[4];
	uint32_t		count;
	sectionList		secList[1];
} header;

typedef struct {
	uint32_t		propertyID;
	uint32_t		sectionOffset;
} propertyList;

typedef struct {
	uint32_t		length;
	uint32_t		numProperties;
	propertyList	properties[1];
} sectionHeader;

typedef struct {
	uint32_t		propertyID;
	uint32_t		data[1];
} property;
