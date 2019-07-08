#ifndef __HTU21D_H__
#define __HTU21D_H__
void HTU21DGPIOInit(void);
void htu21d_reset(void);
void htu21d_mesure_t(void);
void htu21d_mesure_h(void);
float htu21d_read_t(void);
float htu21d_read_h(void);

void htu21d_init(void);
float htu21d_t(void);
float htu21d_h(void);
#endif