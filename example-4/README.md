# Kafka Example four

# What is it?

This is the first test that should enable bridging between two Corda gateways.

# Building

Having run cmake in the top level of the repository already it should be a simple
case of running make

```
make
```

## Build Artifacts

* ***corda-id**:* executable to run for each identity. 

# Setup

Start the Kafka cluster we'll use for testing
    
```
docker-compose up -d
```

This will

1. Start the kafka environment
   1. Zoo Keeper
   2. The Kafka Brokers
2. Start an admin client that can be navigated to at ```localhost:8081```
3. Create the test topic ```my-topic-1``` within Kafka

# Running the example

1. Start the consumer
   ```shell
    ./consumer config.ini
    ```

2. In a second terminal session run the producer
    ```shell
    ./producer config.ini
    ```
