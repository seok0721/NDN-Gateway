#!/bin/sh
# 빌드 스크립트 입니다.

DEVICE_NAME="ssm_echo"
MAJOR=$(cat /proc/devices | grep $DEVICE_NAME | cut -d ' ' -f 1)

echo "Build user program.";
gcc -o ${DEVICE_NAME}_user ${DEVICE_NAME}_user.c

if [ -e "/dev/$DEVICE_NAME" ]; then
  echo "\"$DEVICE_NAME\" module's character file already exists.";
  exit 0;
fi

if [ -z "$MAJOR" ]; then
  echo "\"$DEVICE_NAME\" module is not inserted.";
  exit 1;
fi

echo "Character device major number: $MAJOR";
echo "Make \"$DEVICE_NAME\" module's character file.";

sudo mknod "/dev/$DEVICE_NAME" c $MAJOR 1;

exit 0;
