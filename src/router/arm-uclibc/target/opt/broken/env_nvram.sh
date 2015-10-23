#!/bin/sh
#Script is setting up readycloud_nvram variables needed to register device in cloud


readycloud_nvram set readycloud_fetch_url="https://readycloud.netgear.com/device/entry"
readycloud_nvram set readycloud_hook_url="https://readycloud.netgear.com/device/hook"
readycloud_nvram set readycloud_upload_url="https://readycloud.netgear.com/directio"

#set up variables for readycloud_control
readycloud_nvram set rcagent_path="/opt/rcagent"
readycloud_nvram set readycloud_control_path="/opt/broken" #01/29/2014 modify by Kathy
readycloud_nvram set remote_path="/opt/remote"
readycloud_nvram set leafp2p_path="/opt/leafp2p"
readycloud_nvram set readydrop_path="/opt/readydrop"

readycloud_nvram set readycloud_use_xcloud=1
readycloud_nvram set readycloud_use_lantry=1
readycloud_nvram set rcagent_log_to_console=0
readycloud_nvram set rcagent_log_level=error
readycloud_nvram set rcagent_log_to_file=1

readycloud_nvram set leafp2p_log_type=1
readycloud_nvram set leafp2p_log_file_name=/tmp/leafd.log
readycloud_nvram set leafp2p_debug=5
readycloud_nvram set leafp2p_log_entry_limit=10000
readycloud_nvram set leafp2p_log_entry_flush=1


readycloud_nvram commit
