# qos_sched_custom
> Modified DPDK QoS Scheduler Sample Application

![Status](https://img.shields.io/badge/status-released-green.svg?style=flat-square)
[![Maintenance](https://img.shields.io/badge/maintained-yes-green.svg?style=flat-square)](https://GitHub.com/conorwalsh/qos_sched_custom/graphs/commit-activity)
[![Version](https://img.shields.io/badge/version-v19.11-green.svg?style=flat-square)](https://GitHub.com/conorwalsh/qos_sched_custom/tags/)
![Code Size](https://img.shields.io/github/languages/code-size/conorwalsh/qos_sched_custom.svg?style=flat-square)
[![Lines of Code](https://tokei.rs/b1/github/conorwalsh/qos_sched_custom?style=flat-square)]()
[![GitHub license](https://img.shields.io/badge/license-MIT-green.svg?style=flat-square)](https://github.com/conorwalsh/qos_sched_custom/blob/master/LICENSE)
[![HitCount](http://hits.dwyl.io/conorwalsh/qos_sched_custom.svg)](http://hits.dwyl.io/conorwalsh/qos_sched_custom)

[DPDK](https://dpdk.org) is a set of C libraries for fast packet processing. DPDK has many sample applications for showcasing the features of DPDK.

This app is based on the DPDK Qos Scheduler Sample Application which is designed to showcase what DPDK QoS can do. This app was built to profile the performance of DPDK QoS. This app expands the information that is printed to the user and now displays cycle costs. The app also uses MAC addresses for classifying packets which is easier to use.

## Installation

### Linux:
* Install DPDK 19.11:

    [github.com/DPDK/dpdk/releases/tag/v19.11](https://github.com/DPDK/dpdk/releases/tag/v19.11)
* Build and Setup DPDK
* Edit the profile.cfg to fit your setup
* Build the app:
    ```sh
    make
    ```

_Note: This app has only been tested on Ubuntu 18.04_

## Usage

* Update the configuration options in _profile.cfg_
* Run app
    ```sh
    ./build/qos_sched -l 26,27,71 -w 0000:83:00.0 --telemetry -- --pfc "0,0,27,71" --cfg profile.cfg --mst 26
    ```

Example output of app:

![](/screenshots/qos_sched_custom_1911_screenshot.PNG)

## Classifying Packets
The Destination MAC address is used to set how QoS will classify packet

Each part of the MAC address corresponds to a piece of information

MAC Address: XX:XX:XX:XX:XX:XX

Meaning: COLOUR:XX:SUBPORT:PIPE:TRAFFICCLASS:QUEUE

E.g.:
  - MAC Address: 01:00:01:1F:03:02
    - Colour: 1
    - Blank: 0 (unused - possible future use)
    - Subport: 1
    - Pipe: 31
    - Traffic Class: 3
    - Queue: 2

_The range command in pktgen can be used to vary the mac address of the packets_

## Release History

* v19.11
    * This release coincided with the 19.11 DPDK release and this app is compatible with DPDK 19.05
    * Fixed floating point errors when app started with no traffic flow
* v19.05
    * This release coincided with the 19.05 DPDK release and this app is compatible with DPDK 19.05

## Meta

Conor Walsh – conor@conorwalsh.net

This project is distributed under the MIT license. See ``LICENSE`` for more information.

[https://github.com/conorwalsh/qos_sched_custom](https://github.com/conorwalsh/qos_sched_custom/)

## Why?

This app was made to profile the performance of the DPDK Qos Schedular Application.

It was later used as part of the author’s research into the automation of analysis and optimisation of DPDK apps.
