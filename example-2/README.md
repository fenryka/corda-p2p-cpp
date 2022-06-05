# Kafka Example Two

# What is it?

This is a very basic Kafka producer / consumer example based on Confluents own. Their 
documentation for lib rdkafka focusses on the C api and not C++, so thus
is a basic port of that example to rdkafka++. This is why there is a fair amount
of Cisms left in, especially around parsing the config and printing to the screen.
I couldn't be bothered to alter that code, wanting to focus only on switching over
to the C++ Kafka API.

# Building

Having run cmake in the top level of the repository already it should be a simple
case of running make

```
make
```

## Build Artifacts

* producer
* consumer

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
