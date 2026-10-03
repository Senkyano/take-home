#include "usensor.h"
#include <stdio.h>


uint32_t	fnv1a_32(uint8_t *data, size_t len_data) {
	uint32_t	hash = 2166136261U;

	for (uint8_t i = 0; i < len_data; i++) {
		hash ^= data[i];
		hash = hash * 16777619U;
	}

	return (hash);
}
