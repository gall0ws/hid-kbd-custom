# hid-kbd-custom
Linux module example to add an "Fn" key to your old-fashioned keyboard.

Module is activated just on my keyboard (Filco Majestouch 2 TKL), and it replaces a custom key with a software Fn key.

By default Fn is `KEY_COMPOSE` (that is Menu on my keyboard), but it can be changed with the `fnkeycode` parameter.

Default mapping is pretty basic:

* Fn + F10: Volume Mute
* Fn + F11: Volume Down
* Fn + F12: Volume Up

# FAQ

### Couldn't you just use a random userspace tool to do that?
Probably yes.
