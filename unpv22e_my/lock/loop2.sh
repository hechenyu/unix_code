#!/bin/sh

cat > seqno <<FOO
1
FOO

ls -l seqno
cat seqno

export PATH=$PATH:.

loopfcntl 10000 & loopfcntl 10000 & loopfcntl 10000 & loopfcntl 10000 &
loopfcntl 10000 & loopfcntl 10000 & loopfcntl 10000 & loopfcntl 10000 &
loopfcntl 10000 & loopfcntl 10000 &

wait

cat seqno
