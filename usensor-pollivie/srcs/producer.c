#include "usensor.h"
#include <stdio.h>

void	*producer_routine(void *shared_data)
{
	t_produc_consum		*queue_thread = (t_produc_consum *)shared_data;
	uint8_t				record[RECORD_SIZE];
	uint32_t			len_transmission = 0;

	while ((len_transmission = fread(record, 1, RECORD_SIZE, stdin)) != 0)
	{
		// Check the length if not ignore
		if (len_transmission != (uint32_t)32)
			continue ;

		push_queue_thread(queue_thread, record);
	}

	end_flux_queue(queue_thread);
	return (NULL);
}
