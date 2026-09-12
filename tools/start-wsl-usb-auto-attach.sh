#!/usr/bin/env bash

set -euo pipefail

if [[ $# -ne 1 || ! $1 =~ ^[0-9]+-[0-9]+$ ]]; then
    printf 'Usage: %s <BUSID>\n' "${0##*/}" >&2
    exit 2
fi

bus_id=$1
usbipd_path=$(command -v usbipd.exe || true)

if [[ -z $usbipd_path ]]; then
    usbipd_path='/mnt/c/Program Files/usbipd-win/usbipd.exe'
fi

if [[ ! -x $usbipd_path ]]; then
    printf 'usbipd.exe was not found. Install usbipd-win on Windows.\n' >&2
    exit 1
fi

printf 'Monitoring USB BUSID %s for WSL auto-attach.\n' "$bus_id"
printf 'Keep this terminal open. Press Ctrl+C to stop monitoring.\n'

exec "$usbipd_path" attach --wsl --busid "$bus_id" --auto-attach
