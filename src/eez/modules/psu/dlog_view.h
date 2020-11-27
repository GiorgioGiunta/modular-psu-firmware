/*
* EEZ PSU Firmware
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

#include <eez/gui/gui.h>
#include <eez/gui/widgets/yt_graph.h>
#include <eez/modules/psu/trigger.h>
#include <eez/modules/psu/dlog_view.h>

/* DLOG File Format

OFFSET    TYPE    WIDTH    DESCRIPTION
----------------------------------------------------------------------
0               U32     4        MAGIC1 = 0x2D5A4545L

4               U32     4        MAGIC2 = 0x474F4C44L

8               U16     2        VERSION = 0x0001L

10              U16     2        Flags: RRRR RRRR RRRR RRRJ
                                    J - Jitter enabled
                                    R - Reserverd

12              U32     4        Columns: RRRR RRRR RPIU RPIU RPIU RPIU RPIU RPIU
                                                     Ch6  Ch5  Ch4  Ch3  Ch2  Ch1
                                    U - voltage
                                    I - current
                                    P - power
                                    R - reserved

16              Float   4        Period

20              Float   4        Duration

24              U32     4        Start time, timestamp

28+(n*N+m)*4    Float   4        n-th row and m-th column value, N - number of columns
*/

namespace eez {
namespace psu {
namespace dlog_view {

static const uint32_t MAGIC1 = 0x2D5A4545;
static const uint32_t MAGIC2 = 0x474F4C44;
static const uint16_t VERSION1 = 1;
static const uint16_t VERSION2 = 2;
static const uint32_t DLOG_VERSION1_HEADER_SIZE = 28;

static const int VIEW_WIDTH = 480;
static const int VIEW_HEIGHT = 240;

static const int NUM_HORZ_DIVISIONS = 12;
static const int NUM_VERT_DIVISIONS = 6;

static const int MAX_NUM_OF_Y_AXES = CH_MAX * 3;

static const int MAX_COMMENT_LENGTH = 128;

enum State {
    STATE_STARTING,
    STATE_LOADING,
    STATE_ERROR,
    STATE_READY
};

enum Fields {
    FIELD_ID_COMMENT = 1,

    FIELD_ID_X_UNIT = 10,
    FIELD_ID_X_STEP = 11,
    FIELD_ID_X_RANGE_MIN = 12,
    FIELD_ID_X_RANGE_MAX = 13,
    FIELD_ID_X_LABEL = 14,
    FIELD_ID_X_SCALE = 15, // 0 - linear, 1 - logarithmic

    FIELD_ID_Y_UNIT = 30,
    FIELD_ID_Y_RANGE_MIN = 32,
    FIELD_ID_Y_RANGE_MAX = 33,
    FIELD_ID_Y_LABEL = 34,
    FIELD_ID_Y_CHANNEL_INDEX = 35,
    FIELD_ID_Y_SCALE = 36,

    FIELD_ID_CHANNEL_MODULE_TYPE = 50,
    FIELD_ID_CHANNEL_MODULE_REVISION = 51
};

struct Range {
    float min;
    float max;
};

static const int MAX_LABEL_LENGTH = 32;

enum Scale {
    SCALE_LINEAR,
    SCALE_LOGARITHMIC
};

struct XAxis {
    Unit unit;
    float step;
    Scale scale;
    Range range;
    char label[MAX_LABEL_LENGTH + 1];
};

struct YAxis {
    Unit unit;
    Range range;
    char label[MAX_LABEL_LENGTH + 1];
    int8_t channelIndex;
};

enum LogItemType {
    LOG_ITEM_TYPE_U,
    LOG_ITEM_TYPE_I,
    LOG_ITEM_TYPE_P,
    LOG_ITEM_TYPE_DIN0,
    LOG_ITEM_TYPE_DIN1,
    LOG_ITEM_TYPE_DIN2,
    LOG_ITEM_TYPE_DIN3,
    LOG_ITEM_TYPE_DIN4,
    LOG_ITEM_TYPE_DIN5,
    LOG_ITEM_TYPE_DIN6,
    LOG_ITEM_TYPE_DIN7,
};

struct LogItem {
    uint8_t slotIndex;
    uint8_t subchannelIndex;
    uint8_t type;
};

struct Parameters {
    char filePath[MAX_PATH_LENGTH + 1];
    char comment[MAX_COMMENT_LENGTH + 1];

    XAxis xAxis;

    YAxis yAxis;
    Scale yAxisScale;

    uint8_t numYAxes;
    YAxis yAxes[MAX_NUM_OF_Y_AXES];

    LogItem logItems[MAX_NUM_OF_Y_AXES];
    int numLogItems;
    eez_err_t enableLogItem(int slotIndex, int subchannelIndex, LogItemType type, bool enable);
    bool isLogItemEnabled(int slotIndex, int subchannelIndex, LogItemType type);

    float period;
    float time;
    trigger::Source triggerSource;

private:
    bool findLogItemIndex(int slotIndex, int subchannelIndex, LogItemType type, int &logItemIndex);
};

struct DlogValueParams {
    bool isVisible;
    float offset;
    float div;
};

struct Recording {
    Parameters parameters;

    DlogValueParams dlogValues[MAX_NUM_OF_Y_VALUES];

    uint32_t size;
    uint32_t pageSize;

    float xAxisOffset;
    float xAxisDiv;

    uint32_t cursorOffset;

    float (*getValue)(uint32_t rowIndex, uint8_t columnIndex, float *max);

    uint32_t refreshCounter;

    uint32_t numSamples;
    float xAxisDivMin;
    float xAxisDivMax;

    uint32_t dataOffset;

    uint8_t selectedVisibleValueIndex;
};

extern bool g_showLatest;

// open dlog file for viewing
bool openFile(const char *filePath, int *err = nullptr);

extern State getState();

// this is called from the thread that owns SD card
void loadBlock();

// this should be called during GUI state managment phase
void stateManagment();

Recording &getRecording();

void initAxis(Recording &recording);
void initYAxis(Parameters &parameters, int yAxisIndex);
void initDlogValues(Recording &recording);
int getNumVisibleDlogValues(const Recording &recording);
int getDlogValueIndex(Recording &recording, int visibleDlogValueIndex);
int getVisibleDlogValueIndex(Recording &recording, int dlogValueIndex);
DlogValueParams *getVisibleDlogValueParams(Recording &recording, int visibleDlogValueIndex);
bool isMulipleValuesOverlayHeuristic(Recording &recording);
Unit getXAxisUnit(Recording& recording);
Unit getYAxisUnit(Recording& recording, int dlogValueIndex);

uint32_t getPosition(Recording& recording);
void changeXAxisOffset(Recording &recording, float xAxisOffset);
void changeXAxisDiv(Recording &recording, float xAxisDiv);
float getDuration(Recording &recording);
void getLabel(Recording& recording, int valueIndex, char *text, int count);

void autoScale(Recording &recording);
void scaleToFit(Recording &recording);

float roundValue(float value);

void uploadFile();

eez::gui::SetPage *getParamsPage();

} // namespace dlog_view
} // namespace psu
} // namespace eez
