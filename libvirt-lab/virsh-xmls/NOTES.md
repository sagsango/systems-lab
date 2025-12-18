# What is supported in xml
schemas ❱❱❱ ls
basictypes.rng  domainbackup.rng      domainoverrides.rng  interface.rng      nodedev.rng          privatedata.rng      storagepool.rng
capability.rng  domaincaps.rng        domain.rng           networkcommon.rng  nwfilterbinding.rng  secret.rng           storagevol.rng
cpu.rng         domaincheckpoint.rng  domainsnapshot.rng   networkport.rng    nwfilter_params.rng  storagecommon.rng
cputypes.rng    domaincommon.rng      inactiveDomain.rng   network.rng        nwfilter.rng         storagepoolcaps.rng
schemas ❱❱❱


# cmds
virt-xml-validate <XML>
virsh define <XML>
virsh start name
virsh list
virsh shutdown testvm

# state
root@silicon:/run/libvirt# ls /var/lib/libvirt/swtpm/
c60cb91a-1b8f-413e-9632-c511de181592  my-vm-tpm-dir  testvm
root@silicon:/run/libvirt# ls /var/lib/libvirt/swtpm/c60cb91a-1b8f-413e-9632-c511de181592/
tpm2
root@silicon:/run/libvirt# ls /var/lib/libvirt/swtpm/c60cb91a-1b8f-413e-9632-c511de181592/tpm2/
tpm2-00.permall
root@silicon:/run/libvirt# ls /run/libvirt
common      interface           libvirt-sock     network  nwfilter          qemu     storage               virtlockd-sock       virtlogd-sock
hostdevmgr  libvirt-admin-sock  libvirt-sock-ro  nodedev  nwfilter-binding  secrets  virtlockd-admin-sock  virtlogd-admin-sock
root@silicon:/run/libvirt#

root@silicon:/run/libvirt# ls /run/libvirt
common      interface           libvirt-sock     network  nwfilter          qemu     storage               virtlockd-sock       virtlogd-sock
hostdevmgr  libvirt-admin-sock  libvirt-sock-ro  nodedev  nwfilter-binding  secrets  virtlockd-admin-sock  virtlogd-admin-sock
root@silicon:/run/libvirt# ls /run/libvirt
common      interface           libvirt-sock     network  nwfilter          qemu     storage               virtlockd-sock       virtlogd-sock
hostdevmgr  libvirt-admin-sock  libvirt-sock-ro  nodedev  nwfilter-binding  secrets  virtlockd-admin-sock  virtlogd-admin-sock
root@silicon:/run/libvirt# cd /run/
root@silicon:/run# find * | grep xml
libvirt/qemu/testvm.xml
libvirt/storage/boot.xml
libvirt/storage/libvirt-setup.xml
libvirt/network/default.xml
libvirt/network/default/ports/ae6abcc6-1187-4636-a364-2199b34d161b.xml
find: ‘user/1000/gvfs’: Permission denied
find: ‘user/1000/doc’: Permission denied
root@silicon:/run#
