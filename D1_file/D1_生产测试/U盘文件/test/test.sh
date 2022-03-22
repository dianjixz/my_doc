#!/bin/bash
/mnt/test/can-test >> /dev/null 2>&1 &

/mnt/test/uart-test /dev/ttyS2 >> /dev/null 2>&1 &

/mnt/test/uart-test /dev/ttyS3 >> /dev/null 2>&1 &