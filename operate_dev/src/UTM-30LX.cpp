#include "../include/UTM-30LX.hpp"

int LRF()
{

  int i;
  urg_t urg;
  int ret;
  long *length_data;
  int length_data_size;
  int first_step;
  int last_step;
  int skip_step;
  int skip_scan;
  long timestamp;

  ret = urg_open(&urg, URG_SERIAL, connect_device.c_str(), connect_baudrate);
  length_data = (long *)malloc(sizeof(long) * urg_max_data_size(&urg));

  first_step = urg_rad2step(&urg, -135);
  last_step = urg_rad2step(&urg, +135);
  skip_step = 0;
  ret = urg_set_scanning_parameter(&urg, first_step, last_step, skip_step);


  skip_scan = 0;
  ret = urg_start_measurement(&urg, URG_DISTANCE, scan_times, 0);

  for (i = 0; i < scan_times; ++i) {
    length_data_size = urg_get_distance(&urg, length_data, &timestamp);

    for (i = 0; i < length_data_size; ++i) {

      double radian;
      long length;
      long x;
      long y;

      radian = urg_index2rad(&urg, i);
      length = length_data[i];

      x = (long)(length * cos(radian));
      y = (long)(length * sin(radian));
      printf("(%f, %ld, %ld)\n ", radian, length,timestamp);
    }

    printf("%d\n",i);
  }

  printf("\n");

  urg_close(&urg);

  return 0;
}
