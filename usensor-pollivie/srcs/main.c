#include	"usensor.h"

int	main(void)
{
	uint8_t				header[HEADER_SIZE];
	pthread_t			producer;
	pthread_t			consumer;
	t_produc_consum		queue_thread;

	if (fread(header, 1, HEADER_SIZE, stdin) != HEADER_SIZE) {
		fprintf(stderr, "Error: incomplet header\n");
		return (EXIT_FAILURE);
	}

	// Checksum
	if (fnv1a_32(header, 12) != uint32_from_bytes(&header[12])) {
		fprintf(stderr, "Error: invalid header\n");
		return (EXIT_FAILURE);
	}

	init_queue_thread(&queue_thread);

	pthread_create(&producer, NULL, &producer_routine, &queue_thread);
	pthread_create(&consumer, NULL, &consumer_routine, &queue_thread);

	pthread_join(consumer, NULL);
	pthread_join(producer, NULL);

	destroy_queue_thread(&queue_thread);

	return (0);
}
