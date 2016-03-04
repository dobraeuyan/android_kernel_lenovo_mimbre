#!/bin/bash
/opt/bin/rsign -l "http://bjsign.lenovo.com"
/opt/bin/rsign --bootsign $1 --keyset tablet --keytype verity --platform intel -n $2
