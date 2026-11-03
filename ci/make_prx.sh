#!/bin/bash
# Runs on the GitHub runner (not in the container): ELF -> PRX with create-fself.
# Uses the same OpenOrbis toolchain build that the GoldHEN SDK itself uses in CI.
set -e
cd "$(dirname "$0")/.."

test -s bin/ds5_controller.elf || { echo "ds5_controller.elf is missing or empty"; exit 1; }

T=$(mktemp -d)
echo "Downloading toolchain (for create-fself)..."
curl -fsSL https://github.com/illusion0001/OpenOrbis-PS4-Toolchain/releases/latest/download/toolchain.tar.gz | tar xz -C "$T"

FS=$(find "$T" -type f -name create-fself | head -n1)
if [ -z "$FS" ]; then
  echo "create-fself was not found in the downloaded toolchain. Tools it contains:"
  find "$T" -type f -path '*bin/*' | head -30
  exit 1
fi
chmod +x "$FS"
export OO_PS4_TOOLCHAIN=$(echo "$FS" | sed 's#/bin/linux/create-fself$##')
echo "OO_PS4_TOOLCHAIN=$OO_PS4_TOOLCHAIN"
echo "Using: $FS"
"$FS" -h 2>&1 | head -20 || true

rm -f bin/ds5_controller.prx bin/ds5_controller.oelf
"$FS" -in=bin/ds5_controller.elf -out=bin/ds5_controller.oelf --lib=bin/ds5_controller.prx --paid 0x3800000000000011

# Fail loudly if the PRX is missing or empty (an empty file must never look like success)
test -s bin/ds5_controller.prx || { echo "ERROR: ds5_controller.prx is empty"; exit 1; }
ls -la bin
echo "PRX header bytes:"; head -c 8 bin/ds5_controller.prx | od -An -tx1
