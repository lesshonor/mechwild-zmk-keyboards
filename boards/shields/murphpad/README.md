# On MurphPad

## Using a nice!view

:warning: If you have [modified your MurphPad to power the display from `RAW`/`B+`](#connecting-display-vcc-to-raw), you cannot use a nice!view. The voltage on `RAW`/`B+` is too high and will damage it.

The nice!view's default CS pin is already used by the MurphPad's top-right encoder. There are no free pins on the MurphPad PCB.

- If you put a normal switch there, edit your keymap to delete the node that enables `&encoder_2`/`&top_right_encoder`, then edit the `sensors` node and `sensor-bindings` accordingly.
- If you put an encoder in the top right and want to keep it, you will need to wire the CS pin to one of the extra holes in the board—assuming it has any. (For example, the nice!nano has three in the center: `<&gpio1 1>` `<&gpio1 2>` and `<&gpio1 7>`)

---

## Connecting display VCC to RAW

If the board powering your MurphPad has a `RAW`/`B+` pin (most do) connecting `VCC` on the OLED to it will keep the display on when board `VCC` is turned off. This can be useful if you would like to turn RGB off without affecting the display.

Later revisions of the MurphPad add jumpers to simplify this process for boards with alternate power pins [in the same place as nice!nano `RAW`](https://nicekeyboards.com/docs/nice-nano/pinout-schematic), but this can be done on all versions.

- :warning: This modification requires you to intentionally damage the PCB by cutting traces.
- :warning: If your display isn't from MechWild, [make sure it can tolerate over `3.3V`](#using-a-niceview).
- :warning: Don't perform this modification without a battery if you are using a nice!nano v2. The `RAW` pin does not deliver consistent voltage without one, which almost always results in the OLED not turning on or turning back off immediately after boot.

<details>
<summary><h3>Instructions for MurphPad v3.1</h3></summary>

![photo by Tokolist#9634](https://i.imgur.com/Kr28HRM.png)

1. Disconnect OLED `VCC` from `LED8` by cutting the trace on the PCB.
2. Solder a wire connecting from the `RAW` pin on the PCB to the OLED `VCC` pin.
   - [@tokolist](https://github.com/tokolist) demonstrates an elegant solution by wiring `RAW` to the unused LED strip connection, but the wire can be connected directly.

:warning: If the LED strip is in use, you will also need to cut the trace between its `5V` and OLED `VCC`, and rewire the strip to board `VCC`.
</details>

<details>
<summary><h3>Instructions for MurphPad Rev2 v1.2</h3></summary>

![photo by @lesshonor](https://i.imgur.com/9Ifxaot.jpeg)

1. Disconnect OLED `VCC` from `LED8` by cutting the J1 jumper.
2. Connect OLED `VCC` to board `RAW` by closing the J2 jumper.

:warning: If your board's additional power pin is not in the same place, you will need to wire that yourself instead of closing J2.
</details>
