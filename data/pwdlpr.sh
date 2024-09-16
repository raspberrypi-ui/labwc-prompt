#!/bin/bash
export TEXTDOMAIN=labwc-prompt

. gettext.sh

zenity --password --title "$(gettext "Password Required")"

