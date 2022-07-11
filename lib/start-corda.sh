#!/usr/bin/env bash

echo 'Starting link managers'
java -jar \
    $1/applications/p2p-link-manager/build/bin/corda-p2p-link-manager-$2.jar \
    -k=localhost:19092\
    -i=1 > p2p-deployment/logs/alice/lm.log &

java -jar \
    $1/applications/p2p-link-manager/build/bin/corda-p2p-link-manager-$2.jar \
    -k=localhost:29092\
    -i=1 > p2p-deployment/logs/chip/lm.log &

echo 'starting Gateways'
java -jar \
  -Djdk.net.hosts.file=p2p-deployment/hosts \
  $1/applications/p2p-gateway/build/bin/corda-p2p-gateway-$2.jar \
  -k=localhost:19092 \
  -i=1 > p2p-deployment/logs/alice/gw.log &

java -jar \
  -Djdk.net.hosts.file=p2p-deployment/hosts \
  $1/applications/p2p-gateway/build/bin/corda-p2p-gateway-$2.jar \
  -k=localhost:29092 \
  -i=1 > p2p-deployment/logs/chip/gw.log &

