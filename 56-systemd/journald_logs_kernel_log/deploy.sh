#!/bin/bash

# Put binary in service
make
sudo cp dummy-journald /usr/local/bin/
sudo chmod +x /usr/local/bin/dummy-journald

#create systemd metadata for the deamon
sudo cp dummy-journald.service /etc/systemd/system/dummy-journald.service

#add service in systemd daemon
sudo systemctl daemon-reload
sudo systemctl enable dummy-journald
sudo systemctl start dummy-journald


#print logs
journalctl -u dummy-journald.service -f






