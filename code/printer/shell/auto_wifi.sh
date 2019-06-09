# !/bin/sh

if [ $ACTION = "add" ];
then
	wpa_supplicant -iwlan0 -Dwext  -B -c/etc/wpa_supplicant.conf -dd
	wpa_cli -a/usr/shell/wpa_action.sh -B
else
	killall wpa_supplicant
	killall dhclient
fi
