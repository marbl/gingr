#!/usr/bin/env bash
# Create a user-writable "harvest" install prefix (include/ + lib/) that points
# at the already-built harvest-tools checkout, so gingr can link against it
# without needing root to install harvest into /usr/local.
set -e

HT=/mnt/c/Users/miken/Code/harvest-tools
PFX="$HOME/gingr-deps/harvest"

ls "$HT/libharvest.a" "$HT/src/harvest/HarvestIO.h" \
   "$HT/src/harvest/capnp/harvest.capnp.h" "$HT/src/harvest/pb/harvest.pb.h"

mkdir -p "$PFX/include" "$PFX/lib"
ln -sfn "$HT/src/harvest" "$PFX/include/harvest"
ln -sfn "$HT/libharvest.a" "$PFX/lib/libharvest.a"

echo "=== prefix layout ==="
ls -l "$PFX/include/" "$PFX/lib/"
echo "=== resolve check ==="
ls -lL "$PFX/include/harvest/HarvestIO.h" \
       "$PFX/include/harvest/capnp/harvest.capnp.h" \
       "$PFX/lib/libharvest.a"
echo "HARVEST_PREFIX=$PFX"
