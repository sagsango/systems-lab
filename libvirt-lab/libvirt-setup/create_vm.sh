#!/bin/bash
set -e

# Variables
VM_NAME="testvm"
RAM_MB=2048
VCPUS=2
DISK_SIZE_GB=20
ISO_PATH="/var/lib/libvirt/boot/ubuntu-24.04.3-live-server-amd64.iso"
DISK_PATH="/var/lib/libvirt/images/${VM_NAME}.qcow2"
NETWORK_XML="$(pwd)/default.xml"

# Step 1: Define and start default network if not already defined
if ! virsh net-info default &>/dev/null; then
    echo "Defining default network..."
    virsh net-define "$NETWORK_XML"
fi

virsh net-start default || echo "Default network already active"
virsh net-autostart default

# Step 2: Create VM disk if not exists
if [ ! -f "$DISK_PATH" ]; then
    echo "Creating QCOW2 disk..."
    qemu-img create -f qcow2 "$DISK_PATH" "${DISK_SIZE_GB}G"
fi

# Step 3: Install VM using virt-install
virt-install \
  --name testvm \
  --ram 3072 \
  --vcpus 2 \
  --disk path=/var/lib/libvirt/images/testvm.qcow2,size=20,format=qcow2 \
  --os-variant ubuntu24.04 \
  --network network=default,model=virtio \
  --graphics vnc,listen=0.0.0.0 \
  --cdrom /var/lib/libvirt/boot/ubuntu-24.04.3-live-server-amd64.iso

echo "VM $VM_NAME installation started. Connect via:"
echo "  virsh console $VM_NAME"

