# Prerequisits
## MacOS

```
brew install PkgConfig
brew install glib
brew install librdkafka
```

# Build

For pkg-config to find openssl@1.1 you may need to set:
```
export PKG_CONFIG_PATH="/usr/local/opt/openssl@1.1/lib/pkgconfig"
```

#CLion

For CLion's version of cmake to apply the above path you need to set it in
the cmake settings: 

<img src="clion-1.png" />




