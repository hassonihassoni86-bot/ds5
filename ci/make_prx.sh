#!/bin/bash
set -e
cd "$(dirname "$0")/.."

sudo chown -R "$(id -u):$(id -g)" bin
test -s bin/ds5_controller.elf || { echo "ds5_controller.elf is missing or empty"; exit 1; }

T=$(mktemp -d)
echo "Downloading toolchain (for create-fself)..."
curl -fsSL https://github.com/illusion0001/OpenOrbis-PS4-Toolchain/releases/latest/download/toolchain.tar.gz | tar xz -C "$T"

FS=$(find "$T" -type f -name create-fself | head -n1)
if [ -z "$FS" ]; then
  echo "create-fself was not found"
  find "$T" -type f -path '*bin/*' | head -30
  exit 1
fi
chmod +x "$FS"
export OO_PS4_TOOLCHAIN=$(echo "$FS" | sed 's#/bin/linux/create-fself$##')
echo "Using: $FS"

rm -f bin/ds5_controller.prx bin/ds5_controller.oelf
"$FS" -in=bin/ds5_controller.elf -out=bin/ds5_controller.oelf --lib=bin/ds5_controller.prx --paid 0x3800000000000011

test -s bin/ds5_controller.prx || { echo "ERROR: ds5_controller.prx is empty"; exit 1; }
ls -la bin
echo "PRX header bytes:"; head -c 8 bin/ds5_controller.prx | od -An -tx1
