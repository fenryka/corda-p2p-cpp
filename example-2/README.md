# Kafka Example Two

# What is it?

Whilst example-1 was a modest re-implementation of the basic kafka example (which uses
their C API), this example is essneitally the same, but moves it to a more modern C++
style and convention, whilst example-1 just dealt with the API port.

# Building

Having run cmake in the top level of the repository already it should be a simple
case of running make

```
make
```

## Build Artifacts

* producer2
* consumer2

> ***NOTE*** **:** If I could work out how to make cmake namespace build targets to allow
> multiple targets with the same name, these would have better names

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
