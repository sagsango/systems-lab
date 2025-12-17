#!/bin/bash -x
# 1. Log out of any existing iSCSI session
sudo iscsiadm -m node -T iqn.2025-12.com.example:target1 -p 127.0.0.1:3260 -u || true

# 2. Wait a moment for session to fully close
sleep 2

# 3. Completely delete the old target (this removes everything: LUNs, portals, ACLs, etc.)
sudo targetcli <<EOF
/iscsi delete iqn.2025-12.com.example:target1 || true
clearconfig confirm=true
saveconfig
exit
EOF

# 4. Remove old backing file if it exists
echo "TODO: You might need to run this cmd again!"
sudo rm -f /var/tmp/iscsi_backing.img

# 5. Optional: restart services to clear any cached state
sudo systemctl restart iscsid

sudo targetcli ls                  # Should show no iSCSI targets
sudo iscsiadm -m session           # Should show no sessions
lsblk | grep sda                   # /dev/sda (the 1G disk) should disappear
