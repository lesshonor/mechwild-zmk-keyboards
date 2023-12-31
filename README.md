# MechWild's ZMK Keyboard Repo

## Instructions

Add this repository to your existing `config/west.yml` as shown below. (No idea what that means? [Check this repository instead!](https://github.com/lesshonor/mechwild-zmk-config))

```yaml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware

    # Add the users with the repositories you need to this list.
    - name: mechwild
      url-base: https://github.com/mechwild
    - name: lesshonor
      url-base: https://github.com/lesshonor
  projects:
    - name: zmk
      remote: zmkfirmware
      revision: main
      import: app/west.yml
    - name: mechwild-zmk-keyboards # Add the name of the repository as a project.
      remote: lesshonor            # This has to match the name for the user you put in the "remote" earlier.
      revision: main               # This is the the branch of the repository you need. Most of the time you can leave this as main.
  self:
    path: config
```

After doing so, copy any keymaps you may need out of the appropriate `board` and/or [`shield`](boards/shields/) folders and paste them into your `config` folder for later modification. Don't forget to edit your `build.yaml`.

If you are migrating to this repository from an older MechWild ZMK repository, you may have copies of the board or shield definitions in your `config/boards` or `boards` folders. You should delete those copies; they will override the definitions in this module and likely cause problems.

---

## More Information

### Supported Boards/Shields

| Boards                                    | Shields                                     |
| ----------------------------------------- | ------------------------------------------- |
| BlackPill F401CE[^1] (`blackpill_f401ce`) | BB40 (`bb40`)                               |
| BlackPill F411CE[^1] (`blackpill_f411ce`) | BB65 (`bb65`)                               |
| PillBug v0.1 (`pillbug@0.1`)              | BBPad (`bbpad`)                             |
| PillBug v1.0 (`pillbug`)                  | BDE Rev2 (`bde_rev2`)                       |
| S40NC[^2] (`s40nc`)                       | Mokulua (`mokulua_left`, `mokulua_right`)   |
|                                           | MurphPad (`murphpad`)                       |
|                                           | OBE (`obe`)                                 |
|                                           | Waka60 (`waka60`)                           |

[^1]: Some features are not operational. QMK has the strongest support for this hardware.
[^2]: The S40NC board files are [still hosted in the ZMK main repository](https://github.com/zmkfirmware/zmk/tree/main/app/boards/arm/s40nc) for the time being.

If you bought a PillBug expressly to use with a currently-unsupported shield: file a support ticket [in MechWild's Discord](https://discord.gg/nfxHnsm). Beta/non-public firmware may be available.

<!--
### QDEC Encoder Driver

nRF MCUs can use a dedicated encoder peripheral which may result in better performance. To try it out, replace the `&encoder` node at the top of your keymap with the following:

```dts
&qdec_encoder {
    status = "okay";
};

&sensors {
    sensors = <&qdec_encoder>;
};
```

This driver can only be used with one encoder per board and there are complications with split configuration, so this has not been implemented for Mokulua, MurphPad, Sugar Glider or PuckBuddy. [Reach out to MechWild](https://discord.gg/nfxHnsm) if you have one of these and would like help trying the QDEC driver anyway.
-->

### Alternate Keymap Transforms

If the keyboard you're working with supports multiple layouts, you can switch to them by adding the alternate transform to the top of your keymap file. This can make editing the keymap a little easier in general, and the alternate layout will be shown in [nickcoutsous's Keymap Editor](https://nickcoutsous.github.io/keymap-editor).

E.g., to use the mirrored layout of a Mokulua:
```dts
chosen { zmk,matrix_transform = &mirrored_transform; };
```

All the available layouts a keyboard supports are listed in its `.overlay` file in this repository, and in its Keymap Editor `.json` [over in the MechWild ZMK config repo](https://github.com/lesshonor/mechwild-zmk-config/tree/main/config). If a layout you want is missing, you are welcome to ask about it in Discord or open a GitHub issue.

## Known Issues

### OLEDs

- Affects: MurphPad

If [power is cut to the OLED](https://zmk.dev/docs/behaviors/power#external-power-control-command-defines), it will not automatically turn back on when power is restored. (ZMK issue [#674](https://github.com/zmkfirmware/zmk/issues/674))

Workarounds:

1. Wait at least one minute and press the physical reset button on the board/PCB once.
2. If `CONFIG_ZMK_RGB_UNDERGLOW_EXT_POWER=y` is set in your configuration file, turn on RGB. Then try step 1.
3. Add `&ext_power EP_ON` to your keymap and press the key to *make sure* external power is on. Then try step 2.
4. [Modify the PCB](https://github.com/lesshonor/mechwild-zmk-keyboards/tree/main/boards/shields/murphpad#connecting-display-vcc-to-raw) so the OLED is powered from RAW instead of VCC.

### Indicator LEDs (single color)

- Affects: BB40, OBE

HID indicators (caps/num/scroll/kana lock) don't work yet. *This is in progress.*

### Piezo Speaker

- Affects: Waka60

ZMK does not currently have any form of audio support.

### External EEPROM

- (Potentially) Affects: Waka60, OBE

If you have the now-optional discrete EEPROM IC installed, ZMK doesn't use it. *This doesn't impact anything.*

## Warnings/Caveats

### nice!view (and other low-voltage displays)

(***Pro Micro interconnect shields only***) The following MechWild PCBs connect the display power pin (`VCC`) directly to the `RAW`/`B+` pins. Battery or no battery, this means *you cannot swap in a nice!view* unless you rewire the display to `VCC` on the board.

- BDE Rev2
- Mokulua
- MurphPad *[if you intentionally modified the PCB](https://github.com/lesshonor/mechwild-zmk-keyboards/tree/main/boards/shields/murphpad#connecting-display-vcc-to-raw)*

The nice!view cannot tolerate over 3.6V and will be damaged by higher voltages. The same may be true for other aftermarket (non-OLED) displays. If you're not sure about yours, find out before powering anything up.

### RGB

If wireless is less important to you than full-featured and customizable RGB, ZMK may not be the best fit at this point in time. [The "underglow" feature](https://zmk.dev/docs/features/underglow) is relatively limited, and enabling RGB LEDs can shorten battery life from multiple months to [less than 24 hours](https://zmk.dev/power-profiler).

Consider QMK instead; [MechWild's keyboards are well-supported there](https://github.com/qmk/qmk_firmware/tree/master/keyboards/mechwild) when built with compatible devevelopment boards. (Wireless MCUs such as those used in the nice!nano and PillBug are not compatible.)

#### RGB on BlackPills

Until a compatible GPIO or PWM driver is written for Zephyr, BlackPills have additional limitations on RGB support.

| Shield         | F401               | F411               |
| -------------- | ---------          | ---------          |
| BB65           | :x:                | :white_check_mark: |
| OBE            | :x:                | :x:                |
| Waka60         | :x:                | :white_check_mark: |
<!--
| PuckBuddy      | :x:                | :x:                |
| Sugar Glider   | :white_check_mark: | :white_check_mark: |
-->
