#ifndef HTS221_H
#define HTS221_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool my_hts221_init(void);
void hts221_sample(void);

#ifdef __cplusplus
}
#endif

#endif // HTS221_H