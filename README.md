# Prerequisits

## C++ / CMake Libs

> Note: glib is only required for example 1 which is a very direct port from the Confluent C library example
> of rdkafka

### MacOS

```
brew install PkgConfig
brew install glib
brew install librdkafka
brew install nlohmann-json

# Need to decompress recieved messages as the inner AppMessage is compressed
# inside the Envelope 
brew install zlib
```

#### Google Test

```
git clone https://github.com/google/googletest
cd googletest
mkdir build
cd build
cmake ..
make
make install
```

> *NOTE:* for what I can only assume are bullshit reasons MacOS no longer
> has a default /user/local/include directory. Something about wanting
> Libraries installed in their own apple specific way. Which is of course
> a PitA for anyone wanting to install a generic linux style lib... like
> GTest.
> 
> I recommend just  creating it but that's almost certianly not the 
> recomended aproach. 

### Ububtu Linux

```
sudo apt-get install -y pkg-config
sudo apt-get install -y librdkafka-dev
sudo apt-get install -y librdkafka++1
sudo apt-get install -y libglib2.0-dev
sudo apt-get install -y nlohmann-json-dev

# linux doens't have arc4random as part of its standard lib
sudo apt-get install -y libsd-dev
```

#### Google Test

* sudo apt-get install cmake
* sudo apt-get install libqpid-proton8-dev
* sudo apt-get install libgtest-dev

And now because that installer only pulls down the sources
* cd /usr/src/googletest
* sudo cmake .
* sudo cmake --build . --target install

## Apache AVRO

The C++ Apache AVRO implementation requries BOOST

```
brew install boost 
brew install snappy
```

```
git clone https://github.com/apache/avro
```

```
cd avro/lang/c++
cmake .
make package
```

Then copy the tarball to a convenient location, extract it, and add the bin dir to your path, this includes the avro
generator


## Corda

This is a C++ client library for the Corda P2P layer. As such, unsurprisingly, we need Corda.

1. Clone it

    ```shell
    git clone https://github.com/corda/corda-runtime-os.git
    ```

2. Seemingly obvious step but for completeness

    ```shell
    cd corda-runtime-os
    ```

3. Switch to the QA'd branch

   ```shell
   git checkout p2p-preview
   ```
   
4. Build the parts we need

    ```shell
    ./gradlew \
       :applications:p2p-link-manager:appJar \
       :applications:p2p-gateway:appJar \
       :applications:tools:kafka-setup:appJar \
       :applications:tools:p2p-test:app-simulator:appJar \
       :applications:tools:p2p-test:p2p-setup:appJar \
       :applications:tools:p2p-test:fake-ca:appJar
    ```

> Note: Building Corda 5 requires Java 11.

# Build

## MacOS

For pkg-config to find openssl@1.1 you may need to set:
```
# This used to work
export PKG_CONFIG_PATH="/usr/local/opt/openssl@1.1/lib/pkgconfig"

# On my newer M1 Mac Brew seems to stick things here now
export PKG_CONFIG_PATH="/opt/homebrew/opt/openssl@1.1/lib/pkgconfig"
```

# CLion

For CLion's version of cmake to apply the above path you need to set it in
the cmake settings: 

<img src="./clion-1.png"  alt="meh" />

# Binaries

## example-1

Simple producer/consumer ported from the confluent C example. The primary changes are those surrounding the use
of the C++ API instead of the C one, the supporting code has been left "as is" and as such remains "C"

## example-2

Same as example-1 save the c-isms are removed. Additionally, the config handling is dealt with using the corda-p2p-cpp
config parser (json) instead of the glib ".ini" style provided by confluent.

## example-3

Bidirectional handling over two shared topics where a->b and b->a

## example-4

## example-5
