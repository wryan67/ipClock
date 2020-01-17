IP Clock
--------

Using an LCD1602 display, show the current time and current IP address.  Outupt codes:

* w=wlan0 (wifi)
* e=eth0 (ethernet)

## Requirements
On your Raspberry Pi, please use the raspi-config program to enable the I2C interface.

	$ sudo raspi-config


## Prerequisites

Voltage Catcher is based on [WiringPi](http://wiringpi.com/), so, you'll need make sure you 
have WiringPi installed before you can successfully compile this program.  


## Download
Use git to download the software from github.com:

    $ cd ~/projects { or wherever you keep downloads }
    $ git clone https://github.com/wryan67/ipClock.git

## Install

To compile IP Clock, navigate into the src folder and use the make utility to compile 
and install the program.  Sudo is required for the install command in order to place the 
final "ipClock" executable into /usr/local/bin.  

    $ cd ~/projects { or wherever you keep downloads }
    $ cd ipClock/src
    $ make install


## Usage

Below are the options with a basic description.  

    $ ipClock
    usage: lcdClock [-d] [-a 00-FF] [-p 0-40] [-f n] [-s speed] [-m motd]   
    a = hexadecimal i2c address ($gpio i2cd)
    f = clock format
        1 - Weekday Month Date HH24:MM
        2 - Weekday Date  HH:MM AM/PM


## Examples

The clock will run indefinitely.  

    $ ipClock -a23 -f2
    Press ^c to exit IP Clock

## Crontab

To enable starting the clock on boot, use "crontab -e" to edit the cron and add this line:

    @reboot /usr/local/bin/ipClock -a23 -f2 > /dev/null 2>&1


## Stopping the IP clock

Use the killall command to stop the clock when it is running in the background.

    $ killall ipClock
