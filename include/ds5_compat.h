/*
 * Compatibility layer: types that differ between OpenOrbis toolchain releases.
 *
 * Older toolchains declare OrbisPadData::touch as a raw uint8_t[24] and do not
 * declare OrbisPadInformation at all, so the project uses its own copies. The
 * layouts match the PS4 libScePad ABI (same sizes as the newer toolchain
 * headers).
 */
#ifndef DS5_COMPAT_H
#define DS5_COMPAT_H

#include <stdint.h>
#include <stddef.h>
#include <orbis/_types/pad.h>

typedef struct {
    uint16_t x, y;
    uint8_t  finger;
    uint8_t  pad[3];
} Ds5PadTouch;                      // 8 bytes

// Motion block of OrbisPadData (orientation quaternion, acceleration in g,
// angular velocity in rad/s). Accessed by offset because the field names differ
// between toolchain releases (quat/vel/acell vs orientation/acceleration/...).
typedef struct {
    float quat[4];      // x y z w
    float accel[3];
    float gyro[3];
} Ds5PadMotion;                     // 40 bytes, starts at offset 12

_Static_assert(offsetof(OrbisPadData, touch) == 52, "unexpected OrbisPadData layout");

static inline Ds5PadMotion* ds5_motion(OrbisPadData* p) {
    return (Ds5PadMotion*)(void*)((char*)p + 12);
}

typedef struct {
    uint8_t     fingers;
    uint8_t     pad1[3];
    uint32_t    pad2;
    Ds5PadTouch touch[2];
} Ds5PadTouchData;                  // 24 bytes (== sizeof OrbisPadData::touch)

// Works whether OrbisPadData::touch is a struct or a byte array
static inline Ds5PadTouchData* ds5_touch(OrbisPadData* p) {
    return (Ds5PadTouchData*)(void*)&p->touch;
}

typedef struct {
    float    touchpadDensity;
    uint16_t touchResolutionX;
    uint16_t touchResolutionY;
    uint8_t  stickDeadzoneL;
    uint8_t  stickDeadzoneR;
    uint8_t  connectionType;
    uint8_t  count;
    int32_t  connected;
    int32_t  deviceClass;
    uint8_t  unknown[8];
} Ds5PadInformation;

typedef struct {
    uint8_t lgMotor;
    uint8_t smMotor;
} Ds5PadVibeParam;

typedef struct {
    uint8_t r, g, b, a;
} Ds5PadColor;

#endif // DS5_COMPAT_H
