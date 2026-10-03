#include "usensor.h"

int32_t	int32_from_bytes(uint8_t *record) {
	uint32_t	value = (uint32_t)record[0] |
						((uint32_t)record[1] << 8) |
						((uint32_t)record[2] << 16) | 
						((int32_t)record[3] << 24);
	return ((int32_t)value);
}

uint32_t	uint32_from_bytes(uint8_t *record) {
	return ((uint32_t)record[0] |
			((uint32_t)record[1] << 8) |
			((uint32_t)record[2] << 16) |
			((uint32_t)record[3] << 24));
}

uint64_t	uint64_from_bytes(uint8_t *record) {
	return ((uint64_t)record[0] |
			((uint64_t)record[1] << 8) |
			((uint64_t)record[2] << 16) |
			((uint64_t)record[3] << 24) |
			((uint64_t)record[4] << 32) |
			((uint64_t)record[5] << 40) |
			((uint64_t)record[6] << 48) |
			((uint64_t)record[7] << 56));
}

t_usensor reconstituate(uint8_t *record) {
	t_usensor	data;

	data.timestamp			= uint64_from_bytes(record);
	data.sequence			= uint32_from_bytes(&record[8]);
	data.sensor_id			= record[12];
	data.flags				= record[13];
	data.x					= int32_from_bytes(&record[16]);
	data.y					= int32_from_bytes(&record[20]);
	data.z					= int32_from_bytes(&record[24]);
	return (data);
}
