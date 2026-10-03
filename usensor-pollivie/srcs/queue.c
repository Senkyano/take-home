#include "usensor.h"

void	*init_queue_thread(t_produc_consum *queue_thread)
{
	queue_thread->idx_consum	= 0;
	queue_thread->idx_producer	= 0;
	queue_thread->nbr_data		= 0;
	queue_thread->flux			= 1;

	if (pthread_mutex_init(&queue_thread->lock, NULL))
		return (NULL);
	if (pthread_cond_init(&queue_thread->empty_data_queue, NULL))
		return (NULL);
	if (pthread_cond_init(&queue_thread->data_in_queue, NULL))
		return (NULL);
	return (queue_thread);
}

void	destroy_queue_thread(t_produc_consum *queue_thread)
{
	pthread_mutex_destroy(&queue_thread->lock);
	pthread_cond_destroy(&queue_thread->data_in_queue);
	pthread_cond_destroy(&queue_thread->empty_data_queue);
}

void	push_queue_thread(t_produc_consum *queue_thread, uint8_t *data)
{
	pthread_mutex_lock(&queue_thread->lock);

	while (queue_thread->nbr_data == QUEUE_SIZE) {
		pthread_cond_wait(&queue_thread->empty_data_queue, &queue_thread->lock);
	}

	memcpy(queue_thread->data_queue[queue_thread->idx_producer], data, RECORD_SIZE);
	queue_thread->idx_producer = (queue_thread->idx_producer + 1) % QUEUE_SIZE;
	queue_thread->nbr_data++;

	pthread_cond_signal(&queue_thread->data_in_queue);

	pthread_mutex_unlock(&queue_thread->lock);
}

void	*pop_queue_thread(t_produc_consum *queue_thread, uint8_t *data)
{
	pthread_mutex_lock(&queue_thread->lock);

	while (queue_thread->nbr_data == 0 && queue_thread->flux) {
		pthread_cond_wait(&queue_thread->data_in_queue, &queue_thread->lock);
	}

	if (queue_thread->nbr_data == 0 && !queue_thread->flux) {
		pthread_mutex_unlock(&queue_thread->lock);
		return (NULL);
	}

	memcpy(data, queue_thread->data_queue[queue_thread->idx_consum], RECORD_SIZE);
	queue_thread->nbr_data--;
	queue_thread->idx_consum = (queue_thread->idx_consum + 1) % QUEUE_SIZE;

	pthread_cond_signal(&queue_thread->empty_data_queue);

	pthread_mutex_unlock(&queue_thread->lock);
	return (data);
}

void	end_flux_queue(t_produc_consum *queue_thread)
{
	pthread_mutex_lock(&queue_thread->lock);

	queue_thread->flux	= 0;

	pthread_cond_broadcast(&queue_thread->data_in_queue);
	pthread_mutex_unlock(&queue_thread->lock);
}
