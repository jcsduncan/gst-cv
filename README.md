#GST-CV

### TODO:

* Finish the readme :-)
* Improve python tests.
* Port to gitlab and implement CI.
* Generator for API docs
* Fixup git workflow, branches, branch rules.
* Implement example object detection using TensorRT and OpenVino backends.
* Implement GstVideoOverlayComposition element for rendering meta overlays.

### Build & Test

```shell script
mkdir build && cd build
cmake ..
make -j
make test
```

### Install
Installing will install all libs, plugins (todo), gir typelibs, pkg-config configurations.
 ```shell script
make install
```
