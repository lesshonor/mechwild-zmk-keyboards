/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

/* TODO: does using DRV_COMPAT make sense here??? */
#define DT_DRV_COMPAT mechwild_indicators

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led.h>
#include <zephyr/init.h>
/*#include <zephyr/kernel.h>*/
#include <zephyr/logging/log.h>

#include <zmk/hid_indicators.h>

#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/hid_indicators_changed.h>

#include <dt-bindings/zmk/hid_usage.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

/* TODO: Store or re-obtain the current indicators somehow probably? */

/*
 * TODO: maybe borrow the low-priority RGB queue?
K_WORK_DEFINE(indicator_tick_work, zmk_indicator_tick);
K_TIMER_DEFINE(indicator_tick, zmk_indicator_tick_handler, NULL);
*/

#define INDICATOR_INST(n) \
{ \
    .dev = DEVICE_DT_GET(COND_CODE_1(DT_NODE_HAS_PROP(DT_PHANDLE(n, dev), chain_length), (DT_PHANDLE(n, dev)), (DT_PARENT(DT_PHANDLE(n, dev))))), \
    .leds = COND_CODE_1(DT_NODE_HAS_PROP(n, leds), (DT_PROP(n, leds)), (DT_NODE_CHILD_IDX(DT_PHANDLE(n, dev)))), \
    .hid_indicators = BIT(DT_PROP(n, hid_indicators) - 1), \
    .layers = DT_PROP(n, layers) \
}

struct indicator_cfg {
    const struct device *dev;
    uint8_t hid_indicators;
    uint8_t leds;
    int8_t layers[32]; // maximum of 32 layers...
};

static struct indicator_cfg indicator_configs[] = {
    DT_INST_FOREACH_CHILD_STATUS_OKAY_SEP(0, INDICATOR_INST, (,))
};

static int hid_indicators_changed_listener(const zmk_event_t *eh) {
    struct zmk_hid_indicators_changed *data = as_zmk_hid_indicators_changed(eh);
    if (data == NULL) {
        LOG_DBG("No indicator data sent.");
        return ZMK_EV_EVENT_BUBBLE;
    }
    for (int i = 0; i < ARRAY_SIZE(indicator_configs); i++) {
        if ((data->indicators) & (indicator_configs[i].hid_indicators)) {
            LOG_DBG("new indicators %d, matched indicator %d on led %d", data->indicators, indicator_configs[i].hid_indicators, indicator_configs[i].leds);
            led_set_brightness(indicator_configs[i].dev, indicator_configs[i].leds, 5);
        } else {
            LOG_DBG("new indicators %d, did not match %d on led %d", data->indicators, indicator_configs[i].hid_indicators, indicator_configs[i].leds);
            led_off(indicator_configs[i].dev, indicator_configs[i].leds);
        }
    }
    return ZMK_EV_EVENT_BUBBLE;
}

static int indicators_event_listener(const zmk_event_t *eh) {
    if (as_zmk_hid_indicators_changed(eh)) {
        LOG_DBG("HID indicator change.");
        return hid_indicators_changed_listener(eh);
    } else if (as_zmk_layer_state_changed(eh)) {
        LOG_DBG("Layer change.");
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(indicators, indicators_event_listener);
ZMK_SUBSCRIPTION(indicators, zmk_hid_indicators_changed);
ZMK_SUBSCRIPTION(indicators, zmk_layer_state_changed);
