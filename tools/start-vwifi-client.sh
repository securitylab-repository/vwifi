#!/bin/bash

FOLDER_VWIFI='/hosthome/vwifi'

DEFAULT_PREFIX_MAC_ADDRESS='74:F8:F6'

# -------------------

cd "${FOLDER_VWIFI}"

if (( $# == 0 ))
then
	NbrWifi=2
else
	NbrWifi="$1"
fi

if [ -e /sys/module/mac80211_hwsim ]
then
	OldNbrWifi=$(cat /sys/module/mac80211_hwsim/parameters/radios)
	if (( NbrWifi != OldNbrWifi ))
	then
		modprobe -r mac80211_hwsim
	fi
fi

modprobe mac80211_hwsim radios=$NbrWifi

hexchars="0123456789ABCDEF"
for((i=0;i<NbrWifi;i++))
do
	middle=$( for i in {1..4} ; do echo -n ${hexchars:$(( $RANDOM % 16 )):1} ; done | sed -e 's/\(..\)/:\1/g' )
	MAC_ADDRESS="${DEFAULT_PREFIX_MAC_ADDRESS}${middle}:$(printf "%02d" $i)"
	echo "wlan$i : ${MAC_ADDRESS}"
	ip link set wlan$i addr ${MAC_ADDRESS}
done


if [ "$(tty)" = '/dev/ttyS0' ]
then
	# In Console Mode -> Tmux can be usefull
	if [ -v TMUX ]
	then
		# Already in tmux
		./vwifi-client
	else
		tmux new-session -s vwifi "bash --rcfile <(echo '. ~/.bashrc; ./vwifi-client')" ; detach &> /dev/null

		tmux attach -t vwifi
	fi
else
	./vwifi-client
fi
