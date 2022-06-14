#!/usr/bin/env bash

set -e

### Check if a directory does not exist ###
if [ ! -d "$1" ]; then
    echo "Corda directory \"$1\" does not exist"
    exit 1
fi


echo 'Killing running processes'
ps -ef | grep 'java -jar' | grep -v grep | awk '{print $2}' | xargs kill || echo 'no process are running'
docker-compose --env-file cluster1.env down
docker-compose --env-file cluster2.env down
docker-compose --env-file cluster1.env rm -f -s
docker-compose --env-file cluster2.env rm -f -s

echo 'Removing old data'
rm -rf p2p-deployment/
mkdir -p p2p-deployment/logs


echo 'Starting Kafka 1'
docker-compose --env-file cluster1.env up -d > p2p-deployment/logs/kafka1.log

echo 'Starting Kafka 2'
docker-compose --env-file cluster2.env up -d > p2p-deployment/logs/kafka2.log

echo 'Sleeping 5 seconds'
sleep 5

echo 'Setting topics - kafka 1'
mkdir -p p2p-deployment/logs/alice
mkdir -p p2p-deployment/logs/chip
java -jar \
  -Dbootstrap.servers=localhost:19092 \
  $1/applications/tools/kafka-setup/build/bin/corda-kafka-setup-5.0.0$2.jar \
  --topic \
  $1/applications/tools/p2p-test/p2p-setup/src/main/resources/p2p-kafka-setup-example.conf > p2p-deployment/logs/alice/kafka-setup.log

echo 'Setting topics - kafka 2'
java -jar \
  -Dbootstrap.servers=localhost:29092 \
  $1/applications/tools/kafka-setup/build/bin/corda-kafka-setup-5.0.0$2.jar \
  --topic \
  $1/applications/tools/p2p-test/p2p-setup/src/main/resources/p2p-kafka-setup-example.conf  > p2p-deployment/logs/chip/kafka-setup.log

echo 'Creating session keys'
mkdir -p p2p-deployment/keystores
openssl ecparam -out p2p-deployment/keystores/alice_session_key_private.pem -name prime256v1 -genkey
openssl ecparam -out p2p-deployment/keystores/chip_session_key_private.pem -name prime256v1 -genkey
openssl ec -in p2p-deployment/keystores/alice_session_key_private.pem -pubout -out p2p-deployment/keystores/alice_session_key_public.pem
openssl ec -in p2p-deployment/keystores/chip_session_key_private.pem -pubout -out p2p-deployment/keystores/chip_session_key_public.pem

echo 'Creating TLS keys'
java -jar \
  $1/applications/tools/p2p-test/fake-ca/build/bin/corda-fake-ca-5.0.0$2.jar \
  -m p2p-deployment/keystores/ca \
  ca \
  create-certificate www.alice.net alice.net \
  create-certificate www.chip.net chip.net

echo 'Preparing setup'
cat << EOF > p2p-deployment/alice.setup.json
{
  "groupsToAdd": [{
    "groupId": "group-1",
    "data": {
      "networkType": "CORDA_5",
      "protocolModes": ["AUTHENTICATION_ONLY", "AUTHENTICATED_ENCRYPTION"],
      "trustRootCertificatesFiles": ["p2p-deployment/keystores/ca/ca/root-certificate.pem"]
    }
  }],
  "membersToAdd": [{
      "groupId": "group-1",
      "x500name": "O=Alice, L=London, C=GB",
      "data": {
        "publicSessionKeyFile": "p2p-deployment/keystores/alice_session_key_public.pem",
        "address": "http://www.alice.net:8085"
      }
    },
    {
      "groupId": "group-1",
      "x500name": "O=Chip, L=London, C=GB",
      "data": {
        "publicSessionKeyFile": "p2p-deployment/keystores/chip_session_key_public.pem",
        "address": "https://www.chip.net:8086"
      }
    }
  ],
  "identitiesToAdd": [{
    "x500name": "O=Alice, L=London, C=GB",
    "groupId": "group-1",
    "data": {
      "tlsTenantId": "alice-tls",
      "sessionKeyTenantId": "alice",
      "tlsCertificatesFiles": ["p2p-deployment/keystores/ca/www.alice.net/certificate.pem"],
      "publicSessionKeyFile": "p2p-deployment/keystores/alice_session_key_public.pem"
    }
  }],
  "keysToAdd": [{
      "keysFile": "p2p-deployment/keystores/alice_session_key_private.pem",
      "tenantId": "alice",
      "publishAlias": "alice-session-key"
    },
    {
      "keysFile": "p2p-deployment/keystores/ca/www.alice.net/keys.pem",
      "tenantId": "alice-tls",
      "publishAlias": "alice-tls-key"
    }
  ]
}
EOF

cat << EOF > p2p-deployment/chip.setup.json
{
  "groupsToAdd": [{
    "groupId": "group-1",
    "data": {
      "networkType": "CORDA_5",
      "protocolModes": ["AUTHENTICATION_ONLY", "AUTHENTICATED_ENCRYPTION"],
      "trustRootCertificatesFiles": ["p2p-deployment/keystores/ca/ca/root-certificate.pem"]
    }
  }],
  "membersToAdd": [{
      "groupId": "group-1",
      "x500name": "O=Alice, L=London, C=GB",
      "data": {
        "publicSessionKeyFile": "p2p-deployment/keystores/alice_session_key_public.pem",
        "address": "http://www.alice.net:8085"
      }
    },
    {
      "groupId": "group-1",
      "x500name": "O=Chip, L=London, C=GB",
      "data": {
        "publicSessionKeyFile": "p2p-deployment/keystores/chip_session_key_public.pem",
        "address": "https://www.chip.net:8086"
      }
    }
  ],
  "identitiesToAdd": [{
    "x500name": "O=Chip, L=London, C=GB",
    "groupId": "group-1",
    "data": {
      "tlsTenantId": "chip-tls",
      "sessionKeyTenantId": "chip",
      "tlsCertificatesFiles": ["p2p-deployment/keystores/ca/www.chip.net/certificate.pem"],
      "publicSessionKeyFile": "p2p-deployment/keystores/chip_session_key_public.pem"
    }
  }],
  "keysToAdd": [{
      "keysFile": "p2p-deployment/keystores/chip_session_key_private.pem",
      "tenantId": "chip",
      "publishAlias": "chip-session-key"
    },
    {
      "keysFile": "p2p-deployment/keystores/ca/www.chip.net/keys.pem",
      "tenantId": "chip-tls",
      "publishAlias": "chip-tls-key"
    }
  ]
}
EOF

echo 'Setting up alice on kafka 1'
java -jar \
  $1/applications/tools/p2p-test/p2p-setup/build/bin/corda-p2p-setup-5.0.0$2.jar \
  -k localhost:19092 \
  apply p2p-deployment/alice.setup.json \
  config-link-manager \
  config-gateway --port=8085 > p2p-deployment/logs/alice/p2p-setup.log

echo 'Setting up chip on kafka 2'
java -jar \
  $1/applications/tools/p2p-test/p2p-setup/build/bin/corda-p2p-setup-5.0.0$2.jar \
  -k localhost:29092 \
  apply p2p-deployment/chip.setup.json \
  config-link-manager \
  config-gateway --port=8086 > p2p-deployment/logs/chip/p2p-setup.log


echo 'Starting link managers'
java -jar \
    $1/applications/p2p-link-manager/build/bin/corda-p2p-link-manager-5.0.0$2.jar \
    -k=localhost:19092\
    -i=1 > p2p-deployment/logs/alice/lm.log &

java -jar \
    $1/applications/p2p-link-manager/build/bin/corda-p2p-link-manager-5.0.0$2.jar \
    -k=localhost:29092\
    -i=1 > p2p-deployment/logs/chip/lm.log &


echo 'Starting Gateways'
cat << EOF > p2p-deployment/hosts
127.0.0.1	localhost
127.0.0.1   alice.net
127.0.0.1   www.alice.net
127.0.0.1   chip.net
127.0.0.1   www.chip.net
81.169.184.163 www.tinycert.org
81.169.184.163 tinycert.org
81.169.184.163 crl.tinycert.org
81.169.184.163 ocsp.tinycert.org
255.255.255.255	broadcasthost
::1             localhost
EOF

java -jar \
  -Djdk.net.hosts.file=p2p-deployment/hosts \
  $1/applications/p2p-gateway/build/bin/corda-p2p-gateway-5.0.0$2.jar \
  -k=localhost:19092 \
  -i=1 > p2p-deployment/logs/alice/gw.log &

java -jar \
  -Djdk.net.hosts.file=p2p-deployment/hosts \
  $1/applications/p2p-gateway/build/bin/corda-p2p-gateway-5.0.0$2.jar \
  -k=localhost:29092 \
  -i=1 > p2p-deployment/logs/chip/gw.log &

