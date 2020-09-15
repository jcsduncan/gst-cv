#GST-CV

###TODO:

* Finish the readme :-)
* Improve python tests.
* Port to gitlab and implement CI.
* Implement example object detection using TensorRT and OpenVino backends.
* Implement GstVideoOverlayComposition element for rendering meta overlays.

###Build & Test

```shell script
mkdir build && cd build
cmake ..
make -j
make test
```

###Install
Installing will install all libs, plugins (todo), gir typelibs, pkg-config configurations.
 ```shell script
make install
```
