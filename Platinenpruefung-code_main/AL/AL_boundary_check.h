/*
 * AL_boundary-check.h
 *
 * Author: Andreas Fischer
 */

#ifndef _AL_BOUNDARY_CHECK_H_
#define _AL_BOUNDARY_CHECK_H_

uint8_t boundaryCheck8(uint8_t val, uint8_t min, uint8_t max);
uint16_t boundaryCheck16(uint16_t val, uint16_t min, uint16_t max);
float boundaryCheckFloat(float val, float min, float max);

#endif /* _AL_BOUNDARY_CHECK_H_ */