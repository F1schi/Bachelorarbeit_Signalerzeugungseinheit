/*
 * AL_boundary-check.c
 *
 * Author: Andreas Fischer
 */
#include <Arduino.h>
#include "AL_boundary_check.h"

/*
 * @brief Check if value is within boundarys (uint8_t)
 * @param val the value to check
 * @param min lower boundary
 * @param max high boundary
 * @return value which is within the boundary
 */
uint8_t boundaryCheck8(uint8_t val, uint8_t min, uint8_t max)
{
    if (val >= max)
        return max;
    else if (val <= min)
        return min;
    else
        return val;
}

/*
 * @brief Check if value is within boundarys (uint16_t)
 * @param val the value to check
 * @param min lower boundary
 * @param max high boundary
 * @return value which is within the boundary
 */
uint16_t boundaryCheck16(uint16_t val, uint16_t min, uint16_t max)
{
    if (val >= max)
        return max;
    else if (val <= min)
        return min;
    else
        return val;
}

/*
 * @brief Check if value is within boundarys (float)
 * @param val the value to check
 * @param min lower boundary
 * @param max high boundary
 * @return value which is within the boundary
 */
float boundaryCheckFloat(float val, float min, float max)
{
    if (val >= max)
        return max;
    else if (val <= min)
        return min;
    else
        return val;
}