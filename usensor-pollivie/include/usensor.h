#pragma once

#ifndef	__USENSOR_H__
# define __USENSOR_H__

#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define	HEADER_SIZE 16
#define RECORD_SIZE 32

#define	CAMERA			1
#define IMU				2
#define GPS				3
#define TEMPERATURE		4
#define BUTTON			5

uint32_t	fnv1a_32(uint8_t *data, size_t len_data);

typedef struct s_usensor
{
	uint64_t	timestamp;
	uint32_t	sequence;
	uint8_t		sensor_id;
	uint8_t		flags;
	int32_t		x;
	int32_t		y;
	int32_t		z;
}	t_usensor;

#define QUEUE_SIZE 1024

typedef struct	s_info_json {
	t_usensor	*cam;
	t_usensor	*imu;
	t_usensor	*gps;
	t_usensor	*imu_windows[512];
	uint16_t	data_in_imu;
}	t_info_json;

typedef struct	s_produc_consum {
	uint8_t		data_queue[QUEUE_SIZE][32];
	uint16_t	idx_producer;
	uint16_t	idx_consum;
	uint16_t	nbr_data;
	uint8_t		flux;

	pthread_mutex_t	lock;
	pthread_cond_t	empty_data_queue;
	pthread_cond_t	data_in_queue;
}	t_produc_consum;

uint64_t	uint64_from_bytes(uint8_t *record);
uint32_t	uint32_from_bytes(uint8_t *record);
int32_t	int32_from_bytes(uint8_t *record);

t_usensor reconstituate(uint8_t *record);

void	*init_queue_thread(t_produc_consum *queue_thread);
void	end_flux_queue(t_produc_consum *queue_thread);
void	*pop_queue_thread(t_produc_consum *queue_thread, uint8_t *data);
void	push_queue_thread(t_produc_consum *queue_thread, uint8_t *data);
void	destroy_queue_thread(t_produc_consum *queue_thread);

#define	NANOSEC_PER_MSEC 1000000ULL
#define MS(x) (NANOSEC_PER_MSEC * x)

void	*producer_routine(void *shared_data);
void	*consumer_routine(void *shared_data);

void	select_data_usensor(t_usensor *valid_data,
							const t_usensor *button,
							const uint32_t	idx_head);

void	print_json(t_info_json info, const t_usensor *button);

#endif