#!/usr/bin/env bash
# Build gingr under WSL (Ubuntu) with Qt6 + harvest/protobuf/capnp.
#
#   protobuf : system package           -> /usr      (libprotobuf.a in multiarch dir)
#   capnp    : built from source        -> /usr/local
#   harvest  : harvest-tools checkout   -> ~/gingr-deps/harvest (include/ + lib/ symlinks)
#
# Run from the gingr source dir. Pass "clean" as first arg to rebuild from scratch.
set -e
cd "$(dirname "$0")"

HARVEST_PREFIX="$HOME/gingr-deps/harvest"
PROTOBUF_PREFIX=/usr
CAPNP_PREFIX=/usr/local

if [ "$1" = "clean" ]; then
	make distclean 2>/dev/null || true
	rm -f gingr.pro Makefile
fi

echo "=== [1/4] autoconf ==="
autoconf

echo "=== [2/4] configure ==="
./configure \
	--with-harvest="$HARVEST_PREFIX" \
	--with-protobuf="$PROTOBUF_PREFIX" \
	--with-capnp="$CAPNP_PREFIX"

echo "=== [3/4] qmake6 ==="
qmake6 gingr.pro

echo "=== [4/4] make ==="
make -j"$(nproc)"

echo "=== DONE: $(ls -la gingr 2>/dev/null) ==="
