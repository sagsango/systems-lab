#!/bin/bash -x

set -e

BACKING_FILE="/var/tmp/iscsi_backing.img"
BACKING_SIZE="1G"
BACKING_NAME="backing"
IQN_TARGET="iqn.2025-12.com.example:target1"
IQN_INITIATOR="iqn.2025-12.com.example:initiator1"
PORTAL_IP="0.0.0.0"
PORTAL_PORT="3260"

echo "Creating backing file..."
truncate -s $BACKING_SIZE $BACKING_FILE

echo "Configuring iSCSI target..."
targetcli <<EOF
/backstores/fileio create $BACKING_NAME $BACKING_FILE write_back=false
/iscsi create $IQN_TARGET
/iscsi/$IQN_TARGET/tpg1 set attribute authentication=0 demo_mode_write_protect=0 generate_node_acls=1 cache_dynamic_acls=1
/iscsi/$IQN_TARGET/tpg1/luns create /backstores/fileio/$BACKING_NAME
/iscsi/$IQN_TARGET/tpg1/portals delete 127.0.0.1 3260 || true
/iscsi/$IQN_TARGET/tpg1/portals create $PORTAL_IP $PORTAL_PORT
saveconfig
exit
EOF

echo "Setting up initiator..."
echo "InitiatorName=$IQN_INITIATOR" | tee /etc/iscsi/initiatorname.iscsi > /dev/null
systemctl restart iscsid

echo "Logging in..."
iscsiadm -m discovery -t sendtargets -p 127.0.0.1:$PORTAL_PORT
iscsiadm -m node -T $IQN_TARGET -p 127.0.0.1:$PORTAL_PORT --op update -n node.startup -v automatic
iscsiadm -m node -T $IQN_TARGET -p 127.0.0.1:$PORTAL_PORT -l

echo "Done! Waiting 10 seconds for device attachment..."
sleep 10
echo "Current sessions:"
iscsiadm -m session -P 3
echo "Block devices:"
lsblk
