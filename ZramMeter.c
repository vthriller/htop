/*
htop - ZramMeter.c
(C) 2004-2016 Hisham H. Muhammad
Released under the GNU GPL, see the COPYING file
in the source distribution for its full text.
*/

#include "ZramMeter.h"

#include "CRT.h"
#include "Platform.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/param.h>
#include <assert.h>

/*{
#include "Meter.h"
}*/

int ZramMeter_attributes[] = {
   ZRAM_COMPRESSED, ZRAM_UNCOMPRESSED
};

static void ZramMeter_updateValues(Meter* this, char* buffer, int size) {
   int written;
   Platform_setZramValues(this);

   written = Meter_humanUnit(buffer, this->values[0] + this->values[1], size);
   buffer += written;
   if ((size -= written) > 0) {
      *buffer++ = '/';
      size--;
      Meter_humanUnit(buffer, this->total, size);
   }
}

static void ZramMeter_display(Object* cast, RichString* out) {
   char buffer[50];
   Meter* this = (Meter*)cast;
   RichString_write(out, CRT_colors[METER_TEXT], ":");

   Meter_humanUnit(buffer, this->total, 50);
   RichString_append(out, CRT_colors[METER_VALUE], buffer);

   Meter_humanUnit(buffer, this->values[0] + this->values[1], 50);
   RichString_append(out, CRT_colors[METER_TEXT], " uncompressed:");
   RichString_append(out, CRT_colors[ZRAM_UNCOMPRESSED], buffer);

   Meter_humanUnit(buffer, this->values[0], 50);
   RichString_append(out, CRT_colors[METER_TEXT], " compressed:");
   RichString_append(out, CRT_colors[ZRAM_COMPRESSED], buffer);

   RichString_append(out, CRT_colors[METER_TEXT], " ratio: ");
   if (this->values[0] + this->values[1]) {
      snprintf(buffer, 50, "%.1fx", (double) (this->values[0] + this->values[1]) / this->values[0]);
      RichString_append(out, CRT_colors[ZRAM_COMPRESSED], buffer);
   } else {
      RichString_append(out, CRT_colors[ZRAM_COMPRESSED], "-");
   }
}

MeterClass ZramMeter_class = {
   .super = {
      .extends = Class(Meter),
      .delete = Meter_delete,
      .display = ZramMeter_display,
   },
   .updateValues = ZramMeter_updateValues,
   .defaultMode = BAR_METERMODE,
   .maxItems = 2,
   .total = 100.0,
   .attributes = ZramMeter_attributes,
   .name = "Zram",
   .uiName = "Zram",
   .caption = "Zrm"
};
