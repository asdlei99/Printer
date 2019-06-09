# !/bin/sh

wpa_supplicant -iwlan0 -Dwext  -B -c/etc/wpa_supplicant.conf -dd
