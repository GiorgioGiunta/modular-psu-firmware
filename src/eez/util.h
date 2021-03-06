/*
* EEZ Generic Firmware
* Copyright (C) 2018-present, Envox d.o.o.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdint.h>
#include <stdlib.h>

#include <eez/unit.h>

#define clear_bit(reg, bitmask) *reg &= ~bitmask
#define set_bit(reg, bitmask) *reg |= bitmask
#define util_swap(type, i, j)                                                                      \
    {                                                                                              \
        type t = i;                                                                                \
        i = j;                                                                                     \
        j = t;                                                                                     \
    }

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#define PATH_SEPARATOR "/"

namespace eez {

float remap(float x, float x1, float y1, float x2, float y2);
float remapQuad(float x, float x1, float y1, float x2, float y2);
float remapOutQuad(float x, float x1, float y1, float x2, float y2);
float remapInOutQuad(float x, float x1, float y1, float x2, float y2);
float remapCubic(float x, float x1, float y1, float x2, float y2);
float remapOutCubic(float x, float x1, float y1, float x2, float y2);
float remapExp(float x, float x1, float y1, float x2, float y2);
float remapOutExp(float x, float x1, float y1, float x2, float y2);
float clamp(float x, float min, float max);

void strcatInt(char *str, int value);
void strcatInt32(char *str, int32_t value);
void strcatUInt32(char *str, uint32_t value);
void strcatFloat(char *str, float value);
void strcatFloat(char *str, float value, int numDecimalPlaces);

void strcatVoltage(char *str, float value);
void strcatCurrent(char *str, float value);
void strcatPower(char *str, float value);
void strcatDuration(char *str, float value);
void strcatLoad(char *str, float value);

uint32_t crc32(const uint8_t *message, size_t size);

uint8_t toBCD(uint8_t bin);
uint8_t fromBCD(uint8_t bcd);

float roundPrec(float a, float prec);
float floorPrec(float a, float prec);
float ceilPrec(float a, float prec);

bool isNaN(float x);
bool isNaN(double x);

bool isDigit(char ch);
bool isHexDigit(char ch);
bool isUperCaseLetter(char ch);

char toHexDigit(int num);
int fromHexDigit(char ch);

bool pointInsideRect(int xPoint, int yPoint, int xRect, int yRect, int wRect, int hRect);

void getParentDir(const char *path, char *parentDirPath);

bool parseMacAddress(const char *macAddressStr, size_t macAddressStrLength, uint8_t *macAddress);

int getIpAddressPartA(uint32_t ipAddress);
void setIpAddressPartA(uint32_t *ipAddress, uint8_t value);

int getIpAddressPartB(uint32_t ipAddress);
void setIpAddressPartB(uint32_t *ipAddress, uint8_t value);

int getIpAddressPartC(uint32_t ipAddress);
void setIpAddressPartC(uint32_t *ipAddress, uint8_t value);

int getIpAddressPartD(uint32_t ipAddress);
void setIpAddressPartD(uint32_t *ipAddress, uint8_t value);

void ipAddressToArray(uint32_t ipAddress, uint8_t *ipAddressArray);
uint32_t arrayToIpAddress(uint8_t *ipAddressArray);

uint32_t getIpAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

bool parseIpAddress(const char *ipAddressStr, size_t ipAddressStrLength, uint32_t &ipAddress);
void ipAddressToString(uint32_t ipAddress, char *ipAddressStr);

void macAddressToString(const uint8_t *macAddress, char *macAddressStr);

void formatTimeZone(int16_t timeZone, char *text, int count);
bool parseTimeZone(const char *timeZoneStr, size_t timeZoneLength, int16_t &timeZone);

void replaceCharacter(char *str, char ch, char repl);

int strcicmp(char const *a, char const *b);
int strncicmp(char const *a, char const *b, int n);
bool isStringEmpty(char const *a);
bool startsWith(const char *str, const char *prefix);
bool endsWith(const char *str, const char *suffix);
bool endsWithNoCase(const char *str, const char *suffix);

void formatBytes(uint64_t bytes, char *text, int count);

void getBaseFileName(const char *path, char *baseName, unsigned basenameSize);

} // namespace eez

#ifdef EEZ_PLATFORM_SIMULATOR_WIN32
char *strnstr(const char *s1, const char *s2, size_t n);
#endif