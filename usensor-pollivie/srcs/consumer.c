#include "usensor.h"

void	*consumer_routine(void *shared_data)
{
	t_produc_consum	*queue_thread = (t_produc_consum *)shared_data;
	t_usensor		valid_data[QUEUE_SIZE];
	t_usensor		waiting_button[10];
	uint8_t			record[RECORD_SIZE];
	uint8_t			current_boutton = 0, nbr_wait = 0;
	uint32_t		idx_data = 0, idx_wait = 0;

	memset(valid_data, 0, (sizeof(t_usensor) * QUEUE_SIZE));
	memset(waiting_button, 0, (sizeof(t_usensor) * 10));
	for (; 1; idx_data = (idx_data + 1) % QUEUE_SIZE)
	{
		if (!pop_queue_thread(queue_thread, record))
			break ;

		valid_data[idx_data] = reconstituate(record);

		// Checksum
		if (fnv1a_32(record, 28) != uint32_from_bytes(&record[28]))
			continue ;

		if (!(valid_data[idx_data].flags & 1))
			continue ;

		if (valid_data[idx_data].sensor_id == 5) {
			memcpy(&waiting_button[idx_wait], &valid_data[idx_data], sizeof(t_usensor));
			idx_wait = (idx_wait + 1) % 10;
			nbr_wait++;
		}

		while (nbr_wait > 0 &&
			valid_data[idx_data].timestamp >= waiting_button[current_boutton].timestamp + MS(102))
		{
			select_data_usensor(valid_data, &waiting_button[current_boutton], (uint32_t)idx_data);
			current_boutton = (current_boutton + 1) % 10;
			nbr_wait--;
		}
	}

	while (nbr_wait > 0)
	{
		select_data_usensor(valid_data,
							&waiting_button[current_boutton],
							(uint32_t)idx_data);
		current_boutton = (current_boutton + 1) % 10;
		nbr_wait--;
	}

	return (NULL);
}
