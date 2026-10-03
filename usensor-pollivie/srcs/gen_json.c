#include "usensor.h"

int32_t	cmp_timestamp_seq(const void *ele1, const void *ele2)
{
	t_usensor	*sensor1 = *(t_usensor **)ele1;
	t_usensor	*sensor2 = *(t_usensor **)ele2;

	if (sensor1->timestamp < sensor2->timestamp)
		return (-1);
	if (sensor1->timestamp > sensor2->timestamp)
		return (1);
	if (sensor1->sequence < sensor2->sequence)
		return (-1);
	if (sensor1->sequence > sensor2->sequence)
		return (1);
	return (0);
}

t_usensor	*select_by_time_seq(t_usensor *tmp,
								const t_usensor *button,
								t_usensor *a_best,
								uint64_t	valid_time) 
{
	uint64_t	time_tmp;
	uint64_t	time_best;

	time_tmp =	(tmp->timestamp >= button->timestamp) ? 
				(tmp->timestamp - button->timestamp) : 
				(button->timestamp - tmp->timestamp);
	if (!a_best)
		return ((time_tmp < valid_time) ? tmp : NULL);

	time_best = (a_best->timestamp >= button->timestamp) ? 
				(a_best->timestamp - button->timestamp) : 
				(button->timestamp - a_best->timestamp);

	if (time_tmp == time_best)
		return ((tmp->sequence < a_best->sequence) ? tmp : a_best);
	return ((time_tmp < time_best) ? tmp : a_best);
}

int8_t	add_imu_win(t_usensor *imu, const t_usensor *button, uint64_t valid_time)
{
	uint64_t	time =	(imu->timestamp >= button->timestamp) ? 
						(imu->timestamp - button->timestamp) : 
						(button->timestamp - imu->timestamp);

	if (time <= valid_time)
		return (1);
	return (0);
}

void	init_info_json(t_info_json *json) {
	json->cam			= NULL;
	json->gps			= NULL;
	json->imu 			= NULL;
	json->data_in_imu	= 0;
}

void	select_data_usensor(t_usensor *valid_data,
							const t_usensor *button,
							const uint32_t	idx_head)
{
	t_info_json	b_match;

	init_info_json(&b_match);
	for (uint16_t i = 0; i < idx_head; i++)
	{
		t_usensor	*tmp = &valid_data[i];

		if (!tmp->timestamp)
			continue ;

		if (tmp->timestamp + MS(1000) < button->timestamp)
			continue ;
		if (tmp->timestamp > button->timestamp + MS(100))
			continue ;

		if (tmp->sensor_id == CAMERA)
		{
			b_match.cam = select_by_time_seq(tmp, button, b_match.cam, MS(50));
		}
		else if (tmp->sensor_id == GPS && tmp->timestamp <= button->timestamp)
		{
			if (button->timestamp - tmp->timestamp <= MS(1000)) {
				if (!b_match.gps || tmp->timestamp > b_match.gps->timestamp)
					b_match.gps = tmp;
			}
		}
		else if (tmp->sensor_id == IMU)
		{
			if (tmp->x < 0 || tmp->x > 35999)
				continue ;

			b_match.imu = select_by_time_seq(tmp, button, b_match.imu, MS(20));
			if (add_imu_win(tmp, button, MS(100))) {
				b_match.imu_windows[b_match.data_in_imu] = tmp;
				b_match.data_in_imu = (b_match.data_in_imu + 1) % 512;
			}
		}
	}

	qsort(	b_match.imu_windows,
			b_match.data_in_imu,
			sizeof(t_usensor *),
			&cmp_timestamp_seq);

	print_json(b_match, button);
}

void	print_json(t_info_json info, const t_usensor *button) {
	fprintf(stdout,"{\"event_id\":%d,\"timestamp_ns\":%lu,",button->x, button->timestamp);
	if (!info.cam)
		fprintf(stdout, "\"camera\":null,");
	else
		fprintf(stdout, "\"camera\":{\"timestamp_ns\":%lu,\"frame\":%d},",
						info.cam->timestamp,info.cam->x);
	fprintf(stdout, "\"imu\":");
	if (!info.imu)
		fprintf(stdout, "null,");
	else
		fprintf(stdout, "{\"timestamp_ns\":%lu,\"yaw_cd\":%d,\"pitch_cd\":%d,\"roll_cd\":%d},",
				info.imu->timestamp, info.imu->x, info.imu->y, info.imu->z);
	fprintf(stdout, "\"gps\":");
	if (!info.gps)
		fprintf(stdout, "null,");
	else
		fprintf(stdout, "{\"timestamp_ns\":%lu,\"lat_e7\":%d,\"lon_e7\":%d,\"alt_mm\":%d},",
				info.imu->timestamp, info.gps->x, info.gps->y, info.gps->z);

	fprintf(stdout, "\"imu_window\":[");
    for (uint16_t k = 0; k < info.data_in_imu; k++) {
        fprintf(stdout, "{\"timestamp_ns\":%lu,\"yaw_cd\":%d,\"pitch_cd\":%d,\"roll_cd\":%d}%s",
                info.imu_windows[k]->timestamp,
                info.imu_windows[k]->x,
                info.imu_windows[k]->y,
                info.imu_windows[k]->z,
                (k < info.data_in_imu - 1) ? "," : "");
    }
    fprintf(stdout, "]}\n");
}
