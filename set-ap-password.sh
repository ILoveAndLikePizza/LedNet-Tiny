#!/bin/bash

echo "You are about to generate a password for the LedNet Tiny access point."
echo "Note that this password is saved in plain text in this repository."
echo -e "\e[31;1mDO NOT USE A PASSWORD THAT YOU ALREADY USE SOMEWHERE ELSE!\e[0m\n"

read -sp "Please enter a password: " AP_PASSWORD_RAW
AP_PASSWORD=$(echo $AP_PASSWORD_RAW | sed -E 's/("|\\)/\\\1/g')

if [[ "${#AP_PASSWORD}" < "8" ]]; then
    echo -e "\n\n\e[33;1mThe password must be 8 or more characters to work!"
    exit 1
fi

echo "const char *AP_PASSWORD = \"$AP_PASSWORD\";" > Authentication.h \
 && echo -e "\n\n\e[32;1mThe password has been saved successfully!\n\e[37;1m→ Now, you can launch the Arduino IDE and flash the firmware.\e[0m" \
 || echo -e "\n\n\e[31;1mAn error occured while saving the password.\e[0m"
