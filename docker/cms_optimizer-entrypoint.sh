#!/bin/bash
set -e
USER_ID=${LOCAL_UID:-9001}
GROUP_ID=${LOCAL_GID:-9001}

#echo "updated"
#cut -d: -f1 /etc/group | sort
echo "Starting SSH"
service ssh restart
echo "Starting__ with UID: $USER_ID, GID: $GROUP_ID"
usermod -u $USER_ID simpl
groupmod -g $GROUP_ID simpl
export HOME=/home/simpl
id simpl
exec /usr/sbin/gosu simpl /bin/bash
echo "Done with startup"