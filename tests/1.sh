#!/bin/bash

source tests/prologue.sh
source tests/env.sh

$RINACONF ipcp-create shim-dummy du.IPCP 1 || exit 1
$RINACONF ipcp-create shim-dummy du.IPCP 2 || exit 1
$RINACONF ipcp-create shim-dummy dudu.IPCP 5 || exit 1

$RINACONF assign-to-dif dummy.DIF du.IPCP 1 || exit 1
$RINACONF assign-to-dif dummylo.DIF du.IPCP 2 || exit 1

$RINACONF ipcp-destroy du.IPCP 1 || exit 1
$RINACONF ipcp-destroy du.IPCP 2 || exit 1
$RINACONF ipcp-destroy dudu.IPCP 5 || exit 1

source tests/epilogue.sh
