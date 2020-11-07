/*
 * hid-kbd-custom.c
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>
#include <linux/slab.h>

#define USB_VENDOR_ID_HOLTEK_ALT                0x04d9
#define USB_DEVICE_ID_DIATEC_MAJESTOUCH_2_TKL	0x4545

static unsigned int fnkeycode = KEY_COMPOSE;
module_param(fnkeycode, uint, 0644);
MODULE_PARM_DESC(fnkeycode, "Keycode to use as FN_KEY");

// Basically my keyboard.
static const struct hid_device_id kbd_custom_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_HOLTEK_ALT, USB_DEVICE_ID_DIATEC_MAJESTOUCH_2_TKL) },
	{ }
};

struct kbd_custom_drvdata {
	bool fn_on;
};

struct kbd_custom_key_translation {
	u16 from;
	u16 to;
};

static const struct kbd_custom_key_translation kbd_custom_fn_keys[] = {
	{ KEY_F10,	KEY_MUTE	   },
	{ KEY_F11,	KEY_VOLUMEDOWN	   },
	{ KEY_F12,	KEY_VOLUMEUP	   },
	{ }
};

static u16 kbd_custom_translate_key(u16 from)
{
	const struct kbd_custom_key_translation *p;

	for (p = kbd_custom_fn_keys; p->from; p++) {
		if (p->from == from) {
			return p->to;
		}
	}

	return 0;
}

static int kbd_custom_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	struct kbd_custom_drvdata *drvdata;
	int ret;

	ret = hid_parse(hdev);
	if (ret) {
		hid_err(hdev, "hid_parse failed\n");
		return ret;
	}

	ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
	if (ret) {
		hid_err(hdev, "hid_hw_start failed\n");
		return ret;
	}

	drvdata = devm_kzalloc(&hdev->dev, sizeof(*drvdata), GFP_KERNEL);
	if (!drvdata) {
		hid_err(hdev, "Could not allocate memory for diver data\n");
		hid_hw_stop(hdev);
		return -ENOMEM;
	};

	drvdata->fn_on = 0;
	hid_set_drvdata(hdev, drvdata);

	return 0;
}

static int kbd_custom_event(struct hid_device *hdev, struct hid_field *field,
			    struct hid_usage *usage, __s32 value)
{
	struct kbd_custom_drvdata *drvdata = hid_get_drvdata(hdev);
	u16 code;

	if (usage->code == fnkeycode) {
		drvdata->fn_on = !!value;
		return 1;
	}

	if (drvdata->fn_on) {
		code = kbd_custom_translate_key(usage->code);
		if (code) {
			input_event(field->hidinput->input, usage->type, code, value);
			return 1;
		}
	}

	return 0;
}

MODULE_DEVICE_TABLE(hid, kbd_custom_devices);

static struct hid_driver kbd_custom_driver = {
	.name = "kbd_custom",
	.id_table = kbd_custom_devices,
	.probe = kbd_custom_probe,
	.event = kbd_custom_event
};
module_hid_driver(kbd_custom_driver);

MODULE_LICENSE("GPL"); // I mean, whatever.
