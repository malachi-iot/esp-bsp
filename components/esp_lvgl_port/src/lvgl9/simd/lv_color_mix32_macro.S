/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

 .macro color_mix32 FG, BG, FG_OPA, a, b, c, d
// This macro mixes 32bit color with Opacity

//bg.red = (uint32_t)((uint32_t)fg.red * fg.alpha + (uint32_t)bg.red * (255 - fg.alpha)) >> 8;
//bg.green = (uint32_t)((uint32_t)fg.green * fg.alpha + (uint32_t)bg.green * (255 - fg.alpha)) >> 8;
//bg.blue = (uint32_t)((uint32_t)fg.blue * fg.alpha + (uint32_t)bg.blue * (255 - fg.alpha)) >> 8;

// FG       - Foreground color ARGB8888
// BG       - Background color ARGB8888
// FG_OPA   - Foreground opacity
// a, b, c  - working registers
// d        - resulting color (output register)

    // Extract alpha and prepare constants
    movi  \a, 255                          // a = 255
    sub   \b, \a, \FG_OPA                  // b = 255 - FG OPA

    // Mix Red channel
    extui    \a,   \FG,   16,   8          // a = fg.red
    extui    \c,   \BG,   16,   8          // c = bg.red
    mul16u   \a,   \a,   \FG_OPA           // fg.red * fg.alpha
    mul16u   \c,   \c,   \b                // bg.red * (255 - fg.alpha)
    add      \a,   \a,   \c                // a = fg.red * fg.alpha + bg.red * (255 - fg.alpha)
    srli     \a,   \a,   8                 // Normalize: a = a >> 8
    slli     \d,   \a,   16                // Shift red to correct place

    // Mix Green channel
    extui    \a,   \FG,   8,   8           // a = fg.green
    extui    \c,   \BG,   8,   8           // c = bg.green
    mul16u   \a,   \a,   \FG_OPA           // fg.green * fg.alpha
    mul16u   \c,   \c,   \b                // bg.green * (255 - fg.alpha)
    add      \a,   \a,   \c                // a = fg.green * fg.alpha + bg.green * (255 - fg.alpha)
    srli     \a,   \a,   8                 // Normalize: a = a >> 8
    slli     \a,   \a,   8                 // Shift red to correct place
    or       \d,   \a,   \d                // Combine red and green to d

    // Mix Blue channel
    extui    \a,   \FG,   0,   8           // a = fg.blue
    extui    \c,   \BG,   0,   8           // c = bg.blue
    mul16u   \a,   \a,   \FG_OPA           // fg.blue * fg.alpha
    mul16u   \c,   \c,   \b                // bg.blue * (255 - fg.alpha)
    add      \a,   \a,   \c                // a = fg.blue * fg.alpha + bg.blue * (255 - fg.alpha)
    srli     \a,   \a,   8                 // Normalize: a = a >> 8
    or       \d,   \a,   \d                // Combine all colors to d

.endm
