#!/bin/bash
# Build script run INSIDE the openorbisofficial/toolchain container (see .github/workflows/build.yml)
set -x
TC=/lib/OpenOrbisSDK
SDK=/workspace/sdk/GoldHEN_Plugins_SDK
cd /workspace

# --- 1. This image has no llvm-ar: provide one on top of GNU ar
if ! command -v llvm-ar >/dev/null 2>&1; then
  printf '#!/bin/sh\nif [ "$1" = "--format=bsd" ]; then shift; fi\nexec ar "$@"\n' > /usr/local/bin/llvm-ar
  chmod +x /usr/local/bin/llvm-ar
fi

# --- 2. Diagnostics: what does this toolchain provide?
ls "$TC/bin/linux"
ls "$TC/include/orbis" | tr '\n' ' '; echo
ls "$TC/include/orbis/_types" | tr '\n' ' '; echo
for h in Pad.h Usbd.h UserService.h libkernel.h; do
  [ -f "$TC/include/orbis/$h" ] && echo "have orbis/$h" || echo "MISSING orbis/$h"
done

# --- 3. Show every compile error (default limit is 20 per file)
sed -i "s/^CFLAGS += -Wall/CFLAGS += -Wall -ferror-limit=0/" Makefile

# --- 4. GoldHEN SDK static library
make -C "$SDK" OO_PS4_TOOLCHAIN="$TC" || exit 1

# --- 5. Plugin: -k keeps going so ALL compile errors are listed in one run
make -k dirs bin/ds5_controller.elf OO_PS4_TOOLCHAIN="$TC" GOLDHEN_SDK="$SDK" 2>&1 | tee /workspace/build.log
[ -f bin/ds5_controller.elf ] || { echo "=== ELF NOT BUILT ==="; grep -c "error:" /workspace/build.log; exit 1; }

# --- 6. The ELF is the result of this stage. Turning it into a PRX is done by
#        ci/make_prx.sh on the runner (the create-lib tool in this image can not do it).
ls -la bin
test -s bin/ds5_controller.elf || exit 1
exit 0
