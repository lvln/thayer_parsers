/* 
 * bit2byte.h --- convert spp bit header to byte header
 * 
 * Author: Stephen Taylor
 * Created: 10-07-2024
 * Version: 1.0
 * 
 * Description: A module that can convert a header or an entire packet 
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
/*
 * Converts an spp packet 6-byte bit-based header into a 10-byte based header
 */
void bit2byteHeader(uint8_t bits[6], uint8_t bytes[10]);
/*
 * Converts an spp packet into a byte based header followed by its payload
 */
void bit2bytePacket(uint8_t bits[],int len, uint8_t bytes[], int *newlen);


