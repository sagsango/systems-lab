#!/bin/bash

# Put binary in service
make
sudo cp my_service /usr/local/bin/
sudo chmod +x /usr/local/bin/my_service

#create systemd metadata for the deamon
sudo cp my_service.service /etc/systemd/system/my_service.service

#add service in systemd daemon
sudo systemctl daemon-reload
sudo systemctl enable my_service
sudo systemctl start my_service


#print logs
journalctl -u my_service.service -f






